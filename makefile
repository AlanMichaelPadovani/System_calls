PROGRAM = key_finder
OBJ := main.o figlio.o logger.o nipote.o padre.o utility.o
LD := gcc
#CFLAGS :=
DOXYFILE = doxygen.cfg

%.o: ./include/%.c
	@echo Compiling... $< -> $@
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJ)
	@echo Linking $PROGRAM...
	$(LD) $(LDFLAGS) $(OBJ) -o $PROGRAM

all: $PROGRAM

clean:
	@echo Cleaning...
	@$(RM) *.o $PROGRAM

doc:
	@echo Documenting...
	doxygen $(DOXYFILE)
	
install:
	@echo Installing...
	@mkdir -p bin
	mv $(PROGRAM) ./bin

threads:
	@echo TODO

.PHONY: help

help: makefile
	@sed -n $<

