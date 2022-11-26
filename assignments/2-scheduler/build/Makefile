CC 						:=		gcc
CFLAGS 					:= 		-g -Wall -Wextra -Wpedantic

SRC_PATH 				:= 		../src
SRC_FILES 				:= 		$(wildcard $(SRC_PATH)/*.c)

LIBS_PATH 				:= 		../libs

DYNAMIC_LIB_NAME 		:= 		libscheduler.so
DYNAMIC_LIB 			:= 		$(LIBS_PATH)/$(DYNAMIC_LIB_NAME)
DYNAMIC_OBJ_PATH 		:= 		dynamic_obj
DYNAMIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(DYNAMIC_OBJ_PATH)/%.o,$(SRC_FILES))

.PHONY: all clean

all: $(DYNAMIC_LIB) clean

$(DYNAMIC_LIB): $(DYNAMIC_OFILES)
	@$(CC) -shared -o $@ $^

$(DYNAMIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(DYNAMIC_OBJ_PATH)
	@$(CC) -fPIC $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(DYNAMIC_OBJ_PATH)
