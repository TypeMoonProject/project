#ifndef EXTENSION_MODULE_H_
#define EXTENSION_MODULE_H_

#include <stdbool.h>
#include "core_struct.h"

//店铺升级类型枚举
typedef enum {
    UPGRADE_AUTO_MEAT_CUTTER,  // 自动切肉机
    UPGRADE_GOLD_PLATE,        // 金盘子
    UPGRADE_EXPAND_SHOP        // 扩充店面
} UpgradeType;

// 店铺升级状态结构体
typedef struct {
    bool hasAutoMeatCutter;    // 是否拥有自动切肉机
    bool hasGoldPlate;         // 是否拥有金盘子
    bool hasExpandedShop;      // 是否扩充了店面
} ShopUpgrades;

// 乞丐状态枚举
typedef enum {
    BEGGAR_STATE_WAITING,      // 等待状态
    BEGGAR_STATE_SERVED,       // 已获得食物
    BEGGAR_STATE_RUNNING       // 逃跑状态
} BeggarState;

// 乞丐结构体
typedef struct {
    bool isPresent;            // 是否存在
    BeggarState state;         // 当前状态
    int patienceTimer;         // 耐心计时器
    int runTimer;              // 逃跑计时器（给予玩家抓住的时间）
    int coinReward;            // 金币奖励
} Beggar;


// 店铺升级相关
bool CanUpgradeShop(UpgradeType type, int currentCoins, ShopUpgrades* upgrades);
bool PurchaseUpgrade(UpgradeType type, int* currentCoins, ShopUpgrades* upgrades);
int GetUpgradeCost(UpgradeType type);
const char* GetUpgradeName(UpgradeType type);
const char* GetUpgradeDescription(UpgradeType type);

// 数据持久化相关
bool SaveGame(GameState* saveData, const char* filePath);
bool LoadGame(GameState* saveData, const char* filePath);

// 乞丐机制相关
bool SpawnBeggar(Beggar* beggar);
void UpdateBeggar(Beggar* beggar);
void ServeBeggar(Beggar* beggar);
bool CatchBeggar(Beggar* beggar, int* coins);

#endif // EXTENSION_MODULE_H_