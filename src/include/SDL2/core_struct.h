#ifndef CORE_STRUCT_H
#define CORE_STRUCT_H

#include <stdbool.h>
#include <time.h>

// 店铺升级状态结构体
typedef struct {
    bool hasAutoMeatCutter;    // 是否拥有自动切肉机
    bool hasGoldPlate;         // 是否拥有金盘子
    bool hasExpandedShop;      // 是否扩充了店面
}ShopUpgrades;

typedef struct{
    int money;
    int time;
    bool running_or_not;
    int day;//天数
    ShopUpgrades upgrades;
}GameState;



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
    int pancake_requirement[5][5];
    /*行：分别是5种商品的需求量：0⾁卷饼；1⻩⽠卷饼；2沙司卷饼；3薯条卷饼；4番茄酱卷饼；（同一行配料组成一个卷饼）
    0为不要，1为要
    列：暂定最多点5份
    */
    int patience;
    int patience;
    int original_time;//初始等待时间
    int remaining_time;//剩余等待时间
    bool cola_demand;//是否要可乐
    bool chips_demand;//是否要薯条
    bool order_is_finished;//订单是否完成
}Customer;

#endif // CORE_STRUCT_H

