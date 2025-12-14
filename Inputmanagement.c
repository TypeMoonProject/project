#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <ctype.h>   // 用于toupper
#ifdef _WIN32
#include <windows.h>  // 用于Sleep
#endif 

typedef enum {
    ACTION_NONE = 0,
    
    // 卷饼制作操作
    ACTION_PLACE_BREAD,       // 放置面饼
    ACTION_ADD_MEAT,          // 加肉
    ACTION_ADD_VEG,           // 加蔬菜
    ACTION_ADD_SAUCE,         // 加沙司
    ACTION_ADD_FRIES,         // 加薯条
    ACTION_ADD_KETCHUP,       // 加番茄酱
    ACTION_ROLL,              // 卷起
    ACTION_WRAP,              // 包装
    ACTION_GIVE_BURRITO,      // 给顾客（卷饼）
    
    // 薯条操作
    ACTION_TAKE_BOX,          // 拿盒子
    ACTION_FRY_FRIES,         // 炸薯条
    ACTION_SERVE_FRIES,       // 装薯条
    ACTION_GIVE_FRIES,        // 给顾客（薯条）
    
    // 饮料操作
    ACTION_TAKE_CUP,          // 拿杯子
    ACTION_POUR_DRINK,        // 接可乐
    ACTION_GIVE_DRINK,        // 给顾客（饮料）
    
    // 预处理操作
    ACTION_CUT_MEAT,          // 切肉
    
    // 补货操作
    ACTION_RESTOCK_BREAD,     // 补面饼/纸
    ACTION_RESTOCK_INGREDIENT,// 补配料
    ACTION_RESTOCK_CONTAINER, // 补盒子/杯子
    
    // 系统操作
    ACTION_PAUSE_GAME,        // 暂停游戏
    ACTION_RESUME_GAME,       // 继续游戏
    ACTION_RETURN_MENU,       // 返回菜单
    ACTION_QUIT_GAME,         // 退出游戏
    
    ACTION_COUNT
} PlayerAction;

// ============================================
// 精简版键盘键位码定义
// ============================================

typedef enum {
    // 字母键 (A-Z) - 只保留实际使用的
    KEY_Q = 'Q',          // 拿盒子
    KEY_W = 'W',          // 装薯条
    KEY_E = 'E',          // 给顾客

    KEY_A = 'A',          // 拿杯子
    KEY_S = 'S',          // 接可乐
    KEY_D = 'D',          // 给顾客

    KEY_Z = 'Z',          // 切肉
    KEY_X = 'X',          // 炸薯条
    KEY_C = 'C',          // 补面饼/纸
    KEY_V = 'V',          // 补配料
    KEY_B = 'B',          // 补盒子/杯子
    
    // 数字键 (0-9) - 只保留实际使用的
    KEY_1 = '1',          // 49 - 放置面饼
    KEY_2 = '2',          // 50 - 加肉
    KEY_3 = '3',          // 51 - 加蔬菜
    KEY_4 = '4',          // 52 - 加沙司
    KEY_5 = '5',          // 53 - 加薯条
    KEY_6 = '6',          // 54 - 加番茄酱
    KEY_7 = '7',          // 55 - 卷起
    KEY_8 = '8',          // 56 - 包装
    KEY_9 = '9',          // 57 - 给顾客

        // 扩展模块按键
    KEY_U = 'U',          // 升级菜单
    KEY_R = 'R',          // 保存游戏
    KEY_L = 'L',          // 加载游戏
    KEY_G = 'G',          // 给乞丐食物
    KEY_H = 'H',          // 抓住乞丐
    
    // 升级购买按键
    KEY_F1 = 0x70,        // F1 - 购买自动切肉机
    KEY_F2 = 0x71,        // F2 - 购买金盘子
    KEY_F3 = 0x72,        // F3 - 扩大店铺
} KeyCode;

typedef struct {
    PlayerAction action;      // 游戏操作
    int vk_code;              // Windows虚拟键码
    const char* description;  // 操作描述
    const char* key_name;     // 键位显示名称
} KeyMapping;

