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

//int addr_validation(const char* str){
//    int block_counts[4] = {0};
//    int dot_num = 0;
//    int addrlen = strlen(str);
//
//    for(int i = 0; i < addrlen; i++){
//        if(str[i] == '.') { 
//            dot_num++;
//            continue;
//        }
//        if(str[0] < '0' || str[i] > '9') return 1;
//        if(dot_num > 3) return 1;
//        block_counts[dot_num]++;
//    }
//
//    for(int i = 0; i < 4; i++){
//        if(block_counts[i] == 0 || block_counts[i] > 3) return 1;
//    }
//    if(dot_num > 3) return 1;
//    return 0;
//}
