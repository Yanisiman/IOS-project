CPPFLAGS = -MMD
CC = gcc -pthread -fsanitize=address
CFLAGS = -g -Wall -Wextra -Werror

SRC = main.c commands/pwd/pwd.c commands/ls/ls.c commands/cd/cd.c commands/less/less.c commands/help/help.c commands/man/man.c commands/rm/rm.c commands/echo/echo.c commands/mv/mv.c commands/cat/cat.c commands/cp/cp.c commands/chmod/chmod.c commands/touch/touch.c commands/makedir/makedir.c commands/stat/stat.c commands/wc/wc.c commands/grep/grep.c commands/tail/tail.c commands/head/head.c commands/find/find.c commands/ln/ln.c commands/parser/parser.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main:	${OBJ}

.PHONY: clean

-include ${DEP}

clean: 
	${RM} ${OBJ} ${DEP} main

#END
