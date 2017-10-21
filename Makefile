compiler=clang
out=rotate
incl=include
src=src/rotate.c src/main.c
flags= -Ofast -pedantic -pedantic-errors -lm -Wall -Werror -std=c11 -I include -o $(out) -I $(incl) $(src)

all: $(src)
	$(compiler) $(flags)
clean:
	rm $(out)
