CC := gcc
WFLAG := -Wall
STD := c99

.PHONY: fifo smi_server

fserver:
	@echo 'FIFO server'
	$(CC) $(WFALG) -std=$(STD) fifo_app.c -o fifo_app.o

fclient:
	@echo 'FIFO client'
	$(CC) $(WFALG) -std=$(STD) fifo_app_client.c -o fifo_client.o

fifo: fserver fclient

smi_server:
	@echo 'SMI Server'
	$(CC) $(WFALG) -std=$(STD) smi_server.c -o smi_s.o
