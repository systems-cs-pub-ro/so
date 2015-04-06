# for details on makefile syntax
# see www.delorie.com/gnu/docs/make/make.html


# the platform make runs on
# windows  == run cl
# the rest == run gcc / g++

PLATFORM = linux
#PLATFORM = cygwin
#PLATFORM = mingw
#PLATFORM = djgpp
#PLATFORM = windows




#set up some generic variables

ifeq ($(PLATFORM),linux)
  EXE_EXT =
else
  EXE_EXT = .exe
endif

ifeq ($(PLATFORM),windows)
  OBJ_EXT = .obj
else
  OBJ_EXT = .o
endif

ifeq ($(PLATFORM),windows)
  USE_COMPILER ?= cl
else
  USE_COMPILER ?= gcc
endif

NULL  := 
SPACE := $(NULL) $(NULL)

H_EXT      = .h
C_EXT      = .c
CPP_EXT    = .cpp
LEX_EXT    = .l
YACC_EXT   = .y
YACC_H_EXT = .h


LINKER_FLAGS = $(LINKER_OPTIONS)
C_FLAGS      = $(C_OPTIONS)
CPP_FLAGS    = $(CPP_OPTIONS)
LEX_FLAGS    = $(LEX_OPTIONS)
YACC_FLAGS   = $(YACC_OPTIONS)

ifeq ($(USE_COMPILER),cl)

  LINKER       = cl
  C_COMPILER   = cl
  CPP_COMPILER = cl
  LINKER_OPTIONS += /nologo
  C_OPTIONS      += /nologo
  CPP_OPTIONS    += /nologo
  LINKER_O_FLAG  = /Fe
  COMPILE_AS_CPP = /TP

endif

ifeq ($(USE_COMPILER),gcc)
  ifeq ($(PLATFORM),djgpp)

    LINKER       = gpp
    C_COMPILER   = gcc
    CPP_COMPILER = gpp

  else

    LINKER       = g++
    C_COMPILER   = gcc
    CPP_COMPILER = g++

  endif

  LINKER_O_FLAG  = -o$(SPACE)
  COMPILE_AS_CPP = -x c++

endif

YACC_COMPILER = bison
LEX_COMPILER  = flex
LEX_O_FLAG = -o

