TEXFILES = hw.tex

ifeq (3, $(shell highlight --version | head -2 | tail -1 | rev | cut -d ' ' -f 1 | rev | cut -d '.' -f 1))
	C_HIGHLIGHT_OPTS = -O latex -l -f -t 8 -K tiny -S c -c highlight.sty
	MK_HIGHLIGHT_OPTS = -O latex -l -f -t 8 -K tiny -S Make -c highlight.sty
else
	C_HIGHLIGHT_OPTS = -L -l -f -t 8 -K tiny -S c -c highlight.sty
	MK_HIGHLIGHT_OPTS = -L -l -f -t 8 -K tiny -S Make -c highlight.sty
endif

all: $(TEXFILES)

hw.tex: hw.c
	highlight $(C_HIGHLIGHT_OPTS) -i $< -o $@

.PHONY: clean
clean:
	rm -f $(TEXFILES) highlight.sty *~
