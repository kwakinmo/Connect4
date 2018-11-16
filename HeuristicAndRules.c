#include <stdio.h>
#include <stdbool.h>
#include "HeuristicAndRules.h"

// 보드의 빈 칸이 몇 개인지 확인.
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

// 해당 열에서 비어있는 가장 아랫칸에 돌을 놓습니다.
int put(int board[R][C], int column, int player) {

	int i;

	for (i = 0; i < R; i++) {
		if (board[i][column] == 0) {
			board[i][column] = player;
			return i + 1;
		}
	}
}

// 해당 열의 맨 윗돌을 제거합니다.
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
	// 우선순위 별로 rule 나열.
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
		int worst = checkworst(board, i); // 각 경우에 worst를 체크해보자. (rule3)
		if (rule4candidates[i] == 1) { // candidates이고,
			if (worst == -1) { // worst가 없으면,
				printf("Rule4\n");
				return i; // 둬라.
			}
		}
		if (worst == 1) { // worst 있으면,
			dontDoThat[i] = 1; // 두지말아야하는 곳들 check.
		}
	}

	int rule5candidates[7] = { 0, };
	checklose2(board, rule5candidates); // Rule 5 

	for (int i = 0; i < C; i++) {
		int worst = checkworst(board, i); // 각 경우에 worst를 체크해보자. (rule3)
		if (rule5candidates[i] == 1) { // candidates이고,
			if (worst == -1) { // worst가 없으면,
				printf("Rule5\n");
				return i; // 둬라.
			}
		}
		if (worst == 1) { // worst 있으면,
			dontDoThat[i] = 1; // 두지말아야하는 곳들 check.
		}
	}
	// 여기까지 필수적인거 // 이렇게 다섯개 하면 될듯!

	int rule6candidates[7] = { 0, };
	checkwin3(board, rule6candidates); // Rule 6 

	for (int i = 0; i < C; i++) {
		int worst = checkworst(board, i); // 각 경우에 worst를 체크해보자. (rule3)
		if (rule6candidates[i] == 1) { // candidates이고,
			if (worst == -1) { // worst가 없으면,
				printf("Rule6\n");
				return i; // 둬라.
			}
		}
		if (worst == 1) { // worst 있으면,
			dontDoThat[i] = 1; // 두지말아야하는 곳들 check.
		}
	}

	// Rule 7 : 둬도 되는 곳 중에서 상대방에 놓은 위치 위에 두자. 
	for (int i = 0; i < C; i++) {
		if (dontDoThat[i] == 0 && opponent == i && board[5][i] == 0) {
			printf("Rule6\n");
			return i;
		}
	}

	// Rule 8 : 가득차있으면, 가운데부터 두자.
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

bool checkPutPossible(int board[R][C], int row, int col) { // 밑에 안차있으면 둘수 X
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
	// Rule3 : 내가 뒀을 때 상대방이 다음 수에 이기게 되면 그거는 못두게 끔 하자.
	// candidates로 action을 각각 진행했을 때, 지게 되는지 확인해보자.

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
		if (endgame(map, 1) == 1) // 상대방이 이기면 두면 안 됨!
			return 1;
	}
	return -1;
}

