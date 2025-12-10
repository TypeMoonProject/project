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
    int original_time;//初始等待时间
    int remaining_time;//剩余等待时间
    bool cola_demand;//是否要可乐
    bool chips_demand;//是否要薯条
    bool order_is_finished;//订单是否完成
}Customer;

// ============ 新增：制作状态机相关结构体 ============
// 卷饼制作状态枚举
typedef enum {
    BURRITO_IDLE,           // 空闲状态
    BURRITO_FLATBREAD_PLACED, // 已放置面饼
    BURRITO_HEATED,         // 已加热（可选）
    BURRITO_MEAT_ADDED,     // 已加肉
    BURRITO_CUCUMBER_ADDED, // 已加黄瓜
    BURRITO_SAUCE_ADDED,    // 已加沙司
    BURRITO_CHIPS_ADDED,    // 已加薯条
    BURRITO_KETCHUP_ADDED,  // 已加番茄酱
    BURRITO_ROLLED,         // 已卷起
    BURRITO_WRAPPED         // 已包装
} BurritoState;

// 卷饼制作槽位结构体（支持并行制作）
typedef struct {
    BurritoState state;     // 当前状态
    bool has_meat;          // 是否加了肉
    bool has_cucumber;      // 是否加了黄瓜
    bool has_sauce;         // 是否加了沙司
    bool has_chips;         // 是否加了薯条
    bool has_ketchup;       // 是否加了番茄酱
    int cooking_time;       // 剩余烹饪时间（用于扩展功能）
} BurritoSlot;

// 薯条制作状态枚举
typedef enum {
    FRIES_IDLE,             // 空闲状态
    FRIES_BOX_TAKEN,        // 已拿薯条盒
    FRIES_CHIPS_ADDED,      // 已加薯条
    FRIES_FINISHED          // 已完成
} FriesState;

// 可乐制作状态枚举
typedef enum {
    COLA_IDLE,              // 空闲状态
    COLA_BOTTLE_TAKEN,      // 已拿可乐杯
    COLA_COLA_ADDED,        // 已加可乐
    COLA_FINISHED           // 已完成
} ColaState;

// 操作台状态结构体
typedef struct {
    BurritoSlot flatbread_slot;   // 摊开的面饼槽位（最多1个）
    BurritoSlot wrapped_slots[3]; // 包装好的卷饼槽位（最多3个）
    FriesState fries_state;       // 薯条制作状态
    ColaState cola_state;         // 可乐制作状态
    int wrapped_count;            // 当前包装好的卷饼数量
} Workbenches;


#endif // CORE_STRUCT_H



