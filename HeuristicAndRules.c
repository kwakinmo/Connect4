#include <stdio.h>
#include <stdbool.h>
#include "HeuristicAndRules.h"

// ������ �� ĭ�� �� ������ Ȯ��.
int emptyspace(int board[R][C]) {
	int i, j;
	int es = 0;

	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {
			if (board[i][j] == 0)
				es++;
		}
	}

	return es;
}

// �ش� ������ ����ִ� ���� �Ʒ�ĭ�� ���� �����ϴ�.
int put(int board[R][C], int column, int player) {

	int i;

	for (i = 0; i < R; i++) {
		if (board[i][column] == 0) {
			board[i][column] = player;
			return i + 1;
		}
	}
}

// �ش� ���� �� ������ �����մϴ�.
void rmv(int board[R][C], int column) {
	int i;
	for (i = R - 1; i >= 0; i--) {
		if (board[i][column] != 0) {
			board[i][column] = 0;
			break;
		}
	}
}

int useRule(int board[R][C], int opponent)
{
	// �켱���� ���� rule ����.
	if (checkcenter(board) != -1) {
		printf("Rule 0\n");
		return checkcenter(board);
	}
	if (checkwin(board) != -1) {
		printf("Rule 1\n");
		return checkwin(board);
	}
	if (checklose(board) != -1) {
		printf("Rule 2\n");
		return checklose(board);
	}

	int rule4candidates[7] = { 0, };
	checkwin2(board, rule4candidates); // Rule 4  

	int dontDoThat[7] = { 0, };

	for (int i = 0; i < C; i++) {
		int worst = checkworst(board, i); // �� ��쿡 worst�� üũ�غ���. (rule3)
		if (rule4candidates[i] == 1) { // candidates�̰�,
			if (worst == -1) { // worst�� ������,
				printf("Rule4\n");
				return i; // �ֶ�.
			}
		}
		if (worst == 1) { // worst ������,
			dontDoThat[i] = 1; // �������ƾ��ϴ� ���� check.
		}
	}

	int rule5candidates[7] = { 0, };
	checklose2(board, rule5candidates); // Rule 5 

	for (int i = 0; i < C; i++) {
		int worst = checkworst(board, i); // �� ��쿡 worst�� üũ�غ���. (rule3)
		if (rule5candidates[i] == 1) { // candidates�̰�,
			if (worst == -1) { // worst�� ������,
				printf("Rule5\n");
				return i; // �ֶ�.
			}
		}
		if (worst == 1) { // worst ������,
			dontDoThat[i] = 1; // �������ƾ��ϴ� ���� check.
		}
	}
	// ������� �ʼ����ΰ� // �̷��� �ټ��� �ϸ� �ɵ�!

	int rule6candidates[7] = { 0, };
	checkwin3(board, rule6candidates); // Rule 6 

	for (int i = 0; i < C; i++) {
		int worst = checkworst(board, i); // �� ��쿡 worst�� üũ�غ���. (rule3)
		if (rule6candidates[i] == 1) { // candidates�̰�,
			if (worst == -1) { // worst�� ������,
				printf("Rule6\n");
				return i; // �ֶ�.
			}
		}
		if (worst == 1) { // worst ������,
			dontDoThat[i] = 1; // �������ƾ��ϴ� ���� check.
		}
	}

	// Rule 7 : �ֵ� �Ǵ� �� �߿��� ���濡 ���� ��ġ ���� ����. 
	for (int i = 0; i < C; i++) {
		if (dontDoThat[i] == 0 && opponent == i && board[5][i] == 0) {
			printf("Rule6\n");
			return i;
		}
	}

	// Rule 8 : ������������, ������� ����.
	if (dontDoThat[3] == 0 && board[5][3] == 0) {
		printf("Rule7\n");
		return 3;
	}
	if (dontDoThat[2] == 0 && board[5][2] == 0) {
		printf("Rule7\n");
		return 2;
	}
	if (dontDoThat[4] == 0 && board[5][4] == 0) {
		printf("Rule7\n");
		return 4;
	}
	if (dontDoThat[1] == 0 && board[5][1] == 0) {
		printf("Rule7\n");
		return 1;
	}
	if (dontDoThat[5] == 0 && board[5][5] == 0) {
		printf("Rule7\n");
		return 5;
	}
	if (dontDoThat[0] == 0 && board[5][0] == 0) {
		printf("Rule7\n");
		return 0;
	}
	if (dontDoThat[3] == 0 && board[5][6] == 0) {
		printf("Rule7\n");
		return 3;
	}

	return -1;
}

bool checkPutPossible(int board[R][C], int row, int col) { // �ؿ� ���������� �Ѽ� X
	if (row == 0) {
		return true;
	}
	else
	{
		if (board[row - 1][col] != 0)
			return true;
		else
			return false;
	}
}

