.SUFFIXES: .o .cpp
CC = g++
VPATH = src:build
BIN_DIR = bin
LIB_DIR = build
CFLAGS = -g -I"$(INCLUDE)"
OBJS = cpuoption.o debug_view_simple.o format_chk.o cpu_time.o interpret.o mb.o mem.o messages.o mips.o mmem.o mtsstream.o \
	pipeline.o primitives.o range.o reg_32.o reg_pipeline.o runtime_call.o sc_cpu.o \
	shell.o streams.o states.o superscalar.o syms_table.o osi.o
SHELL_MAIN = shell_entry.o
INCLUDE = include

all: build/libmtcore.a $(BIN_DIR)/simUI $(BIN_DIR)/pshell

build/libmtcore.a: $(OBJS)
	cd build; ar r libmtcore.a $(OBJS)

$(BIN_DIR)/pshell: $(OBJS) $(SHELL_MAIN) $(INCLUDE)
	@ echo "Looking for bin directory..."
	@ if \
		ls $(BIN_DIR); \
	then \
		echo "bin directory exists"; \
	else \
		echo "bin doesn't exist, making bin..." && \
		mkdir $(BIN_DIR); \
	fi
	cd build; $(CC) $(OBJS) $(SHELL_MAIN) -g -o ../$@
$(BIN_DIR)/simUI: build/libmtcore.a
	@ echo "Building simUI..."
	@ if \
		test $(QMAKE); \
	then \
		cd simUI && \
		$(QMAKE) simUI.pro -r -spec linux-g++ CONFIG+=debug && \
		make && \
		cp simUI ../bin; \
	else \
		echo "Could not find qmake. Please specify the path of the qmake executable by defining macro or env. var QMAKE." && \
		echo "Skipping simUI building..."; \
	fi

.cpp.o:
	$(CC) $(CFLAGS) -c $<
	mv $*.o build 
release:
	make all CFLAGS="-O2 -I\"$(INCLUDE)\" -DP_RELEASE"
clean:
	rm build/*.o
	rm build/*.a
	rm simUI/*.o
	rm bin/*
test: $(BIN_DIR)/pshell
	cd testing/unix_test; $(MAKE) "P_LIB=$(OBJS)"; $(MAKE) run
