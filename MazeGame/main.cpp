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
 */

void setMaze(char maze[20][20]){
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

void output(char maze[20][20]) {
    for(int i=0;i <20; i++) {
        for(int j = 0; j < 20; j++) {
            if(maze[i][j] == '0') {
                cout << '#';
            } else if (maze[i][j] == '1'){
                cout << ' ';
            } else if(maze[i][j] == '2') {
                cout << '*';
            } else if(maze[i][j] == '3') {
                cout << '@';
            }
        }
        cout << endl;
    }
}

int main(){
    char strMaze[20][20];
    
    //미로를 설정한다.
    setMaze(strMaze);
    
    while(true) {
        clrscr();
        //미로를 출력한다.
        output(strMaze);

        cout << "w: up, s: down, a: left, d: right, q: quit: ";

        char cInput = (char)getch();

        if(cInput == 'q' || cInput == 'Q') break;

    }
    return 0;
}