int checkworst(int board[R][C], int col) {
	// Rule3 : ���� ���� �� ������ ���� ���� �̱�� �Ǹ� �װŴ� ���ΰ� �� ����.
	// candidates�� action�� ���� �������� ��, ���� �Ǵ��� Ȯ���غ���.

	int map[R][C] = { 0 };
	int putRowPos = 0;

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			map[i][j] = board[i][j];
		}
	}
	for (int i = 0; i < R; i++) {
		if (map[i][col] == 0) {
			putRowPos = i;
			break;
		}
	}
	map[putRowPos][col] = 2;
	if (putRowPos + 1 < R) {
		map[putRowPos + 1][col] = 1;
		if (endgame(map, 1) == 1) // ������ �̱�� �θ� �� ��!
			return 1;
	}
	return -1;
}

void checkwin3(int board[R][C], int rule6candidates[7]) {
	// rule6(checkwin3) : Ȯ���� �̱�� ���� �ƴ����� ������ ������ �� �ִ� ��(0220, 0022, 2200, 2002, 2020,0202)�� �켱���� �ؿ� �־ �� �� ������ ������ �� �� �ֵ��� �Ͽ����ϴ�.

	// ���� check // 0220 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �밢�� check // 0220
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3))
			{
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 0220
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// ���� check // 0022 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 1] = 1;
			}
		}
	}
	// �밢�� check // 0022
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 0022
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 1] = 1;
			}
		}
	}
	// ���� check // 2200 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �밢�� check // 2200
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 2200
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// ���� check // 0202
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// �밢�� check // 0202
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 0202
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// ���� check // 2020
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �밢�� check // 2020
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 2020
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}// ���� check // 2002
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2) && board[i][j + 3] == 2) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// �밢�� check // 2002
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 0) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 2002
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && board[i - 1][j + 1] == 2 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 0) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	return;
}

void checklose2(int board[R][C], int rule5candidates[7]) {
	// Rule5 : ���� ���� ������ �� �� ���Ŀ� ���� �Ǵ� ��츦 �� ������ �̸� ���´�. // 00110,01100,01010
	// ���� ������ �δ� �ſ� ���� ������ �� �� �� �ִ� ��찡 ���� ���� ������, ������ ���� ���ٺ��� ���� ���� �켱 �н�.
	// ���� check // 00110
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 1] = 1;  // ���� ���� j,j+1,j+4 �߿� �ϳ��ε� �̰� ����ġ�� �ٱ�? ���� �׷����� �� �ʹ٤� ����� ������ �켱 �� �־���Ѵ�!
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �밢�� check // 00110
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 1] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 00110
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 1 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 1] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// ���� check // 01100
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3) && board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 3] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �밢�� check // 01100
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3) && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 3] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 01100
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3) && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 3] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}


	// ���� check // 01010 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 1 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 1 && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
			{
				rule5candidates[j] = 1;  // ����� ���°� ���� ������ �켱 �� �ĺ����� �־�����. // ���߿� ����ġ �ٸ��� ���� // ���⵵ ����ġ �� �������!!
				rule5candidates[j + 2] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �밢�� check // 01010
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 1 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 2] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// �ݴ� �밢�� üũ // 01010
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 1 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 2] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	return;
}

void checkwin2(int board[R][C], int rule4candidates[7]) {
	// Rule 4 : ���� �ΰ� ���� ���� ���� �̱�� ���� �д�.  // 02200, 00220, 02020 �̰Ŵ� Ȯ���� �̱�� ��.
	// ������ �δ� �ſ� ���� ������ �� �̱� �� �ִ� ��찡 ���� ���� ������, Ȯ���� �̱�Բ� �ϴ� ��츸 �켱 �ڵ���.

	// ���� check // 02200 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3) && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// �밢�� check // 02200
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3) && (board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// �ݴ� �밢�� üũ // 02200
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3) && (board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// ���� check // 00220 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 2 && board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}
	// �밢�� check // 00220
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}
	// �ݴ� �밢�� üũ // 00220
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}

	// ���� check // 02020
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 2 && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
				rule4candidates[j + 2] = 1;
		}
	}
	// �밢�� check // 02020
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 2 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
				rule4candidates[j + 2] = 1;
		}
	}
	// �ݴ� �밢�� üũ // 02020
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 2 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
				rule4candidates[j + 2] = 1;
		}
	}
	return;
}

