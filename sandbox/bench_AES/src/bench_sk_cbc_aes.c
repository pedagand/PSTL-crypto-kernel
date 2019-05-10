#include <kcapi.h>
#include <time.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <alloca.h>



#define IV_SIZE 16
#define SECONDS 1

static uint8_t key_sizes []={16,24,32,0}; 
static uint32_t block_sizes[]={16,64,256,1024,8192,0}; 

int main(int argc, char const *argv[])
{   
    clock_t start, end ; 
    int nb_operations ; 
    int ciphered ; 
    int i = 0 ; 
    uint8_t* key_len= key_sizes; 

    while(*key_len){

        uint8_t * key = alloca(sizeof(*key_len));   
        if(!key){
            perror("allocation key failed !"); 
            goto out ; 
        }

        if((kcapi_rng_get_bytes(key, (uint32_t)*key_len) != (uint32_t)*key_len)){
            perror("generating KEY not successful");
            goto out ; 
        }

        uint32_t * bsize = block_sizes; 
        while(*bsize){
            uint32_t inputlen = *bsize ; 
            uint8_t * input= alloca(inputlen *  sizeof(uint8_t)) ; 

            if(!input){
                perror("allocatio input failed"); 
                goto out; 
            }

            uint32_t outputlen = inputlen; 
            uint8_t * output = alloca(outputlen * sizeof(uint8_t)) ; 
            if(!output){
                perror("allocatio output failed"); 
                goto out; 
            }

            uint8_t * iv = alloca(IV_SIZE); 
            if(!iv){
                perror("allocation IV failed"); 
            } 


            if(kcapi_rng_get_bytes(input, inputlen) !=inputlen){
                perror("generating random DATA not successful"); 
                goto out; 
            }
            if((kcapi_rng_get_bytes(iv, (uint32_t) IV_SIZE))  != IV_SIZE ) {
                perror("generating IV not successful") ; 
                goto out; 
            }

            ciphered = 0 ; 
            nb_operations = 0 ; 
            int total  = 0 ; 

            start  = clock(); 
            end = start + SECONDS * CLOCKS_PER_SEC ; 
            clock_t now = clock(); 
            while( (end - now ) > 0  ){
                ciphered = kcapi_cipher_enc_aes_cbc(key, (uint32_t ) *key_len, input, inputlen, iv, output, outputlen); 
                    if (ciphered != inputlen){
                        perror("ciphered error ") ; 
                        goto out; 
                    }
                total += ciphered ; 
                nb_operations ++ ; 
                now = clock(); 


            }

            if(total != inputlen * nb_operations){
                perror("error cipher operation"); 
                goto out; 
            }

            printf("test %d (%d bit key, %d byte blocks:) %d operations in %d SECONDS (%d bytes)\n", i, (*key_len) * 8 , *bsize, nb_operations, SECONDS, total) ;

            i++; 
            bsize++ ; 

        }


        key_len++ ; 
    }


    return 0 ; 

    out: 
        puts("exiting program with error"); 
        exit(1); 

}
