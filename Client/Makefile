PATHL  = $(shell pwd)

all:
	gcc  -o Server_Example.out Server_Example.c  -L/$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)
	gcc -o Client_Example.out Client_Example.c  -L/$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)

#	gcc -static -o Server_Example.out Server_Example.c  libm3l.a
#	gcc -static -o Client_Example.out Client_Example.c  libm3l.a

