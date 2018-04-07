//
//  main.cpp
//  MazeGame
//
//  Created by Jayyoung Yang on 02/04/2018.
//  Copyright © 2018 Jayyoung Yang. All rights reserved.
//

#import "myconio_mac.h"
#include <iostream>
#include <time.h>

using namespace std;

/*
   0: 벽
   1: 길
   2: 시작점
   3: 도착점
   4: 폭탄
   5: 파워 아이템
   6: 벽밀기 아이템
   7: 투명 아이템
   8: 웜홀
 */

struct _tagPoint {
        int x;
        int y;
};

//타입을 재정의 한다.
typedef _tagPoint Point;
typedef _tagPoint *PPoint;

typedef struct _tagPlayer {
        _tagPoint tPos;
        bool bWallPush;
        bool bTransparency;
        int iBombPower;
} PLAYER, *PPLAYER;

void setMaze(char maze[20][20], PPLAYER pPlayer, PPoint pStartPos,
             PPoint pEndPos) {
        pStartPos->x = 0;
        pStartPos->y = 0;

        pEndPos->x = 19;
        pEndPos->y = 19;

        pPlayer->tPos = *pStartPos;

        strcpy(maze[0], "21000000000000000000");
        strcpy(maze[1], "01111111111100000000");
        strcpy(maze[2], "00100010000111111100");
        strcpy(maze[3], "01100010000000000100");
        strcpy(maze[4], "01000011110001111100");
        strcpy(maze[5], "01000000001111000000");
        strcpy(maze[6], "01100000001000000000");
        strcpy(maze[7], "00100000001111111000");
        strcpy(maze[8], "00001110000000001000");
        strcpy(maze[9], "01111011111111111000");
        strcpy(maze[10], "01000000000000000000");
        strcpy(maze[11], "01111100111111100000");
        strcpy(maze[12], "00000111100000111110");
        strcpy(maze[13], "01111100000000000010");
        strcpy(maze[14], "01000000001111111110");
        strcpy(maze[15], "01111110011000000000");
        strcpy(maze[16], "01000010010000000000");
        strcpy(maze[17], "01111110011111000000");
        strcpy(maze[18], "01000000000001100000");
        strcpy(maze[19], "11000000000000111113");
}

void output(char maze[20][20], PPLAYER pPlayer) {
        for (int i = 0; i < 20; i++) {
                for (int j = 0; j < 20; j++) {
                        if (maze[i][j] == '4') {
                                cout << "💣";
                        } else if (pPlayer->tPos.x == j && pPlayer->tPos.y == i) {
                                cout << "🦊";
                        } else if (maze[i][j] == '0') {
                                cout << "⬜️";
                        } else if (maze[i][j] == '1') {
                                cout << "  ";
                        } else if (maze[i][j] == '2') {
                                cout << "🔘";
                        } else if (maze[i][j] == '3') {
                                cout << "🔴";
                        } else if (maze[i][j] == '5') {
                                cout << "👹";
                        } else if (maze[i][j] == '6') {
                                cout << "🙌";
                        } else if (maze[i][j] == '7') {
                                cout << "👻";
                        }
                }
                cout << endl;
        }

        cout << "Bomb power: " << pPlayer->iBombPower << endl;
        cout << "Wall passing: ";
        if (pPlayer->bTransparency) {
                cout << "ON\t";
        } else {
                cout << "OFF\t";
        }
        cout << "Wall push: ";
        if (pPlayer->bWallPush) {
                cout << "ON" << endl;
        } else {
                cout << "OFF" << endl;
        }
}

bool addItem(char cItemType, PPLAYER pPlayer) {
        if (cItemType == '5') {
                if (pPlayer->iBombPower < 5) {
                        pPlayer->iBombPower++;
                }
                return true;
        } else if (cItemType == '6') {
                pPlayer->bWallPush = true;
                return true;
        } else if (cItemType == '7') {
                pPlayer->bTransparency = true;
                return true;
        }
        return false;
}

void moveUp(char maze[20][20], PPLAYER pPlayer) {
        if (pPlayer->tPos.y - 1 >= 0) {
                // 벽인지 체크한다.
                if (maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] != '0' &&
                    maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] != '4') {
                        pPlayer->tPos.y--;
                } else if (pPlayer->bTransparency) {
                        pPlayer->tPos.y--;
                }
                if (addItem(maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer)) {
                        maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
                }
        }
}

