# user flags
INCL_FLG := -I hdr/ -include stddef.h -include stdint.h -include stdbool.h
DBUG_FLG := -g -Og
PROF_FLG := -g -pg
OPTM_FLG := -O3
WARN_FLG := -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter

# compile mode
MODE := $(DBUG_FLG)

# user files and libraries
NAME := assembler
SRC := arguments lexer matcher parser linker token pattern symbol object
LIB :=

HDR  := $(addprefix hdr/,$(addsuffix .h,$(SRC)))
OBJ  := $(addprefix obj/,$(addsuffix .o,$(SRC)))
LINK := $(addprefix -l ,$(LIB))

$(NAME): % : obj/$(NAME).o $(OBJ)
	gcc $(MODE) -o $@ $^ $(LINK)

obj/$(NAME).o: % : src/$(NAME).c $(HDR)
	gcc -c $(INCL_FLG) $(MODE) $(WARN_FLG) -o $@ $<

$(OBJ): obj/%.o : src/%.c $(HDR)
	gcc -c $(INCL_FLG) $(MODE) $(WARN_FLG) -o $@ $<