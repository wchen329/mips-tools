.SUFFIXES: .o .cpp

CC = g++
VPATH = ../tests:../src:../common
TESTS = test_assert.o \
	test_mips32fsp_bigstallforward.o \
	test_mips32fsp_core_instructions.o \
	test_mips32fsp_forwarding_basic.o \
	test_mips32fsp_forwarding_control.o \
	test_mips32fsp_load_to_use.o \
	test_mips32fsp_load_to_use_control.o \
	test_mips32fsp_memtomem.o \
	test_mips32fsp_nostall.o \
	test_mips32fsp_registers.o \
	test_mips32fsp_subroutine.o \
	test_mips32fsp_storeclose.o \
	test_register32.o \
	test_sc_cpu_loop.o \
	test_sc_cpu_ops.o \
        test_sc_cpu_registers.o \
	test_sc_cpu_subroutine.o \
	test_sc_cpu_core_instructions.o \
	test_sc_cpu_storeclose.o \
	test_shell_comments.o \
	test_shell_multidirectsymbol.o
MAIN = test_hub.o test_register.o basic_assert.o
INCLUDES = -I"../../include" -I"../include" -I"../common"
CFLAGS = $(INCLUDES) -g

unix_test: $(TESTS) $(MAIN)
	cp ../../build/*.o .
	$(CC) $(CFLAGS) $(MAIN) $(TESTS) $(P_LIB) -o ../tests/unix_test

.cpp.o:
	$(CC) $(CFLAGS) -c $<
clean:
	rm *.o
	rm ../tests/unix_test
run:
	cd ../tests; ./unix_test
