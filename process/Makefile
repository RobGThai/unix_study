default:
	@echo 'default'
	gcc -Wall -std=c99 env.c display.c -o e.o

cl:
	@echo 'Command Line'
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE=200809L cl.c display.c -o cl.o

pc:
	@echo 'Process COntrol'
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE=200809L pc.c display.c -o pc.o

thread:
	@echo 'Thread'
	gcc -Wall -std=c99 -pthread mut.c -o mut.o
