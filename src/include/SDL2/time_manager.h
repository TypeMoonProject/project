#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core_struct.h"
#define MAX_TIMER 20 //暂定最多共20个计时器
typedef struct{
    int id; //计时器编号,0是总计时器，后续是顾客
    int initial_time;
    int present_time;
    int duration;
    int remaining; //剩余时间
    bool is_active; //是否激活
}Timer;

#endif

/*示例
计时器需要在.c中启用
Timer timer0;
while (1){
    timer0.initial_time = time();
    timer0.present_time = time();
    timer0.remaining = timers[0].duration + timers[0].initial_time - timers[0].present_time;
    }
0是总计时器，后续是顾客
根据需要修改
或许可以用Timer的数组

*/
