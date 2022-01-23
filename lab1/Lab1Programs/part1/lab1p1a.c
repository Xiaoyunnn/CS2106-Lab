#include <stdio.h>

int (*fptr)(int); // pointer to a function that returns an “int”, and takes an int as an argument

int func(int x) {
    return 2 * x;
}


int y = 10;

int *(*pfptr)(); // pointer to a function that returns an int pointer

int *func2() {
    // function that returns an int pointer
    return &y;
}

int main() {
    printf("Calling func with value 6: %d\n", func(6));
    printf("Now setting fptr to point to func.\n");
    fptr = func;
    printf("Caling fptr with value 6: %d\n", fptr(6));

    printf("\nNow caling func2 which returns the address of global variable y: %p\n", func2());
    printf("Pointing pfptr to func2.\n");
    pfptr = func2;
    printf("Now calling fpfptr: %p\n", pfptr());
}



