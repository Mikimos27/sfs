#ifndef KEYGEN_H
#define KEYGEN_H
#include <openssl/rsa.h>
#include <unistd.h>

void genkeypair(){
    int kfd = open("../keys.txt", O_CREAT);

    EVP_PKEY* keys = EVP_RSA_gen(8);
    free(keys);
}


#endif
