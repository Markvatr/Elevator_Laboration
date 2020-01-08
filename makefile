OBJ = bin/circular_buffer.o  bin/event_list.o bin/passenger_list.o bin/elevator.o
CC = gcc

build: src/elevator_simulator.c $(OBJ)
	$(CC) src/elevator_simulator.c -g -o bin/elevator_simulator.exe -Lbin $(OBJ)

bin/event_list.o: src/event_list.c src/event_list.h
	$(CC) -c src/event_list.c -o bin/event_list.o

bin/passenger_list.o: src/passenger_list.c src/passenger_list.h
	$(CC) -c src/passenger_list.c -o bin/passenger_list.o

bin/elevator.o: src/elevator.c src/elevator.h
	$(CC) -c src/elevator.c -o bin/elevator.o

bin/circular_buffer.o: src/circular_buffer.c src/circular_buffer.h
	$(CC) -c src/circular_buffer.c -o bin/circular_buffer.o 

clean:
	rm -f bin/*