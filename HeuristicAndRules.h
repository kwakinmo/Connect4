#include <stdio.h>
#include <stdbool.h>

#define R 6 //row
#define C 7 //column

int emptyspace(int board[R][C]);                  // ������ �� ĭ�� �� ������ ���ϴ�.
int  put(int board[R][C], int pos, int player);   // pos ���� �ش� player�� ���� �Ӵϴ�.
void rmv(int board[R][C], int column);            // column �� �� ���� �ִ� ���� �ϳ� ����ϴ�.

int useRule(int board[R][C], int opponent);                             // ����� ���� �����մϴ�.
int checkcenter(int board[R][C]);                                       // ù ���� �����մϴ�.
bool checkPutPossible(int board[R][C], int row, int col);               // �� �ؿ� ���� �ִ��� �������� ���� �� �� �ִ� ������ �Ǵ��մϴ�.
int checkwin(int board[R][C]);                                          // ���� �θ� �̱�� ���.
int checklose(int board[R][C]);                                         // ���� ������ ���� ���.
void checkwin2(int board[R][C], int rule4candidates[7]);                // ���� �θ� �� ���� ���� �ݵ�� �̱�� ���.
void checklose2(int board[R][C], int rule5candidates[7]);               // ���� ������ �� �� ���Ŀ� ���� �Ǵ� ���.
void checkwin3(int board[R][C], int rule6candidates[7]);                // �̱��� �ʾƵ� �����ϰ� �̲�� �� �ִ� ���.
int checkworst(int board[R][C], int col);                               // ���� �θ� ������ �̱�� �Ǵ� ��츦 �����մϴ�.
int  endgame(int board[R][C], int player);                              // ������ �������� ���θ� üũ�մϴ�.
int  heuristic(int board[R][C]);                                        // �޸���ƽ ������ ��ȯ�մϴ�.
int minimax(int board[R][C], int depth, int turn, int alpha, int beta); // ���� ��Ÿ ������� �̿��� �θƽ��� ��ġ�մϴ�.
int  ai(int board[R][C], int depth);                                    // AI�� �ൿ ����Դϴ�.