NAME = testBenchAES
SOURCES = bench_sk_csc_aes.c 
OBJS = $(SOURCES:%.c=%.o )
FLAGS= -std=c11 -Wall 
LIBS = -lkcapi

all: $(NAME) testBenchAESAsync

testBenchAESAsync: bench_ac_ecb_aes.c
	gcc -o $@ $^ $(LIBS)

$(NAME): $(OBJS) 
	gcc  -o $@ $^ $(LIBS)

%.o: %.c
	gcc -c $(FLAGS) $<

clean:
	rm -f $(NAME) $(OBJS) testBenchAESAsync
