CC := gcc
default:
	@echo 'default'
	$(CC) -Wall -std=c99 fifo_app.c -o fifo_app.o

fserver:
	@echo 'default'
	$(CC) -Wall -std=c99 fifo_app.c -o fifo_app.o

fclient:
	@echo 'FIFO client'
	$(CC) -Wall -std=c99 fifo_app_client.c -o fifo_client.o

fifo: fserver fclient

smi_server:
	@echo 'SMI Server'
	$(CC) -Wall -std=c99 smi_server.c -o smi_s.o
