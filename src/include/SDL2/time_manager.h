#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "core_struct.h"
#define MAX_TIMER 20 //暂定最多共20个计时器
struct Timer{
    int id; //计时器编号,0是总计时器，后续是顾客,和后续数组对应
    int initial_time;
    int present_time;
    int duration;
    int remaining; //剩余时间
    bool is_active; //是否激活
};
struct Timer timers[20];

#endif

/*示例
计时器需要在.c中函数内部启用,不能在此处.h赋值
timers[0].initial_time = time(); //在需要启动的时候赋值
while (1){
    for (i=0;i<20;i++){
        timers[i].present_time = time();
        timers[i].remaining = timers[i].duration + timers[i].initial_time - timers[i].present_time;
        }
    } 
0是总计时器，后续是顾客

*/


