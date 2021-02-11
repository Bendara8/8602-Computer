# user flags
INCL_FLG := -I hdr/ -include stddef.h -include stdint.h -include stdbool.h
DBUG_FLG := -g -Og
WARN_FLG := -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
LINK_FLG := 

# user files and libraries
NAME := emulator
SRC := arguments interface command terminal input draw display keyboard interrupt cpu alu control memory
LIB := ncurses allegro allegro_primitives

HDR  := $(addprefix hdr/,$(addsuffix .h,$(SRC)))
OBJ  := $(addprefix obj/,$(addsuffix .o,$(SRC)))
LINK := $(addprefix -l ,$(LIB))

$(NAME): % : obj/$(NAME).o $(OBJ)
	gcc $(LINK_FLG) -o $@ $^ $(LINK)

obj/$(NAME).o: % : src/$(NAME).c $(HDR)
	gcc -c $(INCL_FLG) $(DBUG_FLG) $(WARN_FLG) -o $@ $< 

$(OBJ): obj/%.o : src/%.c $(HDR)
	gcc -c $(INCL_FLG) $(DBUG_FLG) $(WARN_FLG) -o $@ $<