// ============================================
// 精简键位映射表
// ============================================

static KeyMapping g_default_mappings[] = {
    // ===== 卷饼制作流程 (数字键1-9) =====
    {ACTION_PLACE_BREAD,    KEY_1,      "放置面饼",       "1"},
    {ACTION_ADD_MEAT,       KEY_2,      "加肉",           "2"},
    {ACTION_ADD_VEG,        KEY_3,      "加蔬菜",         "3"},
    {ACTION_ADD_SAUCE,      KEY_4,      "加沙司",         "4"},
    {ACTION_ADD_FRIES,      KEY_5,      "加薯条",         "5"},
    {ACTION_ADD_KETCHUP,    KEY_6,      "加番茄酱",       "6"},
    {ACTION_ROLL,           KEY_7,      "卷起",           "7"},
    {ACTION_WRAP,           KEY_8,      "包装",           "8"},
    {ACTION_GIVE_BURRITO,   KEY_9,      "给顾客",         "9"},
    
    // ===== 薯条制作流程 (QWE列) =====
    {ACTION_TAKE_BOX,       KEY_Q,      "拿盒子",         "Q"},
    {ACTION_SERVE_FRIES,    KEY_W,      "装薯条",         "W"},
    {ACTION_GIVE_FRIES,     KEY_E,      "给顾客",         "E"},
    
    // ===== 饮料制作流程 (ASD列) =====
    {ACTION_TAKE_CUP,       KEY_A,      "拿杯子",         "A"},
    {ACTION_POUR_DRINK,     KEY_S,      "接可乐",         "S"},
    {ACTION_GIVE_DRINK,     KEY_D,      "给顾客",         "D"},
    
    // ===== 预处理与补货流程 (ZXCVB列) =====
    {ACTION_CUT_MEAT,       KEY_Z,      "切肉",           "Z"},
    {ACTION_FRY_FRIES,      KEY_X,      "炸薯条",         "X"},
    {ACTION_RESTOCK_BREAD,  KEY_C,      "补面饼/纸",      "C"},
    {ACTION_RESTOCK_INGREDIENT, KEY_V,  "补配料",         "V"},
    {ACTION_RESTOCK_CONTAINER, KEY_B,   "补盒子/杯子",    "B"},

    {ACTION_PAUSE_GAME,        KEY_U,      "显示升级菜单",   "U"},
    {ACTION_RETURN_MENU,       KEY_R,      "保存游戏",      "R"},
    {ACTION_RESUME_GAME,       KEY_L,      "加载游戏",      "L"},
    {ACTION_GIVE_BURRITO,      KEY_G,      "给乞丐食物",    "G"},
    {ACTION_QUIT_GAME,         KEY_H,      "抓住乞丐",      "H"},
    
    // ===== 升级购买操作 =====
    {ACTION_RESTOCK_BREAD,     KEY_F1,     "买自动切肉机", "F1"},
    {ACTION_RESTOCK_INGREDIENT,KEY_F2,     "买金盘子",     "F2"},
    {ACTION_RESTOCK_CONTAINER, KEY_F3,     "扩大店铺",     "F3"},
};

int readInput();
void handle_key_input(int pressed_vk_code);
void perform_action(PlayerAction action);

// 游戏操作函数声明
void place_bread();
void add_meat();
void add_veg();
void add_sauce();
void add_fries();
void add_ketchup();
void roll();
void wrap();
void give_burrito();
void take_box();
void fry_fries();
void serve_fries();
void give_fries();
void take_cup();
void pour_drink();
void give_drink();
void cut_meat();
void restock_bread();
void restock_ingredient();
void restock_container();
void pause_game();
void resume_game();
void return_menu();
void quit_game();

// 游戏操作函数实现
#include "inventory_manager.h"
#include "game_manager.h"

// 全局变量用于追踪当前制作状态
static int current_pancake_ingredients[5] = {0}; // 当前正在制作的卷饼配料
static bool is_rolling = false; // 是否正在卷起
static bool is_wrapping = false; // 是否正在包装

