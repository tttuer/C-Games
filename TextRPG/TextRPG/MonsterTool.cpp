#include <iostream>

#define NAME_SIZE 100

using namespace std;

enum MAP_TYPE
{
    MT_EASY,
    MT_NORMAL,
    MT_HARD,
    MT_END
};

enum MAIN_MENU
{
    MM_NONE,
    MM_ADD,
    MM_LOAD,
    MM_SAVE,
    MM_OUTPUT,
    MM_EXIT
};

struct _tagMonster
{
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

// 숫자 입력을 받는다. 입력 오류까지 처리해 주고 INT_MAX 값을 리턴하면 입력 오류이다.
int inputInt()
{
    int iInput;
    cin >> iInput;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1024, '\n');
        return INT_MAX; //INT_MAX가 리턴이 되면 입력값이 잘못됐다는 뜻이다.
    }

    return iInput;
}

void addMonster(_tagMonster *pMonster)
{
    int iMap = 0;

    while (true)
    {
        system("clear");
        cout << "1. Easy" << endl;
        cout << "2. Normal" << endl;
        cout << "3. Hard" << endl;
        cout << "4. Exit" << endl;
        cout << "Select a Level to add a monster: ";
        iMap = inputInt();

        if (iMap == 4)
            return;

        else if (iMap < 1 || iMap > 4)
        {
            continue;
        }

        // 1부터 시작하기 때문에 1을 감소시켜야 사냥터 인덱스가 된다.
        iMap--;
        break;
    }

    cin.clear();
    cin.ignore(1024, '\n');

    // 몬스터의 이름을 받아온다.
    cout << "Name: ";
    cin.getline(pMonster[iMap].strName, sizeof(pMonster[iMap].strName) - 1);

    cout << "Min attack damage: ";
    pMonster[iMap].iAttackMin = inputInt();
    cout << "Max attack damage: ";
    pMonster[iMap].iAttackMax = inputInt();

    cout << "Min armor: ";
    pMonster[iMap].iArmorMin = inputInt();
    cout << "Max armor: ";
    pMonster[iMap].iArmorMax = inputInt();

    cout << "HP: ";
    pMonster[iMap].iHP = inputInt();
    pMonster[iMap].iHPMax = pMonster[iMap].iHP;

    cout << "MP: ";
    pMonster[iMap].iMP = inputInt();
    pMonster[iMap].iMPMax = pMonster[iMap].iMP;

    cout << "Level: ";
    pMonster[iMap].iLevel = inputInt();

    cout << "Exp player can get: ";
    pMonster[iMap].iEXP = inputInt();

    cout << "Min Gold: ";
    pMonster[iMap].iGoldMin = inputInt();

    cout << "Max Gold: ";
    pMonster[iMap].iGoldMax = inputInt();

    cout << "Monster setting finish" << endl;
    system("read");
}

bool saveMonster(_tagMonster *pMonster)
{
    FILE *pFile = fopen("Monster.mst", "wb");

    if (pFile)
    {
        fwrite(pMonster, sizeof(_tagMonster), MT_END, pFile);

        fclose(pFile);
        return true;
    }
    return false;
}

bool loadMonster(_tagMonster *pMonster)
{
    FILE *pFile = fopen("Monster.mst", "rb");

    if (pFile)
    {
        fread(pMonster, sizeof(_tagMonster), MT_END, pFile);

        fclose(pFile);
        return true;
    }
    return false;
}

void outputMonster(_tagMonster *pMonster)
{
	//몬스터 정보 출력
	cout << "=============== Monster =================" << endl;
	cout << "Name: " << pMonster->strName << "\tLevel: " << pMonster->iLevel << endl;
	cout << "Attack Damage: " << pMonster->iAttackMin << " - " << pMonster->iAttackMax << "\tArmor: " << pMonster->iArmorMin << " - " << pMonster->iArmorMax << endl;
	cout << "HP: " << pMonster->iHP << " / " << pMonster->iHPMax << "\tMP: " << pMonster->iMP << " / " << pMonster->iMPMax << endl;
	cout << "Get EXP: " << pMonster->iEXP << "\tGet gold: " << pMonster->iGoldMin << " - " << pMonster->iGoldMax << endl
		 << endl;
}

void outputMonsters(_tagMonster* pMonsterArr) {
    system("clear");

    for(int i=0; i<MT_END; i++){
        outputMonster(&pMonsterArr[i]);
    }

    system("read");
}

int main()
{
    _tagMonster tMonster[MT_END] = {};

    while (true)
    {
        system("clear");
        cout << "1. Add Monster" << endl;
        cout << "2. Load a file" << endl;
        cout << "3. Save a file" << endl;
        cout << "4. Print monsters" << endl;
        cout << "5. Exit" << endl;
        cout << "Select a Menu: ";
        int iMenu = inputInt();

        if (iMenu == MM_EXIT)
            break;

        switch (iMenu)
        {
        case MM_ADD:
            addMonster(tMonster);
            break;
        case MM_LOAD:
            loadMonster(tMonster);
            break;
        case MM_SAVE:
            saveMonster(tMonster);
            break;
            case MM_OUTPUT:
            outputMonsters(tMonster);
            break;
        }
    }

    // 자동 저장 기능. 저장을 하지 않더라도 프로그램이 종료될 때
    // 자동을 저장되도록 한다.
    saveMonster(tMonster);

    return 0;
}