# these variables hold the name (without extension) for all '.cpp' and '.c' files
ALL_CPP_FILES_IN_DIR = $(basename $(wildcard ./*$(CPP_EXT)))
ALL_C_FILES_IN_DIR   = $(basename $(wildcard ./*$(C_EXT)))

# done setting up generic variables








# set up specific options

C_FILES        = CUseParser
CPP_FILES      = UseParser DisplayStructure
YACC_LEX_FILES = parser
#USE_UNICODE_FOR_WINDOWS = true
#BUILD_LEX_YACC = true
BUILD_LEX_YACC = false
#PARSER_AS_CPP = true

ifeq ($(USE_COMPILER),cl)

  C_OPTIONS   += /W3 /EHsc /Za
  CPP_OPTIONS += /W3 /EHsc
  PRINT_SEPARATOR_LINES = true
  ifeq ($(USE_UNICODE_FOR_WINDOWS),true)
    C_OPTIONS   += /DUNICODE=
    CPP_OPTIONS += /DUNICODE=
  endif

else

  C_OPTIONS   += -Wall -W
  CPP_OPTIONS += -Wall -W

endif








# some "generic" rules

all : build

ifeq ($(PRINT_SEPARATOR_LINES),true)
  LINE_CMD = echo
endif

YACC_INPUT_SOURCES  = $(addsuffix $(YACC_EXT), $(YACC_LEX_FILES))
YACC_OUTPUT_FILES   = $(addsuffix .tab,        $(YACC_LEX_FILES))
YACC_OUTPUT_SOURCES = $(addsuffix $(C_EXT),    $(YACC_OUTPUT_FILES))
YACC_OBJ            = $(addsuffix $(OBJ_EXT),  $(YACC_OUTPUT_FILES))

LEX_INPUT_SOURCES   = $(addsuffix $(LEX_EXT),  $(YACC_LEX_FILES))
LEX_OUTPUT_FILES    = $(addsuffix .yy,         $(YACC_LEX_FILES))
LEX_OUTPUT_SOURCES  = $(addsuffix $(C_EXT),    $(LEX_OUTPUT_FILES))
LEX_OBJ             = $(addsuffix $(OBJ_EXT),  $(LEX_OUTPUT_FILES))

CPP_SOURCES = $(addsuffix $(CPP_EXT), $(CPP_FILES))
CPP_OBJ     = $(addsuffix $(OBJ_EXT), $(CPP_FILES))

C_SOURCES   = $(addsuffix $(C_EXT),   $(C_FILES))
C_OBJ       = $(addsuffix $(OBJ_EXT), $(C_FILES))


ifeq ($(PARSER_AS_CPP),true)

  CPP_OBJ_LIST   = $(CPP_OBJ)
  C_OBJ_LIST     = $(C_OBJ)
  CPP_C_OBJ_LIST = $(YACC_OBJ) $(LEX_OBJ)

else

  CPP_OBJ_LIST = $(CPP_OBJ)
  C_OBJ_LIST   = $(C_OBJ) $(YACC_OBJ) $(LEX_OBJ)

endif


EXE_NAMES    = \
  $(addsuffix $(EXE_EXT), $(CPP_FILES))\
  $(addsuffix $(EXE_EXT), $(C_FILES))

.PHONY : all build build_yacc build_lex build_exe

ifeq ($(BUILD_LEX_YACC),true)
  build : pre_build build_yacc build_lex build_exe post_build
else
  build : pre_build build_exe post_build
endif

pre_build  : 

post_build : 

build_yacc : $(YACC_OUTPUT_SOURCES)

build_lex  : $(LEX_OUTPUT_SOURCES)

build_exe  : $(EXE_NAMES)

build_lex  : build_yacc

$(EXE_NAMES) : %$(EXE_EXT) : %$(OBJ_EXT) $(YACC_OBJ) $(LEX_OBJ)
	@$(LINE_CMD)
	$(LINKER) $(LINKER_FLAGS) $(LINKER_O_FLAG)$@ $^

ifneq ($(DONT_BUILD_LEX_YACC),true)

$(LEX_OBJ) : %.yy$(OBJ_EXT) : %.tab$(YACC_H_EXT)

$(YACC_OUTPUT_SOURCES) : %.tab$(C_EXT) : %$(YACC_EXT)
	@$(LINE_CMD)
	$(YACC_COMPILER) $(YACC_FLAGS) $^

$(LEX_OUTPUT_SOURCES) : %.yy$(C_EXT) : %$(LEX_EXT)
	@$(LINE_CMD)
	$(LEX_COMPILER) $(LEX_O_FLAG)$@ $^

endif

$(CPP_OBJ_LIST) $(C_OBJ_LIST) $(CPP_C_OBJ_LIST) : $(addsuffix $(H_EXT), $(YACC_LEX_FILES))

$(CPP_OBJ_LIST) : %$(OBJ_EXT) : %$(CPP_EXT)
	@$(LINE_CMD)
	$(CPP_COMPILER) $(CPP_FLAGS) -c $(filter-out %.tab$(YACC_H_EXT),$(filter-out %$(H_EXT),$^))

$(C_OBJ_LIST) : %$(OBJ_EXT) : %$(C_EXT)
	@$(LINE_CMD)
	$(C_COMPILER) $(C_FLAGS) -c $(filter-out %.tab$(YACC_H_EXT),$(filter-out %$(H_EXT),$^))

$(CPP_C_OBJ_LIST) : %$(OBJ_EXT) : %$(C_EXT)
	@$(LINE_CMD)
	$(CPP_COMPILER) $(COMPILE_AS_CPP) $(CPP_FLAGS) -c $(filter-out %.tab$(YACC_H_EXT),$(filter-out %$(H_EXT),$^))

.PHONY : clean junk_clean exe_clean obj_clean

clean : junk_clean exe_clean
clean_recompile : exe_clean obj_clean

exe_clean :
	rm -f $(EXE_NAMES) *.stackdump

junk_clean : obj_clean
ifeq ($(BUILD_LEX_YACC),true)
	rm -f $(LEX_OUTPUT_SOURCES) $(YACC_OUTPUT_SOURCES) $(addsuffix $(YACC_H_EXT), $(YACC_OUTPUT_FILES))
endif
	rm -f *~

obj_clean :
	rm -f *$(OBJ_EXT)

zip: clean
	zip -r ../minishell_parser.zip *
