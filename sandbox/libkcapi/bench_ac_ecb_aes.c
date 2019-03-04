#include <kcapi.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEST_COUNT 5
#define SECONDS 1

int test=0;

int test_async(uint8_t key_length,const char* ciphername,const char* file_name){

    //init handle
    struct kcapi_handle *handle1;
    if(kcapi_cipher_init(&handle1,ciphername,KCAPI_INIT_AIO)){
        perror("kcapi_cipher_init ");
        return 1;
    }

    //init key
    uint8_t* key= ( uint8_t *)malloc(key_length  * sizeof(uint8_t));
    if(!key){
        perror("malloc key");
        kcapi_cipher_destroy(handle1);
        return 1;
    }
    if((kcapi_rng_get_bytes(key, (uint32_t) key_length)) != key_length ){
        perror("kcapi_rng_get_bytes") ;
        kcapi_cipher_destroy(handle1);
        return 1;
    }
    if(kcapi_cipher_setkey(handle1,key,key_length)){
        perror("kcapi_cipher_setkey") ;
        kcapi_cipher_destroy(handle1);
        return 1;
    }
    free(key);

    //init input iovecs
    struct iovec input16={NULL,16},input64={NULL,64},input256={NULL,256},input1024={NULL,1024},input8192={NULL,8192};
    struct iovec inIOVecs[TEST_COUNT]={input16,input64,input256,input1024,input8192};
    for(int i=0;i<TEST_COUNT;++i){
        inIOVecs[i].iov_base=malloc(inIOVecs[i].iov_len*sizeof(uint8_t));
        if(!inIOVecs[i].iov_base){
            for(int j=0;j<i;++j)
                free(inIOVecs[j].iov_base);
            kcapi_cipher_destroy(handle1);
            return 1;
        }
    }

    //init iv
    uint32_t ivsize= kcapi_cipher_ivsize(handle1);
    uint8_t iv [ivsize];
    if((kcapi_rng_get_bytes((uint8_t*)&iv, (uint32_t) ivsize))  != ivsize ){
        perror("generating IV not successful") ;
        kcapi_cipher_destroy(handle1);
        return 1;
    }

    for(int i = 0 ; i< TEST_COUNT; i++){
        //init iovecs
        if((kcapi_rng_get_bytes((uint8_t*)(inIOVecs[i].iov_base), (uint32_t) inIOVecs[i].iov_len))  != inIOVecs[i].iov_len ){
            perror("generating input iovec failed ");
            kcapi_cipher_destroy(handle1);
            return 1;
        }
        clock_t start,finish,now;

        long ciphered,total_ciphered=0;
        long operations=0;
        start=clock();
        now=clock();
        finish=start+SECONDS*CLOCKS_PER_SEC;
        while ((finish-now)>0){
            ciphered = kcapi_cipher_encrypt_aio(handle1,&inIOVecs[i],&inIOVecs[i],1,iv,KCAPI_ACCESS_HEURISTIC);
            if(ciphered!= inIOVecs[i].iov_len){
                perror("kcapi_cipher_encrypt_aio");
                printf("cipher failed ! expected %zu ciphered= %ld\n", inIOVecs[i].iov_len,ciphered);
                kcapi_cipher_destroy(handle1);
                return 1;
            }
            total_ciphered+=ciphered;
            operations++;
            now=clock();
        }
        printf("test %d (%d bit key, %zu byte blocks): %li operations in %d seconds (%li bytes)\n",test,key_length*8,inIOVecs[i].iov_len,operations, SECONDS,total_ciphered);
        test++;

    }

    for(int i=0;i<TEST_COUNT;++i){
        free(inIOVecs[i].iov_base);
    }

    kcapi_cipher_destroy(handle1);
    return 0;


}



int main(int argc, char const *argv[])
{
    test_async(16,"ecb(aes)","benchmark_AES_async");
    test_async(24,"ecb(aes)","benchmark_AES_async");
    test_async(32,"ecb(aes)","benchmark_AES_async");

    return 0;
}