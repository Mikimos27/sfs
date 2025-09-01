#include <stdint.h>
#include <stddef.h>

int64_t max(int64_t a, int64_t b){
    return a > b ? a : b;
}

int64_t min(int64_t a, int64_t b){
    return a < b ? a : b;
}


void zero_arr(char* c, size_t size){
    for(size_t i = 0; i < size; i++){
        c[0] = 0;
    }
}