void checkwin3(int board[R][C], int rule6candidates[7]) {
	// rule6(checkwin3) : 확실히 이기는 경우는 아니지만 상대방을 위협할 수 있는 수(0220, 0022, 2200, 2002, 2020,0202)도 우선순위 밑에 넣어서 좀 더 유리한 형세를 띨 수 있도록 하였습니다.

	// 가로 check // 0220 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 대각선 check // 0220
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3))
			{
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 0220
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 가로 check // 0022 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 1] = 1;
			}
		}
	}
	// 대각선 check // 0022
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 0022
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 1] = 1;
			}
		}
	}
	// 가로 check // 2200 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 대각선 check // 2200
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 2200
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j + 2] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 가로 check // 0202
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// 대각선 check // 0202
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 0202
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 2) {
				rule6candidates[j] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// 가로 check // 2020
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 대각선 check // 2020
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 2020
	for (int i = 5; i > 2; i--) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3)) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 3] = 1;
			}
		}
	}// 가로 check // 2002
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2) && board[i][j + 3] == 2) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// 대각선 check // 2002
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 2 && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 0) {
				rule6candidates[j + 1] = 1;
				rule6candidates[j + 2] = 1;
			}
		}
	}
	// 반대 대각선 체크 // 2002
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
	// Rule5 : 내가 막지 않으면 두 수 이후에 지게 되는 경우를 한 수전에 미리 막는다. // 00110,01100,01010
	// 나나 상대방이 두는 거에 따라 실제로 더 질 수 있는 경우가 생길 수도 있지만, 완전히 수를 내다보고 막는 경우는 우선 패스.
	// 가로 check // 00110
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 1 && board[i][j + 3] == 1 && board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4))
			{
				rule5candidates[j] = 1;
				rule5candidates[j + 1] = 1;  // 막는 곳은 j,j+1,j+4 중에 하나인데 이거 가중치를 줄까? ㅇㅇ 그래야할 듯 싶다ㅠ 여기는 무조건 우선 다 넣어야한다!
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// 대각선 check // 00110
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
	// 반대 대각선 체크 // 00110
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
	// 가로 check // 01100
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
	// 대각선 check // 01100
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
	// 반대 대각선 체크 // 01100
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


	// 가로 check // 01010 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 1 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 1 && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
			{
				rule5candidates[j] = 1;  // 가운데를 막는게 가장 낫지만 우선 다 후보군에 넣어주자. // 나중에 가중치 다르게 주자 // 여기도 가중치 꼭 고민하자!!
				rule5candidates[j + 2] = 1;
				rule5candidates[j + 4] = 1;
			}
		}
	}
	// 대각선 check // 01010
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
	// 반대 대각선 체크 // 01010
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
	// Rule 4 : 내가 두고 다음 나의 수에 이기는 수면 둔다.  // 02200, 00220, 02020 이거는 확실히 이기는 거.
	// 상대방이 두는 거에 따라 실제로 더 이길 수 있는 경우가 생길 수도 있지만, 확실히 이기게끔 하는 경우만 우선 코딩함.

	// 가로 check // 02200 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && board[i][j + 2] == 2 && board[i][j + 3] == 0 && checkPutPossible(board, i, j + 3) && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// 대각선 check // 02200
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 0 && checkPutPossible(board, i + 3, j + 3) && (board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// 반대 대각선 체크 // 02200
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 0 && checkPutPossible(board, i - 3, j + 3) && (board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4)))
				rule4candidates[j + 3] = 1;
		}
	}
	// 가로 check // 00220 
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 0 && checkPutPossible(board, i, j + 1) && board[i][j + 2] == 2 && board[i][j + 3] == 2 && board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}
	// 대각선 check // 00220
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 0 && checkPutPossible(board, i + 1, j + 1) && board[i + 2][j + 2] == 2 && board[i + 3][j + 3] == 2 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}
	// 반대 대각선 체크 // 00220
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 0 && checkPutPossible(board, i - 1, j + 1) && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
				rule4candidates[j + 1] = 1;
		}
	}

	// 가로 check // 02020
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < 3; j++) {
			if ((board[i][j] == 0 && checkPutPossible(board, i, j)) && board[i][j + 1] == 2 && (board[i][j + 2] == 0 && checkPutPossible(board, i, j + 2)) && board[i][j + 3] == 2 && (board[i][j + 4] == 0 && checkPutPossible(board, i, j + 4)))
				rule4candidates[j + 2] = 1;
		}
	}
	// 대각선 check // 02020
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i + 1][j + 1] == 2 && board[i + 2][j + 2] == 0 && checkPutPossible(board, i + 2, j + 2) && board[i + 3][j + 3] == 2 && board[i + 4][j + 4] == 0 && checkPutPossible(board, i + 4, j + 4))
				rule4candidates[j + 2] = 1;
		}
	}
	// 반대 대각선 체크 // 02020
	for (int i = 5; i > 3; i--) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == 0 && checkPutPossible(board, i, j) && board[i - 1][j + 1] == 2 && board[i - 2][j + 2] == 0 && checkPutPossible(board, i - 2, j + 2) && board[i - 3][j + 3] == 2 && board[i - 4][j + 4] == 0 && checkPutPossible(board, i - 4, j + 4))
				rule4candidates[j + 2] = 1;
		}
	}
	return;
}

