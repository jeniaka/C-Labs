
# All Targets
all: task0

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task0: task0.o 
	gcc -g -m32 -Wall -o task0 task0.o 

# Depends on the source and header files
task0.o: task3.c 
	gcc -m32 -g -Wall -c -o task0.o task3.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task0
