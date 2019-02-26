NAME = testBenchAES
SOURCES = bench_sk_csc_aes.c 
OBJS = $(SOURCES:%.c=%.o )
FLAGS= -std=c11 -Wall 
LIBS = -lkcapi

all: $(NAME)

$(NAME): $(OBJS) 
	gcc $(LIBS) -o $@ $^ 

%.o: %.c
	gcc -c $(FLAGS) $<

clean:
	rm -f $(NAME) $(OBJS)