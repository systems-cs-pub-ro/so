CC=cl
CFLAGS=/D_CRT_SECURE_NO_DEPRECATE /W3 /nologo 
OBJ_PARSER=parser.tab.obj parser.yy.obj
OBJ=main.obj utils-win.obj
TARGET=mini-shell.exe

build: $(TARGET)

$(TARGET): $(OBJ) $(OBJ_PARSER)
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_PARSER) /Fe$(TARGET)

clean:
	del $(OBJ) $(OBJ_PARSER) $(TARGET) *~