int checklose(int board[R][C]) {
	// Rule 2 : �ȵθ� ���� ��쿡�� ���´�.  0111,1011,1101,0111  
	// ���� check // 0111,1011,1101,1110
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 1)
				return j;
			if (board[i][j] == 1 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 1 && board[i][j + 3] == 1)
				return j + 1;
			if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2) && board[i][j + 3] == 1)
				return j + 2;
			if (board[i][j] == 1 && board[i][j + 1] == 1 && board[i][j + 2] == 1 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3))
				return j + 3;
		}
	}

	// ���� check
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][i] == 0) //
				return j;
		}
	}

	// �밢�� check // 0111,1011,1101,1110
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1)
				return j;
			if (board[i][j] == 1 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 1)
				return j + 1;
			if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 1)
				return j + 2;
			if (board[i][j] == 1 && board[i + 1][j + 1] == 1 && board[i + 2][j + 2] == 1 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3))
				return j + 3;
		}
	}

	// \ �ݴ�밢�� check // 0111,1011,1101,1110
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 1)
				return j;
			if (board[i][j] == 1 && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 1)
				return j + 1;
			if (board[i][j] == 1 && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 1)
				return j + 2;
			if (board[i][j] == 1 && board[i - 1][j + 1] == 1 && board[i - 2][j + 2] == 1 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3))
				return j + 3;
		}
	}

	return -1;

}
int checkwin(int board[R][C])
{
	// Rule 1 : �θ� �̱�� ��쿣 �д�.
	// ���� check // 0222,2022,2202,2220
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 2)
				return j;
			if (board[i][j] == 2 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 2)
				return j + 1;
			if (board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2) && board[i][j + 3] == 2)
				return j + 2;
			if (board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3))
				return j + 3;
		}
	}

	// ���� check
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			if (board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2 && board[i + 3][i] == 0) // 2�� ���� ��ǻ��
				return j;
		}
	}

	// �밢�� check // 0222,2022,2202,2220
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2)
				return j;
			if (board[i][j] == 2 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2)
				return j + 1;
			if (board[i][j] == 2 && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 2)
				return j + 2;
			if (board[i][j] == 2 && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3))
				return j + 3;
		}
	}

	// \ �ݴ�밢�� check // 0222,2022,2202,2220
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2)
				return j;
			if (board[i][j] == 2 && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2)
				return j + 1;
			if (board[i][j] == 2 && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 2)
				return j + 2;
			if (board[i][j] == 2 && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3))
				return j + 3;
		}
	}

	return -1;
}
int checkcenter(int board[R][C])
{
	// Rule 0 : ��밡 �δ� ù ���� ���� ���� ù ���� �޸� �д�.
	// ��밡 ù ���� 1,2�� 1,6���� �����ϴ� ��쿡, ���� 1,4�� �θ� ������� ���� 1,3�� 1,5�� �θ� �ļ��ε� �̱� �� �ִ�. (�̷� ���� ���� ���� �� ���� ����������) 	
	// �� ���� ���� ��� 1,4��!

	int count = 0;

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++)
		{
			if (board[i][j] != 0)
			{
				count++;
			}
		}
	}
	if (count == 1) { // �İ����ڸ��� rule ���� ���
		if (board[0][1] == 1) {
			return 2;
		}
		else if (board[0][5] == 1) {
			return 4;
		}
		else
			return 3;
	}
	else if (count == 0) { //������ rule�� �����ϴ� ��� ù���� 1,3��
		return 2;
	}
	else {
		if (board[0][3] == 0) // ù���� �ƴѵ� �߾��� ����ִ� ���
			return 3;
	}

	return -1;

}

/* ������ �������� Ȯ���մϴ�. player�� ���� 4�� �������� �̾����� player�� �����ϰ�(player�� �¸�), 
   �̾����� �ʾҰ� �� �� ���� ������ 0(���), �̾����� �ʾҴµ� �� �� ���� ������ 3(���)�� ����մϴ�. */
int endgame(int board[R][C], int player) {
	int i, j;

	//check horizontals
	for (i = 0; i < R; i++)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] == player && board[i][j + 1] == player && board[i][j + 2] == player && board[i][j + 3] == player)
				return player;
		}

	//check verticals
	for (i = 0; i <= R - 4; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] == player && board[i + 1][j] == player && board[i + 2][j] == player && board[i + 3][j] == player)
				return player;
		}

	//check main diagonals (\)
	for (i = R - 1; i >= 4 - 1; i--)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] == player && board[i - 1][j + 1] == player && board[i - 2][j + 2] == player && board[i - 3][j + 3] == player)
				return player;
		}

	//check other diagonals (/)
	for (i = 0; i <= R - 4; i++)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] == player && board[i + 1][j + 1] == player && board[i + 2][j + 2] == player && board[i + 3][j + 3] == player)
				return player;
		}


	//check if stalement
	for (i = 0; i < C; i++)
		if (board[R - 1][i] == 0)
			return 0; //game haven't finished yet - there's at least one empty cell in a top of a row

	return 3; //stalemate - board is full
}

