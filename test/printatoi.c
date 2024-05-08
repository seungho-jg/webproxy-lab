#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int res;
    if ( argc != 2){
        fprintf(stderr,"%s : args is 1", argv[0]);
        return 1;
    }
    res = atoi(argv[1]);
    printf("%s -> %d\n", argv[1], res+1);
    return 0;
}