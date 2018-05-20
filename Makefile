odir = obj/
src = src/
header = header/
objs = $(odir)main.o $(odir)fileIO.o $(odir)cpp2c.o $(odir)class.o $(odir)memberFunction.o
flags = -Wall -std=c11 -I$(header) -g
file = assets/first
prog = cpp2c

all : cpp2c

cpp2c : $(objs)
	gcc $(flags) $(objs) -o $@

$(odir)main.o : $(src)main.c $(header)fileIO.h $(header)cpp2c.h
	gcc $(flags) -c $< -o $@

$(odir)fileIO.o : $(src)fileIO.c $(header)fileIO.h
	gcc $(flags) -c $< -o $@

$(odir)cpp2c.o : $(src)cpp2c.c $(header)cpp2c.h
	gcc $(flags) -c $< -o $@

$(odir)class.o : $(src)class.c $(header)class.h $(header)cpp2c.h
	gcc $(flags) -c $< -o $@

$(odir)memberFunction.o : $(src)memberFunction.c $(header)memberFunction.h $(header)cpp2c.h
	gcc $(flags) -c $< -o $@

run :
	./$(prog) $(file).cpp $(file).c
	gnome-terminal -- vim $(shell pwd)/$(file).c

valgrind :
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(prog) $(file).cpp $(file).c

clean :
	rm $(odir)*.o $(prog) assets/*.c
