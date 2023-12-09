all:
	gcc  -c Common.c MessageHandlers.c -lm
	gcc  -g Client.c Common.c MessageHandlers.c -lpthread -o Client.out -lm
	gcc  -g Machine.c Common.c MessageHandlers.c -lpthread -o Machine.out -lm

clean:
	rm Common.o MessageHandlers.o Client.out Machine.out
