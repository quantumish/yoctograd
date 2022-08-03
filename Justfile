strip_flags := if os() == "macos" { "-SXNx"  } else { "--strip-unneeded -R .comment" }

default: build test

build:
	gcc yoctograd.c -c -o yoctograd.o -Os
	strip {{ strip_flags }} yoctograd.o
	wc -c yoctograd.o

test:
	gcc test.c yoctograd.o -o test
	./test
 
nn:
	bash -c 'split -l $[ $(wc -l "./data_banknote_authentication.txt"|cut -d" " -f1) * 80 / 100 ] "./data_banknote_authentication.txt"'
	mv xaa train.txt
	mv xab test.txt
	gcc nn.c yoctograd.o -o nn
	./nn
