.SUFFIXES: .o .cpp

CC = g++
VPATH = ../tests:../src:../common
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
