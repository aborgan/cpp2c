odir = obj/
src = src/
header = header/
objs = $(odir)main.o $(odir)fileIO.o
flags = -Wall -std=c11 -I$(header)

all : cpp2c

cpp2c : $(objs)
	gcc $(flags) $(objs) -o $@

$(odir)main.o : $(src)main.c
	gcc $(flags) -c $< -o $@

$(odir)fileIO.o : $(src)fileIO.c $(header)fileIO.h
	gcc $(flags) -c $< -o $@

clean :
	rm $(odir)*.o cpp2c
