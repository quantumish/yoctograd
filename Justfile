build:
	gcc yoctograd.c -c -o yoctograd.o -Os -s
	strip --strip-unneeded -R .comment -R .gnu.version yoctograd.o
	wc -c yoctograd.o

test:
	gcc test.c yoctograd.o -o test
	./test
 
