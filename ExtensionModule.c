// 核心职责：实现店铺升级、数据保存/读取，以及选定的扩展功能。

// 详细任务（约300行）：

// 店铺升级系统 (120行)：实现升级功能（如自动切肉机、金盘子），包括升级条件的判断、金币扣除和效果应用（如购买后肉库存自动恢复）。

// 数据文件读写 (90行)：编写SaveGame()和LoadGame()函数，将累计金币、升级状态等数据写入文件以及从文件读取。

// 自选扩展功能 (90行)：从文档的扩展功能列表中挑选一个实现，例如“乞丐机制”。实现乞丐的生成、特殊交互和奖励逻辑。

#include "ExtensionModule.h"
#include <stdio.h>
#include <stdlib.h>

// 升级价格常量
#define AUTO_MEAT_CUTTER_COST 500
#define GOLD_PLATE_COST 300
#define EXPAND_SHOP_COST 800

// 获取升级成本
int GetUpgradeCost(UpgradeType type) {
    switch (type) {
        case UPGRADE_AUTO_MEAT_CUTTER:
            return AUTO_MEAT_CUTTER_COST;
        case UPGRADE_GOLD_PLATE:
            return GOLD_PLATE_COST;
        case UPGRADE_EXPAND_SHOP:
            return EXPAND_SHOP_COST;
        default:
            return 0;
    }
}

// 获取升级名称
const char* GetUpgradeName(UpgradeType type) {
    switch (type) {
        case UPGRADE_AUTO_MEAT_CUTTER:
            return "自动切肉机";
        case UPGRADE_GOLD_PLATE:
            return "金盘子";
        case UPGRADE_EXPAND_SHOP:
            return "扩充店面";
        default:
            return "未知升级";
    }
}

// 获取升级描述
const char* GetUpgradeDescription(UpgradeType type) {
    switch (type) {
        case UPGRADE_AUTO_MEAT_CUTTER:
            return "肉的库存为0后自动恢复满库存";
        case UPGRADE_GOLD_PLATE:
            return "卷饼的价值增加";
        case UPGRADE_EXPAND_SHOP:
            return "店铺可以同时容纳更多顾客";
        default:
            return "";
    }
}

// 检查是否可以升级店铺
bool CanUpgradeShop(UpgradeType type, int currentCoins, ShopUpgrades* upgrades) {
    int cost = GetUpgradeCost(type);
    
    // 检查金币是否足够
    if (currentCoins < cost) {
        return false;
    }
    
    // 检查是否已经拥有该升级
    switch (type) {
        case UPGRADE_AUTO_MEAT_CUTTER:
            return !upgrades->hasAutoMeatCutter;
        case UPGRADE_GOLD_PLATE:
            return !upgrades->hasGoldPlate;
        case UPGRADE_EXPAND_SHOP:
            return !upgrades->hasExpandedShop;
        default:
            return false;
    }
}

// 购买升级
bool PurchaseUpgrade(UpgradeType type, int* currentCoins, ShopUpgrades* upgrades) {
    // 先检查是否可以升级
    if (!CanUpgradeShop(type, *currentCoins, upgrades)) {
        return false;
    }
    
    // 扣除金币
    *currentCoins -= GetUpgradeCost(type);
    
    // 应用升级效果
    switch (type) {
        case UPGRADE_AUTO_MEAT_CUTTER:
            upgrades->hasAutoMeatCutter = true;
            break;
        case UPGRADE_GOLD_PLATE:
            upgrades->hasGoldPlate = true;
            break;
        case UPGRADE_EXPAND_SHOP:
            upgrades->hasExpandedShop = true;
            break;
        default:
            return false;
    }
    
    return true;
}

// 保存游戏数据
bool SaveGame(GameState* saveData, const char* filePath) {
    FILE* file = fopen(filePath, "wb");
    if (file == NULL) {
        return false;
    }
    
    // 写入游戏数据
    fwrite(saveData, sizeof(GameState), 1, file);
    
    fclose(file);
    return true;
}

// 读取游戏数据
bool LoadGame(GameState* saveData, const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) {
        // 如果文件不存在,初始化默认数据
        saveData->money = 0;
        saveData->time = 0;
        saveData->running_or_not = false;
        saveData->day = 0;
        saveData->upgrades.hasAutoMeatCutter = false;
        saveData->upgrades.hasGoldPlate = false;
        saveData->upgrades.hasExpandedShop = false;
        return false; 
    }
    
    // 读取游戏数据
    size_t readCount = fread(saveData, sizeof(GameState), 1, file);
    fclose(file);
    
    return (readCount == 1);
}

