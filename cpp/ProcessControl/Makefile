all: process_control

clean:
	rm -f process_control process_control.o
	
process_control: process_control.o
	gcc -g -o process_control process_control.o

process_control.o: process_control.c
	gcc -c -g process_control.c