void moveDown(char maze[20][20], PPLAYER pPlayer) {
        if (pPlayer->tPos.y + 1 <= 19) {
                if (maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] != '0' &&
                    maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] != '4') {
                        pPlayer->tPos.y++;
                } else if (pPlayer->bTransparency) {
                        pPlayer->tPos.y++;
                }
                if (addItem(maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer)) {
                        maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
                }
        }
}

void moveLeft(char maze[20][20], PPLAYER pPlayer) {
        if (pPlayer->tPos.x - 1 >= 0) {
                if (maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] != '0' &&
                    maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] != '4') {
                        pPlayer->tPos.x--;
                } else if (pPlayer->bTransparency) {
                        pPlayer->tPos.x--;
                }
                if (addItem(maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer)) {
                        maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
                }
        }
}

void moveRight(char maze[20][20], PPLAYER pPlayer) {
        if (pPlayer->tPos.x + 1 <= 19) {
                if (maze[pPlayer->tPos.y][pPlayer->tPos.x + 1] != '0' &&
                    maze[pPlayer->tPos.y][pPlayer->tPos.x + 1] != '4') {
                        pPlayer->tPos.x++;
                } else if (pPlayer->bTransparency) {
                        pPlayer->tPos.x++;
                }
                if (addItem(maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer)) {
                        maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
                }
        }
}

void movePlayer(char maze[20][20], PPLAYER pPlayer, char cInput) {
        switch (cInput) {
        case 'w':
        case 'W':
                moveUp(maze, pPlayer);
                break;
        case 's':
        case 'S':
                moveDown(maze, pPlayer);
                break;
        case 'a':
        case 'A':
                moveLeft(maze, pPlayer);
                break;
        case 'd':
        case 'D':
                moveRight(maze, pPlayer);
                break;
        }
}

// 포인터 변수를 const로 생성하면 가리키는 대상의 값을 변경할 수 없다.
void createBomb(char maze[20][20], const PPLAYER pPlayer, PPoint pBombArr,
                int *pBombCount) {
        if (*pBombCount == 5)
                return;

        if (maze[pPlayer->tPos.y][pPlayer->tPos.x] == '0') {
                return;
        }

        for (int i = 0; i < *pBombCount; i++) {
                if (pPlayer->tPos.x == pBombArr[i].x && pPlayer->tPos.y == pBombArr[i].y) {
                        return;
                }
        }

        pBombArr[*pBombCount] = pPlayer->tPos;
        (*pBombCount)++;

        maze[pPlayer->tPos.y][pPlayer->tPos.x] = '4';
}

