#include <iostream>
#include <time.h>

using namespace std;

enum MAIN_MENU {
	MM_NONE,
	MM_MAP,
	MM_STORE,
	MM_INVENTORY,
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

enum ITEM_TYPE {
	IT_NONE,
	IT_WEAPON,
	IT_ARMOR,
	IT_BACK
};

enum EQUIP {
	EQ_WEAPON,
	EQ_ARMOR,
	EQ_MAX
};

enum BATTLE {
	BATTLE_NONE,
	BATTLE_ATTACK,
	BATTLE_BACK,
	BATTLE_END
};

enum STORE_MENU {
	SM_NONE,
	SM_WEAPON,
	SM_ARMOR,
	SM_BACK
};

#define NAME_SIZE       		100
#define ITEM_DESC_LENGTH        512
#define INVENTORY_MAX           20
#define STORE_WEAPON_MAX        3
#define STORE_ARMOR_MAX         3
#define LEVEL_MAX               10

//레벨업에 필요한 경험치 목록을 만든다.
const int g_iLevelUpEXP[LEVEL_MAX] = {4000, 10000, 20000, 35000, 50000, 70000, 100000, 150000, 200000, 500000};

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

_tagLevelUpStatus g_tLvUpTable[CLASS_END - 1] = {};

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
	cout << "Choose the map: " ;

	int iMenu = inputInt();

	if(iMenu == INT_MAX || iMenu <= MT_NONE || iMenu > MT_BACK) return MT_NONE;

	return iMenu;
}

void outputBattleTag(int iMenu) {
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
}

void outputPlayer(_tagPlayer *pPlayer) {
	//플레이어의 정보를 출력해준다.
	cout << "=============== Player =================" << endl;
	cout << "Name: " << pPlayer->strName << "\tClass: " << pPlayer->strClassName << endl;
	cout << "Level: " << pPlayer->iLevel << "\tEXP: " << pPlayer->iEXP  << " / " << g_iLevelUpEXP[pPlayer->iLevel - 1] << endl;
    //무기를 장착하고 있을 경우 공격력에 무기 공격력을 추가하여 출력한다.
	if(pPlayer->bEquip[EQ_WEAPON] == true) {
		cout << "Attack Damage: " << pPlayer->iAttackMin << " + " << pPlayer->tEquip[EQ_WEAPON].iMin << " - "
		<< pPlayer->iAttackMax << " + " << pPlayer->tEquip[EQ_WEAPON].iMax;
	} else {
		cout << "Attack Damage: " << pPlayer->iAttackMin << " - " << pPlayer->iAttackMax;
	}

    //방어구를 장착하고 있을 경우 방어력에 방어구 방여력을 추가하여 출력한다.
	if(pPlayer->bEquip[EQ_ARMOR] == true) {
		cout << "\tArmor: " << pPlayer->iArmorMin << " + " << pPlayer->tEquip[EQ_ARMOR].iMin << " - "
		<< pPlayer->iArmorMax << " + " << pPlayer->tEquip[EQ_ARMOR].iMax << endl;;
	} else {
		cout << "\tArmor: " << pPlayer->iArmorMin << " - " << pPlayer->iArmorMax << endl;
	}            

	cout << "HP: " << pPlayer->iHP << " / " << pPlayer->iHPMax <<
	"\tMP: " << pPlayer->iMP << " / " << pPlayer->iMPMax << endl;

	if(pPlayer->bEquip[EQ_WEAPON])
		cout << "Equipped weapon: " << pPlayer->tEquip[EQ_WEAPON].strName;
	else
		cout << "Equipped weapon: None";
	if(pPlayer->bEquip[EQ_ARMOR])
		cout << "\tEquipped armor: " << pPlayer->tEquip[EQ_ARMOR].strName << endl;
	else
		cout << "\tEquipped armor: None" << endl;

	cout << "Gold: " << pPlayer->tInventory.iGold << " G" << endl << endl;
}

