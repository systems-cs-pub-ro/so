# user could issue 'make BASENAME=xx' for compiling for a specific lab
BASENAME=02

ifneq (lab, $(findstring lab, $(notdir $(BASENAME))))
	PDF = $(addprefix lab, $(addsuffix -refcard.pdf, $(notdir $(BASENAME))))
	TEX = $(addprefix lab, $(addsuffix -refcard.tex, $(notdir $(BASENAME))))
else
	PDF = $(addsuffix .pdf, $(BASENAME))
	TEX = $(addsuffix .tex, $(BASENAME))
endif

PDFLATEX = pdflatex
OUT_DIR = texfiles
STY_DIR = ../../sty

.PHONY: clean all $(PDF)

all: $(PDF)

$(PDF): $(TEX)
	# Create out directory
	echo $(BB)
	-test -d $(OUT_DIR) || mkdir $(OUT_DIR)
	# Twice, so TOC is also updated
	TEXINPUTS=$(STY_DIR)//: $(PDFLATEX) -output-directory $(OUT_DIR) $<
	TEXINPUTS=$(STY_DIR)//: $(PDFLATEX) -output-directory $(OUT_DIR) $<
	-cp $(OUT_DIR)/$(PDF) .

clean:
	-rm -fr $(OUT_DIR) *.pdf *~
