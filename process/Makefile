default:
	@echo 'default'
	gcc -Wall -std=c99 env.c -o e.o

cl:
	@echo 'Command Line'
	gcc -Wall -std=c99 cl.c -o cl.o

pc:
	@echo 'Process COntrol'
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE=200809L pc.c display.c -o pc.o
