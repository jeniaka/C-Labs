#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: manu_map

# Tool invocations
manu_map: manu_map.o 
	gcc -g -m32 -Wall -o manu_map manu_map.o 

# Depends on the source and header files
manu_map.o: manu_map.c 
	gcc -m32 -g -Wall -c -o manu_map.o  manu_map.c
	

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o manu_map
