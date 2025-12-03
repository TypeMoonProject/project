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


#endif // EXTENSION_MODULE_H_