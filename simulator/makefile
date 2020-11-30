CFLG := -g -Wall -Wextra -Wpedantic -Werror
LFLG := -g

NAME := simulator

SRC := circuit chip net interface keyboard display error
DAT := chip-data
HDR := $(addprefix hdr/,$(addsuffix .h,$(SRC))) $(addprefix hdr/,$(addsuffix .h,$(DAT)))
OBJ := $(addprefix obj/,$(addsuffix .o,$(SRC)))
LIB := -l allegro

$(NAME): % : obj/$(NAME).o $(OBJ)
	gcc $(LFLG) $(LIB) -o $@ $^

obj/$(NAME).o: % : src/$(NAME).c $(HDR)
	gcc -c -I hdr/ $(CFLG) -o $@ $< 

$(OBJ): obj/%.o : src/%.c $(HDR)
	gcc -c -I hdr/ $(CFLG) -o $@ $<