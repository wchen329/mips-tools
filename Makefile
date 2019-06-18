.SUFFIXES: .o .cpp
CC = g++
VPATH = src build
BIN_DIR = bin
LIB_DIR = build
CFLAGS = -g -I"$(INCLUDE)" $(RELEASE)
OBJS = format_chk.o cpu_time.o interpret.o mb.o mem.o messages.o mips.o mmem.o mtsstream.o \
	parser_err.o pipeline.o primitives.o range.o reg_32.o reg_pipeline.o runtime_call.o sc_cpu.o \
	shell.o states.o syms_table.o
SHELL_MAIN = shell_entry.o
INCLUDE = include

all: $(BIN_DIR)/mtshell
$(BIN_DIR)/mtshell: $(OBJS) $(SHELL_MAIN)
	@ echo "Looking for bin directory..."
	@ if \
		ls $(BIN_DIR); \
	then \
		echo "bin directory exists"; \
	else \
		echo "bin doesn't exist, making bin..." && \
		mkdir $(BIN_DIR); \
	fi
	cd build; $(CC) $(OBJS) $(SHELL_MAIN) -g -o ../$(BIN_DIR)/mtshell
.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv $*.o build 
clean:
	rm build/*.o
	rm bin/*
test: $(BIN_DIR)/mtshell
	cd testing/unix_test; $(MAKE) "P_LIB=$(OBJS)"; $(MAKE) run