int checklose(int board[R][C]) {
	// Rule 2 : 안두면 지는 경우에는 막는다.  0111,1011,1101,0111  
	// 가로 check // 0111,1011,1101,1110
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

	// 세로 check
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			if (board[i][j] == 1 && board[i + 1][j] == 1 && board[i + 2][j] == 1 && board[i + 3][i] == 0) //
				return j;
		}
	}

	// 대각선 check // 0111,1011,1101,1110
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

	// \ 반대대각선 check // 0111,1011,1101,1110
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
	// Rule 1 : 두면 이기는 경우엔 둔다.
	// 가로 check // 0222,2022,2202,2220
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

	// 세로 check
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			if (board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 2 && board[i + 3][i] == 0) // 2가 나의 컴퓨터
				return j;
		}
	}

	// 대각선 check // 0222,2022,2202,2220
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

	// \ 반대대각선 check // 0222,2022,2202,2220
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
	// Rule 0 : 상대가 두는 첫 수에 따라 나의 첫 수를 달리 둔다.
	// 상대가 첫 수를 1,2나 1,6으로 시작하는 경우에, 내가 1,4를 두면 비기지만 각각 1,3과 1,5에 두면 후수로도 이길 수 있다. (이런 경우는 거의 없을 것 같긴 하지만ㅎㅎ) 	
	// 이 밖의 경우는 모두 1,4에!

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
	if (count == 1) { // 후공하자마자 rule 쓰는 경우
		if (board[0][1] == 1) {
			return 2;
		}
		else if (board[0][5] == 1) {
			return 4;
		}
		else
			return 3;
	}
	else if (count == 0) { //선공을 rule로 시작하는 경우 첫수는 1,3에
		return 2;
	}
	else {
		if (board[0][3] == 0) // 첫수가 아닌데 중앙이 비어있는 경우
			return 3;
	}

	return -1;

}

/* 게임이 끝났는지 확인합니다. player의 돌이 4개 연속으로 이어지면 player를 리턴하고(player의 승리), 
   이어지지 않았고 더 둘 곳이 있으면 0(계속), 이어지지 않았는데 더 둘 곳도 없으면 3(비김)을 출력합니다. */
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

