compiler=gcc
out=rotate
incl=include
src=src/rotate.c
flags= -Ofast -pedantic -pedantic-errors -Wall -Werror -std=c11 -I include -o $(out) -I $(incl) $(src)

all: $(src)
	$(compiler) $(flags)
clean:
	rm $(out)
