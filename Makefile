CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRC = main.c commands/pwd/pwd.c commands/ls/ls.c commands/cd/cd.c commands/less/less.c commands/help/help.c commands/man/man.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main:	${OBJ}

.PHONY: clean

-include ${DEP}

clean: 
	${RM} ${OBJ} ${DEP} main

#END