// 정해진 조건에 따라 가산, 감산이 이루어집니다.
int heuristic(int board[R][C]) {
	int result = 0;
	int i, j;

	// 가로 확인. 늘어선 돌의 수가 많을수록 가산 또는 감산. (4개는 이 함수로 넘어오기 전에 처리되므로 고려하지 않음.)
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
	// 이렇게 하면 두 개짜리를 검사할 때 세 개짜리에 두 번 점수를 준다. 당연히 이 또한 고려해서 점수를 계산했다. 아래 확인 2 경우도 마찬가지로 고려해서 점수를 줬다.

	// 가로 확인 2. 한 개의 돌과 두 개의 돌이 빈 공간 하나를 사이에 두고 늘어선 경우. (3개짜리와 동등한 점수를 받아야 한다고 생각.)
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

	// 세로 확인.
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

	// 세로는 중간이 비는 경우가 있을 수 없으므로 확인 2가 필요가 없다.

	// 좌상 대각선 확인.
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

	// 좌상 대각선 확인 2.
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

	// 우상 대각선 확인.
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

	// 우상 대각선 확인 2.
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

	// 한 개짜리 돌을 확인.
	for (i = 0; i < R; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2)
				result++;
			if (board[i][j] != 1)
				result--;
		}


	// 직후 4개를 완성시킬 수 있는 경우
	// 가로 4개 완성
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

	// 세로 4개 완성
	for (i = 0; i <= R - 4; i++)
		for (j = 0; j < C; j++) {
			if (board[i][j] != 2 && board[i + 1][j] != 2 && board[i + 2][j] != 2 && board[i + 3][j] != 0)
				result += 7;
			if (board[i][j] != 1 && board[i + 1][j] != 1 && board[i + 2][j] != 1 && board[i + 3][j] != 0)
				result -= 7;
		}

	// 대각선 4개 완성
	for (i = 0; i <= R - 5; i++)
		for (j = 0; j <= C - 4; j++) {
			// 대각선 맨 아래를 마지막으로 둬서 완성
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
			// 그 외
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

	// 행과 열마다 가산점 부여, 상대방이 두면 감점.
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

	// 상대방 돌에 가로막히면 감점. 많은 방향이 막혀있을수록 감점이 커진다.
	for (int i = 1; i <= R - 2; i++)
		for (int j = 1; j <= C - 2; j++) {
			// 가로세로 방향
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

			// 대각선 방향
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

// 알파 베타 프루닝 도입
int minimax(int board[R][C], int depth, int turn, int alpha, int beta) {
	int e;
	int col;
	int n;

	if (depth == 0)
		return (heuristic(board)); // depth가 0에 도달하면 지금까지의 보드 상태의 heuristic 값을 리턴합니다.

	if (turn == 2) {
		for (col = 0; col < C; col++)
			if (board[R - 1][col] == 0) {  // 열을 하나씩 증가시키면서
				put(board, col, turn);     // 돌을 둬봅니다.
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
				} // 게임이 끝나는 경우는 바로바로 체크해서 리턴합니다. 이렇게 하면 다음 depth로 넘어가지 않아도 됩니다.

				n = minimax(board, depth - 1, 3 - turn, alpha, beta); // 돌을 둔 상태에서 다음 depth로 넘어갑니다.

				if (n < beta)
					beta = n;

				if (beta <= alpha) {
					rmv(board, col);
					return beta;
				}

				rmv(board, col); // 두었던 돌을 제거합니다.
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

// AI의 동작 양상입니다.
int ai(int board[R][C], int depth) {
	int e, i;
	int c = 0, v = 0;
	int col, move;
	int record[C];
	int visited[C] = { 0, 0, 0, 0, 0, 0, 0 };
	int n, val = -10001;

	for (col = 0; col < C; col++)
		if (board[R - 1][col] == 0) {  // 열을 증가시키며
			put(board, col, 2);        // 돌을 놓아봅니다.
			e = endgame(board, 2);     // minimax를 돌리기 전에, 놓아본 돌이 게임을 끝내는지 여부부터 확인합니다.
			if (e == 2 || e == 3) {
				move = col;
				rmv(board, col);
				return move;           // 끝난다면, 바로 그 값을 리턴합니다.
			}
			n = minimax(board, depth, 1, -10001, +10001); // minimax 트리 서치를 시작합니다.
			record[col] = n; // 점수를 기록해 둡니다.

			printf("Score of column %d : %d\n", col + 1, n); // 각 column에서의 휴리스틱 점수 (낮을수록 좋습니다.) 

			if (-n > val) {
				val = -n;
				move = col;
			}                // 낮은 점수가 나올 때마다 갱신합니다.

			// 만약 점수가 같은 column들이 있다면 가운데 열을 우선해서 두도록 합니다.
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

			visited[col] = 1;  // 꽉 찬 열은 넘어가기 때문에 이 열은 서치했다는 것을 기록합니다.
			v++;               // 이에 따라, v는 서치한 열의 갯수가 됩니다.
			rmv(board, col);   // 두었던 돌을 제거합니다.
		}

	// 서치한 열의 점수가 10000인 경우를 체크합니다. 
	for (i = 0; i < C; i++) {
		if (visited[i] == 1) {
			if (record[i] == 10000)
				c++; 
		}
	}

	// Depth 내에서 모든 수가 패배로 연결된다면 바로 다음 수만 읽어 최선의 수를 찾습니다.
	if (v == c && depth != 1) { 
		printf("\nSearching again for better solution...\n\n");
		ai(board, 1);
	} // 서치한 열의 개수와 서치한 열의 점수가 10000인 경우의 수가 같으면 depth를 1로 낮추어 다시 탐색합니다.

	// 첫 턴에 가운데 두기 방지용
	if (emptyspace(board) == 42) {
		if (move == 3)
			return move - 1;
	}

	return move;
}