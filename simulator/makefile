CFLG := -g -Wall -Wextra -Wpedantic -Werror
LFLG := -g

NAME := simulator

SRC := circuit chip chip-data net interface keyboard display lexer token error
HDR := $(addprefix hdr/,$(addsuffix .h,$(SRC)))
OBJ := $(addprefix obj/,$(addsuffix .o,$(SRC)))
LIB := -l allegro

$(NAME): % : obj/$(NAME).o $(OBJ)
	gcc $(LFLG) $(LIB) -o $@ $^

obj/$(NAME).o: % : src/$(NAME).c $(HDR)
	gcc -c -I hdr/ $(CFLG) -o $@ $< 

$(OBJ): obj/%.o : src/%.c $(HDR)
	gcc -c -I hdr/ $(CFLG) -o $@ $<