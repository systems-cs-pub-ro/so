# user could issue 'make BASENAME=xx' for compiling for a specific lab
BASENAME = 11

ifneq (lab, $(findstring lab, $(notdir $(BASENAME))))
	PDF = $(addprefix lab, $(addsuffix -slides.pdf, $(notdir $(BASENAME))))
	TEX = $(addprefix lab, $(addsuffix -slides.tex, $(notdir $(BASENAME))))
else
	PDF = $(addsuffix .pdf, $(BASENAME))
	TEX = $(addsuffix .tex, $(BASENAME))
endif


HIGHLIGHT = tex/code
PDFLATEX = pdflatex
OUT_DIR = texfiles
STY_DIR = ../../sty
CODE_DIR = code

.PHONY: clean all $(PDF)

all: $(PDF)

$(PDF): $(TEX)
	# Create out directory
	test -d $(OUT_DIR) || mkdir $(OUT_DIR)
	# Twice, so TOC is also updated
	TEXINPUTS=$(STY_DIR)//: $(PDFLATEX) -output-directory $(OUT_DIR) $<
	TEXINPUTS=$(STY_DIR)//: $(PDFLATEX) -output-directory $(OUT_DIR) $<
	cp $(OUT_DIR)/$(PDF) .

clean:
	rm -fr $(OUT_DIR)/* $(OUT_DIR) $(CODE_DIR)/hw.tex $(CODE_DIR)/highlight.sty *.pdf *~
