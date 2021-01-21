#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <link.h>
#include <unistd.h>

#include <dlfcn.h>
#include <sys/mman.h>

#include "hooks.h"


int get_page_prot(ElfW(Addr) addr)
{
	FILE *f;
	char line[256];
	ElfW(Addr) start, end;
	char r, w, x;
	int prot = -1;

	f = fopen("/proc/self/maps", "r");
	if (!f)
		return -1;

	while(fgets(line, 256, f) != NULL) {
		sscanf(line, "%lx-%lx %c%c%c", (unsigned long *)&start, (unsigned long *)&end, &r, &w, &x);

		if (addr >= start && addr < end) {
			prot = 0;
			if (r == 'r')
				prot |= PROT_READ;
			if (w == 'w')
				prot |= PROT_WRITE;
			if (x == 'x')
				prot |= PROT_EXEC;
			break;
		}
	}

	fclose(f);

	return prot;
}

struct dyn_info {
	ElfW(Addr) l_addr;
	ElfW(Sym) *symtab;
	char *strtab;

	/* DT_JMPREL - PLT relocations, both in 32 and 64-bit */
	void *plt_rel;
	int plt_rel_type;
	unsigned int plt_rel_sz;

	/* DT_RELA - usually only on 64-bit */
	ElfW(Rela) *rela;
	unsigned int rela_sz;

	/* DT_REL - usually only on 32-bit */
	ElfW(Rel) *rel;
	unsigned int rel_sz;
};

void parse_dyn(struct link_map *l, ElfW(Dyn) *dyn, struct dyn_info *di)
{
	int i;

	struct dyn_info init = {
		.l_addr = l->l_addr,
		.symtab = NULL,
		.strtab = NULL,
		.plt_rel = NULL,
		.plt_rel_type = 0,
		.plt_rel_sz = 0,
		.rela = NULL,
		.rela_sz = 0,
		.rel = NULL,
		.rela_sz = 0,
	};

	*di = init;

	for (i = 0; dyn[i].d_tag; i++) {
		// dynamic symbol table
		if (dyn[i].d_tag == DT_SYMTAB)
			di->symtab = (ElfW(Sym) *)dyn[i].d_un.d_ptr;

		// dynamic string table
		if (dyn[i].d_tag == DT_STRTAB)
			di->strtab = (char *)dyn[i].d_un.d_ptr;

		// .rela.dyn relocations
		if (dyn[i].d_tag == DT_RELA)
			di->rela = (ElfW(Rela) *)dyn[i].d_un.d_ptr;

		// .rela.dyn relocation entry size
		if (dyn[i].d_tag == DT_RELASZ)
			di->rela_sz = dyn[i].d_un.d_val;

		// .plt relocations
		if (dyn[i].d_tag == DT_JMPREL)
			di->plt_rel = (ElfW(Rela) *)dyn[i].d_un.d_ptr;

		// .plt relocation entry size
		if (dyn[i].d_tag == DT_PLTRELSZ)
			di->plt_rel_sz = dyn[i].d_un.d_val;

		if (dyn[i].d_tag == DT_PLTREL)
			di->plt_rel_type = dyn[i].d_un.d_val;

		// .rel.dyn relocations
		if (dyn[i].d_tag == DT_REL)
			di->rel = (ElfW(Rel) *)dyn[i].d_un.d_ptr;

		// .rel.dyn relocation entry size
		if (dyn[i].d_tag == DT_RELSZ)
			di->rel_sz = dyn[i].d_un.d_val;
	}
}


int try_hook_rela(struct dyn_info *di, ElfW(Rela) *rela, int sz, struct func_hook *hook)
{
	int num;
	int i;
	int sym_idx;
	int rel_type;
	int prot;
	int ret;
	char *sym_name;
	int pagesize;
	ElfW(Sym) *sym;
	ElfW(Addr) *addr;
	ElfW(Addr) addr_page;

	pagesize = getpagesize();

	num = sz / sizeof(ElfW(Rela));

	for (i = 0; i < num; i++) {
		if (__WORDSIZE == 32) {
			sym_idx = ELF32_R_SYM(rela[i].r_info);
			sym = &di->symtab[sym_idx];

			rel_type = ELF32_R_TYPE(rela[i].r_info);
			if (rel_type != R_386_JMP_SLOT)
				continue;

			sym_name = &di->strtab[sym->st_name];
		} else {
			sym_idx = ELF64_R_SYM(rela[i].r_info);
			sym = &di->symtab[sym_idx];

			rel_type = ELF64_R_TYPE(rela[i].r_info);
			if (rel_type != R_X86_64_JUMP_SLOT && rel_type != R_X86_64_GLOB_DAT)
				continue;

			sym_name = &di->strtab[sym->st_name];
		}

		if (!strcmp(sym_name, hook->name)) {
			addr = (ElfW(Addr) *)((char *)di->l_addr + rela[i].r_offset);
			addr_page = (ElfW(Addr))addr & (~(pagesize - 1));

			prot = get_page_prot((ElfW(Addr))addr);
			if (prot < 0)
				return -1;

			if (prot == PROT_READ) {
				ret = mprotect((void *)addr_page, pagesize, PROT_READ|PROT_WRITE);
				if (ret < 0)
					return -1;
			}

			*addr = hook->addr;

			if (prot == PROT_READ) {
				ret = mprotect((void *)addr_page, pagesize, PROT_READ);
				if (ret < 0)
					return -1;
			}

			return 0;
		}
	}

	return -1;
}