// 卷饼制作流程
void place_bread() {
    if (state2->flatbread > 0) {
        if (UseInventory(state2, "flatbread", 1)) {
            printf("放置了一张面饼\n");
            // 重置配料数组
            for (int i = 0; i < 5; i++) {
                current_pancake_ingredients[i] = 0;
            }
            is_rolling = false;
            is_wrapping = false;
        }
    } else {
        printf("面饼库存不足！\n");
    }
}

void add_meat() {
    if (state2->meat > 0) {
        if (UseInventory(state2, "meat", 1)) {
            current_pancake_ingredients[0] = 1; // 肉是第一种配料
            printf("添加了肉\n");
        }
    } else {
        printf("肉库存不足！\n");
    }
}

void add_veg() {
    if (state2->cucumber > 0) {
        if (UseInventory(state2, "cucumber", 1)) {
            current_pancake_ingredients[1] = 1; // 黄瓜是第二种配料
            printf("添加了黄瓜\n");
        }
    } else {
        printf("黄瓜库存不足！\n");
    }
}

void add_sauce() {
    if (state2->sauce > 0) {
        if (UseInventory(state2, "sauce", 1)) {
            current_pancake_ingredients[2] = 1; // 沙司是第三种配料
            printf("添加了沙司\n");
        }
    } else {
        printf("沙司库存不足！\n");
    }
}

void add_fries() {
    if (state2->finished_chips > 0) {
        if (UseInventory(state2, "finished_chips", 1)) {
            current_pancake_ingredients[3] = 1; // 薯条是第四种配料
            printf("添加了薯条\n");
        }
    } else {
        printf("成品薯条库存不足！\n");
    }
}

void add_ketchup() {
    if (state2->ketchup > 0) {
        if (UseInventory(state2, "ketchup", 1)) {
            current_pancake_ingredients[4] = 1; // 番茄酱是第五种配料
            printf("添加了番茄酱\n");
        }
    } else {
        printf("番茄酱库存不足！\n");
    }
}

void roll() {
    // 检查是否有面饼（至少有一种配料）
    bool has_ingredient = false;
    for (int i = 0; i < 5; i++) {
        if (current_pancake_ingredients[i] > 0) {
            has_ingredient = true;
            break;
        }
    }
    
    if (has_ingredient) {
        is_rolling = true;
        printf("卷起了卷饼\n");
    } else {
        printf("卷饼没有任何配料，无法卷起！\n");
    }
}

void wrap() {
    if (is_rolling && state2->wrapper > 0) {
        if (UseInventory(state2, "wrapper", 1)) {
            is_wrapping = true;
            // 添加到成品卷饼库存
            if (state2->wrapped_pancake < MAX_WRAPPED_PANCAKE) {
                state2->wrapped_pancake++;
                printf("包装了一个卷饼，现在共有%d个成品卷饼\n", state2->wrapped_pancake);
                // 重置制作状态
                for (int i = 0; i < 5; i++) {
                    current_pancake_ingredients[i] = 0;
                }
                is_rolling = false;
                is_wrapping = false;
            } else {
                printf("成品卷饼库存已满！\n");
            }
        }
    } else if (!is_rolling) {
        printf("卷饼还没有卷起，无法包装！\n");
    } else {
        printf("包装纸库存不足！\n");
    }
}

void give_burrito() {
    if (state2->wrapped_pancake > 0) {
        if (UseInventory(state2, "wrapped_pancake", 1)) {
            printf("将卷饼交给了顾客\n");
            // 这里可以添加收益计算和顾客满意度提升
        }
    } else {
        printf("没有成品卷饼可以交给顾客！\n");
    }
}

// 薯条制作流程
void take_box() {
    if (state2->box > 0) {
        printf("拿起了一个薯条盒\n");
        // 这里可以添加状态追踪
    } else {
        printf("薯条盒库存不足！\n");
    }
}

