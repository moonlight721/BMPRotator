compiler=gcc
out=program
incl=include
src=src/
flags= -Ofast -pedantic -pedantic-errors -Wall -Werror -std=c11 -I include -o $(out) -I $(incl) $(src)

all: $(src)
	$(compiler) $(flags)
clean:
	rm $(out)
