
CFLAGS:=-g -Wall
PROGRAM:=dsh

OBJ=dsh.o\
    history.o\
    cmd.o\
    builtin.o\

all: $(PROGRAM)

$(PROGRAM): $(OBJ)

clean:
	$(RM) *.o $(PROGRAM)

.PHONY: all clean
