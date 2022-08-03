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
    head ./data_banknote_authentication.txt -n 1096 > train.txt
    tail ./data_banknote_authentication.txt -n 274 > test.txt   
    gcc nn.c yoctograd.o -o nn
    ./nn
