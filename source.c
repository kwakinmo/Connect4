#include <stdio.h>
#include <stdbool.h>
#include "HeuristicAndRules.h"

void display(int board[R][C]);                    // 보드를 표시합니다.
int  possible(int board[R][C], int column);       // 가능한 수인지 판단합니다.
int  playerTurn(void);                            // 플레이어 차례일 때.
int complayerTurn(void);                          // AI 차례인데 플레이어가 컨트롤할 때.
int  computerTurn(int board[R][C], int depth);    // AI 차례일 때.
void emptyBoard(int board[R][C]);                 // 보드를 비웁니다.

int main(void) {
	int move;                   // 여기 해당하는 열에 돌을 두게 됩니다.
	int turn;                   // 1은 플레이어 차례, 2는 AI 차례를 나타냅니다.
	int howto;
	int record[2] = { -1, -1 }; // 이전 한 수를 저장해둡니다. [0]은 나의 수, [1]은 AI의 수.
	int depth = 9;
	int board[R][C];

	emptyBoard(board);
	display(board);             // 우선 보드를 비우고 빈 보드를 표시합니다.

	printf("1 : AI first | 2 : Player first\n");

	do {
		scanf_s("%d", &turn);
		if (turn != 1 && turn != 2)
			printf("invalid\n");
	} while (turn != 1 && turn != 2);   // 누가 먼저 둘 지 선택합니다.

	while (endgame(board, turn) == 0) { // 게임이 끝날 때까지 계속됩니다.
		turn = 3 - turn;                // 턴을 앞에서 미리 변경시켜야 while문이 제 때 끝납니다.
		if (turn == 1) {
			printf("WHAT WILL YOU DO? - Play (1)  |  Redo (2)\n"); // 수를 둘 건가 한 수 무를 건가 고릅니다. 무르는 건 한 턴에 한 번씩만 가능합니다.
			do {
				scanf_s("%d", &howto);
				if (howto != 1 && howto != 2)
					printf("invalid\n");
			} while (howto != 1 && howto != 2);                    // 제대로 된 숫자를 입력할 때까지 반복합니다.

			if (howto == 2) {
				if (record[0] == -1)
					printf("You can't redo from the beginning.\n"); // 물론 두지도 않았는데 무를 수는 없습니다.
				else {
					rmv(board, record[1]);
					rmv(board, record[0]);

					display(board);

					printf("Done.\n"); // 무를 경우, AI와 플레이어의 수를 각각 한 수씩 지우고 보드를 출력합니다.
				}
			}

			do {
				move = playerTurn();
			} while (possible(board, move) == 0);
			record[0] = move; // 내 수를 기록합니다.
		}
		else {
			do {
				printf("HOW TO USE AI -  Search (1)  |  Rule (2)  | Player (3)\n");
				scanf_s("%d", &howto);
				if (howto == 1) { // AI가 휴리스틱으로 서치합니다.
					move = computerTurn(board, depth);
					printf(" Computer move is %d \n ", move + 1);
				}
				else if (howto == 2)// AI가 룰로 서치합니다.
				{
					int opponent = move;
					move = useRule(board, opponent);
					printf(" Computer move is %d \n ", move + 1);
				}
				else // AI를 플레이어가 컨트롤합니다.
				{
					move = complayerTurn();
					printf(" Computer move is %d \n ", move + 1);
				}
			} while (possible(board, move) == 0);
			record[1] = move; // AI 수를 기록합니다.
		}
		put(board, move, turn);
		display(board);         // 나온 move에 따라 돌을 두고 결과 보드를 표시합니다.
		printf("\n");
	}

	// 게임이 끝나면 결과를 출력합니다.
	switch (endgame(board, turn)) {
	case 1:
		printf("Player wins!\n");
		break;
	case 2:
		printf("AI wins!\n");
		break;
	case 3:
		printf("Draw!\n");
		break;
	default:
		printf("Result Error!\n");
	}

	return 0;
}

// 보드를 표시합니다.
void display(int board[R][C]) {

	int i, j, k, m;
	char vert_line = '|';
	char horz_line_segment[5] = "---+";
	char horz_line_segment_prefix = '+';

	printf("\n");
	for (i = 0; i < C; i++)
		printf("  %d ", i + 1);
	printf("\n");

	for (i = R - 1; i >= 0; i--) {
		printf("%c", vert_line);
		for (j = 0; j < C; j++) {
			printf(" %d %c", board[i][j], vert_line);
		}
		printf("\n");
		printf("%c", horz_line_segment_prefix);
		for (k = 0; k < C; k++)
			printf("%s", horz_line_segment);

		printf("\n");
	}
	for (m = 1; m <= C; m++) {
		(m > 9) ? printf(" ") : printf("  ");
	}
	printf("\n");
}

// 가능한 수인지 체크합니다. 열 범위를 벗어나거나 이미 꽉 찬 열이면 불가능하다고 표시합니다.
int possible(int board[R][C], int column)
{
	int i;
	int occupied = 0;

	// 게임을 처음 시작했을 때는 가운데 열에 둘 수 없게 합니다.
	if (emptyspace(board) == 42 && column == 3) {
		printf("You can't put there when making first move\n");
		return 0;
	}

	if (column >= C || column < 0) {

		printf("Invalid move\n");
		return 0;
	}

	for (i = 0; i < R; i++) {
		if (board[i][column] != 0)
			occupied++;
	}

	if (occupied == R) {
		printf("\n invalid move \n");
		return 0;
	}

	else
		return 1;
}

// 플레이어 턴입니다. 입력을 받습니다.
int playerTurn() {
	int pos;
	printf("It's player turn > ");
	scanf_s("%d", &pos);

	return pos - 1;
}

// 플레이어가 조작하는 AI의 턴입니다. 입력을 받습니다.
int complayerTurn() {
	int pos;
	printf("It's complayer turn > ");
	scanf_s("%d", &pos);

	return pos - 1;
}

// AI의 턴입니다. AI 함수를 불러냅니다.
int computerTurn(int board[R][C], int depth) {

	return ai(board, depth);
}

// 보드를 비웁니다.
void emptyBoard(int board[R][C]) {
	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			board[i][j] = 0;
}