CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g 

SRC = main.c commands/pwd.c commands/ls.c commands/cd.c commands/less.c commands/help.c commands/rm.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main:	${OBJ}

.PHONY: clean

-include ${DEP}

clean: 
	${RM} ${OBJ} ${DEP} main

#END
