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
	head -n 1096 ./data_banknote_authentication.txt > train.txt
	tail -n 274 ./data_banknote_authentication.txt > test.txt
	gcc nn.c yoctograd.o -o nn
	./nn
