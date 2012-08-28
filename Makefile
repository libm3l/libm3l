#FILES = $(shell find ./ -name "*.c" | sed 's/.\///' )
FILES = $(shell ls  *.c )
PATHL  = $(shell pwd )

OBJS=$(FILES:%.c=%.o)

CFLAGS = -fbounds-check -fstack-check -g 
LFLAGS = -fbounds-check -fstack-check -g 
#CFLAGS =
#LFLAGS =

#Autodependencies with GNU make
#Scott McPeak, November 2001 
#
# link
#
main: $(OBJS)
	gcc -shared -Wl,-soname,libm3l.so.1.0 -o libm3l.so.1.0   $(OBJS) 
	ln -sf libm3l.so.1.0 libm3l.so

	set PWD=`pwd`
	cp libm3l.org_h libm3l.h
	sed -i 's:ActualPWD:'$$PWD':' libm3l.h


	gcc -g -o LinkedStr_TEST.out LinkedStr_TEST.c  -L$(PATHL)  -lm3l -Wl,-rpath=$(PATHL)

-include $(OBJS:.o=.d)

%.o: %.c
	gcc -c -g -fPIC $*.c -o $*.o
	gcc -MM -g -fPIC $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
clean:
	rm -f LinkedStr_TEST.out *.o *.d
