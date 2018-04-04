//
//  main.cpp
//  TextRPG
//
//  Created by Jayyoung Yang on 26/03/2018.
//  Copyright © 2018 Jayyoung Yang. All rights reserved.
//

#include <iostream>
#include <time.h>

using namespace std;

#define NAME_SIZE       100

enum MAIN_MENU {
    MM_NONE,
    MM_MAP,
    MM_STORE,
    MM_INVENTORY,
    MM_EXIT
};

enum MAP_TYPE{
    MT_NONE,
    MT_EASY,
    MT_NORMAL,
    MT_HARD,
    MT_BACK
};

enum CLASS{
    CLASS_NONE,
    CLASS_KNIGHT,
    CLASS_ARCHER,
    CLASS_WIZARD,
    CLASS_END
};

enum BATTLE {
    BATTLE_NONE,
    BATTLE_ATTACK,
    BATTLE_BACK
};

enum ITEM_TYPE {
    IT_NONE,
    IT_WEAPON,
    IT_ARMOR,
    IT_BACK
};

enum STORE_MENU {
    SM_NONE,
    SM_WEAPON,
    SM_ARMOR,
    SM_BACK
};

enum EQUIP {
    EQ_WEAPON,
    EQ_ARMOR,
    EQ_MAX
};

#define ITEM_DESC_LENGTH        512
#define INVENTORY_MAX           20
#define STORE_WEAPON_MAX        3
#define STORE_ARMOR_MAX         3
#define LEVEL_MAX               10

struct _tagItem {
    char strName[NAME_SIZE];
    char strTypeName[NAME_SIZE];
    ITEM_TYPE eType;
    int iMin;
    int iMax;
    int iPrice;
    int iSell;
    char strDesc[ITEM_DESC_LENGTH];
};

struct _tagInventory {
    _tagItem tItem[INVENTORY_MAX];
    int iItemCount;
    int iGold;
};

struct _tagPlayer {
    char strName[NAME_SIZE];
    char strClassName[NAME_SIZE];
    CLASS eClass;
    int iAttackMin;
    int iAttackMax;
    int iArmorMin;
    int iArmorMax;
    int iHP;
    int iHPMax;
    int iMP;
    int iMPMax;
    int iEXP;
    int iLevel;
    _tagItem tEquip[EQ_MAX];
    bool bEquip[EQ_MAX];
    _tagInventory tInventory;
};

struct _tagMonster {
    char strName[NAME_SIZE];
    int iAttackMin;
    int iAttackMax;
    int iArmorMin;
    int iArmorMax;
    int iHP;
    int iHPMax;
    int iMP;
    int iMPMax;
    int iLevel;
    int iEXP;
    int iGoldMin;
    int iGoldMax;
};

struct _tagLevelUpStatus {
    int iAttackMin;
    int iAttackMax;
    int iArmorMin;
    int iArmorMax;
    int iHPMin;
    int iHPMax;
    int iMPMin;
    int iMPMax;
};

// 숫자 입력을 받는다. 입력 오류까지 처리해 주고 INT_MAX 값을 리턴하면 입력 오류이다.
int inputInt() {
    int iInput;
    cin >> iInput;

    if(cin.fail()) {
        cin.clear();
        cin.ignore(1024,'\n');
        return INT_MAX; //INT_MAX가 리턴이 되면 입력값이 잘못됐다는 뜻이다.
    }

    return iInput;
}

// 메인 메뉴를 화면에 보여주고 입력받은 메뉴를 반환한다.
// 입력 오류일 경우 혹은 잘못된 메뉴를 선택한 경우 MM_NONE을 반환해 준다.
int outputMainMenu() {
    system("clear");

    cout << "================== LOBBY =================" << endl;
    cout <<"1. Map" << endl;
    cout << "2. Store" << endl;
    cout << "3. Inventory" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose the menu: " ;

    int iMenu = inputInt();

    if(iMenu == INT_MAX || iMenu <= MM_NONE || iMenu > MM_EXIT) return MM_NONE;

    return iMenu;
}
void setLvUpTable(_tagLevelUpStatus *pArray, CLASS eClass);