void outputMonster(_tagMonster* pMonster) {
	//몬스터 정보 출력
	cout << "=============== Monster =================" << endl;
	cout << "Name: " << pMonster->strName << "\tLevel: " << pMonster->iLevel << endl;
	cout << "Attack Damage: " << pMonster->iAttackMin << " - " << pMonster->iAttackMax << "\tArmor: " <<
	pMonster->iArmorMin << " - " << pMonster->iArmorMax << endl;
	cout << "HP: " << pMonster->iHP << " / " << pMonster->iHPMax << "\tMP: " << pMonster->iMP << " / " << pMonster->iMPMax << endl;
	cout << "Get EXP: " << pMonster->iEXP << "\tGet gold: " << pMonster->iGoldMin << " - " << pMonster->iGoldMax << endl <<endl;

}

int outputBattleMenu() {
	cout << "1. Attack" << endl;
	cout << "2. Back" << endl;
	cout << "Select the Menu: ";

	int iMenu = inputInt();

	if(iMenu == INT_MAX || iMenu <= BATTLE_NONE || iMenu >= BATTLE_END) {
		return BATTLE_NONE;
	}

	return iMenu;
}

void battle(_tagPlayer* pPlayer, _tagMonster* pMonster) {
	int iAttackMin = pPlayer->iAttackMin;
	int iAttackMax = pPlayer->iAttackMax;

                        //무기를 장착하고 있을 경우 무기의 min,max를 더해준다.
	if(pPlayer->bEquip[EQ_WEAPON]) {
		iAttackMin += pPlayer->tEquip[EQ_WEAPON].iMin;
		iAttackMax += pPlayer->tEquip[EQ_WEAPON].iMax;
	}

	int iAttack = rand() % (iAttackMax - iAttackMin + 1) + iAttackMin;
	int iArmor = rand() % (pMonster->iArmorMax - pMonster->iArmorMin + 1) + pMonster->iArmorMin;

	int iDamage = iAttack - iArmor;
	iDamage = iDamage < 1 ? 1 : iDamage;

                                //몬스터 HP를 감소 시킨다.
	pMonster->iHP -= iDamage;

	cout << pPlayer->strName << " has given " << pPlayer->strName << " " << iDamage << " damage!" << endl;

                                //몬스터가 죽었을 경우를 처리한다.
	if(pMonster->iHP <= 0) {
		cout << pMonster->strName << "\' dead." << endl;

		pPlayer->iEXP += pMonster->iEXP;
		int iGold = rand() % (pMonster->iGoldMax - pMonster->iGoldMin + 1) + pMonster->iGoldMin;
		pPlayer->tInventory.iGold += iGold;

		cout << "You got " << pMonster->iEXP << "EXP" << endl;
		cout << "You got " << iGold <<  "G" << endl;

		pMonster->iHP = pMonster->iHPMax;
		pMonster->iMP = pMonster->iMPMax;

		if(pPlayer->iEXP >= g_iLevelUpEXP[pPlayer->iLevel - 1]) {
                                //플레이어 경험치를 레벨업에 필요한 경험치만큼 차감한다.
			pPlayer->iEXP -= g_iLevelUpEXP[pPlayer->iLevel - 1];

                                // 레벨을 증가 시킨다.
			pPlayer->iLevel++;

			cout << "LEVEL UP!" << endl;

                                //능력을 향상 시킨다.
                                //직업 인덱스를 구한다.
			int iClassIdx = pPlayer->eClass - 1;
			int iHPUp = rand() % (g_tLvUpTable[iClassIdx].iHPMax - g_tLvUpTable[iClassIdx].iHPMin + 1)
			+ g_tLvUpTable[iClassIdx].iHPMin;
			int iMPUp = rand() % (g_tLvUpTable[iClassIdx].iMPMax - g_tLvUpTable[iClassIdx].iMPMin + 1)
			+ g_tLvUpTable[iClassIdx].iMPMin;

			pPlayer->iAttackMin += g_tLvUpTable[iClassIdx].iAttackMin;
			pPlayer->iAttackMax += g_tLvUpTable[iClassIdx].iAttackMax;
			pPlayer->iArmorMin += g_tLvUpTable[iClassIdx].iArmorMin;
			pPlayer->iArmorMax += g_tLvUpTable[iClassIdx].iArmorMax;
			pPlayer->iHPMax += iHPUp;
			pPlayer->iMPMax += iMPUp;

                                //체력과 마나를 회복시킨다.
			pPlayer->iHP = pPlayer->iHPMax;
			pPlayer->iMP = pPlayer->iMPMax;
		}
		return;
	}

	int iArmorMin = pPlayer->iArmorMin;
	int iArmorMax = pPlayer->iArmorMax;

	if(pPlayer->bEquip[EQ_ARMOR]) {
		iArmorMin += pPlayer->tEquip[EQ_ARMOR].iMin;
		iArmorMax += pPlayer->tEquip[EQ_ARMOR].iMax;
	}

                                //몬스터가 살아있다면 플레이어를 공격
	iAttack = rand() % (pMonster->iAttackMax - pMonster->iAttackMin + 1) + pMonster->iAttackMin;
	iArmor = rand() % (iArmorMax - iArmorMin + 1) + iArmorMin;

	iDamage = iAttack - iArmor;
	iDamage = iDamage < 1 ? 1 : iDamage;

	cout << pMonster->strName << " has given " << pPlayer->strName << " " << iDamage << " damage!" << endl;


                                //플레이어의 HP를 감소 시킨다.
	pPlayer->iHP -= iDamage;

                                //플레이어가 죽었을 경우
	if(pPlayer->iHP <= 0){
		cout << pPlayer->strName << "\'s dead." << endl;

		int iEXP = pPlayer->iEXP * 0.1f;
		int iGold = pPlayer->tInventory.iGold * 0.1f;

		pPlayer->iEXP -= iEXP;
		pPlayer->tInventory.iGold -= iGold;

		cout << "You lost " << iEXP << "EXP" << endl;
		cout << "You lost " << iGold << "G" << endl;

		pPlayer->iHP = pPlayer->iHPMax;
		pPlayer->iMP = pPlayer->iMPMax;

	}
}

