# yoctagrad

A very tiny autodiff library. It supports addition, and multiplication, and ReLU.

Object file clocks in at 1680 bytes, and runtime memory footprint of a `Var` is 28 bytes. The object file compresses down to 667 bytes with `gzip` on my machine. Source code is only 38 lines long!

For comparison: [micrograd](https://github.com/karpathy/micrograd)'s `pyc` file is 3712 bytes on my machine, and the size of a `Value` 48 bytes. Its `pyc` file(s) compresses down to 1799 bytes on my machine with `gzip`.

## building 
You can build the library and see the size using `just build`.

```
$ just build
gcc yoctagrad.c -c -o yoctagrad.o -Os -s
strip --strip-unneeded -R .comment -R .gnu.version yoctagrad.o
wc -c yoctagrad.o
1680 yoctagrad.o
```

## usage 

You can see an extremely simple demo in `test.c`, but here's a condensed version:

```C
#include "yoctagrad.h"

int main() {
    Var* a = v_const(-4);
    Var* b = v_const(2);
    Var* c = v_mul(a, b);
    Var* d = v_add(v_mul(c, a), v_const(2));
    d->grad = 1; // unfortunately manual for now
    v_back(d);
}
```

Try out the full example (with debug printouts) yourself with `just test`!
```
$ just test
gcc test.c yoctagrad.o -o test
./test
Var sizeof: 28
a grad: -16.000000
b grad: 16.000000
c grad: -4.000000
d grad: 1.000000
```
