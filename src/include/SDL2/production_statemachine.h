#ifndef PRODUCTION_STATEMACHINE_H
#define PRODUCTION_STATEMACHINE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "core_struct.h"
#include "inventory_manager.h"
//函数声明
//需要调用的人只要看注释后面带*的即可
//注意action的指令必须和注释里的一致 否则会报错
void InitWorkbenches(Workbenches* work);//初始化操作台*
bool ProcessBurritoAction(Workbenches* work, Inventory* inv, const char* action);//处理卷饼制作动作*
bool PlaceFlatbread(Workbenches* work, Inventory* inv);//放置面饼
bool AddMeat(Workbenches* work, Inventory* inv);//添加肉
bool AddCucumber(Workbenches* work, Inventory* inv);//添加黄瓜
bool AddSauce(Workbenches* work, Inventory* inv);//添加沙司
bool AddChips(Workbenches* work, Inventory* inv);//添加薯条（作为卷饼配料）
bool AddKetchup(Workbenches* work, Inventory* inv);//添加番茄酱
bool RollBurrito(Workbenches* work, Inventory* inv);//卷起卷饼
bool WrapBurrito(Workbenches* work, Inventory* inv);//包装卷饼
bool ProcessFriesAction(Workbenches* work, Inventory* inv, const char* action);//处理薯条制作动作*
bool TakeFriesBox(Workbenches* work, Inventory* inv);//拿薯条盒
bool AddFries(Workbenches* work, Inventory* inv);//添加薯条到盒子
bool FinishFries(Workbenches* work, Inventory* inv);//完成薯条制作
bool ProcessColaAction(Workbenches* work, Inventory* inv, const char* action);//处理可乐制作动作*
bool TakeColaBottle(Workbenches* work, Inventory* inv);//拿可乐杯
bool AddCola(Workbenches* work, Inventory* inv);//添加可乐到杯子
bool FinishCola(Workbenches* work, Inventory* inv);//完成可乐制作
// ==================== 卷饼制作状态机 ====================

// 初始化操作台
void InitWorkbenches(Workbenches* work) {
    if (!work) return;
    
    // 初始化面饼槽位
    work->flatbread_slot.state = BURRITO_IDLE;
    work->flatbread_slot.has_meat = false;
    work->flatbread_slot.has_cucumber = false;
    work->flatbread_slot.has_sauce = false;
    work->flatbread_slot.has_chips = false;
    work->flatbread_slot.has_ketchup = false;
    work->flatbread_slot.cooking_time = 0;
    
    // 初始化包装好的卷饼槽位
    for (int i = 0; i < 3; i++) {
        work->wrapped_slots[i].state = BURRITO_IDLE;
        work->wrapped_slots[i].has_meat = false;
        work->wrapped_slots[i].has_cucumber = false;
        work->wrapped_slots[i].has_sauce = false;
        work->wrapped_slots[i].has_chips = false;
        work->wrapped_slots[i].has_ketchup = false;
        work->wrapped_slots[i].cooking_time = 0;
    }
    
    // 初始化其他状态
    work->fries_state = FRIES_IDLE;
    work->cola_state = COLA_IDLE;
    work->wrapped_count = 0;
    
    printf("操作台初始化完成！\n");
}

// 处理卷饼制作动作
bool ProcessBurritoAction(Workbenches* work, Inventory* inv, const char* action) {
    if (!work || !inv) return false;
    
    // 检查动作是否合法
    if (strcmp(action, "place_flatbread") == 0) {
        return PlaceFlatbread(work, inv);
    } else if (strcmp(action, "add_meat") == 0) {
        return AddMeat(work, inv);
    } else if (strcmp(action, "add_cucumber") == 0) {
        return AddCucumber(work, inv);
    } else if (strcmp(action, "add_sauce") == 0) {
        return AddSauce(work, inv);
    } else if (strcmp(action, "add_chips") == 0) {
        return AddChips(work, inv);
    } else if (strcmp(action, "add_ketchup") == 0) {
        return AddKetchup(work, inv);
    } else if (strcmp(action, "roll_burrito") == 0) {
        return RollBurrito(work, inv);
    } else if (strcmp(action, "wrap_burrito") == 0) {
        return WrapBurrito(work, inv);
    } 
    else {
        printf("未知的卷饼制作动作: %s\n", action);
        return false;
    }
}