void fire(char maze[20][20], PPLAYER pPlayer, PPoint pBombArr,
          int *pBombCount) {
        for (int i = 0; i < *pBombCount; i++) {
                maze[pBombArr[i].y][pBombArr[i].x] = '1';

                //플레어가 폭탄에 맞았을 때 시작점으로 보낸다.
                if (pPlayer->tPos.x == pBombArr[i].x && pPlayer->tPos.y == pBombArr[i].y) {
                        pPlayer->tPos.x = pPlayer->tPos.y = 0;
                }

                for (int j = 1; j <= pPlayer->iBombPower; j++) {
                        if (pBombArr[i].y - j >= 0) {
                                if (maze[pBombArr[i].y - j][pBombArr[i].x] == '0') {
                                        // 아이템 드랍 확률을 구한다.
                                        if(rand() % 100 < 20) {
                                                int iPercent = rand() % 100;
                                                if(iPercent < 70) {
                                                        maze[pBombArr[i].y - j][pBombArr[i].x] = '5';
                                                } else if (iPercent < 80) {
                                                        maze[pBombArr[i].y - j][pBombArr[i].x] = '6';
                                                } else {
                                                        maze[pBombArr[i].y - j][pBombArr[i].x] = '7';
                                                }
                                        } else {
                                                maze[pBombArr[i].y - j][pBombArr[i].x] = '1';
                                        }
                                }
                                if (pPlayer->tPos.x == pBombArr[i].x &&
                                    pPlayer->tPos.y == pBombArr[i].y - j) {
                                        pPlayer->tPos.x = pPlayer->tPos.y = 0;
                                }
                        }

                        if (pBombArr[i].y + j < 20) {
                                if (maze[pBombArr[i].y + j][pBombArr[i].x] == '0') {
                                        // 아이템 드랍 확률을 구한다.
                                        if(rand() % 100 < 20) {
                                                int iPercent = rand() % 100;
                                                if(iPercent < 70) {
                                                        maze[pBombArr[i].y + j][pBombArr[i].x] = '5';
                                                } else if (iPercent < 80) {
                                                        maze[pBombArr[i].y + j][pBombArr[i].x] = '6';
                                                } else {
                                                        maze[pBombArr[i].y + j][pBombArr[i].x] = '7';
                                                }
                                        } else
                                                maze[pBombArr[i].y + j][pBombArr[i].x] = '1';
                                }
                                if (pPlayer->tPos.x == pBombArr[i].x &&
                                    pPlayer->tPos.y == pBombArr[i].y + j) {
                                        pPlayer->tPos.x = pPlayer->tPos.y = 0;
                                }
                        }

                        if (pBombArr[i].x - j >= 0) {
                                if (maze[pBombArr[i].y][pBombArr[i].x - j] == '0') {
                                        // 아이템 드랍 확률을 구한다.
                                        if(rand() % 100 < 20) {
                                                int iPercent = rand() % 100;
                                                if(iPercent < 70) {
                                                        maze[pBombArr[i].y][pBombArr[i].x - j] = '5';
                                                } else if (iPercent < 80) {
                                                        maze[pBombArr[i].y][pBombArr[i].x - j] = '6';
                                                } else {
                                                        maze[pBombArr[i].y][pBombArr[i].x - j] = '7';
                                                }
                                        } else
                                                maze[pBombArr[i].y][pBombArr[i].x - j] = '1';
                                }
                                if (pPlayer->tPos.x == pBombArr[i].x - j &&
                                    pPlayer->tPos.y == pBombArr[i].y) {
                                        pPlayer->tPos.x = pPlayer->tPos.y = 0;
                                }
                        }

                        if (pBombArr[i].x + j < 20) {
                                if (maze[pBombArr[i].y][pBombArr[i].x + j] == '0') {
                                        // 아이템 드랍 확률을 구한다.
                                        if(rand() % 100 < 20) {
                                                int iPercent = rand() % 100;
                                                if(iPercent < 70) {
                                                        maze[pBombArr[i].y][pBombArr[i].x + j] = '5';
                                                } else if (iPercent < 80) {
                                                        maze[pBombArr[i].y][pBombArr[i].x + j] = '6';
                                                } else {
                                                        maze[pBombArr[i].y][pBombArr[i].x + j] = '7';
                                                }
                                        } else
                                                maze[pBombArr[i].y][pBombArr[i].x + j] = '1';
                                }

                                if (pPlayer->tPos.x == pBombArr[i].x + j &&
                                    pPlayer->tPos.y == pBombArr[i].y) {
                                        pPlayer->tPos.x = pPlayer->tPos.y = 0;
                                }
                        }
                }
        }

        *pBombCount = 0;
}

int main() {
        srand((unsigned int)time(0));

        char strMaze[20][20];

        // 0으로 초기화
        PLAYER tPlayer = {};
        Point tStartPos;
        Point tEndPos;

        tPlayer.iBombPower = 1;

        int iBombCount = 0;

        Point tBombPos[5];

        //미로를 설정한다
        setMaze(strMaze, &tPlayer, &tStartPos, &tEndPos);

        while (true) {
                clrscr();
                //미로를 출력한다.
                output(strMaze, &tPlayer);

                if (tPlayer.tPos.x == tEndPos.x && tPlayer.tPos.y == tEndPos.y) {
                        cout << "Finish!" << endl;
                        system("read");
                        break;
                }

                cout << "t: bomb install, u: run bomb, i: push a wall" << endl;
                cout << "w: up, s: down, a: left, d: right, q: quit: ";

                char cInput = (char)getch();

                if (cInput == 'q' || cInput == 'Q')
                        break;

                if (cInput == 't' || cInput == 'T') {
                        createBomb(strMaze, &tPlayer, tBombPos, &iBombCount);
                }
                if (cInput == 'u' || cInput == 'U') {
                        fire(strMaze, &tPlayer, tBombPos, &iBombCount);
                }
                movePlayer(strMaze, &tPlayer, cInput);
        }
        // github test
        return 0;
}
