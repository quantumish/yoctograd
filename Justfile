build:
	gcc yoctagrad.c -c -o yoctagrad.o -Os -s
	strip --strip-unneeded -R .comment -R .gnu.version yoctagrad.o
	wc -c yoctagrad.o

test:
	gcc test.c yoctagrad.o -o test
	./test
 
