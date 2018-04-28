

using namespace std;

enum GAME_MODE
{
    GM_NONE,
    GM_NEW,
    GM_LOAD,
    GM_END
};

enum MAIN_MENU
{
    MM_NONE,
    MM_MAP,
    MM_STORE,
    MM_INVENTORY,
    MM_EXIT
};

enum MAP_TYPE
{
    MT_NONE,
    MT_EASY,
    MT_NORMAL,
    MT_HARD,
    MT_BACK
};

enum CLASS
{
    CLASS_NONE,
    CLASS_KNIGHT,
    CLASS_ARCHER,
    CLASS_WIZARD,
    CLASS_END
};

enum ITEM_TYPE
{
    IT_NONE,
    IT_WEAPON,
    IT_ARMOR,
    IT_BACK
};

enum EQUIP
{
    EQ_WEAPON,
    EQ_ARMOR,
    EQ_MAX
};

enum BATTLE
{
    BATTLE_NONE,
    BATTLE_ATTACK,
    BATTLE_BACK,
    BATTLE_END
};

enum STORE_MENU
{
    SM_NONE,
    SM_WEAPON,
    SM_ARMOR,
    SM_BACK
};

#define NAME_SIZE 100
#define ITEM_DESC_LENGTH 512
#define INVENTORY_MAX 20
#define STORE_WEAPON_MAX 3
#define STORE_ARMOR_MAX 3
#define LEVEL_MAX 10

//레벨업에 필요한 경험치 목록을 만든다.
const int g_iLevelUpEXP[LEVEL_MAX] = {4000, 10000, 20000, 35000, 50000, 70000, 100000, 150000, 200000, 500000};

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

struct _tagInventory
{
    _tagItem tItem[INVENTORY_MAX];
    int iItemCount;
    int iGold;
};

struct _tagPlayer
{
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

struct _tagLevelUpStatus
{
    int iAttackMin;
    int iAttackMax;
    int iArmorMin;
    int iArmorMax;
    int iHPMin;
    int iHPMax;
    int iMPMin;
    int iMPMax;
};

extern _tagLevelUpStatus g_tLvUpTable[CLASS_END - 1];

int inputInt();
int outputMainMenu();
void outputGameMode(_tagPlayer* pPlayer);
void outputBattleTag(int iMenu);
void outputPlayer(_tagPlayer *pPlayer);
void outputMonster(_tagMonster *pMonster);
int outputBattleMenu();
void battle(_tagPlayer *pPlayer, _tagMonster *pMonster);
void runBattle(_tagPlayer *pPlayer, _tagMonster *pMonsterArr, int iMenu);
int outputMapMenu();
int selectClass();
void setPlayer(_tagPlayer *pPlayer);
bool loadPlayer(_tagPlayer *pPlayer);
bool savePlayer(_tagPlayer *pPlayer);
_tagMonster createMonster(const char *pName, int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, int iHP, int iMP, int iLevel, int iEXP, int iGoldMin, int iGoldMax);
void setMonster(_tagMonster *pMonsterArr);
// MAP에 관련된 동작을 하게 한다.
void runMap(_tagPlayer *pPlayer, _tagMonster *pMonsterArr);
_tagLevelUpStatus createLvUpStatus(int iAttackMin, int iAttackMax, int iArmorMin, int iArmorMax, int iHPMin, int iHPMax, int iMPMin, int iMPMax);
int outputStoreMenu();
int outputStoreItemList(_tagInventory *pInventory, _tagItem *pStore, int iItemCount);
void buyItem(_tagInventory *pInventory, _tagItem *pStore, int iItemCount, int iStoreType);
void runStore(_tagInventory *pInventory, _tagItem *pWeapon, _tagItem *pArmor);
_tagItem createItem(const char *pName, ITEM_TYPE eType, int iMin, int iMax, int iPrice, int iSell, const char *pDesc);
int outputInventory(_tagPlayer *pPlayer);
EQUIP computeEquipType(ITEM_TYPE eType);
void runInventory(_tagPlayer *pPlayer);