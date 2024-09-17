all: build 

build:
	gcc -Wall -o wcc src/init.c

clean:
	rm ./wcc