void runBattle(_tagPlayer* pPlayer, _tagMonster* pMonsterArr, int iMenu) {
	_tagMonster tMonster = pMonsterArr[iMenu - 1];

	while(true) {
		system("clear");
		outputBattleTag(iMenu);

		//플레이어 출력
		outputPlayer(pPlayer);

		// 몬스터 출력
		outputMonster(&tMonster);

		switch(outputBattleMenu()) {
			case BATTLE_ATTACK:
			battle(pPlayer,&tMonster);
			system("read");
			break;
			case BATTLE_BACK:
			return;
		}


	}
}

int outputMapMenu() {
	system("clear");

	cout << "================== MAP =================" << endl;
	cout <<"1. EASY" << endl;
	cout << "2. NORMAL" << endl;
	cout << "3. HARD" << endl;
	cout << "4. Exit" << endl;
	cout << "Choose the menu: " ;

	int iMenu = inputInt();

	if(iMenu == INT_MAX || iMenu <= MM_NONE || iMenu > MM_EXIT) return MM_NONE;

	return iMenu;	
}

int selectClass() {
	int iClass = CLASS_NONE;
	while(iClass == CLASS_NONE) {
		system("clear");

		cout << "1. Knight" << endl;
		cout << "2. Archer" << endl;
		cout << "3. Wizard" << endl;
		cout << "Choose the class: ";
		iClass = inputInt();

		if(iClass == INT_MAX || iClass <= CLASS_NONE || iClass >= CLASS_END){
			iClass = CLASS_NONE;
		}
	}

	return iClass;
}

void setPlayer(_tagPlayer *pPlayer) {
	//플레이어 이름을 입력받는다.
	cout << "Enter the name: ";
	cin.getline(pPlayer->strName, NAME_SIZE - 1);

	pPlayer->eClass = (CLASS)selectClass();
	pPlayer->iLevel = 1;
	pPlayer->iEXP = 0;
	pPlayer->tInventory.iGold = 10000;

	switch (pPlayer->eClass) {
		case CLASS_KNIGHT:
		strcpy(pPlayer->strClassName, "Knight");
		pPlayer->iAttackMin = 5;
		pPlayer->iAttackMax = 10;
		pPlayer->iArmorMin = 15;
		pPlayer->iArmorMax = 20;
		pPlayer->iHP = 500;
		pPlayer->iHPMax = 500;
		pPlayer->iMP = 100;
		pPlayer->iMPMax = 100;
		break;
		case CLASS_ARCHER:
		strcpy(pPlayer->strClassName, "Archer");
		pPlayer->iAttackMin = 10;
		pPlayer->iAttackMax = 15;
		pPlayer->iArmorMin = 10;
		pPlayer->iArmorMax = 15;
		pPlayer->iHP = 400;
		pPlayer->iHPMax = 400;
		pPlayer->iMP = 200;
		pPlayer->iMPMax = 200;
		break;
		case CLASS_WIZARD:
		strcpy(pPlayer->strClassName, "Wizard");
		pPlayer->iAttackMin = 15;
		pPlayer->iAttackMax = 20;
		pPlayer->iArmorMin = 5;
		pPlayer->iArmorMax = 10;
		pPlayer->iHP = 300;
		pPlayer->iHPMax = 300;
		pPlayer->iMP = 300;
		pPlayer->iMPMax = 300;
		break;
		default:
		break;
	}
}

