# yoctograd

A very tiny autodiff library. It supports addition, and multiplication, and ReLU.

It's not much, but it's all you need for a simple neural network!

Object file clocks in at 1680 bytes, and runtime memory footprint of a `Var` is 28 bytes. The object file compresses down to 667 bytes with `gzip` on my machine. Source code is only 38 lines long! The object file size is obviously very platform-specific as it depends on how spacious the object file format is. On an M1 Mac I've even seen it as low as ~600 bytes uncompressed.

## building
`yoctograd` uses the build tool `Just` for compilation.

You can build the library and see the size using `just build`.

```
$ just build
gcc yoctograd.c -c -o yoctograd.o -Os -s
strip --strip-unneeded -R .comment -R .gnu.version yoctograd.o
wc -c yoctograd.o
1680 yoctograd.o
```

Running `just` will build and test.

## usage

You can see an extremely simple demo in `test.c`, but here's a condensed version:

```C
#include "yoctograd.h"

int main() {
    Var* a = v_const(-4);
    Var* b = v_const(2);
    Var* c = v_mul(a, b);
    Var* d = v_add(v_mul(c, a), v_const(2));
    d->grad = 1; // unfortunately manual for now
    v_back(d);
}
```

For a beefier example, check out `nn.c` and run `just nn` after building!
```
$ just nn
...
epoch 91/100: avg train loss of 0.021786, avg val loss of 0.088175
epoch 92/100: avg train loss of 0.021793, avg val loss of 0.085264
epoch 93/100: avg train loss of 0.021709, avg val loss of 0.088108
epoch 94/100: avg train loss of 0.021665, avg val loss of 0.087116
epoch 95/100: avg train loss of 0.021636, avg val loss of 0.086510
epoch 96/100: avg train loss of 0.021531, avg val loss of 0.085612
epoch 97/100: avg train loss of 0.021738, avg val loss of 0.084265
epoch 98/100: avg train loss of 0.021440, avg val loss of 0.085571
epoch 99/100: avg train loss of 0.021322, avg val loss of 0.083980
epoch 100/100: avg train loss of 0.021538, avg val loss of 0.084937
```

Or try out the tinier example shown originally (with debug printouts) yourself with `just test`!
```
$ just test
gcc test.c yoctograd.o -o test
./test
Var sizeof: 28
a grad: -16.000000
b grad: 16.000000
c grad: -4.000000
d grad: 1.000000
```
