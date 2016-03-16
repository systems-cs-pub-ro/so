CC=gcc
CFLAGS=-g -Wall
OBJ_PARSER=parser.tab.o parser.yy.o
OBJ=main.o utils-lin.o
TARGET=mini-shell

build: $(TARGET)

$(TARGET): $(OBJ) $(OBJ_PARSER)
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_PARSER) -o $(TARGET)

clean:
	rm -rf $(OBJ) $(OBJ_PARSER) $(TARGET) *~
