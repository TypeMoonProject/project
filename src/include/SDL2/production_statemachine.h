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
#endif // PRODUCTION_STATEMACHINE_H
