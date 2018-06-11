### SOURCES ###

SRCS := utility.c padre.c logger.c figlio.c nipote.c main.c
OBJS := $(SRCS:.c=.o)
SRCS_THREAD := utility.c padre_t.c logger.c figlio_t.c nipote_t.c main_t.c
OBJS_THREAD := $(SRCS_THREAD:.c=.o)

### CONFIGURATION ###

PROGRAM := key_finder
PROGRAM_THREAD := key_finder_t
CC := gcc
LD := gcc
CFLAGS := -c
LDFLAGS := -o
LDFLAGS_THREAD := -pthread $(LDFLAGS)
DOXYFILE := doxygen.cfg

### INTERMEDIATE TARGETS ###

%.o: ./src/%.c
	@echo Compiling $@...
	$(CC) $(CFLAGS) $(LDFLAGS) $@ $^

$(PROGRAM): $(OBJS)
	@echo Linking $@...
	@$(LD) $(LDFLAGS) $@ $^
	@mkdir -p ./build
	@mv $^ ./build
	@mv $@ ./build

%_t.o: ./src/thread/%_t.c
	@echo Compiling $@...
	$(CC) $(CFLAGS) $(LDFLAGS) $@ $^

$(PROGRAM_THREAD): $(OBJS_THREAD)
	@echo Linking $@...
	@$(LD) $(LDFLAGS_THREAD) $@ $^
	@mkdir -p ./build/thread
	@mv $^ ./build/thread
	@mv $@ ./build/thread

### TARGETS ###

all: $(PROGRAM)

clean:
	@echo Cleaning...
	@rm -fr ./bin/*
	@rm -fr ./build/*

doc:
	@echo Documenting...
	doxygen $(DOXYFILE)

help:
	@echo Targets:
	@echo all
	@echo clean
	@echo doc
	@echo help
	@echo install
	@echo threads

install:
	@echo Installing...
ifneq ($(wildcard ./build/$(PROGRAM)),)
		@mkdir -p ./bin
		@cp ./build/$(PROGRAM) bin
endif
ifneq ($(wildcard ./build/thread/$(PROGRAM_THREAD)),)
		@mkdir -p ./bin/thread
		@cp ./build/thread/$(PROGRAM_THREAD) ./bin/thread
endif

threads: $(PROGRAM_THREAD)

### PHONY TARGETS ###

.PHONY: all clean doc help install threads

