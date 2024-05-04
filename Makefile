stock: ./src/*.c ./src/*.h
	$(CC) ./src/*.c -o stock -lraylib -lm -Wall -Wextra -pedantic -g
