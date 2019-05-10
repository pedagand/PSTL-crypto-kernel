#include "aes_cbc_mb_mgr.h" 
#include<linux/string.h>

#define AES_BLOCK_SIZE 16
#define AES_KEYSIZE_128 16
#define AES_KEYSIZE_192 24
#define AES_KEYSIZE_256 32


void aes_cbc_enc_128_x8(struct aes_cbc_mb_mgr_inorder_x8 *,u64);
void aes_cbc_enc_192_x8(struct aes_cbc_mb_mgr_inorder_x8 *,u64);
void aes_cbc_enc_256_x8(struct aes_cbc_mb_mgr_inorder_x8 *,u64);

void aes_cbc_submit_job_ooo_x8(struct aes_cbc_mb_mgr_inorder_x8 *state,
		struct job_aes_cbc *job, u32 key_len)
{
        u32 lane;
        u8 index;
        u64 min;
        int i;
        lane = state->unused_lanes & 0xF ;
        state->unused_lanes = state->unused_lanes >> 4;
        state->job_in_lane[lane] = job;
        state->lens[lane] = job->len / AES_BLOCK_SIZE ;
        state->args.arg_in[lane] = job->plaintext ;
        state->args.arg_out[lane] = job->ciphertext ;
        state->args.arg_keys[lane] = job->keys ;
        
        /* copy 128-bit IV */ 
        memcpy(&(state->args.arg_iv[lane]),&(job->iv),sizeof(u128));
        
        job->status = STS_BEING_PROCESSED ;

        // not enough jobs 
        if(state->unused_lanes != 0xF){
                return ;
        }
                
        
        // search for min buffer length and its index
        index =0 ; 
        min = state->lens[0]; 
        for(i =1; i<8; i++){
                if(state->lens[i]<min){
                        min = state->lens[i]; 
                        index = i; 

                }
        } 
        // substract min from lens 
        for(i=0; i<8; i++){
                state->lens[i] = state->lens[i] - min; 
        }

        // align stack ??? 
        if(key_len == AES_KEYSIZE_128)
                aes_cbc_enc_128_x8(state, min); 
        else if(key_len == AES_KEYSIZE_192)
                aes_cbc_enc_192_x8(state,min); 
        else if(key_len == AES_KEYSIZE_256)
                aes_cbc_enc_256_x8(state,min); 
        
        // align stack ??? 


        do{
                
                job = state->job_in_lane[index];
                job->status = STS_COMPLETED;
                state->job_in_lane[index] = NULL;
                state->unused_lanes=((state->unused_lanes << 4) | index);

                memcpy(&(job->iv),&(state->args.arg_iv[index]),sizeof(u128));
                state->lens[index]= 0xFFFF;
                index =0 ; 
                min = state->lens[0]; 
                for(i =1; i<8; i++){
                        if(state->lens[i]<min){
                                min = state->lens[i]; 
                                index = i; 
                        }
                }
        } while(min == 0); 
        
}

void aes_cbc_submit_job_ooo_128x8(struct aes_cbc_mb_mgr_inorder_x8 *state,
		struct job_aes_cbc *job)
{
     aes_cbc_submit_job_ooo_x8(state,job,AES_KEYSIZE_128);
}


void aes_cbc_submit_job_ooo_192x8(struct aes_cbc_mb_mgr_inorder_x8 *state,
		struct job_aes_cbc *job)
{
     aes_cbc_submit_job_ooo_x8(state,job,AES_KEYSIZE_192);
}

void aes_cbc_submit_job_ooo_256x8(struct aes_cbc_mb_mgr_inorder_x8 *state,
		struct job_aes_cbc *job)
{
     aes_cbc_submit_job_ooo_x8(state,job,AES_KEYSIZE_256);
}

