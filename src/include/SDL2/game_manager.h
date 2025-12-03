#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_struct.h"
#include "time_manager.h"
GameState* state1 = (GameState*)malloc(sizeof(GameState));
Inventory* state2 = (Inventory*)malloc(sizeof(Inventory));
Customer* state3 = (Customer*)malloc(sizeof(Customer));
GameState initialize(void){
    memset(state1, 0, sizeof(GameState));//清空
    state1->running_or_not = 1;
    state1->money = 100;//初始金币100，暂定
    memset(state2, 0, sizeof(Inventory));
    memset(state3, 0, sizeof(Customer));//顾客相关内容需要在后续程序再次处理，此处只是全部清零
}//初始化
GameState new_day(void){
    state1->time = 0;
    state1->day ++;
}//新的一天
void destroy_game_state(void) {
        free(state1);
        free(state2);
        free(state3);   
}//销毁存档

#endif