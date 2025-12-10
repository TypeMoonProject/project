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

int main(){
    
    while (1)
    {
        int c=readInput();
        Sleep(10);
    }
    
    return 0;
}

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