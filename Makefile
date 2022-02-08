wordle: wordle.c
	gcc -o wordle wordle.c && ./wordle

clean:
	rm wordle