_tagMonster createMonster(const char *pName, int iAttackMin, int iAttackMax, 
	int iArmorMin, int iArmorMax, int iHP, int iMP, int iLevel, int iEXP, int iGoldMin, int iGoldMax) {
	_tagMonster tMonster = {};

	strcpy(tMonster.strName, pName);
	tMonster.iAttackMin = iAttackMin;
	tMonster.iAttackMax = iAttackMax;
	tMonster.iArmorMin = iArmorMin;
	tMonster.iArmorMax = iArmorMax;
	tMonster.iHP = iHP;
	tMonster.iHPMax = iHP;
	tMonster.iMP = iMP;
	tMonster.iMPMax = iMP;
	tMonster.iLevel = iLevel;
	tMonster.iEXP = iEXP;
	tMonster.iGoldMin = iGoldMin;
	tMonster.iGoldMax = iGoldMax;

	return tMonster;
}

void setMonster(_tagMonster *pMonsterArr) {
	pMonsterArr[0] = createMonster("Goblin", 20, 35, 2, 5, 100, 10, 1, 1000, 500, 1500);
	pMonsterArr[1] = createMonster("Troll", 80, 130, 60, 90, 2000, 100, 5, 7000, 6000, 8000);
	pMonsterArr[2] = createMonster("Dragon", 250, 500, 200, 400, 30000, 20000, 10, 30000, 20000, 50000);
}

// MAP에 관련된 동작을 하게 한다.
void runMap(_tagPlayer* pPlayer, _tagMonster* pMonsterArr) {
	bool bLoop = true;

	while(bLoop) {
		int iMenu = outputMapMenu();

		if(iMenu == MT_BACK) return;

		// 전투를 시작한다.
		runBattle(pPlayer, pMonsterArr, iMenu);


	}
}

_tagLevelUpStatus createLvUpStatus(int iAttackMin, int iAttackMax, int iArmorMin, 
	int iArmorMax, int iHPMin, int iHPMax, int iMPMin, int iMPMax) {
	_tagLevelUpStatus tStatus = {};

	tStatus.iAttackMin = iAttackMin;
	tStatus.iAttackMax = iAttackMax;
	tStatus.iArmorMin = iArmorMin;
	tStatus.iArmorMax = iArmorMax;
	tStatus.iHPMin = iHPMin;
	tStatus.iHPMax = iHPMax;
	tStatus.iMPMin = iMPMin;
	tStatus.iMPMax = iMPMax;

	return tStatus;
}

int outputStoreMenu() {
	system("clear");
	cout << "================== STORE =================" << endl;
	cout << "1. Weapon store" << endl;
	cout << "2. Armor store" << endl;
	cout << "3. Back" << endl;
	cout << "Select the store: ";

	int iMenu = inputInt();

	if(iMenu == INT_MAX || iMenu < SM_NONE || iMenu > SM_BACK) return SM_NONE;

	return iMenu;
}

int outputStoreItemList(_tagInventory* pInventory, _tagItem* pStore, int iItemCount) {
	//판매 메뉴를 보여준다.
	for(int i=0;i<iItemCount; i++) {
		cout << i + 1 <<". Name: " << pStore[i].strName << "\tType: " << pStore[i].strTypeName << endl;
		cout << "Attack: " << pStore[i].iMin << " - " << pStore[i].iMax << endl;
		cout << "Price: " << pStore[i].iPrice << "\tSelling Price: " << pStore[i].iSell << endl;
		cout << "Description: " << pStore[i].strDesc << endl << endl;
	}

	cout << iItemCount + 1 << ". Back" << endl;
	cout << "Player Gold: " << pInventory->iGold << "G" << endl;
	cout << "Available Inventories:  " << INVENTORY_MAX - pInventory->iItemCount << endl << endl;
	cout << "Select the weapon: ";
	
	int iMenu = inputInt();

	if(iMenu < 1 || iMenu > iItemCount + 1) return INT_MAX;

	return iMenu;
}

