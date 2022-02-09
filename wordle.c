#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define DEFAULT   "\x1b[0m"
#define MAX_WORD_LENGTH 100
#define WORDLE_LENGTH 5

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _wordList {
    size_t size;
    char** list;
} wordList;

wordList* wordList_init() {
    wordList* words = malloc(sizeof(wordList));
    words->size = 0;
    words->list = NULL;
    return words;
}

void wordList_add(wordList* words, char* word) {
    if(words->size == 0) {
        words->list = malloc(sizeof(char*));
    }
    else {
        words->list = realloc(words->list, (words->size+1) * sizeof(char*));
    }
    words->list[words->size] = malloc(strlen(word)+1);
    strcpy(words->list[words->size], word);
    words->size++;
}

void wordList_print(wordList* wl) {
    for(size_t i = 0; i < wl->size; i++) {
        printf("%s\n", wl->list[i]);
    }
}

void wordList_destroy(wordList* words) {
    for(size_t i = 0; i < words->size; i++) {
        free(words->list[i]);
    }
    free(words->list);
    free(words);
}


int containsSpecial(char* str) {
    size_t len = strlen(str);
    for(size_t i = 0; i < len; i++) {
        if(str[i] < 97 || str[i] > 126) {
            return 1;
        }
    }
    return 0;
}


void strip(char* str) {
	size_t length = strlen(str);
	for(size_t i = 0; i < length; i++) {
		if(str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}
}


char* getRandomWord(void) {
	wordList* wl = wordList_init();
    FILE* allwords = fopen("/usr/share/dict/words", "r");
    char line[MAX_WORD_LENGTH];
    int linecount = 0;
	while(!feof(allwords)) {
		fgets(line, MAX_WORD_LENGTH, allwords);
		if(feof(allwords)) {
			break;
		}
		strip(line);
		if(strlen(line) == WORDLE_LENGTH && !containsSpecial(line)) {
			wordList_add(wl, line);
	    	linecount++;
        }
    }
    fclose(allwords);
    srand(time(NULL));
    int randIndex = rand() % linecount;
    int index = 0;
    char* retLine = malloc(WORDLE_LENGTH+1);
	strcpy(retLine, wl->list[randIndex]);
	wordList_destroy(wl);
	return retLine;
}

void play(char* randomWord) {
    char randomWordCopy[WORDLE_LENGTH];
    char guess[MAX_WORD_LENGTH];
    do {
        strcpy(randomWordCopy, randomWord);
        do {
            printf("Your guess: ");
            scanf("%s", guess); 
        } while(strlen(guess) != WORDLE_LENGTH || containsSpecial(guess));
        char coloredIndices[WORDLE_LENGTH];
        for(size_t i = 0; i < WORDLE_LENGTH; i++) {
            coloredIndices[i] = 0;
        }
        for(size_t i = 0; i < WORDLE_LENGTH; i++) {
            if(randomWord[i] == guess[i]) {
                guess[i] = '_';
                randomWordCopy[i] = '_';
                coloredIndices[i] = 2;
            }
        }
        for(size_t i = 0; i < WORDLE_LENGTH; i++) {
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

        for(size_t i = 0; i < WORDLE_LENGTH; i++) {
            if(coloredIndices[i] == 0) {
                printf("%c", guess[i]);
            }
            else if(coloredIndices[i] == 1) {
                printf(YELLOW "%c" DEFAULT, guess[i]);
            }
            else {
                printf(GREEN "%c" DEFAULT, randomWord[i]);
            }
        }
        printf("\n");
    } while(strcmp(guess, "_____") != 0);   
}

int main(void) {
    char* randomWord = getRandomWord();
    play(randomWord);
    free(randomWord);
    printf("You won!\n");
    return 0;
}


