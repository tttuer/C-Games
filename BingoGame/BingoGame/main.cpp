//
//  main.cpp
//  BingoGame
//
//  Created by Jayyoung Yang on 22/03/2018.
//  Copyright © 2018 Jayyoung Yang. All rights reserved.
//

#include <iostream>
#include <time.h>

using namespace std;

enum AIMode {
    AM_EASY = 1,
    AM_HARD
};

enum LINE_NUMBER {
    LN_H1,
    LN_H2,
    LN_H3,
    LN_H4,
    LN_H5,
    LN_V1,
    LN_V2,
    LN_V3,
    LN_V4,
    LN_V5,
    LN_LT,
    LN_RT

};

//함수는 선언과 정의 부분으로 나눌 수 있다.
void setNumber(int *pArray);
void shuffle(int *pArray);
AIMode selectAIMode();
void outputNumber(int *pArray, int iBingo);
bool changeNumber(int *pArray, int iInput);
int selectAINumber(int *pArray, AIMode eMode);
int bingoCounting(int *pArray);
int bingoCountingH(int *pArray);
int bingoCountingV(int *pArray);
int bingoCountingLTD(int *pArray);
int bingoCountingRTD(int *pArray);

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(0));
    
    int iNumber[25] = {};
    int iAINumber[25] = {};

    setNumber(iNumber);
    setNumber(iAINumber);

    shuffle(iNumber);
    shuffle(iAINumber);
    
    int iBingo = 0, iAIBingo = 0;

    // choose AI's Level
    AIMode eAIMode = selectAIMode();

    while(true) {
        system("clear");
        cout << "================== Player ====================" << endl;
        outputNumber(iNumber, iBingo);
        
        cout << "================== AI ====================" << endl;
        
        switch (eAIMode) {
            case AM_EASY:
            cout << "AI Mode: Easy" <<endl;
            break;
            case AM_HARD:
            cout << "AI Mode: Hard" <<endl;
            default:
            break;
        }

        outputNumber(iAINumber, iAIBingo);
        
        if(iBingo >= 5) {
            cout << "Player win!" << endl;
            break;
        } else if (iAIBingo >= 5){
            cout << "AI win!" << endl;
            break;
        }

        cout << "enter number(0: exit)>> ";
        int iInput;
        cin >> iInput;
        
        if(iInput == 0) break;
        
        if(iInput > 25 || iInput < 1) continue;
        
        // 중복 입력을 체크하기 위한 변수이다.
        // 기본적으로 중복이 되었다고 하기 위해 true로 잡아주었다.
        bool bAcc = changeNumber(iNumber, iInput);
        
        if(bAcc) continue;

        //중복이 아니라면 AI의 숫자도 찾아내서 *로 바꿔준다.
        changeNumber(iAINumber, iInput);

        //AI가 숫자를 선택한다. AI 모드에 따라서 숫자를 선택하도록 함수가 구성되어 있다.
        iInput = selectAINumber(iAINumber, eAIMode);
        cout << "Selected Number by AI: " << iInput << endl;
        system("read");
        
        //AI가 숫자를 선택했으므로 플레이어와 AI의 숫자를 *로 바꿔준다.
        changeNumber(iNumber, iInput);
        changeNumber(iAINumber, iInput);

        iBingo = bingoCounting(iNumber);
        iAIBingo = bingoCounting(iAINumber);
    }
    return 0;
}

void setNumber(int *pArray) {
    for(int i=0; i<25; i++) {
        pArray[i] = i+1;
    }
}

void shuffle(int *pArray) {
    int iTemp, idx1,idx2;

    for(int i=0; i<100; i++) {

        idx1 = rand() % 25;
        idx2 = rand() % 25;
        
        iTemp = pArray[idx1];
        pArray[idx1] = pArray[idx2];
        pArray[idx2] = iTemp;
    }
}

AIMode selectAIMode() {
    int iAIMode;

    while(true) {
        system("clear");
        cout << "1. Easy" << endl;
        cout << "2. Hard" << endl;
        cout << "Choose AI Level: " << endl;
        cin >> iAIMode;
        
        if(iAIMode >= AM_EASY && iAIMode <= AM_HARD) break;
    }

    return (AIMode)iAIMode;
}

void outputNumber(int *pArray, int iBingo) {
    for(int i=0; i<5; i++) {
        for(int j=0; j<5; j++) {
            if(pArray[i*5 + j] == INT_MAX){
                cout << "*\t";
            } else {
                cout << pArray[i*5 + j] << "\t";
            }
        }
        cout << endl;
    }
    cout << "Bingo count: " << iBingo << endl<<endl;
}

bool changeNumber(int *pArray, int iInput) {
    for(int i=0; i<25; i++) {
        if (pArray[i] == iInput) {

            pArray[i] = INT_MAX;
            
            return false;
        }
    }

    // 여기까지 오면 중복된 숫자를 입력했다는 말이므로 true를 리턴한다.
    return true;
}

