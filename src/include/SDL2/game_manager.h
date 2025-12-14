#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_struct.h"
#include "time_manager.h"

// 全局变量声明
extern GameState* state1;
extern Inventory* state2;
extern Customer* state3;

// 函数声明
GameState initialize(void);
GameState new_day(void);
void destroy_game_state(void);

#endif