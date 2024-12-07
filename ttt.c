#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void cls() {
	printf("\e[1;1H\e[2J");
}

void print_board(char board[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				printf(" "); // beginning space
			}
			printf("%c", board[i][j]);
			if (j != 2) { // dont place separator at the end
				printf(" | "); 
			}
		}
		if (i != 2) { // dont place separator at the end
			printf("\n---+---+---\n");
		}
	}

	printf("\n");
}

char game_winner(char board[3][3])
{
	// check rows and cols
	for (int i = 0; i < 3; i++) {
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
			return board[i][0];
		} if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
			return board[0][i];
		}
	}

	// check diags
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
		return board[0][0];
	} if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
		return board[0][2];
	}

	return '\0';
}

bool board_full(char board[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') {
				return false;
			}
		}
	}
	
	return true;
}

int get_turn(char turn, char board[3][3])
{
	long num;
	char buf[256];
	bool success;

	do {
		printf("%c: ", turn);
		if (!fgets(buf, 256, stdin)) {
			// input failed
			return -1;
		}

		char *endptr;
		
		errno = 0;
		num = strtol(buf, &endptr, 10);
		if (errno == ERANGE || num < 1 || num > 9) {
			printf("Out of range! enter a number 1-9.\n");
			success = false;
		} else if (endptr == buf) {
			// no chars read
			success = false;
		} else if (*endptr && *endptr != '\n') {
			// entire string not parsed
			success = false;
		} else { // check if space is occupied
			int row = (num-1)/3;
			int col = (num-1)%3;

			if (board[row][col] != ' ') {
				printf("That space is filled!\n");
				success = false;
			} else {
				success = true;
			}
		}
	} while (!success);

	return num;
}

void swap_turn(char *turn) {
	if (*turn == 'X') {
		*turn = 'O';
	} else {
		*turn = 'X';
	}
}

void place_turn(char *turn, int pos, char (*board)[3][3])
{
	int row = (pos-1)/3;
	int col = (pos-1)%3;

	(*board)[row][col] = *turn;
}

int main(void)
{
	char board[3][3];
	char (*pBoard)[3][3] = &board;
	char turn = 'X';
	char *pTurn = &turn;
	memset(board, ' ', sizeof(board));
	
	cls();	

	printf("Enter a board space from 1-9.\n\n");
	
	char winner;
	while (!board_full(board) && (winner = game_winner(board)) == '\0') {
		print_board(board);
		printf("\n");
		int pos = get_turn(turn, board);
		place_turn(pTurn, pos, pBoard);

		swap_turn(pTurn);
		cls();
		printf("\n");
	}

	print_board(board);
	printf("\n");

	// game over, either:
	// 1. a line was formed, (winner)
	// 2. the board is full, (tie)
	// 3. both (winner)

	// 1. + 2.
	if (winner != '\0') {
		printf("%c wins!\n\n", winner);
	} else if (board_full) { // 3. this SHOULD always run if the above check failed, but oh well the cpu will live
		printf("Tie!\n\n");
	}

	return 0;
}
