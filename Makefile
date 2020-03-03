CPPFLAGS = -MMD
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c99 -O1

SRC = main.c 
#commands/ls.c commands/pwd.c commands/cd.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

.PHONY: clean

-include ${DEP}

clean: 
	${RM} ${OBJ} ${DEP} main

#END
