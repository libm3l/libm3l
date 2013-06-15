FILES = $(shell ls  *.c )
PATHL  = $(shell pwd )
HFILES = $(shell ls  *.h  |   sed 's/internal_format_type.h//'  | sed 's/Header.h//' | sed 's/format\_type.h//' )
PWD = $(shell pwd)
OBJS=$(FILES:%.c=%.o)

CFLAGS = -0
LFLAGS = 
#
#Autodependencies with GNU make
#this makefile is a modification of makefile from Scott McPeak web page, November 2001
#
main:
	@cat COPYRIGHT_NOTICE > libm3l.h
	@echo >> libm3l.h
	@echo "#ifndef   __LIBM3L_H__" >> libm3l.h
	@echo "#define   __LIBM3L_H__" >> libm3l.h

	@echo "#include " \"$$PWD/Header.h\">>libm3l.h
	@echo "#include " \"$$PWD/format_type.h\">>libm3l.h
	@echo "#include " \"$$PWD/internal_format_type.h\">>libm3l.h

	@$(foreach file,$(HFILES),  echo "#include " \"$$PWD/$(file)\">>libm3l.h;)
	@echo "#endif" >> libm3l.h

	make prog

prog: $(OBJS)


	gcc -shared -Wl,-soname,libm3l.so.1.0 -o libm3l.so.1.0   $(OBJS)
	ln -sf libm3l.so.1.0 libm3l.so

	ar  rcs libm3l.a    $(OBJS)

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
	rm -f *.o *.d
