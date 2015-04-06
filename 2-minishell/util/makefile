OBJ_LIST = parser.tab.obj parser.yy.obj
CPPFLAGS = /nologo /W3 /EHsc
CFLAGS   = /nologo /W3 /EHsc /Za

EXE_NAMES = CUseParser.exe UseParser.exe DisplayStructure.exe

all : $(EXE_NAMES)

CUseParser.exe : CUseParser.obj $(OBJ_LIST)
  $(CPP) $(CPPFLAGS) /Fe$@ $**

UseParser.exe : UseParser.obj $(OBJ_LIST)
  $(CPP) $(CPPFLAGS) /Fe$@ $**

DisplayStructure.exe : DisplayStructure.obj $(OBJ_LIST)
  $(CPP) $(CPPFLAGS) /Fe$@ $**

clean : exe_clean obj_clean

obj_clean :
	del /Q /S *.obj *~

exe_clean :
	del /Q /S $(EXE_NAMES)
