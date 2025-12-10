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
bool AddInventory(Inventory* inv, const char* item, int quantity);
bool PreprocessMeat(Inventory* inv, int quantity);
bool PreprocessChips(Inventory* inv, int quantity);
void PrintInventoryStatus(Inventory* inv);    //打印全部库存状态

// 库存管理函数实现

//库存的初始化  
//原料都是最大值     成品（卷饼，可乐，薯条）都是0
void InitInventory(Inventory *inv)
{
    inv->flatbread=MAX_FLATBREAD;
    inv->meat=MAX_MEAT;
    inv->cucumber=MAX_CUCUMBER;
    inv->sauce=MAX_SAUCE;
    inv->chips=MAX_CHIPS_RAW;
    inv->ketchup=MAX_KETCHUP;
    inv->cola=MAX_COLA_RAW;
    inv->wrapper=MAX_WRAPPER;
    inv->box=MAX_BOX;
    inv->bottle=MAX_BOTTLE;
    inv->wrapped_pancake=0;
    inv->finished_chips=0;
    inv->finished_cola=0;
}

// 检查对应物品的库存   (给玩家看的)
//例如要查找面饼的数量：
//int CheckInventoryAmount(&inv,"flatbread");即可
int CheckInventoryAmount(Inventory* inv, const char* item) {
    if (strcmp(item, "flatbread") == 0) return inv->flatbread;
    if (strcmp(item, "meat") == 0) return inv->meat;
    if (strcmp(item, "cucumber") == 0) return inv->cucumber;
    if (strcmp(item, "sauce") == 0) return inv->sauce ;
    if (strcmp(item, "chips") == 0) return inv->chips ;
    if (strcmp(item, "ketchup") == 0) return inv->ketchup;
    if (strcmp(item, "cola") == 0) return inv->cola;
    if (strcmp(item, "wrapper") == 0) return inv->wrapper ;
    if (strcmp(item, "box") == 0) return inv->box ;
    if (strcmp(item, "bottle") == 0) return inv->bottle ;
    if (strcmp(item, "wrapped_pancake") == 0) return inv->wrapped_pancake;
    if (strcmp(item, "finished_cola") == 0) return inv->finished_cola ;
    if (strcmp(item, "finished_chips") == 0) return inv->finished_chips ;
    
    return false;
}

// 检查对应物品的库存   （系统看，可忽略）
bool CheckInventory(Inventory* inv, const char* item, int quantity) {
    if (!inv || quantity <= 0) return false;
    
    if (strcmp(item, "flatbread") == 0) return inv->flatbread >= quantity;
    if (strcmp(item, "meat") == 0) return inv->meat >= quantity;
    if (strcmp(item, "cucumber") == 0) return inv->cucumber >= quantity;
    if (strcmp(item, "sauce") == 0) return inv->sauce >= quantity;
    if (strcmp(item, "chips") == 0) return inv->chips >= quantity;
    if (strcmp(item, "ketchup") == 0) return inv->ketchup >= quantity;
    if (strcmp(item, "cola") == 0) return inv->cola >= quantity;
    if (strcmp(item, "wrapper") == 0) return inv->wrapper >= quantity;
    if (strcmp(item, "box") == 0) return inv->box >= quantity;
    if (strcmp(item, "bottle") == 0) return inv->bottle >= quantity;
    if (strcmp(item, "wrapped_pancake") == 0) return inv->wrapped_pancake >= quantity;
    if (strcmp(item, "finished_cola") == 0) return inv->finished_cola >= quantity;
    if (strcmp(item, "finished_chips") == 0) return inv->finished_chips >= quantity;
    return false;
}

