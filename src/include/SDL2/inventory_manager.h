#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "core_struct.h"
#include "game_manager.h"
#include  <windows.h>


// 库存上限定义 对应coure_struct.h中的Invertory
#define MAX_FLATBREAD 10
#define MAX_MEAT 10
#define MAX_CUCUMBER 10
#define MAX_SAUCE 10
#define MAX_CHIPS_RAW 10    // 薯条原料上限
#define MAX_KETCHUP 10
#define MAX_COLA_RAW 10     // 可乐原料上限
#define MAX_WRAPPER 10      //包装纸
#define MAX_BOX 10          //薯条盒
#define MAX_BOTTLE 10      //可乐杯
#define MAX_WRAPPED_PANCAKE 3  // 成品卷饼上限
#define MAX_FINISHED_COLA 5    // 成品可乐上限
#define MAX_FINISHED_CHIPS 10  // 成品薯条上限

// 库存管理函数声明
void InitInventory(Inventory *inv);
int CheckInventoryAmount(Inventory* inv, const char* item);//检查对应物品的库存 （玩家输入特定的物品名称，返回对应的数量） 
bool UseInventory(Inventory* inv, const char* item, int quantity);
bool AddInventory(Inventory* inv, const char* item);
bool PreprocessMeat(Inventory* inv);
bool PreprocessChips(Inventory* inv, int quantity);
void PrintInventoryStatus(Inventory* inv);    //打印全部库存状态

// 内部辅助函数声明
bool CheckInventory(Inventory* inv, const char* item, int quantity);

#endif // INVENTORY_MANAGER_H