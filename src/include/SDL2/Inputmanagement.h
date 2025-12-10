#ifndef NP
#define NP

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
#endif
