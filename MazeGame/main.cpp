//
//  main.cpp
//  MazeGame
//
//  Created by Jayyoung Yang on 02/04/2018.
//  Copyright © 2018 Jayyoung Yang. All rights reserved.
//

#include <iostream>
#import "myconio_mac.h"


using namespace std;

/*
 0: 벽
 1: 길
 2: 시작점
 3: 도착점
 4: 폭탄
 */

struct _tagPoint
{
    int x;
    int y;
};

//타입을 재정의 한다.
typedef _tagPoint Point;
typedef _tagPoint* PPoint;

void setMaze(char maze[20][20], PPoint pPlayerPos, PPoint pStartPos, PPoint pEndPos){
    pStartPos->x = 0;
    pStartPos->y = 0;
    
    pEndPos->x = 19;
    pEndPos->y = 19;
    
    *pPlayerPos = *pStartPos;
    
    strcpy(maze[0],  "21000000000000000000");
    strcpy(maze[1],  "01111111111100000000");
    strcpy(maze[2],  "00100010000111111100");
    strcpy(maze[3],  "01100010000000000100");
    strcpy(maze[4],  "01000011110001111100");
    strcpy(maze[5],  "01000000001111000000");
    strcpy(maze[6],  "01100000001000000000");
    strcpy(maze[7],  "00100000001111111000");
    strcpy(maze[8],  "00001110000000001000");
    strcpy(maze[9],  "01111011111111111000");
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

void output(char maze[20][20], PPoint pPlayerPos) {
    for(int i=0;i <20; i++) {
        for(int j = 0; j < 20; j++) {
            if (maze[i][j] == '4') {
                cout << "💣";
            }
            else if(pPlayerPos->x == j && pPlayerPos->y == i){
                cout << "🦊";
            }
            else if(maze[i][j] == '0') {
                cout << "⬜️";
            } else if (maze[i][j] == '1'){
                cout << "  ";
            } else if(maze[i][j] == '2') {
                cout << "🔘";
            } else if(maze[i][j] == '3') {
                cout << "🔴";
            }
        }
        cout << endl;
    }
}

void moveUp(char maze[20][20], PPoint pPlayerPos) {
    if(pPlayerPos->y - 1 >= 0) {
        // 벽인지 체크한다.
        if(maze[pPlayerPos->y - 1][pPlayerPos->x] != '0' && 
            maze[pPlayerPos->y - 1][pPlayerPos->x] != '4') {
            pPlayerPos->y--;
    }
}
}

void moveDown(char maze[20][20], PPoint pPlayerPos) {
    if(pPlayerPos->y + 1 <= 19) {
        if(maze[pPlayerPos->y + 1][pPlayerPos->x] != '0' &&
            maze[pPlayerPos->y + 1][pPlayerPos->x] != '4') {
            pPlayerPos->y++;
    }
}
}

void moveLeft(char maze[20][20], PPoint pPlayerPos) {
    if(pPlayerPos->x - 1 >= 0) {
        if(maze[pPlayerPos->y][pPlayerPos->x - 1] != '0' &&
            maze[pPlayerPos->y][pPlayerPos->x - 1] != '4') {
            pPlayerPos->x--;
    }
}
}

void moveRight(char maze[20][20], PPoint pPlayerPos) {
    if(pPlayerPos->x + 1 <= 19) {
        if(maze[pPlayerPos->y][pPlayerPos->x + 1] != '0' &&
            maze[pPlayerPos->y][pPlayerPos->x + 1] != '4') {
            pPlayerPos->x++;
    }
}
}

void movePlayer(char maze[20][20], PPoint pPlayerPos, char cInput) {
    switch(cInput) {
        case 'w':
        case 'W':
        moveUp(maze, pPlayerPos);
        break;
        case 's':
        case 'S':
        moveDown(maze, pPlayerPos);
        break;
        case 'a':
        case 'A':
        moveLeft(maze, pPlayerPos);
        break;
        case 'd':
        case 'D':
        moveRight(maze, pPlayerPos);
        break;
    }
}

// 포인터 변수를 const로 생성하면 가리키는 대상의 값을 변경할 수 없다.
void createBomb(char maze[20][20] ,const PPoint pPlayer, PPoint pBombArr, int* pBombCount) {
    if(*pBombCount == 5) return;

    for(int i=0; i<*pBombCount; i++) {
        if(pPlayer->x == pBombArr[i].x && pPlayer->y == pBombArr[i].y) {
            return;
        }
    }

    pBombArr[*pBombCount] = *pPlayer;
    (*pBombCount)++;

    maze[pPlayer->y][pPlayer->x] = '4';
}

void fire(char maze[20][20], PPoint pPlayer, PPoint pBombArr, int* pBombCount) {
    for(int i=0; i<*pBombCount; i++) {
        maze[pBombArr[i].y][pBombArr[i].x] = '1';

        //플레어가 폭탄에 맞았을 때 시작점으로 보낸다.
        if(pPlayer->x == pBombArr[i].x && pPlayer->y == pBombArr[i].y){
            pPlayer->x = pPlayer->y = 0;
        }
        if(pBombArr[i].y - 1 >= 0) {
            if(maze[pBombArr[i].y - 1][pBombArr[i].x] == '0'){
                maze[pBombArr[i].y - 1][pBombArr[i].x] = '1';
            }
            if(pPlayer->x == pBombArr[i].x && pPlayer->y == pBombArr[i].y - 1){
                pPlayer->x = pPlayer->y = 0;
            }
        }

        if(pBombArr[i].y + 1 < 20) {
            if(maze[pBombArr[i].y + 1][pBombArr[i].x] == '0'){
                maze[pBombArr[i].y + 1][pBombArr[i].x] = '1';
            }
            if(pPlayer->x == pBombArr[i].x && pPlayer->y == pBombArr[i].y + 1){
                pPlayer->x = pPlayer->y = 0;
            }
        }

        if(pBombArr[i].x - 1 >= 0) {
            if(maze[pBombArr[i].y][pBombArr[i].x - 1] == '0'){
                maze[pBombArr[i].y][pBombArr[i].x - 1] = '1';
            }

            if(pPlayer->x == pBombArr[i].x - 1 && pPlayer->y == pBombArr[i].y){
                pPlayer->x = pPlayer->y = 0;
            }
        }

        if(pBombArr[i].x + 1 < 20) {
            if(maze[pBombArr[i].y][pBombArr[i].x + 1] == '0'){
                maze[pBombArr[i].y][pBombArr[i].x + 1] = '1';
            }

            if(pPlayer->x == pBombArr[i].x + 1&& pPlayer->y == pBombArr[i].y){
                pPlayer->x = pPlayer->y = 0;
            }
        }
    }

    *pBombCount = 0;
}

int main(){
    char strMaze[20][20];

    Point tPlayerPos;
    Point tStartPos;
    Point tEndPos;

    int iBombCount = 0;

    Point tBombPos[5];

    //미로를 설정한다.
    setMaze(strMaze, &tPlayerPos, &tStartPos, &tEndPos);

    while(true) {
        clrscr();
        //미로를 출력한다.
        output(strMaze, &tPlayerPos);

        if(tPlayerPos.x == tEndPos.x && tPlayerPos.y == tEndPos.y){
            cout << "Finish!" << endl;
            system("read");
            break;
        }

        cout << "t: bomb install, u: run bomb" << endl;
        cout << "w: up, s: down, a: left, d: right, q: quit: ";

        char cInput = (char)getch();

        if(cInput == 'q' || cInput == 'Q') break;

        if(cInput == 't' || cInput == 'T') {
            createBomb(strMaze,&tPlayerPos, tBombPos, &iBombCount);
        }
        if(cInput == 'u' || cInput == 'U') {
            fire(strMaze, &tPlayerPos, tBombPos, &iBombCount);
        }


        movePlayer(strMaze, &tPlayerPos, cInput);
    }
    //github test
    return 0;
}