void buyItem(_tagInventory* pInventory, _tagItem* pStore, int iItemCount, int iStoreType) {
	while(true){
		system("clear");
		switch(iStoreType){
			case IT_WEAPON:
			cout << "================== WEAPON STORE =================" << endl;
			break;
			case IT_ARMOR:
			cout << "================== ARMOR STORE =================" << endl;
			break;
		}
		int iInput = outputStoreItemList(pInventory, pStore, iItemCount);

		if(iInput == INT_MAX) {
			cout << "You've selected wrong menu!" << endl;
			system("read");
			continue;
		}

		if(iInput == iItemCount + 1) {
			return;
		}
		//상점판매목록 배열의 인덱스를 구해준다.
		int iIndex = iInput - 1;

                        //인벤토리가 꽉 찼는지 검사한다.
		if(pInventory->iItemCount == INVENTORY_MAX) {
			cout << "Your Inventory is full." << endl;
			system("read");
			continue;
		}

                        //돈이 부족할 경우
		if(pInventory->iGold < pStore[iIndex].iPrice) {
			cout << "You don't have enough money" << endl;
			system("read");
			continue;
		}

                        //처음에는 하나도 구입을 하지 않은 상태이므로 0으로 초기화 되어있다. 그리고 0번째 인덱스에 아이템을 추가하게 된다.
		pInventory->tItem[pInventory->iItemCount] = pStore[iIndex];
		pInventory->iItemCount++;

                        //골드를 차감한다.
		pInventory->iGold -= pStore[iIndex].iPrice;

		cout << "You've bought " << pStore[iIndex].strName << endl;
		system("read"); 
	}
}

void runStore(_tagInventory *pInventory, _tagItem* pWeapon, _tagItem* pArmor) {
	while(true) {
		switch(outputStoreMenu()) {
			case SM_WEAPON:
			buyItem(pInventory, pWeapon, STORE_WEAPON_MAX, SM_WEAPON);
			break;
			case SM_ARMOR:
			buyItem(pInventory, pArmor, STORE_ARMOR_MAX, SM_ARMOR);
			break;
			case SM_BACK:
			return;
		}
	}

}

_tagItem createItem(const char* pName, ITEM_TYPE eType, int iMin, int iMax, int iPrice, int iSell, const char* pDesc) {
	_tagItem tItem = {};

	strcpy(tItem.strName, pName);
	strcpy(tItem.strDesc, pDesc);

	tItem.eType = eType;

	switch(eType) {
		case IT_WEAPON:
		strcpy(tItem.strTypeName, "Weapon");
		break;
		case IT_ARMOR:
		strcpy(tItem.strTypeName, "Armor");
		break;
		default:
		break;
	}

	tItem.iMin = iMin;
	tItem.iMax = iMax;
	tItem.iPrice = iPrice;
	tItem.iSell = iSell;

	return tItem;
}

int outputInventory(_tagPlayer* pPlayer) {
	system("clear");
	cout << "================== INVENTORY =================" << endl;

	outputPlayer(pPlayer);

	for(int i=0; i<pPlayer->tInventory.iItemCount; i++) {
		cout << i + 1 <<". Name: " << pPlayer->tInventory.tItem[i].strName << "\tType: " << pPlayer->tInventory.tItem[i].strTypeName << endl;
		cout << "Attack: " << pPlayer->tInventory.tItem[i].iMin << " - " << pPlayer->tInventory.tItem[i].iMax << endl;
		cout << "Price: " << pPlayer->tInventory.tItem[i].iPrice << "\tSelling Price: " << pPlayer->tInventory.tItem[i].iSell << endl;
		cout << "Description: " << pPlayer->tInventory.tItem[i].strDesc << endl << endl;
	}

	cout << pPlayer->tInventory.iItemCount + 1 << ". Back" << endl << endl;
	cout << "Select the item you want to equip: ";
	

	int iMenu = inputInt();

	if(iMenu < 1 || iMenu > pPlayer->tInventory.iItemCount + 1) return INT_MAX;

	return iMenu;
}

