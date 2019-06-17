.SUFFIXES: .o .cpp

CC = g++
VPATH = ../tests
TESTS = test_assert.o test_register32.o test_sc_cpu_loop.o test_sc_cpu_ops.o test_sc_cpu_subroutine.o test_shell_comments.o
MAIN = test_hub.o
INCLUDES = -I"../../include" -I"../include" -I"../common"
CFLAGS = $(INCLUDES)

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
