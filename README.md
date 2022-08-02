# yoctagrad

A very tiny autodiff library.

Object file clocks in at 1680 bytes, and runtime memory footprint of a `Var` is 28 bytes. The object file compresses down to 667 bytes with `gzip` on my machine.

For comparison: `micrograd`'s `pyc` file is 3712 bytes on my machine, and the size of a `Value` 48 bytes. Its `pyc` file(s) compresses down to 1799 bytes on my machine with `gzip`.
