LDLIBS = -lreadline -L/lib/x86_64-linux-gnu  
LCURSES = -lcurses -L/lib/x86_64-linux-gnu
objects = main.o calculator.o
calculate : $(objects)
	gcc -o calculate $(objects) $(LDLIBS) $(LCURSES)

main.o:main.c calculator.h
	gcc -c main.c
calculator.o:calculator.c
	gcc -c calculator.c


.PHONY:clean
clean:
	rm -fr *.o 
