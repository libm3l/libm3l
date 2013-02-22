PATHL  = $(shell pwd )

main:

	ln -sf ../../Source/libm3l.* .
	gcc  -g -o LinkedList_Example.out LinkedList_Example.c -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)

clean:
	rm -f *.o
