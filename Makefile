.SUFFIXES: .o .cpp
CC = g++
VPATH = src build
BIN_DIR = bin
LIB_DIR = build
CFLAGS = -g -I"$(INCLUDE)" 
OBJS = format_chk.o interpret.o mb.o mem.o messages.o mips.o mmem.o mtsstream.o \
	parser_err.o pipeline.o primitives.o range.o reg_32.o reg_pipeline.o runtime_call.o sc_cpu.o \
	shell.o shell_entry.o states.o syms_table.o
INCLUDE = include

all: $(BIN_DIR)/mtshell
$(BIN_DIR)/mtshell: $(OBJS)
	@ echo "Looking for bin directory..."
	@ if \
		ls $(BIN_DIR); \
	then \
		echo "bin directory exists"; \
	else \
		echo "bin doesn't exist, making bin..." && \
		mkdir $(BIN_DIR); \
	fi
	cd build; $(CC) $(OBJS) -g -o ../$(BIN_DIR)/mtshell
.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv $*.o build 
clean:
	rm build/*.o
	rm bin/*
