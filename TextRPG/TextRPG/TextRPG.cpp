#include <iostream>
#include <time.h>
#include <fstream>
#include "TextRPGFunctions.h"

_tagLevelUpStatus g_tLvUpTable[CLASS_END - 1] = {};

int main()
{
	srand((unsigned int)time(0));

	//게임을 시작할 때 플레이어 정보를 입력하도록 한다.
	_tagPlayer tPlayer = {};

	// 게임 모드를 선택하도록 한다.
	outputGameMode(&tPlayer);

	//플레이어의 정보를 정의한다.
	// setPlayer(&tPlayer);

	//몬스터 생성
	_tagMonster tMonsterArr[MT_BACK - 1] = {};
	setMonster(tMonsterArr);

	g_tLvUpTable[CLASS_KNIGHT - 1] = createLvUpStatus(4, 10, 8, 16, 50, 100, 10, 20);
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

	while (bLoop)
	{
		switch (outputMainMenu())
		{
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

	savePlayer(&tPlayer);
}