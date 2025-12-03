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

// 扩展模块函数声明
// 店铺升级相关
bool CanUpgradeShop(UpgradeType type, int currentCoins, ShopUpgrades* upgrades);    // 检查是否可以升级
bool PurchaseUpgrade(UpgradeType type, int* currentCoins, ShopUpgrades* upgrades);  // 购买升级并更新金币和状态
int GetUpgradeCost(UpgradeType type);                                               // 获取升级成本
const char* GetUpgradeName(UpgradeType type);                                       // 获取升级名称
const char* GetUpgradeDescription(UpgradeType type);                                // 获取升级描述

// 数据持久化相关
bool SaveGame(GameState* saveData, const char* filePath);                           // 保存游戏数据到文件
bool LoadGame(GameState* saveData, const char* filePath);                           // 从文件加载游戏数据

// 乞丐机制相关
bool SpawnBeggar(Beggar* beggar);                                                   // 生成乞丐
void ResetBeggar(Beggar* beggar);                                                   // 重置乞丐状态
void UpdateBeggar(Beggar* beggar);                                                  // 更新乞丐状态
void ServeBeggar(Beggar* beggar);                                                   // 为乞丐提供食物
bool CatchBeggar(Beggar* beggar, int* coins);                                       // 抓住逃跑的乞丐并获得奖励

#endif // EXTENSION_MODULE_H_