void fry_fries() {
    if (state2->chips > 0) {
        if (UseInventory(state2, "chips", 1)) {
            printf("炸好了一份薯条\n");
            // 添加到成品薯条库存
            if (state2->finished_chips < MAX_FINISHED_CHIPS) {
                state2->finished_chips++;
            } else {
                printf("成品薯条库存已满！\n");
            }
        }
    } else {
        printf("薯条原料库存不足！\n");
    }
}

void serve_fries() {
    if (state2->finished_chips > 0 && state2->box > 0) {
        if (UseInventory(state2, "finished_chips", 1) && UseInventory(state2, "box", 1)) {
            printf("装好了一份薯条\n");
            // 这里可以添加到待服务的薯条列表
        }
    } else if (state2->finished_chips <= 0) {
        printf("成品薯条库存不足！\n");
    } else {
        printf("薯条盒库存不足！\n");
    }
}

void give_fries() {
    printf("将薯条交给了顾客\n");
    // 这里可以添加收益计算和顾客满意度提升
}

// 可乐制作流程
void take_cup() {
    if (state2->bottle > 0) {
        printf("拿起了一个可乐杯\n");
        // 这里可以添加状态追踪
    } else {
        printf("可乐杯库存不足！\n");
    }
}

void pour_drink() {
    if (state2->cola > 0 && state2->bottle > 0) {
        if (UseInventory(state2, "cola", 1) && UseInventory(state2, "bottle", 1)) {
            // 添加到成品可乐库存
            if (state2->finished_cola < MAX_FINISHED_COLA) {
                state2->finished_cola++;
                printf("倒好了一杯可乐，现在共有%d个成品可乐\n", state2->finished_cola);
            } else {
                printf("成品可乐库存已满！\n");
            }
        }
    } else if (state2->cola <= 0) {
        printf("可乐原料库存不足！\n");
    } else {
        printf("可乐杯库存不足！\n");
    }
}

void give_drink() {
    if (state2->finished_cola > 0) {
        if (UseInventory(state2, "finished_cola", 1)) {
            printf("将可乐交给了顾客\n");
            // 这里可以添加收益计算和顾客满意度提升
        }
    } else {
        printf("没有成品可乐可以交给顾客！\n");
    }
}

// 预处理和补货流程
void cut_meat() {
    printf("开始切肉...\n");
    if (PreprocessMeat(state2)) {
        printf("切肉完成，肉库存已补满！\n");
    }
}

void restock_bread() {
    if (AddInventory(state2, "flatbread")) {
        printf("面饼已补满！\n");
    }
    if (AddInventory(state2, "wrapper")) {
        printf("包装纸已补满！\n");
    }
}

void restock_ingredient() {
    if (AddInventory(state2, "cucumber")) {
        printf("黄瓜已补满！\n");
    }
    if (AddInventory(state2, "sauce")) {
        printf("沙司已补满！\n");
    }
    if (AddInventory(state2, "ketchup")) {
        printf("番茄酱已补满！\n");
    }
    if (AddInventory(state2, "cola")) {
        printf("可乐原料已补满！\n");
    }
}

void restock_container() {
    if (AddInventory(state2, "box")) {
        printf("薯条盒已补满！\n");
    }
    if (AddInventory(state2, "bottle")) {
        printf("可乐杯已补满！\n");
    }
}

// 系统操作
void pause_game() {
    printf("游戏已暂停\n");
    // 这里可以添加暂停逻辑
}

void resume_game() {
    printf("游戏已继续\n");
    // 这里可以添加继续逻辑
}

void return_menu() {
    printf("返回主菜单\n");
    // 这里可以添加返回主菜单逻辑
}

void quit_game() {
    printf("退出游戏\n");
    // 这里可以添加退出游戏逻辑
}

/*
int main(){
    
    while (1)
    {
        int c=readInput();
        Sleep(10);
    }
    
    return 0;
}
*/

