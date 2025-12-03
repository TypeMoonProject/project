#ifndef CORE_STRUCT_H
#define CORE_STRUCT_H

#include <stdbool.h>
#include <time.h>
typedef struct{
    int money;
    int time;
    bool running_or_not;
    int day;//天数
    ShopUpgrades upgrades;
}GameState;

// 店铺升级状态结构体
typedef struct {
    bool hasAutoMeatCutter;    // 是否拥有自动切肉机
    bool hasGoldPlate;         // 是否拥有金盘子
    bool hasExpandedShop;      // 是否扩充了店面
} ShopUpgrades;

typedef struct{
    int flatbread;//面饼
    int meat;
    int cucumber;
    int sauce;
    int chips;//薯条原料
    int ketchup;
    int cola;//可乐原料
    int wrapper;//包装纸
    int box;//薯条盒
    int bottle;//可乐杯
    int wrapped_pancake;//成品卷饼
    int finished_cola;//成品可乐
    int finished_chips;//成品薯条
}Inventory;

typedef struct{
    int requirement[7][5];
    //列：分别是7种商品的需求量：0⾁卷饼；1⻩⽠卷饼；2沙司卷饼；3薯条卷饼；4番茄酱卷饼；5可乐；6薯条
    //行：暂定最多点5份
    int patience;
    int original_time;//初始等待时间
    int remaining_time;//剩余等待时间
    bool order_is_finished;
}Customer;
#endif // CORE_STRUCT_H