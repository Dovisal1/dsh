
CFLAGS := -g -Wall

PROGRAM := dsh

all: $(PROGRAM)

$(PROGRAM): dsh.o history.o

dsh.o: history.o history.h

history.o: history.h

clean:
	$(RM) *.o $(PROGRAM)

.PHONY: all clean
