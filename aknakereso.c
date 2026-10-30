#define MERET_X 10
#define MERET_Y 10
#define MIN_MINE 3
#define MAX_MINE 30
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int checkMineCount(int mineCount) {
	if (mineCount >= MERET_X * MERET_Y) {
		printf("Impossible configuration! Mine count %d is bigger than map size %d\n", mineCount, MERET_X * MERET_Y);
		return 1;
	}
	if (mineCount < MIN_MINE || mineCount > MAX_MINE) {
		printf("Impossible configuration! Mine count %d is outside of defined range! Range is %d <= mines <= %d\n", mineCount, MIN_MINE, MAX_MINE);
		return 1;
	}
	return 0;
}

int offTheBoard(int guessX, int guessY) {
    if (guessX < 0 || guessX >= MERET_X || guessY < 0 || guessY >= MERET_Y) {
        return 1;
    }
    return 0;
}

int initTable(int table[MERET_Y][MERET_X], int firstGuessX, int firstGuessY, int mineCount) {
	if (offTheBoard(firstGuessX, firstGuessY)) {
		printf("Couldn't calculate table since first guess is not on the table! (%d, %d)\n", firstGuessY + 1, firstGuessX + 1);
		return 1;
	}
	if (checkMineCount(mineCount)) {
		return 1;
	}
	int available = MERET_X * MERET_Y;
	for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!offTheBoard(firstGuessX + i, firstGuessY + j)) {
                table[firstGuessY + j][firstGuessX + i] = -1;
                available--;
            }
        }
	}
	for (int i = 0; i < mineCount; i++) {
		int randMine = rand() % available;
		for (int j = 0; j < MERET_X * MERET_Y; j++) {
			if (table[j / MERET_X][j % MERET_X] != -1) {
				if (randMine == 0) {
					table[j / MERET_X][j % MERET_X] = -1;
					available--;
					break;
				} else {
					randMine--;
				}
			}
		}
	}
	for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!offTheBoard(firstGuessX + i, firstGuessY + j)) {
                table[firstGuessY + j][firstGuessX + i] = 0;
            }
        }
	}
	return 0;
}

void countNeighbouringMines(int table[MERET_Y][MERET_X]) {
    for (int i = 0; i < MERET_X; i++) {
        for (int j = 0; j < MERET_Y; j++) {
            if (table[j][i] >= 0) {
                int count = 0;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        if (!offTheBoard(i + k, j + l)) {
                            if (table[j + l][i + k] < 0) {
                                count++;
                            }
                        }
                    }
                }
                table[j][i] = count;
            } else {
                table[j][i] = -1;
            }
        }
    }
}

char* centerText(char* output, int text, int letter) {
    output[0] = ' ';
    output[1] = ' ';
    output[2] = ' ';
    if (text < 0) return output;
    if (letter) {
        if (text > 26 * 27) {
            output[0] = 'A' + ((text - 1) % 18278 - 702) / 676;
            output[1] = 'A' + ((text - 1) % 18278 - 702) % 676 / 26;
            output[2] = 'A' + (text - 1) % 26;
        } else if (text > 26) {
            output[0] = 'A' + (text - 1) / 26 - 1;
            output[1] = 'A' + (text - 1) % 26;
            output[2] = ' ';
        } else if (text > 0) {
            output[0] = ' ';
            output[1] = 'A' + text - 1;
            output[2] = ' ';
        }
    } else {
        if (text >= 100) {
            text %= 1000;
            output[0] = '0' + text / 100;
            text %= 100;
            output[1] = '0' + text / 10;
            text %= 10;
            output[2] = '0' + text;
        } else if (text >= 10) {
            output[0] = ' ';
            text %= 100;
            output[1] = '0' + text / 10;
            text %= 10;
            output[2] = '0' + text;
        } else if (text >= 0) {
            output[0] = ' ';
            text %= 10;
            output[1] = '0' + text;
            output[2] = ' ';
        }
    }
    return output;
}