// 使用库存
bool UseInventory(Inventory* inv, const char* item, int quantity) {
    if (!inv || quantity <= 0) return false;
    
    // 检查库存是否足够
    if (!CheckInventory(inv, item, quantity)) {
        printf("库存不足: %s (需要%d，当前库存不足)\n", item, quantity);
        return false;
    }
    
    // 减少库存
    if (strcmp(item, "flatbread") == 0) inv->flatbread -= quantity;
    else if (strcmp(item, "meat") == 0) inv->meat -= quantity;
    else if (strcmp(item, "cucumber") == 0) inv->cucumber -= quantity;
    else if (strcmp(item, "sauce") == 0) inv->sauce -= quantity;
    else if (strcmp(item, "chips") == 0) inv->chips -= quantity;
    else if (strcmp(item, "ketchup") == 0) inv->ketchup -= quantity;
    else if (strcmp(item, "cola") == 0) inv->cola -= quantity;
    else if (strcmp(item, "wrapper") == 0) inv->wrapper -= quantity;
    else if (strcmp(item, "box") == 0) inv->box -= quantity;
    else if (strcmp(item, "bottle") == 0) inv->bottle -= quantity;
    else if (strcmp(item, "wrapped_pancake") == 0) inv->wrapped_pancake -= quantity;
    else if (strcmp(item, "finished_cola") == 0) inv->finished_cola -= quantity;
    else if (strcmp(item, "finished_chips") == 0) inv->finished_chips -= quantity;
    else {
        printf("未知的物品: %s\n", item);
        return false;
    }
    
    printf("已使用 %d 个%s\n", quantity, item);
    return true;
}

// 添加库存（直接补货到最大值）
//例如补充面饼   AddInventory(&inv,"flatbread");
//将面饼补充到最大值
//成功返回true 失败返回false

bool AddInventory(Inventory* inv, const char* item) {
    if (strcmp(item, "flatbread") == 0) inv->flatbread=MAX_FLATBREAD;
    else if (strcmp(item, "cucumber") == 0) inv->cucumber=MAX_CUCUMBER;
    else if (strcmp(item, "sauce") == 0) inv->sauce=MAX_SAUCE;
    else if (strcmp(item, "ketchup") == 0) inv->ketchup=MAX_KETCHUP;
    else if (strcmp(item, "cola") == 0) inv->cola=MAX_COLA_RAW;
    else if (strcmp(item, "wrapper") == 0) inv->wrapper=MAX_WRAPPER;
    else if (strcmp(item, "box") == 0)  inv->box=MAX_BOX;
    else if (strcmp(item, "bottle") == 0) inv->bottle=MAX_BOTTLE;
    else {
        printf("该物品不能直接补货: %s\n", item);
        return false;
    }
    return true;
}

// 预处理肉类（切肉）   包含了切肉时间(切一份是一秒钟)
//成功返回true 失败返回false
bool PreprocessMeat(Inventory* inv) {
    int target;
    if(inv->meat==MAX_MEAT)
    {
        return true;
    }
    target=MAX_MEAT-inv->meat;
    Sleep(1000*target);
    inv->meat=MAX_MEAT;
    return true;
}
// 打印库存状态
void PrintInventoryStatus(Inventory* inv) {
    if (!inv) return;
    
    printf("\n=== 库存状态 ===\n");
    printf("[主料]\n");
    printf("  面饼: %d/%d\n", inv->flatbread, MAX_FLATBREAD);
    printf("  肉: %d/%d\n", inv->meat, MAX_MEAT);
    
    printf("[配料]\n");
    printf("  黄瓜: %d/%d\n", inv->cucumber, MAX_CUCUMBER);
    printf("  沙司: %d/%d\n", inv->sauce, MAX_SAUCE);
    printf("  番茄酱: %d/%d\n", inv->ketchup, MAX_KETCHUP);
    printf("  薯条原料: %d/%d\n", inv->chips, MAX_CHIPS_RAW);
    
    printf("[耗材]\n");
    printf("  包装纸: %d/%d\n", inv->wrapper, MAX_WRAPPER);
    printf("  薯条盒: %d/%d\n", inv->box, MAX_BOX);
    printf("  可乐杯: %d/%d\n", inv->bottle, MAX_BOTTLE);
    
    printf("[饮料原料]\n");
    printf("  可乐: %d/%d\n", inv->cola, MAX_COLA_RAW);
    
    printf("[成品]\n");
    printf("  包装卷饼: %d/%d\n", inv->wrapped_pancake, MAX_WRAPPED_PANCAKE);
    printf("  成品可乐: %d/%d\n", inv->finished_cola, MAX_FINISHED_COLA);
    printf("  成品薯条: %d/%d\n", inv->finished_chips, MAX_FINISHED_CHIPS);
    printf("================\n");
}

#endif // INVENTORY_MANAGER_H
