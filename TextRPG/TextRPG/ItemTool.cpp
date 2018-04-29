#include <iostream>

#define NAME_SIZE 100
#define STORE_WEAPON_MAX 3
#define STORE_ARMOR_MAX 3
#define ITEM_DESC_LENGTH 512

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

enum STORE_MENU
{
    SM_NONE,
    SM_WEAPON,
    SM_ARMOR,
    SM_BACK
};

enum ITEM_TYPE
{
    IT_NONE,
    IT_WEAPON,
    IT_ARMOR,
    IT_BACK
};

struct _tagItem
{
    char strName[NAME_SIZE];
    char strTypeName[NAME_SIZE];
    ITEM_TYPE eType;
    int iMin;
    int iMax;
    int iPrice;
    int iSell;
    char strDesc[ITEM_DESC_LENGTH];
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

void addItem(_tagItem *pWeaponStore, _tagItem *pArmorStore)
{
    int iStore = 0;

    while (true)
    {
        system("clear");
        cout << "1. Weapon Store" << endl;
        cout << "2. Armor Store" << endl;
        cout << "3. Exit" << endl;
        cout << "Select a Store to add a item: ";
        iStore = inputInt();

        if (iStore == 3)
            return;

        else if (iStore < 1 || iStore > 3)
        {
            continue;
        }
        break;
    }

    int iItemLevel = 0;
    while (true)
    {
        system("clear");
        cout << "1. Easy" << endl;
        cout << "2. Normal" << endl;
        cout << "3. Hard" << endl;
        cout << "4. Exit" << endl;
        cout << "Select a Level to add an item: ";
        iItemLevel = inputInt();

        if (iItemLevel == 4)
        {
            return;
        }

        else if (iItemLevel < 1 || iItemLevel > 4)
        {
            continue;
        }

        iItemLevel--;
        break;
    }

    cin.clear();
    cin.ignore(1024, '\n');

    _tagItem *pItem = NULL;

    // 인자로 무기상점에서 사용할 아이템 배열과 방어구 상점에서 사용할
    // 아이템 배열을 따로 넣어주었다. 아이템을 추가할 때에도
    // 추가할 상점을 선택하고 아이템 난이도를 선택해주었다.
    // 그러므로 복잡하기 때문에 여기에서 추가할 아이템의 메모리
    // 주소를 얻어와서 거기에 추가해주면 쉽게 처리가 가능하다.
    if (iStore == SM_WEAPON)
    {
        pItem = &pWeaponStore[iItemLevel];
        pItem->eType = IT_WEAPON;
        strcpy(pItem->strTypeName, "Weapon");
    }
    else
    {
        pItem = &pArmorStore[iItemLevel];
        pItem->eType = IT_ARMOR;
        strcpy(pItem->strTypeName, "Armor");
    }

    // 아이템의 이름을 받아온다.
    cout << "Name: ";
    cin.getline(pItem->strName, sizeof(pItem->strName) - 1);

    cout << "Description of item: ";
    cin.getline(pItem->strDesc, sizeof(pItem->strDesc) - 1);

    cout << "Min ability: ";
    pItem->iMin = inputInt();
    cout << "Max ability: ";
    pItem->iMax = inputInt();

    cout << "Price: ";
    pItem->iPrice = inputInt();
    cout << "Selling Price: ";
    pItem->iSell = inputInt();

    cout << "Add an item finish" << endl;
    system("read");
}

bool saveStore(_tagItem *pWeapon, _tagItem *pArmor)
{
    FILE *pFile = fopen("Store.str", "wb");

    if (pFile)
    {
        fwrite(pWeapon, sizeof(_tagItem), STORE_WEAPON_MAX, pFile);
        fwrite(pArmor, sizeof(_tagItem), STORE_ARMOR_MAX, pFile);

        fclose(pFile);
        return true;
    }
    return false;
}

bool loadStore(_tagItem *pWeapon, _tagItem *pArmor)
{
    FILE *pFile = fopen("Store.str", "rb");

    if (pFile)
    {
        fread(pWeapon, sizeof(_tagItem), STORE_WEAPON_MAX, pFile);
        fread(pArmor, sizeof(_tagItem), STORE_ARMOR_MAX, pFile);

        fclose(pFile);
        return true;
    }
    return false;
}

void outputItems(_tagItem *pWeapon, _tagItem *pArmor)
{
    system("clear");

    cout << "==================== Store ======================" << endl;
    //판매 메뉴를 보여준다.
    for (int i = 0; i < STORE_WEAPON_MAX; i++)
    {
        cout << i + 1 << ". Name: " << pWeapon[i].strName << "\tType: " << pWeapon[i].strTypeName << endl;
        cout << "Attack: " << pWeapon[i].iMin << " - " << pWeapon[i].iMax << endl;
        cout << "Price: " << pWeapon[i].iPrice << "\tSelling Price: " << pWeapon[i].iSell << endl;
        cout << "Description: " << pWeapon[i].strDesc << endl
             << endl;
    }

    //판매 메뉴를 보여준다.
    for (int i = 0; i < STORE_ARMOR_MAX; i++)
    {
        cout << i + 1 << ". Name: " << pArmor[i].strName << "\tType: " << pArmor[i].strTypeName << endl;
        cout << "Armor: " << pArmor[i].iMin << " - " << pArmor[i].iMax << endl;
        cout << "Price: " << pArmor[i].iPrice << "\tSelling Price: " << pArmor[i].iSell << endl;
        cout << "Description: " << pArmor[i].strDesc << endl
             << endl;
    }

    system("read");
}

int main()
{
    _tagItem tWeapon[STORE_WEAPON_MAX] = {};
    _tagItem tArmor[STORE_ARMOR_MAX] = {};

    while (true)
    {
        system("clear");
        cout << "1. Add Item" << endl;
        cout << "2. Load a file" << endl;
        cout << "3. Save a file" << endl;
        cout << "4. Print items" << endl;
        cout << "5. Exit" << endl;
        cout << "Select a Menu: ";
        int iMenu = inputInt();

        if (iMenu == MM_EXIT)
            break;

        switch (iMenu)
        {
        case MM_ADD:
            addItem(tWeapon, tArmor);
            break;
        case MM_LOAD:
            loadStore(tWeapon, tArmor);
            break;
        case MM_SAVE:
            saveStore(tWeapon, tArmor);
            break;
        case MM_OUTPUT:
            outputItems(tWeapon, tArmor);
            break;
        }
    }

    // 자동 저장 기능. 저장을 하지 않더라도 프로그램이 종료될 때
    // 자동을 저장되도록 한다.
    saveStore(tWeapon, tArmor);

    return 0;
}