void drawTable(int table[MERET_Y][MERET_X], int shownTable[MERET_Y][MERET_X]) {
    char centered[] = "   ";
	printf("   ");
	for (int i = 1; i <= MERET_X; i++) {
		printf(" %s", centerText(centered, i, 0));
	}
	printf("\n   +");
    for (int j = 0; j < MERET_X; j++) {
        printf("---+");
    }
    printf("\n");
	for (int i = 0; i < MERET_Y; i++) {
        printf("%s|", centerText(centered, i + 1, 1));
		for (int j = 0; j < MERET_X; j++) {
			printf("%s|", !shownTable[i][j] ? "???" : table[i][j] < 0 ? " # " : table[i][j] > 0 ? centerText(centered, table[i][j], 0) : "   ");
		}
		printf("\n   +");
		for (int j = 0; j < MERET_X; j++) {
            printf("---+");
		}
		printf("\n");
	}
}

unsigned int readInput() {
    char c = getchar();
    unsigned short row = 0;
    while (c != '\n' && ('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z')) {
        row *= 'Z' - 'A' + 1;
        row += c - ('a' <= c ? 'a' : 'A') + 1;
        c = getchar();
    }
    unsigned short column = 0;
    while (c != '\n' && '0' <= c && c <= '9') {
        column *= 10;
        column += c - '0';
        c = getchar();
    }
    while (c != '\n') {
        c = getchar();
    }
    unsigned int result = 0;
    result += row;
    result <<= 16;
    result += column;
    return result;
}

int excavate(int table[MERET_Y][MERET_X], int shownTable[MERET_Y][MERET_X], int x, int y, int remaining) {
    if (!shownTable[y][x]) {
        shownTable[y][x] = 1;
        remaining--;
    }
    if (table[y][x] < 0) {
        return -1;
    }
    if (table[y][x] == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if ((i != 0 || j != 0) && !offTheBoard(x + i, y + j) && !shownTable[y + j][x + i]) {
                    remaining = excavate(table, shownTable, x + i, y + j, remaining);
                }
            }
        }
    }
    return remaining;
}

void showMines(int table[MERET_Y][MERET_X], int shownTable[MERET_Y][MERET_X]) {
    for (int i = 0; i < MERET_X; i++) {
        for (int j = 0; j < MERET_Y; j++) {
            if (table[j][i] < 0) {
                shownTable[j][i] = 1;
            }
        }
    }
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
    /*
	if (argc < 2) {
		printf("Insufficent configuration! Missing run parameter for mine count!\n");
		printf("Program terminated\n");
		return -1;
	}
	int mineCount = atoi(argv[1]);
	*/
	int mineCount = 15;

	if (checkMineCount(mineCount)) {
		printf("Program terminated\n");
		return -1;
	}
	printf("Starting Minesweeper! Map is defined %d by %d, where %d mines have been placed!\n\n", MERET_Y, MERET_X, mineCount);

	int table[MERET_Y][MERET_X] = {0};
	int showntable[MERET_Y][MERET_X] = {0};

	int guessX, guessY;
	int try = 0;
    do {
        drawTable(table, showntable);
        printf(!try ? "\nChoose a square to start! You can give your guesses in the format: A1\n" : "You falied your first guess! Try again! Format: A1\n");

        unsigned int input = readInput();
        guessX = input % ((unsigned int) 1 << 16) - 1;
        guessY = input / ((unsigned int) 1 << 16) - 1;
    } while (++try && initTable(table, guessX, guessY, mineCount));

    countNeighbouringMines(table);

    int remaining = MERET_X * MERET_Y;

    remaining = excavate(table, showntable, guessX, guessY, remaining);
    while (remaining > mineCount) {
        drawTable(table, showntable);
        printf("Make a guess! Format: A1\n");
        unsigned int input = readInput();
        guessX = input % ((unsigned int) 1 << 16) - 1;
        guessY = input / ((unsigned int) 1 << 16) - 1;

        if (offTheBoard(guessX, guessY)) {
            printf("Your guess is outside the board! Try again!\n");
        } else {
            remaining = excavate(table, showntable, guessX, guessY, remaining);
        }
    }

    if (remaining < 0) {
        showMines(table, showntable);
        drawTable(table, showntable);
        printf("Oh no! You excavated a mine and exploded! Better luck next time!\n");
    } else {
        drawTable(table, showntable);
        printf("Mission complete! You excavated every safe square! Great job!\n");
    }

	return 0;
}
