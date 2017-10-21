compiler=clang
out=rotate.o
incl=include
src=src/image.c src/main.c src/bmp.c
flags= -Ofast -pedantic -pedantic-errors -Wall -Werror -std=gnu11 -I include -o $(out) -lm -I $(incl)

all: $(src)
	$(compiler) $(flags) $(src)
clean:
	rm $(out)
