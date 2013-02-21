#FILES = $(shell find ./ -name "*.c" | sed 's/.\///' )
FILES = $(shell ls  *.c )
PATHL  = $(shell pwd )

OBJS=$(FILES:%.c=%.o)

CFLAGS = 
LFLAGS = 


#Autodependencies with GNU make
#Scott McPeak, November 2001 
#
# link
#
main: $(OBJS)

	gcc -shared -Wl,-soname,libm3l.so.1.0 -o libm3l.so.1.0   $(OBJS) 
	ln -sf libm3l.so.1.0 libm3l.so

-include $(OBJS:.o=.d)

%.o: %.c
	gcc -c -O -fPIC $*.c -o $*.o
	gcc -MM -O -fPIC $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp
clean:
	rm -f *.o *.d
Hfile:
	set PWD=`pwd`
	cp libm3l.org_h libm3l.h
	sed -i 's:ActualPWD:'$$PWD':' libm3l.h