int main(int argc, const char * argv[]) {
    // insert code here...
    srand((unsigned int)time(0));

    //레벨업에 필요한 경험치 목록을 만든다.
    const int iLevelUpEXP[LEVEL_MAX] = {4000, 10000, 20000, 35000, 50000, 70000, 100000, 150000, 200000, 500000};

    // CLASS_END를 사용해서 나중에 직업을 더 추가하더라도 크게 변화없이 사용 가능하게 한다.
    _tagLevelUpStatus tLvUpTable[CLASS_END - 1] = {};

    tLvUpTable[CLASS_KNIGHT - 1].iAttackMin = 4;
    tLvUpTable[CLASS_KNIGHT - 1].iAttackMax = 10;
    tLvUpTable[CLASS_KNIGHT - 1].iArmorMin = 8;
    tLvUpTable[CLASS_KNIGHT - 1].iArmorMax = 16;
    tLvUpTable[CLASS_KNIGHT - 1].iHPMin = 50;
    tLvUpTable[CLASS_KNIGHT - 1].iHPMax = 100;
    tLvUpTable[CLASS_KNIGHT - 1].iMPMin = 10;
    tLvUpTable[CLASS_KNIGHT - 1].iMPMax = 20;

    tLvUpTable[CLASS_ARCHER - 1].iAttackMin = 10;
    tLvUpTable[CLASS_ARCHER - 1].iAttackMax = 15;
    tLvUpTable[CLASS_ARCHER - 1].iArmorMin = 5;
    tLvUpTable[CLASS_ARCHER - 1].iArmorMax = 10;
    tLvUpTable[CLASS_ARCHER - 1].iHPMin = 30;
    tLvUpTable[CLASS_ARCHER - 1].iHPMax = 50;
    tLvUpTable[CLASS_ARCHER - 1].iMPMin = 30;
    tLvUpTable[CLASS_ARCHER - 1].iMPMax = 50;

    tLvUpTable[CLASS_WIZARD - 1].iAttackMin = 15;
    tLvUpTable[CLASS_WIZARD - 1].iAttackMax = 20;
    tLvUpTable[CLASS_WIZARD - 1].iArmorMin = 3;
    tLvUpTable[CLASS_WIZARD - 1].iArmorMax = 7;
    tLvUpTable[CLASS_WIZARD - 1].iHPMin = 20;
    tLvUpTable[CLASS_WIZARD - 1].iHPMax = 40;
    tLvUpTable[CLASS_WIZARD - 1].iMPMin = 50;
    tLvUpTable[CLASS_WIZARD - 1].iMPMax = 100;
    //게임을 시작할 때 플레이어 정보를 입력하도록 한다.
    _tagPlayer tPlayer = {};
    //플레이어 이름을 입력받는다.
    cout << "Enter the name: ";
    cin.getline(tPlayer.strName, NAME_SIZE - 1);
    
    int iClass = CLASS_NONE;
    while(iClass == CLASS_NONE) {
        system("clear");
        
        cout << "1. Knight" << endl;
        cout << "2. Archer" << endl;
        cout << "3. Wizard" << endl;
        cout << "Choose the class: ";
        cin >> iClass;
        
        if(cin.fail()) {
            cin.clear();
            cin.ignore(1024,'\n');
            continue;
        }
        
        if(iClass <= CLASS_NONE || iClass >= CLASS_END){
            iClass = CLASS_NONE;
        }
    }
    
    tPlayer.iLevel = 1;
    tPlayer.iEXP = 0;
    tPlayer.eClass = (CLASS)iClass;
    tPlayer.tInventory.iGold = 10000;
    
    switch (tPlayer.eClass) {
        case CLASS_KNIGHT:
        strcpy(tPlayer.strClassName, "Knight");
        tPlayer.iAttackMin = 5;
        tPlayer.iAttackMax = 10;
        tPlayer.iArmorMin = 15;
        tPlayer.iArmorMax = 20;
        tPlayer.iHP = 500;
        tPlayer.iHPMax = 500;
        tPlayer.iMP = 100;
        tPlayer.iMPMax = 100;
        break;
        case CLASS_ARCHER:
        strcpy(tPlayer.strClassName, "Archer");
        tPlayer.iAttackMin = 10;
        tPlayer.iAttackMax = 15;
        tPlayer.iArmorMin = 10;
        tPlayer.iArmorMax = 15;
        tPlayer.iHP = 400;
        tPlayer.iHPMax = 400;
        tPlayer.iMP = 200;
        tPlayer.iMPMax = 200;
        break;
        case CLASS_WIZARD:
        strcpy(tPlayer.strClassName, "Wizard");
        tPlayer.iAttackMin = 15;
        tPlayer.iAttackMax = 20;
        tPlayer.iArmorMin = 5;
        tPlayer.iArmorMax = 10;
        tPlayer.iHP = 300;
        tPlayer.iHPMax = 300;
        tPlayer.iMP = 300;
        tPlayer.iMPMax = 300;
        break;
        default:
        break;
    }
    
    //몬스터 생성
    _tagMonster tMonsterArr[MT_BACK - 1] = {};
    
    //고블린 생성
    strcpy(tMonsterArr[0].strName, "Goblin");
    tMonsterArr[0].iAttackMin = 20;
    tMonsterArr[0].iAttackMax = 30;
    tMonsterArr[0].iArmorMin  = 2;
    tMonsterArr[0].iArmorMax = 5;
    tMonsterArr[0].iHP = 100;
    tMonsterArr[0].iHPMax = 100;
    tMonsterArr[0].iMP = 10;
    tMonsterArr[0].iMPMax = 10;
    tMonsterArr[0].iLevel = 1;
    tMonsterArr[0].iEXP = 1000;
    tMonsterArr[0].iGoldMin = 500;
    tMonsterArr[0].iGoldMax = 1500;
    
    //트롤 생성
    strcpy(tMonsterArr[1].strName, "Troll");
    tMonsterArr[1].iAttackMin = 80;
    tMonsterArr[1].iAttackMax = 130;
    tMonsterArr[1].iArmorMin  = 60;
    tMonsterArr[1].iArmorMax = 90;
    tMonsterArr[1].iHP = 2000;
    tMonsterArr[1].iHPMax = 2000;
    tMonsterArr[1].iMP = 100;
    tMonsterArr[1].iMPMax = 100;
    tMonsterArr[1].iLevel = 5;
    tMonsterArr[1].iEXP = 7000;
    tMonsterArr[1].iGoldMin = 6000;
    tMonsterArr[1].iGoldMax = 8000;
    
    //드래곤 생성
    strcpy(tMonsterArr[2].strName, "Drangon");
    tMonsterArr[2].iAttackMin = 250;
    tMonsterArr[2].iAttackMax = 500;
    tMonsterArr[2].iArmorMin  = 200;
    tMonsterArr[2].iArmorMax = 400;
    tMonsterArr[2].iHP = 30000;
    tMonsterArr[2].iHPMax = 30000;
    tMonsterArr[2].iMP = 20000;
    tMonsterArr[2].iMPMax = 20000;
    tMonsterArr[2].iLevel = 10;
    tMonsterArr[2].iEXP = 30000;
    tMonsterArr[2].iGoldMin = 20000;
    tMonsterArr[2].iGoldMax = 50000;

    //상점에서 판매할 아이템 목록을 생성한다.
    _tagItem tStoreWeapon[STORE_WEAPON_MAX] = {};
    _tagItem tStoreArmor[STORE_ARMOR_MAX] = {};

    //각 아이템 정보들을 설정해준다.
    //무기 정보 추가
    strcpy(tStoreWeapon[0].strName, "Knife");
    strcpy(tStoreWeapon[0].strTypeName, "Weapon");
    tStoreWeapon[0].eType = IT_WEAPON;
    tStoreWeapon[0].iMin = 5;
    tStoreWeapon[0].iMax = 10;
    tStoreWeapon[0].iPrice = 1000;
    tStoreWeapon[0].iSell = 500;
    strcpy(tStoreWeapon[0].strDesc, "Knife for beginner");

    strcpy(tStoreWeapon[1].strName, "Bow");
    strcpy(tStoreWeapon[1].strTypeName, "Weapon");
    tStoreWeapon[1].eType = IT_WEAPON;
    tStoreWeapon[1].iMin = 20;
    tStoreWeapon[1].iMax = 40;
    tStoreWeapon[1].iPrice = 7000;
    tStoreWeapon[1].iSell = 3500;
    strcpy(tStoreWeapon[1].strDesc, "Bow for beginner");

    strcpy(tStoreWeapon[2].strName, "Wand");
    strcpy(tStoreWeapon[2].strTypeName, "Weapon");
    tStoreWeapon[2].eType = IT_WEAPON;
    tStoreWeapon[2].iMin = 90;
    tStoreWeapon[2].iMax = 150;
    tStoreWeapon[2].iPrice = 30000;
    tStoreWeapon[2].iSell = 15000;
    strcpy(tStoreWeapon[2].strDesc, "Wand for beginner");

    //방어구 정보 추가
    strcpy(tStoreArmor[0].strName, "Cloth Armor");
    strcpy(tStoreArmor[0].strTypeName, "Armor");
    strcpy(tStoreArmor[0].strDesc, "Armor made with cloth");
    tStoreArmor[0].eType = IT_ARMOR;
    tStoreArmor[0].iMin = 2;
    tStoreArmor[0].iMax = 5;
    tStoreArmor[0].iPrice = 1000;
    tStoreArmor[0].iSell = 500;

    strcpy(tStoreArmor[1].strName, "Leather Armor");
    strcpy(tStoreArmor[1].strTypeName, "Armor");
    strcpy(tStoreArmor[1].strDesc, "Armor made with leather");
    tStoreArmor[1].eType = IT_ARMOR;
    tStoreArmor[1].iMin = 10;
    tStoreArmor[1].iMax = 20;
    tStoreArmor[1].iPrice = 7000;
    tStoreArmor[1].iSell = 3500;

    strcpy(tStoreArmor[2].strName, "Full Plate Armor");
    strcpy(tStoreArmor[2].strTypeName, "Armor");
    strcpy(tStoreArmor[2].strDesc, "Armor made with plate");
    tStoreArmor[2].eType = IT_ARMOR;
    tStoreArmor[2].iMin = 70;
    tStoreArmor[2].iMax = 90;
    tStoreArmor[2].iPrice = 30000;
    tStoreArmor[2].iSell = 15000;

    bool bLoop = true;

    while(bLoop) {

        switch (outputMainMenu()) {
            case MM_MAP:
            while(true) {
                system("clear");
                cout << "================== MAP =================" << endl;
                cout <<"1. Easy" << endl;
                cout <<"2. Normal" << endl;
                cout <<"3. Hard" << endl;
                cout <<"4. Back" << endl;
                cout << "Choose the map: ";

                cin >> iMenu;

                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(1024,'\n');
                    continue;
                }

                    //이 while문에 속한 break는 게임 맵 선택 메뉴를 빠져나가기 위한 break이다.
                if(iMenu == MT_BACK) break;

                if(iMenu < MT_NONE || iMenu > MT_BACK) {
                    cout << "You've selected wrong menu!" << endl;
                    system("read");
                    continue;
                }

                    //선택한 맵에서 -1을 해주면 해당 몬스터에 해당하는 인덱스가 된다.
                _tagMonster tMonster = tMonsterArr[iMenu - 1];

                while(true) {
                    switch (iMenu) {
                        case MT_EASY:
                        system("clear");
                        cout << "================== EASY =================" << endl;

                        break;
                        case MT_NORMAL:
                        system("clear");
                        cout << "================== NORMAL =================" << endl;

                        break;
                        case MT_HARD:
                        system("clear");
                        cout << "================== HARD =================" << endl;

                        break;
                    }

                        //플레이어의 정보를 출력해준다.
                    cout << "=============== Player =================" << endl;
                    cout << "Name: " << tPlayer.strName << "\tClass: " << tPlayer.strClassName << endl;
                    cout << "Level: " << tPlayer.iLevel << "\tEXP: " << tPlayer.iEXP  << " / " << iLevelUpEXP[tPlayer.iLevel - 1] << endl;
                    //무기를 장착하고 있을 경우 공격력에 무기 공격력을 추가하여 출력한다.
                    if(tPlayer.bEquip[EQ_WEAPON] == true) {
                        cout << "Attack Damage: " << tPlayer.iAttackMin << " + " << tPlayer.tEquip[EQ_WEAPON].iMin << " - "
                        << tPlayer.iAttackMax << " + " << tPlayer.tEquip[EQ_WEAPON].iMax;
                    } else {
                        cout << "Attack Damage: " << tPlayer.iAttackMin << " - " << tPlayer.iAttackMax;
                    }

                    //방어구를 장착하고 있을 경우 방어력에 방어구 방여력을 추가하여 출력한다.
                    if(tPlayer.bEquip[EQ_ARMOR] == true) {
                        cout << "\tArmor: " << tPlayer.iArmorMin << " + " << tPlayer.tEquip[EQ_ARMOR].iMin << " - "
                        << tPlayer.iArmorMax << " + " << tPlayer.tEquip[EQ_ARMOR].iMax << endl;;
                    } else {
                        cout << "\tArmor: " << tPlayer.iArmorMin << " - " << tPlayer.iArmorMax << endl;
                    }            

                    cout << "HP: " << tPlayer.iHP << " / " << tPlayer.iHPMax <<
                    "\tMP: " << tPlayer.iMP << " / " << tPlayer.iMPMax << endl;

                    if(tPlayer.bEquip[EQ_WEAPON])
                        cout << "Equipped weapon: " << tPlayer.tEquip[EQ_WEAPON].strName;
                    else
                        cout << "Equipped weapon: None";
                    if(tPlayer.bEquip[EQ_ARMOR])
                        cout << "\tEquipped armor: " << tPlayer.tEquip[EQ_ARMOR].strName << endl;
                    else
                        cout << "\tEquipped armor: None" << endl;

                    cout << "Gold: " << tPlayer.tInventory.iGold << " G" << endl << endl;

                        //몬스터 정보 출력
                    cout << "=============== Monster =================" << endl;
                    cout << "Name: " << tMonster.strName << "\tLevel: " << tMonster.iLevel << endl;
                    cout << "Attack Damage: " << tMonster.iAttackMin << " - " << tMonster.iAttackMax << "\tArmor: " <<
                    tMonster.iArmorMin << " - " << tMonster.iArmorMax << endl;
                    cout << "HP: " << tMonster.iHP << " / " << tMonster.iHPMax << "\tMP: " << tMonster.iMP << " / " << tMonster.iMPMax << endl;
                    cout << "Get EXP: " << tMonster.iEXP << "\tGet gold: " << tMonster.iGoldMin << " - " << tMonster.iGoldMax << endl <<endl;

                    cout << "1. Attack" << endl;
                    cout << "2. Back" << endl;
                    cout << "Select the Menu: ";

                    int iBattleMenu;
                    cin >> iBattleMenu;

                    if(cin.fail()) {
                        cin.clear();
                        cin.ignore(1024, '\n');
                        continue;
                    } 

                    if(iBattleMenu == BATTLE_BACK) {
                        break;
                    }

                    if(iBattleMenu < BATTLE_NONE || iBattleMenu > BATTLE_BACK){
                        continue;
                    }

                    switch(iBattleMenu) {
                        case BATTLE_ATTACK:

                        int iAttackMin = tPlayer.iAttackMin;
                        int iAttackMax = tPlayer.iAttackMax;

                        //무기를 장착하고 있을 경우 무기의 min,max를 더해준다.
                        if(tPlayer.bEquip[EQ_WEAPON]) {
                            iAttackMin += tPlayer.tEquip[EQ_WEAPON].iMin;
                            iAttackMax += tPlayer.tEquip[EQ_WEAPON].iMax;
                        }

                        int iAttack = rand() % (iAttackMax - iAttackMin + 1) + iAttackMin;
                        int iArmor = rand() % (tMonster.iArmorMax - tMonster.iArmorMin + 1) + tMonster.iArmorMin;

                        int iDamage = iAttack - iArmor;
                        iDamage = iDamage < 1 ? 1 : iDamage;

                                //몬스터 HP를 감소 시킨다.
                        tMonster.iHP -= iDamage;

                        cout << tPlayer.strName << " has given " << tMonster.strName << " " << iDamage << " damage!" << endl;

                                //몬스터가 죽었을 경우를 처리한다.
                        if(tMonster.iHP <= 0) {
                            cout << tMonster.strName << "\' dead." << endl;

                            tPlayer.iEXP += tMonster.iEXP;
                            int iGold = rand() % (tMonster.iGoldMax - tMonster.iGoldMin + 1) + tMonster.iGoldMin;
                            tPlayer.tInventory.iGold += iGold;

                            cout << "You got " << tMonster.iEXP << "EXP" << endl;
                            cout << "You got " << iGold <<  "G" << endl;

                            tMonster.iHP = tMonster.iHPMax;
                            tMonster.iMP = tMonster.iMPMax;

                            if(tPlayer.iEXP >= iLevelUpEXP[tPlayer.iLevel - 1]) {
                                //플레이어 경험치를 레벨업에 필요한 경험치만큼 차감한다.
                                tPlayer.iEXP -= iLevelUpEXP[tPlayer.iLevel - 1];

                                // 레벨을 증가 시킨다.
                                tPlayer.iLevel++;

                                cout << "LEVEL UP!" << endl;

                                //능력을 향상 시킨다.
                                //직업 인덱스를 구한다.
                                int iClassIdx = tPlayer.eClass - 1;
                                int iHPUp = rand() % (tLvUpTable[iClassIdx].iHPMax - tLvUpTable[iClassIdx].iHPMin + 1)
                                + tLvUpTable[iClassIdx].iHPMin;
                                int iMPUp = rand() % (tLvUpTable[iClassIdx].iMPMax - tLvUpTable[iClassIdx].iMPMin + 1)
                                + tLvUpTable[iClassIdx].iMPMin;

                                tPlayer.iAttackMin += tLvUpTable[iClassIdx].iAttackMin;
                                tPlayer.iAttackMax += tLvUpTable[iClassIdx].iAttackMax;
                                tPlayer.iArmorMin += tLvUpTable[iClassIdx].iArmorMin;
                                tPlayer.iArmorMax += tLvUpTable[iClassIdx].iArmorMax;
                                tPlayer.iHPMax += iHPUp;
                                tPlayer.iMPMax += iMPUp;

                                //체력과 마나를 회복시킨다.
                                tPlayer.iHP = tPlayer.iHPMax;
                                tPlayer.iMP = tPlayer.iMPMax;
                            }
                            system("read");
                            break;
                        }

                        int iArmorMin = tPlayer.iArmorMin;
                        int iArmorMax = tPlayer.iArmorMax;

                        if(tPlayer.bEquip[EQ_ARMOR]) {
                            iArmorMin += tPlayer.tEquip[EQ_ARMOR].iMin;
                            iArmorMax += tPlayer.tEquip[EQ_ARMOR].iMax;
                        }

                                //몬스터가 살아있다면 플레이어를 공격
                        iAttack = rand() % (tMonster.iAttackMax - tMonster.iAttackMin + 1) + tMonster.iAttackMin;
                        iArmor = rand() % (iArmorMax - iArmorMin + 1) + iArmorMin;

                        iDamage = iAttack - iArmor;
                        iDamage = iDamage < 1 ? 1 : iDamage;

                        cout << tMonster.strName << " has given " << tPlayer.strName << " " << iDamage << " damage!" << endl;


                                //플레이어의 HP를 감소 시킨다.
                        tPlayer.iHP -= iDamage;

                                //플레이어가 죽었을 경우
                        if(tPlayer.iHP <= 0){
                            cout << tPlayer.strName << "\'s dead." << endl;

                            int iEXP = tPlayer.iEXP * 0.1f;
                            int iGold = tPlayer.tInventory.iGold * 0.1f;

                            tPlayer.iEXP -= iEXP;
                            tPlayer.tInventory.iGold -= iGold;

                            cout << "You lost " << iEXP << "EXP" << endl;
                            cout << "You lost " << iGold << "G" << endl;

                            tPlayer.iHP = tPlayer.iHPMax;
                            tPlayer.iMP = tPlayer.iMPMax;

                        }
                        system("read");
                        break;
                    }
                }

            }
            break;
            case MM_STORE:
            while(true) {
                system("clear");
                cout << "================== STORE =================" << endl;
                cout << "1. Weapon store" << endl;
                cout << "2. Armor store" << endl;
                cout << "3. Back" << endl;
                cout << "Select the store: ";

                cin >> iMenu;

                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(1024, '\n');
                    continue;
                }

                if(iMenu == SM_BACK) {
                    break;
                }

                switch(iMenu) {
                    case SM_WEAPON:
                    while(true) {
                        system("clear");
                        cout << "================== WEAPON STORE =================" << endl;

                        //판매 메뉴를 보여준다.
                        for(int i=0;i<STORE_WEAPON_MAX; i++) {
                            cout << i + 1 <<". Name: " << tStoreWeapon[i].strName << "\tType: " << tStoreWeapon[i].strTypeName << endl;
                            cout << "Attack: " << tStoreWeapon[i].iMin << " - " << tStoreWeapon[i].iMax << endl;
                            cout << "Price: " << tStoreWeapon[i].iPrice << "\tSelling Price: " << tStoreWeapon[i].iSell << endl;
                            cout << "Description: " << tStoreWeapon[i].strDesc << endl << endl;
                        }

                        cout << STORE_WEAPON_MAX + 1 << ". Back" << endl;
                        cout << "Player Gold: " << tPlayer.tInventory.iGold << "G" << endl;
                        cout << "Available Inventories:  " << INVENTORY_MAX - tPlayer.tInventory.iItemCount << endl << endl;
                        cout << "Select the weapon: ";
                        cin >> iMenu;

                        if(cin.fail()) {
                            cin.clear();
                            cin.ignore(1024,'\n');
                            continue;
                        }

                        if(iMenu == STORE_WEAPON_MAX + 1) break;

                        if(iMenu < 1 || iMenu > STORE_WEAPON_MAX + 1) {
                            cout << "You've selected wrong menu" << endl;
                            system("read");
                            continue;
                        }

                        //상점판매목록 배열의 인덱스를 구해준다.
                        int iWeaponIndex = iMenu - 1;

                        //인벤토리가 꽉 찼는지 검사한다.
                        if(tPlayer.tInventory.iItemCount == INVENTORY_MAX) {
                            cout << "Your Inventory is full." << endl;
                            system("read");
                            continue;
                        }

                        //돈이 부족할 경우
                        if(tPlayer.tInventory.iGold < tStoreWeapon[iWeaponIndex].iPrice) {
                            cout << "You don't have enough money" << endl;
                            system("read");
                            continue;
                        }

                        //처음에는 하나도 구입을 하지 않은 상태이므로 0으로 초기화 되어있다. 그리고 0번째 인덱스에 아이템을 추가하게 된다.
                        tPlayer.tInventory.tItem[tPlayer.tInventory.iItemCount] = tStoreWeapon[iWeaponIndex];
                        tPlayer.tInventory.iItemCount++;

                        //골드를 차감한다.
                        tPlayer.tInventory.iGold -= tStoreWeapon[iWeaponIndex].iPrice;

                        cout << "You've bought " << tStoreWeapon[iWeaponIndex].strName << endl;
                        system("read"); 

                    }
                    break;
                    case SM_ARMOR:
                    while(true) {
                        system("clear");
                        cout << "================== ARMOR STORE =================" << endl;

                        //판매 메뉴를 보여준다.
                        for(int i=0;i<STORE_ARMOR_MAX; i++) {
                            cout << i + 1 <<". Name: " << tStoreArmor[i].strName << "\tType: " << tStoreArmor[i].strTypeName << endl;
                            cout << "Armor: " << tStoreArmor[i].iMin << " - " << tStoreArmor[i].iMax << endl;
                            cout << "Price: " << tStoreArmor[i].iPrice << "\tSelling Price: " << tStoreArmor[i].iSell << endl;
                            cout << "Description: " << tStoreArmor[i].strDesc << endl << endl;
                        }

                        cout << STORE_ARMOR_MAX + 1 << ". Back" << endl;
                        cout << "Player Gold: " << tPlayer.tInventory.iGold << "G" << endl;
                        cout << "Available Inventories:  " << INVENTORY_MAX - tPlayer.tInventory.iItemCount << endl << endl;
                        cout << "Select the armor: ";
                        cin >> iMenu;

                        if(cin.fail()) {
                            cin.clear();
                            cin.ignore(1024,'\n');
                            continue;
                        }

                        if(iMenu == STORE_ARMOR_MAX + 1) break;

                        if(iMenu < 1 || iMenu > STORE_ARMOR_MAX + 1) {
                            cout << "You've selected wrong menu" << endl;
                            system("read");
                            continue;
                        }

                        //상점판매목록 배열의 인덱스를 구해준다.
                        int iArmorIndex = iMenu - 1;

                        //인벤토리가 꽉 찼는지 검사한다.
                        if(tPlayer.tInventory.iItemCount == INVENTORY_MAX) {
                            cout << "Your Inventory is full." << endl;
                            system("read");
                            continue;
                        }

                        //돈이 부족할 경우
                        if(tPlayer.tInventory.iGold < tStoreArmor[iArmorIndex].iPrice) {
                            cout << "You don't have enough money" << endl;
                            system("read");
                            continue;
                        }

                        //처음에는 하나도 구입을 하지 않은 상태이므로 0으로 초기화 되어있다. 그리고 0번째 인덱스에 아이템을 추가하게 된다.
                        tPlayer.tInventory.tItem[tPlayer.tInventory.iItemCount] = tStoreArmor[iArmorIndex];
                        tPlayer.tInventory.iItemCount++;

                        //골드를 차감한다.
                        tPlayer.tInventory.iGold -= tStoreArmor[iArmorIndex].iPrice;

                        cout << "You've bought " << tStoreArmor[iArmorIndex].strName << endl;
                        system("read"); 

                    }
                    break;

                }
            }
            break;
            case MM_INVENTORY:
            while(true) {
                system("clear");
                cout << "================== INVENTORY =================" << endl;
                cout << "=============== Player =================" << endl;
                cout << "Name: " << tPlayer.strName << "\tClass: " << tPlayer.strClassName << endl;
                cout << "Level: " << tPlayer.iLevel << "\tEXP: " << tPlayer.iEXP << iLevelUpEXP[tPlayer.iLevel - 1] << endl;

                //무기를 장착하고 있을 경우 공격력에 무기 공격력을 추가하여 출력한다.
                if(tPlayer.bEquip[EQ_WEAPON] == true) {
                    cout << "Attack Damage: " << tPlayer.iAttackMin << " + " << tPlayer.tEquip[EQ_WEAPON].iMin << " - "
                    << tPlayer.iAttackMax << " + " << tPlayer.tEquip[EQ_WEAPON].iMax;
                } else {
                    cout << "Attack Damage: " << tPlayer.iAttackMin << " - " << tPlayer.iAttackMax;
                }

                //방어구를 장착하고 있을 경우 방어력에 방어구 방여력을 추가하여 출력한다.
                if(tPlayer.bEquip[EQ_ARMOR] == true) {
                    cout << "\tArmor: " << tPlayer.iArmorMin << " + " << tPlayer.tEquip[EQ_ARMOR].iMin << " - "
                    << tPlayer.iArmorMax << " + " << tPlayer.tEquip[EQ_ARMOR].iMax << endl;
                } else {
                    cout << "\tArmor: " << tPlayer.iArmorMin << " - " << tPlayer.iArmorMax << endl;
                }                

                cout << "HP: " << tPlayer.iHP << " / " << tPlayer.iHPMax 
                << "\tMP: " << tPlayer.iMP << " / " << tPlayer.iMPMax << endl;

                if(tPlayer.bEquip[EQ_WEAPON])
                    cout << "Equipped weapon: " << tPlayer.tEquip[EQ_WEAPON].strName;
                else
                    cout << "Equipped weapon: None";
                if(tPlayer.bEquip[EQ_ARMOR])
                    cout << "\tEquipped armor: " << tPlayer.tEquip[EQ_ARMOR].strName << endl;
                else
                    cout << "\tEquipped armor: None" << endl;

                cout << "Gold: " << tPlayer.tInventory.iGold << " G" << endl << endl;

                for(int i=0; i<tPlayer.tInventory.iItemCount; i++) {
                    cout << i + 1 <<". Name: " << tPlayer.tInventory.tItem[i].strName << "\tType: " << tPlayer.tInventory.tItem[i].strTypeName << endl;
                    cout << "Attack: " << tPlayer.tInventory.tItem[i].iMin << " - " << tPlayer.tInventory.tItem[i].iMax << endl;
                    cout << "Price: " << tPlayer.tInventory.tItem[i].iPrice << "\tSelling Price: " << tPlayer.tInventory.tItem[i].iSell << endl;
                    cout << "Description: " << tPlayer.tInventory.tItem[i].strDesc << endl << endl;
                }

                cout << tPlayer.tInventory.iItemCount + 1 << ". Back" << endl << endl;
                cout << "Select the item you want to equip: ";
                cin >> iMenu;

                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(1024,'\n');
                    continue;
                } 

                if(iMenu == tPlayer.tInventory.iItemCount + 1) break;

                if(iMenu < 1 || iMenu > tPlayer.tInventory.iItemCount + 1) {
                    cout << "You've selected wrong menu." << endl;
                    system("read");
                    continue;
                }

                //아이템 인덱스를 구해준다.
                int idx = iMenu - 1;

                //제대로 선택했을 경우 아이템의 종류에 따라 장착 부위를 결정하게 한다.
                EQUIP eq;

                switch(tPlayer.tInventory.tItem[idx].eType) {
                    case IT_WEAPON:
                    eq = EQ_WEAPON;
                    break;
                    case IT_ARMOR:
                    eq = EQ_ARMOR;
                    break;
                    default:
                    break;
                }

                //아이템이 장착되어 있을 경우 장착할 아이템과 장착되어 있는 아이템을 교체해 주어야 한다. Swap 알고리즘을 활용한다.
                if (tPlayer.bEquip[eq] == true) {
                    _tagItem tSwap = tPlayer.tEquip[eq];
                    tPlayer.tEquip[eq] = tPlayer.tInventory.tItem[idx];
                    tPlayer.tInventory.tItem[idx] = tSwap;
                } else {
                    // 장착되어 있지 않은 경우 장착이 되고 인벤토리 칸은 1칸 비워지게 된다.
                    tPlayer.tEquip[eq] = tPlayer.tInventory.tItem[idx];

                    for(int i=idx; i<tPlayer.tInventory.iItemCount - 1; i++) {
                        tPlayer.tInventory.tItem[i] = tPlayer.tInventory.tItem[i + 1];
                    }

                    tPlayer.tInventory.iItemCount--;

                    //장착을 했기 때문에 true로 바꿔준다.
                    tPlayer.bEquip[eq] = true;
                }

                cout << "You've equipped " << tPlayer.tEquip[eq].strName << endl;


                system("read");
            }
            break;
            case MM_EXIT:
            bLoop = false;
            break;
        }
    }
    return 0;
}



