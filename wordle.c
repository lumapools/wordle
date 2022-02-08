#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int containsSpecial(char* str) {
    size_t len = strlen(str);
    for(size_t i = 0; i < len; i++) {
        if(str[i] < 97 || str[i] > 126) {
            return 1;
        }
    }
    return 0;
}

char* getRandomWord(void) {
    system("cat /usr/share/dict/words > words.txt");
    FILE* allwords = fopen("words.txt", "r");
    FILE* newwords = fopen("wordles.txt", "w");
    char* line = malloc(100);
    size_t read;
    size_t len = 100;
    int linecount = 0;
    while((read = getline(&line, &len, allwords)) != EOF) {
        int i = 0;
        while(line[i++] != '\n');
        line[i-1] = '\0';
        if(strlen(line) == 5 && !containsSpecial(line)) {
            fprintf(newwords, "%s", line);
            fprintf(newwords, "\n");
	    linecount++;
        }
    }
    free(line);
    fclose(allwords);
    fclose(newwords);
    newwords = fopen("wordles.txt", "r");
    srand(time(NULL));
    int randIndex = rand() % linecount;
    int index = 0;
    char* retLine = malloc(6);
    while(index < randIndex) {
	fgets(retLine, 6, newwords);
        index++;
    }
    fclose(newwords);
    remove("words.txt");
    remove("wordles.txt");
    return retLine;
}

void play(char* randomWord) {
    char* randomWordCopy = malloc(5);
    strcpy(randomWordCopy, randomWord);
    char guess[200];
    do {
        printf("Your guess: ");
        scanf("%s", guess); 
    } while(strlen(guess) != 5);
    char coloredIndices[5];
    for(int i = 0; i < 5; i++) {
        coloredIndices[i] = 0;
    }
    for(int i = 0; i < 5; i++) {
        if(randomWord[i] == guess[i]) {
            guess[i] = '_';
            randomWordCopy[i] = '_';
            coloredIndices[i] = 2;
        }
    }
    for(int i = 0; i < 5; i++) {
        if(guess[i] != '_') {
            for(int j = 0; j < 5; j++) {
                if(randomWordCopy[j] != '_') {
                    if(guess[i] == randomWordCopy[j]) {
                        randomWordCopy[j] = '-';
                        coloredIndices[i] = 1;
                    }	
                }
            }
        }
    }

    for(int i = 0; i < 5; i++) {
        if(coloredIndices[i] == 0) {
            printf("%c", guess[i]);
        }
        else if(coloredIndices[i] == 1) {
            printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, guess[i]);
        }
        else {
            printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, randomWord[i]);
        }
    }
    printf("\n");
    if(strcmp(guess, "_____") != 0){
        play(randomWord);
    }
    free(randomWordCopy);
}

int main(void) {
    char* randomWord;
    do {
        randomWord = getRandomWord();
    } while(strlen(randomWord) != 5);
    play(randomWord);
    free(randomWord);
    printf("You won!\n");
    return 0;
}