// ������ ���ǿ� ���� ����, ������ �̷�����ϴ�.
int heuristic(int board[R][C]) {
	int result = 0;
	int i, j;

	// ���� Ȯ��. �þ ���� ���� �������� ���� �Ǵ� ����. (4���� �� �Լ��� �Ѿ���� ���� ó���ǹǷ� ������� ����.)
	for (i = 0; i < R; i++)
		for (j = 0; j <= C - 3; j++) {
			if (board[i][j] != 2 && board[i][j + 1] != 2 && board[i][j + 2] != 2)
				result += 2;
			if (board[i][j] != 1 && board[i][j + 1] != 1 && board[i][j + 2] != 1)
				result -= 2;
		}

	for (i = 0; i < R; i++)
		for (j = 0; j <= C - 2; j++) {
			if (board[i][j] != 2 && board[i][j + 1] != 2)
				result++;
			if (board[i][j] != 1 && board[i][j + 1] != 1)
				result--;
		}
	// �̷��� �ϸ� �� ��¥���� �˻��� �� �� ��¥���� �� �� ������ �ش�. �翬�� �� ���� ����ؼ� ������ ����ߴ�. �Ʒ� Ȯ�� 2 ��쵵 ���������� ����ؼ� ������ ���.

	// ���� Ȯ�� 2. �� ���� ���� �� ���� ���� �� ���� �ϳ��� ���̿� �ΰ� �þ ���. (3��¥���� ������ ������ �޾ƾ� �Ѵٰ� ����.)
	for (i = 0; i < R; i++)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] != 2 && board[i][j + 1] != 0 && board[i][j + 2] != 2 && board[i][j + 3] != 2)
				result += 3;
			if (board[i][j] != 2 && board[i][j + 1] != 2 && board[i][j + 2] != 0 && board[i][j + 3] != 2)
				result += 3;
			if (board[i][j] != 1 && board[i][j + 1] != 0 && board[i][j + 2] != 1 && board[i][j + 3] != 1)
				result -= 3;
			if (board[i][j] != 1 && board[i][j + 1] != 1 && board[i][j + 2] != 0 && board[i][j + 3] != 1)
				result -= 3;
		}

	// ���� Ȯ��.
	for (i = 0; i <= R - 3; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2 && board[i + 1][j] != 2 && board[i + 2][j] != 2)
				result += 2;
			if (board[i][j] != 1 && board[i + 1][j] != 1 && board[i + 2][j] != 1)
				result -= 2;
		}

	for (i = 0; i <= R - 2; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2 && board[i + 1][j] != 2)
				result++;
			if (board[i][j] != 1 && board[i + 1][j] != 1)
				result--;
		}

	// ���δ� �߰��� ��� ��찡 ���� �� �����Ƿ� Ȯ�� 2�� �ʿ䰡 ����.

	// �»� �밢�� Ȯ��.
	for (i = 0; i <= R - 3; i++)
		for (j = 0; j <= C - 3; j++) {
			if (board[i + 2][j] != 2 && board[i + 1][j + 1] != 2 && board[i][j + 2] != 2)
				result += 2;
			if (board[i + 2][j] != 1 && board[i + 1][j + 1] != 1 && board[i][j + 2] != 1)
				result -= 2;
		}

	for (i = 0; i <= R - 2; i++)
		for (j = 0; j <= C - 2; j++) {
			if (board[i + 1][j] != 2 && board[i][j + 1] != 2)
				result++;
			if (board[i + 1][j] != 1 && board[i][j + 1] != 1)
				result--;
		}

	// �»� �밢�� Ȯ�� 2.
	for (i = R - 1; i >= 3; i--)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] != 2 && board[i - 1][j + 1] != 0 && board[i - 2][j + 2] != 2 && board[i - 3][j + 3] != 2)
				result += 3;
			if (board[i][j] != 2 && board[i - 1][j + 1] != 2 && board[i - 2][j + 2] != 0 && board[i - 3][j + 3] != 2)
				result += 3;
			if (board[i][j] != 1 && board[i - 1][j + 1] != 0 && board[i - 2][j + 2] != 1 && board[i - 3][j + 3] != 1)
				result -= 3;
			if (board[i][j] != 1 && board[i - 1][j + 1] != 1 && board[i - 2][j + 2] != 0 && board[i - 3][j + 3] != 1)
				result -= 3;
		}

	// ��� �밢�� Ȯ��.
	for (i = 0; i <= R - 3; i++)
		for (j = 0; j <= C - 3; j++) {
			if (board[i][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 2][j + 2] != 2)
				result += 2;
			if (board[i][j] != 1 && board[i + 1][j + 1] != 1 && board[i + 2][j + 2] != 1)
				result -= 2;
		}

	for (i = 0; i <= R - 2; i++)
		for (j = 0; j <= C - 2; j++) {
			if (board[i][j] != 2 && board[i + 1][j + 1] != 2)
				result++;
			if (board[i][j] != 1 && board[i + 1][j + 1] != 1)
				result--;
		}

	// ��� �밢�� Ȯ�� 2.
	for (i = 0; i <= R - 4; i++)
		for (j = 0; j <= C - 4; j++) {
			if (board[i][j] != 2 && board[i + 1][j + 1] != 0 && board[i + 2][j + 2] != 2 && board[i + 3][j + 3] != 2)
				result += 3;
			if (board[i][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 2][j + 2] != 0 && board[i + 3][j + 3] != 2)
				result += 3;
			if (board[i][j] != 1 && board[i + 1][j + 1] != 0 && board[i + 2][j + 2] != 1 && board[i + 3][j + 3] != 1)
				result -= 3;
			if (board[i][j] != 1 && board[i + 1][j + 1] != 1 && board[i + 2][j + 2] != 0 && board[i + 3][j + 3] != 1)
				result -= 3;
		}

	// �� ��¥�� ���� Ȯ��.
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2)
				result++;
			if (board[i][j] != 1)
				result--;
		}


	// ���� 4���� �ϼ���ų �� �ִ� ���
	// ���� 4�� �ϼ�
	for (i = 0; i <= R - 2; i++)
		for (j = 0; j <= C - 4; j++) {
			if ((board[i][j] != 1 || board[i][j] != 2) && board[i + 1][j] != 0 && board[i + 1][j + 1] != 2 && board[i + 1][j + 2] != 2 && board[i + 1][j + 3] != 2)
				result += 7;
			if ((board[i][j + 1] != 1 || board[i][j + 1] != 2) && board[i + 1][j] != 2 && board[i + 1][j + 1] != 0 && board[i + 1][j + 2] != 2 && board[i + 1][j + 3] != 2)
				result += 7;
			if ((board[i][j + 2] != 1 || board[i][j + 2] != 2) && board[i + 1][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 1][j + 2] != 0 && board[i + 1][j + 3] != 2)
				result += 7;
			if ((board[i][j + 3] != 1 || board[i][j + 3] != 2) && board[i + 1][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 1][j + 2] != 2 && board[i + 1][j + 3] != 0)
				result += 7;
			if ((board[i][j] != 1 || board[i][j] != 2) && board[i + 1][j] != 0 && board[i + 1][j + 1] != 1 && board[i + 1][j + 2] != 1 && board[i + 1][j + 3] != 1)
				result -= 7;
			if ((board[i][j + 1] != 1 || board[i][j + 1] != 2) && board[i + 1][j] != 1 && board[i + 1][j + 1] != 0 && board[i + 1][j + 2] != 1 && board[i + 1][j + 3] != 1)
				result -= 7;
			if ((board[i][j + 2] != 1 || board[i][j + 2] != 2) && board[i + 1][j] != 1 && board[i + 1][j + 1] != 1 && board[i + 1][j + 2] != 0 && board[i + 1][j + 3] != 1)
				result -= 7;
			if ((board[i][j + 3] != 1 || board[i][j + 3] != 2) && board[i + 1][j + 1] != 1 && board[i + 1][j + 2] != 1 && board[i + 1][j + 3] != 0)
				result -= 7;
		}

	for (j = 0; j <= C - 4; j++) {
		if (board[0][j] != 0 && board[0][j + 1] != 2 && board[0][j + 2] != 2 && board[0][j + 3] != 2)
			result += 7;
		if (board[0][j] != 2 && board[0][j + 1] != 0 && board[0][j + 2] != 2 && board[0][j + 3] != 2)
			result += 7;
		if (board[0][j] != 2 && board[0][j + 1] != 2 && board[0][j + 2] != 0 && board[0][j + 3] != 2)
			result += 7;
		if (board[0][j] != 2 && board[0][j + 1] != 2 && board[0][j + 2] != 2 && board[0][j + 3] != 0)
			result += 7;
		if (board[0][j] != 0 && board[0][j + 1] != 1 && board[0][j + 2] != 1 && board[0][j + 3] != 1)
			result -= 7;
		if (board[0][j] != 1 && board[0][j + 1] != 0 && board[0][j + 2] != 1 && board[0][j + 3] != 1)
			result -= 7;
		if (board[0][j] != 1 && board[0][j + 1] != 1 && board[0][j + 2] != 0 && board[0][j + 3] != 1)
			result -= 7;
		if (board[0][j] != 1 && board[0][j + 1] != 1 && board[0][j + 2] != 1 && board[0][j + 3] != 0)
			result -= 7;
	}

	// ���� 4�� �ϼ�
	for (i = 0; i <= R - 4; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2 && board[i + 1][j] != 2 && board[i + 2][j] != 2 && board[i + 3][j] != 0)
				result += 7;
			if (board[i][j] != 1 && board[i + 1][j] != 1 && board[i + 2][j] != 1 && board[i + 3][j] != 0)
				result -= 7;
		}

	// �밢�� 4�� �ϼ�
	for (i = 0; i <= R - 5; i++)
		for (j = 0; j <= C - 4; j++) {
			// �밢�� �� �Ʒ��� ���������� �ּ� �ϼ�
			if ((board[i][j] != 1 || board[i][j] != 2) && board[i + 1][j] != 0 && board[i + 2][j + 1] != 2 && board[i + 3][j + 2] != 2 && board[i + 4][j + 3] != 2)
				result += 7;
			if ((board[i][j + 3] != 1 || board[i][j + 3] != 2) && board[i + 1][j + 3] != 0 && board[i + 2][j + 2] != 2 && board[i + 3][j + 1] != 2 && board[i + 4][j] != 2)
				result += 7;
			if ((board[i][j] != 1 || board[i][j] != 2) && board[i + 1][j] != 0 && board[i + 2][j + 1] != 1 && board[i + 3][j + 2] != 1 && board[i + 4][j + 3] != 1)
				result -= 7;
			if ((board[i][j + 3] != 1 || board[i][j + 3] != 2) && board[i + 1][j + 3] != 0 && board[i + 2][j + 2] != 1 && board[i + 3][j + 1] != 1 && board[i + 4][j] != 1)
				result -= 7;
		}

	for (j = 0; j <= C - 4; j++) {
		if (board[0][j] != 0 && board[1][j + 1] != 2 && board[2][j + 2] != 2 && board[3][j + 3] != 2)
			result += 7;
		if (board[0][j + 3] != 0 && board[1][j + 2] != 2 && board[2][j + 1] != 2 && board[3][j] != 2)
			result += 7;
		if (board[0][j] != 0 && board[1][j + 1] != 1 && board[2][j + 2] != 1 && board[3][j + 3] != 1)
			result -= 7;
		if (board[0][j + 3] != 0 && board[1][j + 2] != 1 && board[2][j + 1] != 1 && board[3][j] != 1)
			result -= 7;
	}

	for (i = 0; i <= R - 4; i++)
		for (j = 0; j <= C - 4; j++) {
			// �� ��
			if ((board[i][j + 1] != 1 || board[i][j + 1] != 2) && board[i + 1][j + 1] != 0 && board[i][j] != 2 && board[i + 2][j + 2] != 2 && board[i + 3][j + 3] != 2)
				result += 7;
			if ((board[i + 1][j + 2] != 1 || board[i + 1][j + 2] != 2) && board[i + 2][j + 2] != 0 && board[i][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 3][j + 3] != 2)
				result += 7;
			if ((board[i + 2][j + 3] != 1 || board[i + 2][j + 3] != 2) && board[i + 3][j + 3] != 0 && board[i][j] != 2 && board[i + 1][j + 1] != 2 && board[i + 2][j + 2] != 2)
				result += 7;
			if ((board[i][j + 2] != 1 || board[i][j + 2] != 2) && board[i + 1][j + 2] != 0 && board[i][j + 3] != 2 && board[i + 2][j + 1] != 2 && board[i + 3][j] != 2)
				result += 7;
			if ((board[i + 1][j + 1] != 1 || board[i + 1][j + 1] != 2) && board[i + 2][j + 1] != 0 && board[i][j + 3] != 2 && board[i + 1][j + 2] != 2 && board[i + 3][j] != 2)
				result += 7;
			if ((board[i + 2][j] != 1 || board[i + 2][j] != 2) && board[i + 3][j] != 0 && board[i][j + 3] != 2 && board[i + 1][j + 2] != 2 && board[i + 2][j + 1] != 2)
				result += 7;
			if ((board[i][j + 1] != 1 || board[i][j + 1] != 2) && board[i + 1][j + 1] != 0 && board[i][j] != 1 && board[i + 2][j + 2] != 1 && board[i + 3][j + 3] != 1)
				result -= 7;
			if ((board[i + 1][j + 2] != 1 || board[i + 1][j + 2] != 2) && board[i + 2][j + 2] != 0 && board[i][j] != 1 && board[i + 1][j + 1] != 1 && board[i + 3][j + 3] != 1)
				result -= 7;
			if ((board[i + 2][j + 3] != 1 || board[i + 2][j + 3] != 2) && board[i + 3][j + 3] != 0 && board[i][j] != 1 && board[i + 1][j + 1] != 1 && board[i + 2][j + 2] != 1)
				result -= 7;
			if ((board[i][j + 2] != 1 || board[i][j + 2] != 2) && board[i + 1][j + 2] != 0 && board[i][j + 3] != 1 && board[i + 2][j + 1] != 1 && board[i + 3][j] != 1)
				result -= 7;
			if ((board[i + 1][j + 1] != 1 || board[i + 1][j + 1] != 2) && board[i + 2][j + 1] != 0 && board[i][j + 3] != 1 && board[i + 1][j + 2] != 1 && board[i + 3][j] != 1)
				result -= 7;
			if ((board[i + 2][j] != 1 || board[i + 2][j] != 2) && board[i + 3][j] != 0 && board[i][j + 3] != 1 && board[i + 1][j + 2] != 1 && board[i + 2][j + 1] != 1)
				result -= 7;
		}

	// ��� ������ ������ �ο�, ������ �θ� ����.
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			switch (i) {
			case 0:
				if (board[i][j] != 2)
					result++;
				else if (board[i][j] != 1)
					result--;
				break;

			case 1:
				if (board[i][j] != 2)
					result += 3;
				else if (board[i][j] != 1)
					result -= 3;
				break;

			case 2:
				if (board[i][j] != 2)
					result += 4;
				else if (board[i][j] != 1)
					result -= 4;
				break;

			case 3:
				if (board[i][j] != 2)
					result += 3;
				else if (board[i][j] != 1)
					result -= 3;
				break;

			case 4:
				if (board[i][j] != 2)
					result += 1;
				else if (board[i][j] != 1)
					result -= 1;
				break;

			case 5:
				result += 0;
				break;
			}
		}

	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			switch (j) {
			case 0:
				if (board[i][j] != 2)
					result++;
				else if (board[i][j] != 1)
					result--;
				break;

			case 1:
				if (board[i][j] != 2)
					result += 3;
				else if (board[i][j] != 1)
					result -= 3;
				break;

			case 2:
				if (board[i][j] != 2)
					result += 5;
				else if (board[i][j] != 1)
					result -= 5;
				break;

			case 3:
				if (board[i][j] != 2)
					result += 6;
				else if (board[i][j] != 1)
					result -= 6;
				break;

			case 4:
				if (board[i][j] != 2)
					result += 5;
				else if (board[i][j] != 1)
					result -= 5;
				break;

			case 5:
				if (board[i][j] != 2)
					result += 3;
				else if (board[i][j] != 1)
					result -= 3;
				break;

			case 6:
				if (board[i][j] != 2)
					result++;
				else if (board[i][j] != 1)
					result--;
				break;
			}
		}

	// ���� ���� ���θ����� ����. ���� ������ ������������ ������ Ŀ����.
	for (int i = 1; i <= R - 2; i++)
		for (int j = 1; j <= C - 2; j++) {
			// ���μ��� ����
			if (board[i][j] != 1 && board[i - 1][j] != 2 && board[i + 1][j] != 2 && board[i][j - 1] != 2 && board[i][j + 1] != 2)
				result += 4;
			else if ((board[i][j] != 1 && board[i - 1][j] != 2 && board[i + 1][j] != 2 && board[i][j - 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j] != 2 && board[i + 1][j] != 2 && board[i][j + 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j] != 2 && board[i][j - 1] != 2 && board[i][j + 1] != 2)
				|| (board[i][j] != 1 && board[i + 1][j] != 2 && board[i][j - 1] != 2 && board[i][j + 1] != 2))
				result += 3;
			else if ((board[i][j] != 1 && board[i - 1][j] != 2 && board[i + 1][j] != 2)
				|| (board[i][j] != 1 && board[i - 1][j] != 2 && board[i][j - 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j] != 2 && board[i][j + 1] != 2)
				|| (board[i][j] != 1 && board[i + 1][j] != 2 && board[i][j - 1] != 2)
				|| (board[i][j] != 1 && board[i + 1][j] != 2 && board[i][j + 1] != 2)
				|| (board[i][j] != 1 && board[i][j - 1] != 2 && board[i][j + 1] != 2))
				result += 2;
			else if ((board[i][j] != 1 && board[i - 1][j] != 2) || (board[i][j] != 1 && board[i + 1][j] != 2)
				|| (board[i][j] != 1 && board[i][j - 1] != 2) || (board[i][j] != 1 && board[i][j + 1] != 2))
				result += 1;
			if (board[i][j] != 2 && board[i - 1][j] != 1 && board[i + 1][j] != 1 && board[i][j - 1] != 1 && board[i][j + 1] != 1)
				result -= 4;
			else if ((board[i][j] != 2 && board[i - 1][j] != 1 && board[i + 1][j] != 1 && board[i][j - 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j] != 1 && board[i + 1][j] != 1 && board[i][j + 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j] != 1 && board[i][j - 1] != 1 && board[i][j + 1] != 1)
				|| (board[i][j] != 2 && board[i + 1][j] != 1 && board[i][j - 1] != 1 && board[i][j + 1] != 1))
				result -= 3;
			else if ((board[i][j] != 2 && board[i - 1][j] != 1 && board[i + 1][j] != 1)
				|| (board[i][j] != 2 && board[i - 1][j] != 1 && board[i][j - 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j] != 1 && board[i][j + 1] != 1)
				|| (board[i][j] != 2 && board[i + 1][j] != 1 && board[i][j - 1] != 1)
				|| (board[i][j] != 2 && board[i + 1][j] != 1 && board[i][j + 1] != 1)
				|| (board[i][j] != 2 && board[i][j - 1] != 1 && board[i][j + 1] != 1))
				result -= 2;
			else if ((board[i][j] != 2 && board[i - 1][j] != 1) || (board[i][j] != 2 && board[i + 1][j] != 1)
				|| (board[i][j] != 2 && board[i][j - 1] != 1) || (board[i][j] != 2 && board[i][j + 1] != 1))
				result -= 1;

			// �밢�� ����
			if (board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i - 1][j + 1] != 2 && board[i + 1][j - 1] != 2 && board[i + 1][j + 1] != 2)
				result += 4;
			else if ((board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i - 1][j + 1] != 2 && board[i + 1][j - 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i - 1][j + 1] != 2 && board[i + 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i + 1][j - 1] != 2 && board[i + 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j + 1] != 2 && board[i + 1][j - 1] != 2 && board[i + 1][j + 1] != 2))
				result += 3;
			else if ((board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i - 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i + 1][j - 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j - 1] != 2 && board[i + 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j + 1] != 2 && board[i + 1][j - 1] != 2)
				|| (board[i][j] != 1 && board[i - 1][j + 1] != 2 && board[i + 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i + 1][j - 1] != 2 && board[i + 1][j + 1] != 2))
				result += 2;
			else if ((board[i][j] != 1 && board[i - 1][j - 1] != 2) || (board[i][j] != 1 && board[i - 1][j + 1] != 2)
				|| (board[i][j] != 1 && board[i + 1][j - 1] != 2) || (board[i][j] != 1 && board[i + 1][j + 1] != 2))
				result += 1;
			if (board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i - 1][j + 1] != 1 && board[i + 1][j - 1] != 1 && board[i + 1][j + 1] != 1)
				result -= 4;
			else if ((board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i - 1][j + 1] != 1 && board[i + 1][j - 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i - 1][j + 1] != 1 && board[i + 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i + 1][j - 1] != 1 && board[i + 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j + 1] != 1 && board[i + 1][j - 1] != 1 && board[i + 1][j + 1] != 1))
				result -= 3;
			else if ((board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i - 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i + 1][j - 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j - 1] != 1 && board[i + 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j + 1] != 1 && board[i + 1][j - 1] != 1)
				|| (board[i][j] != 2 && board[i - 1][j + 1] != 1 && board[i + 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i + 1][j - 1] != 1 && board[i + 1][j + 1] != 1))
				result -= 2;
			else if ((board[i][j] != 2 && board[i - 1][j - 1] != 1) || (board[i][j] != 2 && board[i - 1][j + 1] != 1)
				|| (board[i][j] != 2 && board[i + 1][j - 1] != 1) || (board[i][j] != 2 && board[i + 1][j + 1] != 1))
				result -= 1;
		}

	return result;
}

