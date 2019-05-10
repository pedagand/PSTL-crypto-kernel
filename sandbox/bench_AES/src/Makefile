FLAGS= -std=c11 -Wall 
LIBS = -lkcapi

all: bench_sk_cbc_aes bench_ak_ecb_aes

bench_sk_cbc_aes: bench_sk_cbc_aes.c 
	gcc $(FLAGS) -o $@ $^ $(LIBS)

bench_ak_ecb_aes: bench_ak_ecb_aes.c
	gcc $(FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(NAME) bench_ak_ecb_aes bench_sk_cbc_aes
