# The library libstatistik

Its meant to be used in different kind of applications, even embedded hardware. 
Not really finnished yet, but maybe soon.

## How to build:

make it as a shared library with:

```makefile
cd ./src/
make
```

or try the embedded hardware solution:

```makefile
cd ./test/
make noallocrun
```

## Other, maybe useful information...

In order to use this without the malloc function, then you probably need to modify the ./src/nomalloc.h file.
Also the demo just includes the other library files and wont do any linkage.
This in order to speed up your development.

Btw, feel free to make pull requests. Then just make sure the code looks clean and i will be happy :P.
