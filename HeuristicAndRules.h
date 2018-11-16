#include <stdio.h>
#include <stdbool.h>

#define R 6 //row
#define C 7 //column

int emptyspace(int board[R][C]);                  // 보드의 빈 칸이 몇 개인지 셉니다.
int  put(int board[R][C], int pos, int player);   // pos 열에 해당 player의 돌을 둡니다.
void rmv(int board[R][C], int column);            // column 열 맨 위에 있는 돌을 하나 지웁니다.

int useRule(int board[R][C], int opponent);                             // 사용할 룰을 결정합니다.
int checkcenter(int board[R][C]);                                       // 첫 수를 결정합니다.
bool checkPutPossible(int board[R][C], int row, int col);               // 그 밑에 돌이 있는지 없는지를 통해 둘 수 있는 곳인지 판단합니다.
int checkwin(int board[R][C]);                                          // 여기 두면 이기는 경우.
int checklose(int board[R][C]);                                         // 막지 않으면 지는 경우.
void checkwin2(int board[R][C], int rule4candidates[7]);                // 여기 두면 그 다음 수에 반드시 이기는 경우.
void checklose2(int board[R][C], int rule5candidates[7]);               // 막지 않으면 두 수 이후에 지게 되는 경우.
void checkwin3(int board[R][C], int rule6candidates[7]);                // 이기진 않아도 유리하게 이끌어갈 수 있는 경우.
int checkworst(int board[R][C], int col);                               // 내가 두면 상대방이 이기게 되는 경우를 방지합니다.
int  endgame(int board[R][C], int player);                              // 게임이 끝나는지 여부를 체크합니다.
int  heuristic(int board[R][C]);                                        // 휴리스틱 점수를 반환합니다.
int minimax(int board[R][C], int depth, int turn, int alpha, int beta); // 알파 베타 프루닝을 이용해 민맥스를 서치합니다.
int  ai(int board[R][C], int depth);                                    // AI의 행동 양상입니다.