int selectAINumber(int *pArray, AIMode eMode) {
    int iNoneSelected[25] = {};
    int iNoneSelectedCount = 0;

    //AI가 선택을 한다. AI가 선택하는 것은 AI모드에 따라서 다르게 된다.
    switch (eMode) {
        case AM_EASY:
        iNoneSelectedCount = 0;

        for (int i=0; i<25; i++) {
            if(pArray[i] != INT_MAX) {
                iNoneSelected[iNoneSelectedCount] = pArray[i];
                iNoneSelectedCount++;
            }
        }

        return iNoneSelected[rand() % iNoneSelectedCount];
        break;

        case AM_HARD:
        int iLine = 0;
        int iStarCount = 0;
        int iSaveCount = 0;

        for (int i=0; i<5; i++) {
            iStarCount = 0;
            for (int j=0; j<5; j++) {
                if(pArray[i * 5 + j] == INT_MAX) {
                    iStarCount++;
                }
            }

            if(iStarCount < 5 && iSaveCount < iStarCount) {
                iLine = i;
                iSaveCount = iStarCount;
            }
        }

        for (int i=0; i<5; i++) {
            iStarCount = 0;
            for (int j=0; j<5; j++) {
                if(pArray[j * 5 + i] == INT_MAX) {
                    iStarCount++;
                }
            }

            if(iStarCount < 5 && iSaveCount < iStarCount) {
                iLine = i + 5;
                iSaveCount = iStarCount;
            }
        }

        iStarCount = 0;
        for(int i=0; i<25; i+=6) {
            if(pArray[i] == INT_MAX) {
                iStarCount++;
            }
        }

        if(iStarCount < 5 && iSaveCount < iStarCount) {
            iLine = LN_LT;
            iSaveCount = iStarCount;
        }

        iStarCount = 0;
        for(int i=4; i<21; i+=4) {
            if(pArray[i] == INT_MAX) {
                iStarCount++;
            }
        }

        if(iStarCount < 5 && iSaveCount < iStarCount) {
            iLine = LN_RT;
            iSaveCount = iStarCount;
        }

        if(iLine <= LN_H5) {
            for (int i=0; i<5; i++) {
                if(pArray[iLine * 5 + i] != INT_MAX) {
                    return pArray[iLine * 5 + i];
                    break;
                }
            }
        } else if(iLine <= LN_V5) {
            for (int i=0; i<5; i++) {
                if(pArray[i * 5 + iLine - 5] != INT_MAX) {
                    return pArray[i * 5 + iLine - 5];
                    break;
                }
            }
        } else if (iLine == LN_LT) {
            for (int i=0; i<25; i+=6) {
                if(pArray[i] != INT_MAX) {
                    return pArray[i];
                    break;
                }
            }
        } else if(iLine == LN_RT) {
            for (int i=4; i<21; i+=4) {
                if(pArray[i] != INT_MAX) {
                    return pArray[i];
                    break;
                }
            }
        }
        break;
    }

    return -1;
}

int bingoCounting(int *pArray) {
    int iBingo = 0;

    iBingo += bingoCountingH(pArray);
    iBingo += bingoCountingV(pArray);
    iBingo += bingoCountingLTD(pArray);
    iBingo += bingoCountingRTD(pArray);

    return iBingo;
}
int bingoCountingH(int *pArray) {
    //가로줄 수를 구해준다.
    int iStar1 = 0, iBingo = 0;

    for (int i=0; i<5; i++) {
        //한 줄을 체크하기 전에 0으로 초기화 해준다.
        iStar1 = 0;

        for(int j=0; j<5; j++) {
            //가로 별 수를 구해준다.
            if(pArray[i * 5 + j] == INT_MAX) {
                iStar1++;
            }
        }

        if(iStar1 == 5) {
            iBingo++;
        }
    }

    return iBingo;
}
int bingoCountingV(int *pArray) {
    //세로줄 수를 구해준다.
    int iStar1 = 0, iBingo = 0;

    for (int i=0; i<5; i++) {
        //한 줄을 체크하기 전에 0으로 초기화 해준다.
        iStar1 = 0;

        for(int j=0; j<5; j++) {
            //세로 별 수를 구해준다.
            if(pArray[j * 5 + i] == INT_MAX) {
                iStar1++;
            }
        }

        if(iStar1 == 5) {
            iBingo++;
        }
    }

    return iBingo;
}
int bingoCountingLTD(int *pArray) {
    //왼쪽 상단 -> 오른쪽 하단 대각선 체크
    int iStar1 = 0, iBingo = 0;

    for(int i=0 ; i<25; i += 6) {
        if(pArray[i] == INT_MAX){
            iStar1++;
        }
    }
    if(iStar1 == 5) {
        iBingo++;
    }

    return iBingo;
}
int bingoCountingRTD(int *pArray) {
    //오른쪽 상단 -> 왼쪽 하단 대각선 체크
    int iStar1 = 0, iBingo = 0;

    for(int i=4 ; i<21; i += 4) {
        if(pArray[i] == INT_MAX){
            iStar1++;
        }
    }
    if(iStar1 == 5) {
        iBingo++;
    }

    return iBingo;
}