EQUIP computeEquipType(ITEM_TYPE eType) {
	EQUIP eq;

	switch(eType) {
		case IT_WEAPON:
		eq = EQ_WEAPON;
		break;
		case IT_ARMOR:
		eq = EQ_ARMOR;
		break;
		default:
		break;
	}

	return eq;
}

void runInventory(_tagPlayer* pPlayer) {
	while(true) {
		int iInput = outputInventory(pPlayer);

		if(iInput == INT_MAX) continue;

		if(iInput == pPlayer->tInventory.iItemCount + 1) break;

		//아이템 인덱스를 구해준다.
		int idx = iInput - 1;

                //제대로 선택했을 경우 아이템의 종류에 따라 장착 부위를 결정하게 한다.
		EQUIP eq = computeEquipType(pPlayer->tInventory.tItem[idx].eType);

                //아이템이 장착되어 있을 경우 장착할 아이템과 장착되어 있는 아이템을 교체해 주어야 한다. Swap 알고리즘을 활용한다.
		if (pPlayer->bEquip[eq] == true) {
			_tagItem tSwap = pPlayer->tEquip[eq];
			pPlayer->tEquip[eq] = pPlayer->tInventory.tItem[idx];
			pPlayer->tInventory.tItem[idx] = tSwap;
		} else {
                    // 장착되어 있지 않은 경우 장착이 되고 인벤토리 칸은 1칸 비워지게 된다.
			pPlayer->tEquip[eq] = pPlayer->tInventory.tItem[idx];

			for(int i=idx; i<pPlayer->tInventory.iItemCount - 1; i++) {
				pPlayer->tInventory.tItem[i] = pPlayer->tInventory.tItem[i + 1];
			}

			pPlayer->tInventory.iItemCount--;

                    //장착을 했기 때문에 true로 바꿔준다.
			pPlayer->bEquip[eq] = true;
		}

		cout << "You've equipped " << pPlayer->tEquip[eq].strName << endl;


		system("read");
	}
}

int main() {
	srand((unsigned int)time(0));

	//게임을 시작할 때 플레이어 정보를 입력하도록 한다.
	_tagPlayer tPlayer = {};

    //플레이어의 정보를 정의한다.
	setPlayer(&tPlayer);

	//몬스터 생성
	_tagMonster tMonsterArr[MT_BACK - 1] = {};
	setMonster(tMonsterArr);

	g_tLvUpTable[CLASS_KNIGHT - 1] = createLvUpStatus(4, 10, 8 ,16, 50, 100, 10, 20);
	g_tLvUpTable[CLASS_ARCHER - 1] = createLvUpStatus(10, 15, 5, 10, 30, 50, 30, 50);
	g_tLvUpTable[CLASS_WIZARD - 1] = createLvUpStatus(15, 20, 3, 7, 20, 40, 50, 100);

	    //상점에서 판매할 아이템 목록을 생성한다.
	_tagItem tStoreWeapon[STORE_WEAPON_MAX] = {};
	_tagItem tStoreArmor[STORE_ARMOR_MAX] = {};

	tStoreWeapon[0] = createItem("Wooden sword", IT_WEAPON, 5, 10, 1000, 500, "sword made with wood");
	tStoreWeapon[1] = createItem("Long bow", IT_WEAPON, 20, 40, 7000, 3500, "Bow for the beginner");
	tStoreWeapon[2] = createItem("Wand", IT_WEAPON, 90, 150, 30000, 15000, "Wand for beginner");

	tStoreArmor[0] = createItem("Cloth armor", IT_ARMOR, 2, 5, 1000, 500, "Armor made with cloth");
	tStoreArmor[1] = createItem("Leather armor", IT_ARMOR, 10, 20, 7000, 3500, "Armor made with leather");
	tStoreArmor[2] = createItem("Plate armor", IT_ARMOR, 70, 90, 30000, 15000, "Armor made with plate");
	bool bLoop = true;

	while(bLoop) {
		switch(outputMainMenu()) {
			case MM_MAP:
			runMap(&tPlayer, tMonsterArr);
			break;
			case MM_STORE:
			runStore(&tPlayer.tInventory, tStoreWeapon, tStoreArmor);
			break;
			case MM_INVENTORY:
			runInventory(&tPlayer);
			break;
			case MM_EXIT:
			bLoop = false;
			break;
		}
	}
}