int try_hook_rel(struct dyn_info *di, ElfW(Rel) *rel, int sz, struct func_hook *hook)
{
	int num;
	int i;
	int sym_idx;
	int rel_type;
	int prot;
	int ret;
	char *sym_name;
	int pagesize;
	ElfW(Sym) *sym;
	ElfW(Addr) *addr;
	ElfW(Addr) addr_page;

	pagesize = getpagesize();

	num = sz / sizeof(ElfW(Rel));

	for (i = 0; i < num; i++) {
		if (__WORDSIZE == 32) {
			sym_idx = ELF32_R_SYM(rel[i].r_info);
			sym = &di->symtab[sym_idx];

			rel_type = ELF32_R_TYPE(rel[i].r_info);
			if (rel_type != R_386_JMP_SLOT && rel_type != R_386_GLOB_DAT)
				continue;

			sym_name = &di->strtab[sym->st_name];
		} else {
			sym_idx = ELF64_R_SYM(rel[i].r_info);
			sym = &di->symtab[sym_idx];

			rel_type = ELF64_R_TYPE(rel[i].r_info);
			if (rel_type != R_X86_64_JUMP_SLOT && rel_type != R_X86_64_GLOB_DAT)
				continue;

			sym_name = &di->strtab[sym->st_name];
		}

		if (!strcmp(sym_name, hook->name)) {
			addr = (ElfW(Addr) *)((char *)di->l_addr + rel[i].r_offset);
			addr_page = (ElfW(Addr))addr & (~(pagesize - 1));

			prot = get_page_prot((ElfW(Addr))addr);
			if (prot < 0)
				return -1;

			if (prot == PROT_READ) {
				ret = mprotect((void *)addr_page, pagesize, PROT_READ|PROT_WRITE);
				if (ret < 0)
					return -1;
			}

			*addr = hook->addr;

			if (prot == PROT_READ) {
				ret = mprotect((void *)addr_page, pagesize, PROT_READ);
				if (ret < 0)
					return -1;
			}

			return 0;
		}
	}

	return -1;
}

/*
 * Hook all calls made from target_lib_name to each function in the hooks array
 *
 * We do this by modifying the GOT entries.
 *
 * The GOT offset for one particular symbol is found in the
 * r_offset field of the Elf_Rela structure associated with that symbol
 *
 */
int install_hooks(char *target_lib_name, struct func_hook *hooks, int hook_num)
{
	struct link_map *l;
	ElfW(Dyn) *dyn;
	int i;
	struct dyn_info di;
	int ret;

	l = dlopen(target_lib_name, RTLD_LAZY | RTLD_NOLOAD);
	if (!l)
		return -1;

	for (i = 0; i < hook_num; i++) {
		hooks[i].orig_addr = (unsigned long)dlsym(RTLD_DEFAULT, hooks[i].name);
		if (!hooks[i].orig_addr)
			goto out_dlclose;
	}

	dyn = l->l_ld;

	parse_dyn(l, dyn, &di);

	for (i = 0; i < hook_num; i++) {
		if (di.plt_rel) {
			if (di.plt_rel_type == DT_REL)
				ret = try_hook_rel(&di, di.plt_rel, di.plt_rel_sz, &hooks[i]);
			else
				ret = try_hook_rela(&di, di.plt_rel, di.plt_rel_sz, &hooks[i]);
			if (ret == 0)
				continue;
		}
		if (di.rela) {
			ret = try_hook_rela(&di, di.rela, di.rela_sz, &hooks[i]);
			if (ret == 0)
				continue;
		}
		if (di.rel) {
			ret = try_hook_rel(&di, di.rel, di.rel_sz, &hooks[i]);
			if (ret == 0)
				continue;
		}
	}

	return 0;

out_dlclose:
	dlclose(l);

	return -1;
}