// ���� ��Ÿ ����� ����
int minimax(int board[R][C], int depth, int turn, int alpha, int beta) {
	int e;
	int col;
	int n;

	if (depth == 0)
		return (heuristic(board)); // depth�� 0�� �����ϸ� ���ݱ����� ���� ������ heuristic ���� �����մϴ�.

	if (turn == 2) {
		for (col = 0; col < C; col++)
			if (board[R - 1][col] == 0) {  // ���� �ϳ��� ������Ű�鼭
				put(board, col, turn);     // ���� �ֺ��ϴ�.
				e = endgame(board, turn);
				if (e == 3) {
					beta = 0;
					rmv(board, col);
					return beta;
				}
				else if (e == turn) {
					beta = -10000;
					rmv(board, col);
					return beta;
				} // ������ ������ ���� �ٷιٷ� üũ�ؼ� �����մϴ�. �̷��� �ϸ� ���� depth�� �Ѿ�� �ʾƵ� �˴ϴ�.

				n = minimax(board, depth - 1, 3 - turn, alpha, beta); // ���� �� ���¿��� ���� depth�� �Ѿ�ϴ�.

				if (n < beta)
					beta = n;

				if (beta <= alpha) {
					rmv(board, col);
					return beta;
				}

				rmv(board, col); // �ξ��� ���� �����մϴ�.
			}
		return beta;
	}

	else {
		for (col = 0; col < C; col++)
			if (board[R - 1][col] == 0) {
				put(board, col, turn);
				e = endgame(board, turn);
				if (e == 3) {
					alpha = 0;
					rmv(board, col);
					return alpha;
				}
				else if (e == turn) {
					alpha = 10000;
					rmv(board, col);
					return alpha;
				}

				n = minimax(board, depth - 1, 3 - turn, alpha, beta);

				if (n > alpha)
					alpha = n;

				if (beta <= alpha) {
					rmv(board, col);
					return alpha;
				}

				rmv(board, col);
			}
		return alpha;
	}
}

