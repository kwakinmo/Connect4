#include <stdio.h>
#include <stdbool.h>
#include "HeuristicAndRules.h"

void display(int board[R][C]);                    // ���带 ǥ���մϴ�.
int  possible(int board[R][C], int column);       // ������ ������ �Ǵ��մϴ�.
int  playerTurn(void);                            // �÷��̾� ������ ��.
int complayerTurn(void);                          // AI �����ε� �÷��̾ ��Ʈ���� ��.
int  computerTurn(int board[R][C], int depth);    // AI ������ ��.
void emptyBoard(int board[R][C]);                 // ���带 ���ϴ�.

int main(void) {
	int move;                   // ���� �ش��ϴ� ���� ���� �ΰ� �˴ϴ�.
	int turn;                   // 1�� �÷��̾� ����, 2�� AI ���ʸ� ��Ÿ���ϴ�.
	int howto;
	int record[2] = { -1, -1 }; // ���� �� ���� �����صӴϴ�. [0]�� ���� ��, [1]�� AI�� ��.
	int depth = 9;
	int board[R][C];

	emptyBoard(board);
	display(board);             // �켱 ���带 ���� �� ���带 ǥ���մϴ�.

	printf("1 : AI first | 2 : Player first\n");

	do {
		scanf_s("%d", &turn);
		if (turn != 1 && turn != 2)
			printf("invalid\n");
	} while (turn != 1 && turn != 2);   // ���� ���� �� �� �����մϴ�.

	while (endgame(board, turn) == 0) { // ������ ���� ������ ��ӵ˴ϴ�.
		turn = 3 - turn;                // ���� �տ��� �̸� ������Ѿ� while���� �� �� �����ϴ�.
		if (turn == 1) {
			printf("WHAT WILL YOU DO? - Play (1)  |  Redo (2)\n"); // ���� �� �ǰ� �� �� ���� �ǰ� ���ϴ�. ������ �� �� �Ͽ� �� ������ �����մϴ�.
			do {
				scanf_s("%d", &howto);
				if (howto != 1 && howto != 2)
					printf("invalid\n");
			} while (howto != 1 && howto != 2);                    // ����� �� ���ڸ� �Է��� ������ �ݺ��մϴ�.

			if (howto == 2) {
				if (record[0] == -1)
					printf("You can't redo from the beginning.\n"); // ���� ������ �ʾҴµ� ���� ���� �����ϴ�.
				else {
					rmv(board, record[1]);
					rmv(board, record[0]);

					display(board);

					printf("Done.\n"); // ���� ���, AI�� �÷��̾��� ���� ���� �� ���� ����� ���带 ����մϴ�.
				}
			}

			do {
				move = playerTurn();
			} while (possible(board, move) == 0);
			record[0] = move; // �� ���� ����մϴ�.
		}
		else {
			do {
				printf("HOW TO USE AI -  Search (1)  |  Rule (2)  | Player (3)\n");
				scanf_s("%d", &howto);
				if (howto == 1) { // AI�� �޸���ƽ���� ��ġ�մϴ�.
					move = computerTurn(board, depth);
					printf(" Computer move is %d \n ", move + 1);
				}
				else if (howto == 2)// AI�� ��� ��ġ�մϴ�.
				{
					int opponent = move;
					move = useRule(board, opponent);
					printf(" Computer move is %d \n ", move + 1);
				}
				else // AI�� �÷��̾ ��Ʈ���մϴ�.
				{
					move = complayerTurn();
					printf(" Computer move is %d \n ", move + 1);
				}
			} while (possible(board, move) == 0);
			record[1] = move; // AI ���� ����մϴ�.
		}
		put(board, move, turn);
		display(board);         // ���� move�� ���� ���� �ΰ� ��� ���带 ǥ���մϴ�.
		printf("\n");
	}

	// ������ ������ ����� ����մϴ�.
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

// ���带 ǥ���մϴ�.
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

// ������ ������ üũ�մϴ�. �� ������ ����ų� �̹� �� �� ���̸� �Ұ����ϴٰ� ǥ���մϴ�.
int possible(int board[R][C], int column)
{
	int i;
	int occupied = 0;

	// ������ ó�� �������� ���� ��� ���� �� �� ���� �մϴ�.
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

// �÷��̾� ���Դϴ�. �Է��� �޽��ϴ�.
int playerTurn() {
	int pos;
	printf("It's player turn > ");
	scanf_s("%d", &pos);

	return pos - 1;
}

// �÷��̾ �����ϴ� AI�� ���Դϴ�. �Է��� �޽��ϴ�.
int complayerTurn() {
	int pos;
	printf("It's complayer turn > ");
	scanf_s("%d", &pos);

	return pos - 1;
}

// AI�� ���Դϴ�. AI �Լ��� �ҷ����ϴ�.
int computerTurn(int board[R][C], int depth) {

	return ai(board, depth);
}

// ���带 ���ϴ�.
void emptyBoard(int board[R][C]) {
	int i, j;
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++)
			board[i][j] = 0;
}