// 放置面饼
bool PlaceFlatbread(Workbenches* work, Inventory* inv) {
    // 检查是否已经有面饼在制作
    if (work->flatbread_slot.state != BURRITO_IDLE) {
        printf("操作台上已有面饼，请先完成当前制作！\n");
        return false;
    }
    
    // 检查面饼库存
    if (!CheckInventory(inv, "flatbread", 1)) {
        printf("面饼库存不足！\n");
        return false;
    }
    
    // 消耗面饼
    if (!UseInventory(inv, "flatbread", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.state = BURRITO_FLATBREAD_PLACED;
    printf("已放置面饼！现在可以添加配料了。\n");
    
    // 显示可用配料提示
    printf("可用配料：肉(meat)、黄瓜(cucumber)、沙司(sauce)、薯条(chips)、番茄酱(ketchup)\n");
    printf("添加配料顺序不限，但必须在卷起前完成。\n");
    
    return true;
}

// 添加肉
bool AddMeat(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state >= BURRITO_ROLLED) {
        printf("卷饼已卷起，无法再添加配料！\n");
        return false;
    }
    
    // 检查是否已加过肉
    if (work->flatbread_slot.has_meat) {
        printf("已添加过肉！\n");
        return false;
    }
    
    // 检查肉库存
    if (!CheckInventory(inv, "meat", 1)) {
        printf("肉库存不足！\n");
        return false;
    }
    
    // 消耗肉
    if (!UseInventory(inv, "meat", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.has_meat = true;
    work->flatbread_slot.state = BURRITO_MEAT_ADDED;
    printf("已添加肉！\n");
    
    return true;
}

// 添加黄瓜
bool AddCucumber(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state >= BURRITO_ROLLED) {
        printf("卷饼已卷起，无法再添加配料！\n");
        return false;
    }
    
    // 检查是否已加过黄瓜
    if (work->flatbread_slot.has_cucumber) {
        printf("已添加过黄瓜！\n");
        return false;
    }
    
    // 检查黄瓜库存
    if (!CheckInventory(inv, "cucumber", 1)) {
        printf("黄瓜库存不足！\n");
        return false;
    }
    
    // 消耗黄瓜
    if (!UseInventory(inv, "cucumber", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.has_cucumber = true;
    work->flatbread_slot.state = BURRITO_CUCUMBER_ADDED;
    printf("已添加黄瓜！\n");
    
    return true;
}

// 添加沙司
bool AddSauce(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state >= BURRITO_ROLLED) {
        printf("卷饼已卷起，无法再添加配料！\n");
        return false;
    }
    
    // 检查是否已加过沙司
    if (work->flatbread_slot.has_sauce) {
        printf("已添加过沙司！\n");
        return false;
    }
    
    // 检查沙司库存
    if (!CheckInventory(inv, "sauce", 1)) {
        printf("沙司库存不足！\n");
        return false;
    }
    
    // 消耗沙司
    if (!UseInventory(inv, "sauce", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.has_sauce = true;
    work->flatbread_slot.state = BURRITO_SAUCE_ADDED;
    printf("已添加沙司！\n");
    
    return true;
}

// 添加薯条（作为卷饼配料）
bool AddChips(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state >= BURRITO_ROLLED) {
        printf("卷饼已卷起，无法再添加配料！\n");
        return false;
    }
    
    // 检查是否已加过薯条
    if (work->flatbread_slot.has_chips) {
        printf("已添加过薯条！\n");
        return false;
    }
    
    // 检查薯条原料库存
    if (!CheckInventory(inv, "chips", 1)) {
        printf("薯条原料库存不足！\n");
        return false;
    }
    
    // 消耗薯条原料
    if (!UseInventory(inv, "chips", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.has_chips = true;
    work->flatbread_slot.state = BURRITO_CHIPS_ADDED;
    printf("已添加薯条！\n");
    
    return true;
}

// 添加番茄酱
bool AddKetchup(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state >= BURRITO_ROLLED) {
        printf("卷饼已卷起，无法再添加配料！\n");
        return false;
    }
    
    // 检查是否已加过番茄酱
    if (work->flatbread_slot.has_ketchup) {
        printf("已添加过番茄酱！\n");
        return false;
    }
    
    // 检查番茄酱库存
    if (!CheckInventory(inv, "ketchup", 1)) {
        printf("番茄酱库存不足！\n");
        return false;
    }
    
    // 消耗番茄酱
    if (!UseInventory(inv, "ketchup", 1)) {
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.has_ketchup = true;
    work->flatbread_slot.state = BURRITO_KETCHUP_ADDED;
    printf("已添加番茄酱！\n");
    
    return true;
}

// 卷起卷饼
bool RollBurrito(Workbenches* work, Inventory* inv) {
    // 检查是否已放置面饼
    if (work->flatbread_slot.state == BURRITO_IDLE) {
        printf("请先放置面饼！\n");
        return false;
    }
    
    // 检查是否已添加至少一种配料
    if (!work->flatbread_slot.has_meat && 
        !work->flatbread_slot.has_cucumber && 
        !work->flatbread_slot.has_sauce && 
        !work->flatbread_slot.has_chips && 
        !work->flatbread_slot.has_ketchup) {
        printf("请至少添加一种配料！\n");
        return false;
    }
    
    // 检查是否已卷起
    if (work->flatbread_slot.state == BURRITO_ROLLED) {
        printf("卷饼已卷起！\n");
        return false;
    }
    
    // 更新状态
    work->flatbread_slot.state = BURRITO_ROLLED;
    printf("卷饼已卷起！现在可以包装了。\n");
    
    return true;
}

// 包装卷饼
bool WrapBurrito(Workbenches* work, Inventory* inv) {
    // 检查是否已卷起
    if (work->flatbread_slot.state != BURRITO_ROLLED) {
        printf("请先卷起卷饼！\n");
        return false;
    }
    
    // 检查包装纸库存
    if (!CheckInventory(inv, "wrapper", 1)) {
        printf("包装纸库存不足！\n");
        return false;
    }
    
    // 检查包装槽位是否已满
    if (work->wrapped_count >= 3) {
        printf("包装槽位已满（最多3个）！请先交付给顾客。\n");
        return false;
    }
    
    // 消耗包装纸
    if (!UseInventory(inv, "wrapper", 1)) {
        return false;
    }
    
    // 找到空闲的包装槽位
    int slot_index = -1;
    for (int i = 0; i < 3; i++) {
        if (work->wrapped_slots[i].state == BURRITO_IDLE) {
            slot_index = i;
            break;
        }
    }
    
    if (slot_index == -1) {
        printf("包装槽位已满！\n");
        return false;
    }
    
    // 将卷饼移动到包装槽位
    work->wrapped_slots[slot_index] = work->flatbread_slot;
    work->wrapped_slots[slot_index].state = BURRITO_WRAPPED;
    work->wrapped_count++;
    
    // 清空面饼槽位
    work->flatbread_slot.state = BURRITO_IDLE;
    work->flatbread_slot.has_meat = false;
    work->flatbread_slot.has_cucumber = false;
    work->flatbread_slot.has_sauce = false;
    work->flatbread_slot.has_chips = false;
    work->flatbread_slot.has_ketchup = false;
    
    // 更新库存中的成品卷饼
    inv->wrapped_pancake++;
    
    printf("卷饼已包装！当前有%d个包装好的卷饼。\n", work->wrapped_count);
    
    return true;
}

// ==================== 薯条制作状态机 ====================

// 处理薯条制作动作
bool ProcessFriesAction(Workbenches* work, Inventory* inv, const char* action) {
    if (!work || !inv) return false;
    
    if (strcmp(action, "take_box") == 0) {
        return TakeFriesBox(work, inv);
    } else if (strcmp(action, "add_fries") == 0) {
        return AddFries(work, inv);
    } else if (strcmp(action, "finish_fries") == 0) {
        return FinishFries(work, inv);
    } else {
        printf("未知的薯条制作动作: %s\n", action);
        return false;
    }
}

// 拿薯条盒
bool TakeFriesBox(Workbenches* work, Inventory* inv) {
    // 检查是否已在制作薯条
    if (work->fries_state != FRIES_IDLE) {
        printf("已经在制作薯条！\n");
        return false;
    }
    
    // 检查薯条盒库存
    if (!CheckInventory(inv, "box", 1)) {
        printf("薯条盒库存不足！\n");
        return false;
    }
    
    // 消耗薯条盒
    if (!UseInventory(inv, "box", 1)) {
        return false;
    }
    
    // 更新状态
    work->fries_state = FRIES_BOX_TAKEN;
    printf("已拿薯条盒！现在可以添加薯条了。\n");
    
    return true;
}

// 添加薯条到盒子
bool AddFries(Workbenches* work, Inventory* inv) {
    // 检查是否已拿薯条盒
    if (work->fries_state != FRIES_BOX_TAKEN) {
        printf("请先拿薯条盒！\n");
        return false;
    }
    
    // 检查薯条原料库存
    if (!CheckInventory(inv, "chips", 1)) {
        printf("薯条原料库存不足！\n");
        return false;
    }
    
    // 消耗薯条原料
    if (!UseInventory(inv, "chips", 1)) {
        return false;
    }
    
    // 更新状态
    work->fries_state = FRIES_CHIPS_ADDED;
    printf("已添加薯条到盒子！现在可以完成了。\n");
    
    return true;
}

// 完成薯条制作
bool FinishFries(Workbenches* work, Inventory* inv) {
    // 检查是否已添加薯条
    if (work->fries_state != FRIES_CHIPS_ADDED) {
        printf("请先添加薯条到盒子！\n");
        return false;
    }
    
    // 更新状态
    work->fries_state = FRIES_FINISHED;
    
    // 更新库存中的成品薯条
    inv->finished_chips++;
    
    // 重置状态，准备制作下一份
    work->fries_state = FRIES_IDLE;
    
    printf("薯条制作完成！已添加到成品库存。\n");
    
    return true;
}

// ==================== 可乐制作状态机 ====================

// 处理可乐制作动作
bool ProcessColaAction(Workbenches* work, Inventory* inv, const char* action) {
    if (!work || !inv) return false;
    
    if (strcmp(action, "take_bottle") == 0) {
        return TakeColaBottle(work, inv);
    } else if (strcmp(action, "add_cola") == 0) {
        return AddCola(work, inv);
    } else if (strcmp(action, "finish_cola") == 0) {
        return FinishCola(work, inv);
    } else {
        printf("未知的可乐制作动作: %s\n", action);
        return false;
    }
}

// 拿可乐杯
bool TakeColaBottle(Workbenches* work, Inventory* inv) {
    // 检查是否已在制作可乐
    if (work->cola_state != COLA_IDLE) {
        printf("已经在制作可乐！\n");
        return false;
    }
    
    // 检查可乐杯库存
    if (!CheckInventory(inv, "bottle", 1)) {
        printf("可乐杯库存不足！\n");
        return false;
    }
    
    // 消耗可乐杯
    if (!UseInventory(inv, "bottle", 1)) {
        return false;
    }
    
    // 更新状态
    work->cola_state = COLA_BOTTLE_TAKEN;
    printf("已拿可乐杯！现在可以添加可乐了。\n");
    
    return true;
}

// 添加可乐到杯子
bool AddCola(Workbenches* work, Inventory* inv) {
    // 检查是否已拿可乐杯
    if (work->cola_state != COLA_BOTTLE_TAKEN) {
        printf("请先拿可乐杯！\n");
        return false;
    }
    
    // 检查可乐原料库存
    if (!CheckInventory(inv, "cola", 1)) {
        printf("可乐原料库存不足！\n");
        return false;
    }
    
    // 消耗可乐原料
    if (!UseInventory(inv, "cola", 1)) {
        return false;
    }
    
    // 更新状态
    work->cola_state = COLA_COLA_ADDED;
    printf("已添加可乐到杯子！现在可以完成了。\n");
    
    return true;
}

// 完成可乐制作
bool FinishCola(Workbenches* work, Inventory* inv) {
    // 检查是否已添加可乐
    if (work->cola_state != COLA_COLA_ADDED) {
        printf("请先添加可乐到杯子！\n");
        return false;
    }
    
    // 更新状态
    work->cola_state = COLA_FINISHED;
    
    // 更新库存中的成品可乐
    inv->finished_cola++;
    
    // 重置状态，准备制作下一杯
    work->cola_state = COLA_IDLE;
    
    printf("可乐制作完成！已添加到成品库存。\n");
    
    return true;
}

// ==================== 状态查询函数 ====================

// 显示操作台状态
void ShowWorkbenchStatus(Workbenches* work) {
    if (!work) return;
    
    printf("\n=== 操作台状态 ===\n");
    
    // 显示面饼槽位状态
    printf("[面饼槽位] ");
    switch(work->flatbread_slot.state) {
        case BURRITO_IDLE:
            printf("空\n");
            break;
        case BURRITO_FLATBREAD_PLACED:
            printf("已放置面饼\n");
            break;
        case BURRITO_HEATED:
            printf("已加热\n");
            break;
        case BURRITO_ROLLED:
            printf("已卷起\n");
            break;
        default:
            printf("制作中");
            if (work->flatbread_slot.has_meat) printf(" 肉");
            if (work->flatbread_slot.has_cucumber) printf(" 黄瓜");
            if (work->flatbread_slot.has_sauce) printf(" 沙司");
            if (work->flatbread_slot.has_chips) printf(" 薯条");
            if (work->flatbread_slot.has_ketchup) printf(" 番茄酱");
            printf("\n");
    }
    
    // 显示包装好的卷饼
    printf("[包装好的卷饼] %d/3个\n", work->wrapped_count);
    
    // 显示薯条制作状态
    printf("[薯条制作] ");
    switch(work->fries_state) {
        case FRIES_IDLE: printf("空闲\n"); break;
        case FRIES_BOX_TAKEN: printf("已拿薯条盒\n"); break;
        case FRIES_CHIPS_ADDED: printf("已加薯条\n"); break;
        case FRIES_FINISHED: printf("已完成\n"); break;
    }
    
    // 显示可乐制作状态
    printf("[可乐制作] ");
    switch(work->cola_state) {
        case COLA_IDLE: printf("空闲\n"); break;
        case COLA_BOTTLE_TAKEN: printf("已拿可乐杯\n"); break;
        case COLA_COLA_ADDED: printf("已加可乐\n"); break;
        case COLA_FINISHED: printf("已完成\n"); break;
    }
    printf("===================\n");
}

#endif // PRODUCTION_STATEMACHINE_H
