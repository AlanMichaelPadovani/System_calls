PROJECT := key_finder
OBJ := main.o figlio.o logger.o nipote.o padre.o
LD := gcc
#CFLAGS :=

%.o: ./include/%.c
	@echo Compiling $< -> $@
	$(CC) $(CFLAGS) -c $< -o $@

key_finder: $(OBJ)
	@echo Linking key_finder
	$(LD) $(LDFLAGS) $(OBJ) -o key_finder


all: key_finder

install:
	cp key_finder ./bin/

clean:
	@echo Cleaning
	rm *.o my_exec

.PHONY: all install clean
