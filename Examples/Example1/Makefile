PATHL  = $(shell pwd )

main:

	ln -sf ../../Source/data_util/libm3l.* .
	gcc  -g -o LinkedList_Example.out LinkedList_Example.c -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL) -lm

clean:
	rm -f *.o