int readInput(void) {
    if (_kbhit()) {
        int ch = _getch();
        
        // 忽略所有扩展键
        if (ch == 0 || ch == 224) {
            _getch();  // 读取并丢弃扩展键码
            return 0;
        }
        
        // 将小写字母转换为大写
        if (ch >= 'a' && ch <= 'z') {
            ch = toupper(ch);
        }
        
        // 只返回我们需要的按键
        switch(ch) {
            // 数字键 1-9
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
            // 字母键 QWEASDZXCVB
            case 'Q': case 'W': case 'E':
            case 'A': case 'S': case 'D':
            case 'Z': case 'X': case 'C': case 'V': case 'B':
            // 系统键
            case ' ':  // 空格
            case 27:   // ESC
                return ch;
            default:
                return 0;  // 其他按键不响应
        }
    }
    return 0;
}

void handle_key_input(int pressed_vk_code) {
    int mapping_count = sizeof(g_default_mappings) / sizeof(g_default_mappings[0]);
    
    for (int i = 0; i < mapping_count; i++) {
        if (g_default_mappings[i].vk_code == pressed_vk_code) {
            // 执行对应的游戏操作
            //   验证
            perform_action(g_default_mappings[i].action);
            break;
        }
    }
}

// 执行具体操作的函数
// 执行具体操作的函数
void perform_action(PlayerAction action) {
    switch (action) {
        case ACTION_NONE:
            break;
            
        case ACTION_PLACE_BREAD:
            place_bread();  // 放置面饼的具体实现
            break;
            
        case ACTION_ADD_MEAT:
            add_meat();     // 加肉的具体实现
            break;
            
        case ACTION_ADD_VEG:
            add_veg();      // 加蔬菜的具体实现
            break;
            
        case ACTION_ADD_SAUCE:
            add_sauce();    // 加沙司的具体实现
            break;
            
        case ACTION_ADD_FRIES:
            add_fries();    // 加薯条的具体实现
            break;
            
        case ACTION_ADD_KETCHUP:
            add_ketchup();  // 加番茄酱的具体实现
            break;
            
        case ACTION_ROLL:
            roll();         // 卷起的具体实现
            break;
            
        case ACTION_WRAP:
            wrap();         // 包装的具体实现
            break;
            
        case ACTION_GIVE_BURRITO:
            give_burrito(); // 给顾客（卷饼）的具体实现
            break;
            
        case ACTION_TAKE_BOX:
            take_box();     // 拿盒子的具体实现
            break;
            
        case ACTION_FRY_FRIES:
            fry_fries();    // 炸薯条的具体实现
            break;
            
        case ACTION_SERVE_FRIES:
            serve_fries();  // 装薯条的具体实现
            break;
            
        case ACTION_GIVE_FRIES:
            give_fries();   // 给顾客（薯条）的具体实现
            break;
            
        case ACTION_TAKE_CUP:
            take_cup();     // 拿杯子的具体实现
            break;
            
        case ACTION_POUR_DRINK:
            pour_drink();   // 接可乐的具体实现
            break;
            
        case ACTION_GIVE_DRINK:
            give_drink();   // 给顾客（饮料）的具体实现
            break;
            
        case ACTION_CUT_MEAT:
            cut_meat();     // 切肉的具体实现
            break;
            
        case ACTION_RESTOCK_BREAD:
            restock_bread();        // 补面饼/纸的具体实现
            break;
            
        case ACTION_RESTOCK_INGREDIENT:
            restock_ingredient();   // 补配料的具体实现
            break;
            
        case ACTION_RESTOCK_CONTAINER:
            restock_container();    // 补盒子/杯子的具体实现
            break;
            
        case ACTION_PAUSE_GAME:
            pause_game();   // 暂停游戏的具体实现
            break;
            
        case ACTION_RESUME_GAME:
            resume_game();  // 继续游戏的具体实现
            break;
            
        case ACTION_RETURN_MENU:
            return_menu();  // 返回菜单的具体实现
            break;
            
        case ACTION_QUIT_GAME:
            quit_game();    // 退出游戏的具体实现
            break;
        
            
        default:
            break;
    }
}