// AI�� ���� ����Դϴ�.
int ai(int board[R][C], int depth) {
	int e, i;
	int c = 0, v = 0;
	int col, move;
	int record[C];
	int visited[C] = { 0, 0, 0, 0, 0, 0, 0 };
	int n, val = -10001;

	for (col = 0; col < C; col++)
		if (board[R - 1][col] == 0) {  // ���� ������Ű��
			put(board, col, 2);        // ���� ���ƺ��ϴ�.
			e = endgame(board, 2);     // minimax�� ������ ����, ���ƺ� ���� ������ �������� ���κ��� Ȯ���մϴ�.
			if (e == 2 || e == 3) {
				move = col;
				rmv(board, col);
				return move;           // �����ٸ�, �ٷ� �� ���� �����մϴ�.
			}
			n = minimax(board, depth, 1, -10001, +10001); // minimax Ʈ�� ��ġ�� �����մϴ�.
			record[col] = n; // ������ ����� �Ӵϴ�.

			printf("Score of column %d : %d\n", col + 1, n); // �� column������ �޸���ƽ ���� (�������� �����ϴ�.) 

			if (-n > val) {
				val = -n;
				move = col;
			}                // ���� ������ ���� ������ �����մϴ�.

			// ���� ������ ���� column���� �ִٸ� ��� ���� �켱�ؼ� �ε��� �մϴ�.
			else if (-n == val) {
				if (col == 3)
					move = col;
				else {
					if (move == 3)
						move;
					else {
						if (col == 2 || col == 4)
							move = col;
						else {
							if (move == 2 || move == 3 || move == 4)
								move;
							else {
								if (col == 1 || col == 5)
									move = col;
								else {
									if (move == 1 || move == 2 || move == 3 || move == 4 || move == 5)
										move;
									else
										move = col;
								}
							}
						}

					}
				}
			}

			visited[col] = 1;  // �� �� ���� �Ѿ�� ������ �� ���� ��ġ�ߴٴ� ���� ����մϴ�.
			v++;               // �̿� ����, v�� ��ġ�� ���� ������ �˴ϴ�.
			rmv(board, col);   // �ξ��� ���� �����մϴ�.
		}

	// ��ġ�� ���� ������ 10000�� ��츦 üũ�մϴ�. 
	for (i = 0; i < C; i++) {
		if (visited[i] == 1) {
			if (record[i] == 10000)
				c++; 
		}
	}

	// Depth ������ ��� ���� �й�� ����ȴٸ� �ٷ� ���� ���� �о� �ּ��� ���� ã���ϴ�.
	if (v == c && depth != 1) { 
		printf("\nSearching again for better solution...\n\n");
		ai(board, 1);
	} // ��ġ�� ���� ������ ��ġ�� ���� ������ 10000�� ����� ���� ������ depth�� 1�� ���߾� �ٽ� Ž���մϴ�.

	// ù �Ͽ� ��� �α� ������
	if (emptyspace(board) == 42) {
		if (move == 3)
			return move - 1;
	}

	return move;
}