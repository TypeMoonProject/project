#include <stdio.h>
#include <stdlib.h>
#include <time.h>    
#include <stdbool.h>
#include "core_struct.h"
#include "customer.h"
#include "time_manager.h"



// 函数声明
Customer GenerateCustomer(void);
void UpdateCustomer(Customer* customer);
bool SubmitOrder(Customer customer, int wrapped_pancake[5][5], int cola_status, int chips_status);
int CalculateEarnings(Customer customer);
int Pancake_demand(int pancake_requirement[5][5]);
int bool_number_generator(void);
int check_pancake_order_completion(int pancake_requirement[5][5], int wrapped_pancake[5][5]);
void ResetCustomerSystem(void);
int GetTotalEarnings(void);

int customer_type = 0;
int total_earnings = 0; // 全局总收益
int pancake_amount = 0; // 仅作初始化用途
int customer_id_counter = 1;

// 主函数用于测试顾客系统（调用customer.c时必看参考）
/*int main() {   
    srand(time(NULL));
    ResetCustomerSystem();
    printf("total_earnings initial:%d\n", GetTotalEarnings());
    Customer customer1 = GenerateCustomer();
    customer1.customer_id = customer_id_counter++;
    
    printf("=== 快餐店模拟游戏开始 ===\n");
    printf("按's'或'S'提交订单，其他键继续等待\n");
    printf("=======================\n\n");
    
    // 模拟多次更新，每次更新1秒
    for (int i = 0; i < 51; i++) {
        printf("\n=== 时间: 第%d秒 ===\n", i+1);
        
        // 更新顾客状态
        UpdateCustomer(&customer1, 1);
        
        // 如果订单已完成或顾客离开，继续下一个循环
        if (customer1.order_is_finished) {
            continue;
        }
        
        // 检查顾客是否还在（耐心大于0）
        if (customer1.patience <= 0) {
            // UpdateCustomer函数已经处理了顾客离开和新顾客生成
            continue;
        }
        
        // 提示用户是否提交订单
        printf("按's'提交订单，其他键继续等待: ");
        char user_input;
        scanf(" %c", &user_input);
        
        if (user_input == 's' || user_input == 'S') {
            printf("\n正在提交订单...\n");
            
            // 准备模拟的已完成订单（假设顾客需求完全满足）
            int wrapped_pancake[5][5];
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    wrapped_pancake[x][y] = customer1.pancake_requirement[x][y];
                }
            }
            
            int cola_status = customer1.cola_demand ? 1 : 0;
            int chips_status = customer1.chips_demand ? 1 : 0;
            
            customer1.order_is_finished = SubmitOrder(customer1, wrapped_pancake, cola_status, chips_status);
            
            if (customer1.order_is_finished) {
                // 计算收益
                int order_earnings = CalculateEarnings(customer1);
                printf("本订单收益: $%d\n", order_earnings);
                printf("累计总收益: $%d\n", total_earnings);
                
                // 订单完成后，生成新顾客
                printf("\n顾客%d订单完成，生成新顾客...\n", customer1.customer_id);
                customer1 = GenerateCustomer();
                customer1.customer_id = customer_id_counter++;
            } else {
                printf("订单未完成，顾客继续等待...\n");
            }
        }
        
        // 检查是否结束游戏
        if (i == 50) {
            printf("\n=== 游戏结束 ===\n");
            printf("最终总收益: $%d\n", total_earnings);
            printf("共服务了%d位顾客\n", customer_id_counter - 1);
        }
    }
    
    return 0;
}
*/
/*顾客需求中卷饼部分的生成*/    
int Pancake_demand(int pancake_requirement[5][5]) {
    pancake_amount = rand() % 5 + 1;
    
    int initial_ingredient;
    for (int i = 0; i < pancake_amount; i++) {
        initial_ingredient = rand() % 5;
        pancake_requirement[i][initial_ingredient] = 1;
        
        for (int j = 0; j < 5; j++) {
            pancake_requirement[i][j] = bool_number_generator();
        }
    }
    return pancake_amount;
}

/*状态判断码生成*/
int bool_number_generator(void) {
    return rand() % 2;
}

/*顾客生成与更新*/
Customer GenerateCustomer(void) {
    Customer customer1;
    customer1.patience = 0;
    customer1.original_time = 0;
    customer1.remaining_time = 0;
    customer1.cola_demand = false;
    customer1.chips_demand = false;
    customer1.order_is_finished = false;
    customer1.customer_id = 0;
    
    /*初始化顾客需求数组为0*/
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            customer1.pancake_requirement[i][j] = 0;
        }
    }

    customer_type = rand() % 7 + 1;
    
    switch(customer_type) {
        case 1:
            customer1.patience = 10;
            customer1.cola_demand = true;
            break;
        case 2:
            customer1.patience = 15;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 3:
            customer1.chips_demand = true;
            customer1.patience = 10;
            break;
        case 4:
            customer1.patience = 15;
            customer1.cola_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 5:
            customer1.patience = 12;
            customer1.cola_demand = true;
            customer1.chips_demand = true;
            break;
        case 6:
            customer1.patience = 18;
            customer1.chips_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 7:
            customer1.patience = 20;
            customer1.cola_demand = true;
            customer1.chips_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
    }
    
    customer1.original_time = customer1.patience;
    customer1.remaining_time = customer1.patience;
    
    printf("\n新顾客生成!\n");
    printf("顾客类型: %d\n", customer_type);
    printf("需要可乐: %s\n", customer1.cola_demand ? "是" : "否");
    printf("需要薯条: %s\n", customer1.chips_demand ? "是" : "否");
    
    int pancake_count = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (customer1.pancake_requirement[i][j] == 1) {
                pancake_count++;
                break;
            }
        }
    }
    if (pancake_count > 0) {
        printf("需要卷饼个数: %d\n", pancake_count);
    }else {
        printf("不需要卷饼\n");
    }
    printf("初始耐心值: %d秒\n", customer1.patience);

    return customer1;    
}

void UpdateCustomer(Customer* customer) {
    // 如果订单已完成，直接返回
    if (customer->order_is_finished) {
        return;
    }
    
    // 静态变量记录上次输出的时间
    static time_t last_output_time = 0;
    static int output_counter = 0;
    static int current_customer_id = -1;
    
    // 检查是否切换了顾客
    if (current_customer_id != customer->customer_id) {
        last_output_time = 0;
        output_counter = 0;
        current_customer_id = customer->customer_id;
        
        // 为新顾客启动计时器
        int timer_id = customer->customer_id;
        if (timer_id >= 0 && timer_id < MAX_TIMER) {
            timers[timer_id].id = timer_id;
            timers[timer_id].initial_time = time(NULL);
            timers[timer_id].present_time = time(NULL);
            timers[timer_id].duration = customer->original_time;
            timers[timer_id].remaining = customer->original_time;
            timers[timer_id].is_active = true;
        }
    }
    
    // 获取当前时间
    time_t current_time = time(NULL);
    
    // 如果是第一次调用，初始化上次输出时间
    if (last_output_time == 0) {
        last_output_time = current_time;
    }
    
    // 更新顾客的计时器
    int timer_id = customer->customer_id;
    if (timer_id >= 0 && timer_id < MAX_TIMER && timers[timer_id].is_active) {
        // 更新计时器的当前时间
        timers[timer_id].present_time = current_time;
        
        // 计算剩余时间
        timers[timer_id].remaining = timers[timer_id].duration - 
                                    (timers[timer_id].present_time - timers[timer_id].initial_time);
        
        // 确保剩余时间不为负数
        if (timers[timer_id].remaining < 0) {
            timers[timer_id].remaining = 0;
        }
        
        // 更新顾客的耐心值
        customer->patience = timers[timer_id].remaining;
        customer->remaining_time = timers[timer_id].remaining;
    }
    
    output_counter++;
    if (output_counter >= 1) {
        printf("顾客%d耐心值: %d秒\n", customer->customer_id, customer->patience);
        
        // 检查耐心值状态并给出相应提示
        if (customer->patience <= 0) {
            printf("  警告：顾客%d已完全失去耐心！\n", customer->customer_id);
            printf("  顾客%d离开，生成新顾客...\n", customer->customer_id);
            
            // 停止当前计时器
            if (timer_id >= 0 && timer_id < MAX_TIMER) {
                timers[timer_id].is_active = false;
            }
            
            // 生成新顾客覆盖原顾客
            Customer new_customer = GenerateCustomer();
            new_customer.customer_id = customer_id_counter++;
            
            // 复制新顾客数据到原顾客指针
            *customer = new_customer;
            
            // 重置静态变量以处理新顾客
            last_output_time = 0;
            output_counter = 0;
            current_customer_id = customer->customer_id;
            
            // 为新顾客启动计时器
            int new_timer_id = customer->customer_id;
            if (new_timer_id >= 0 && new_timer_id < MAX_TIMER) {
                timers[new_timer_id].id = new_timer_id;
                timers[new_timer_id].initial_time = time(NULL);
                timers[new_timer_id].present_time = time(NULL);
                timers[new_timer_id].duration = customer->original_time;
                timers[new_timer_id].remaining = customer->original_time;
                timers[new_timer_id].is_active = true;
            }
            
            printf("  新顾客%d已生成，耐心值: %d秒\n", customer->customer_id, customer->patience);
            
            return;
        } else if (customer->patience <= customer->original_time * 0.3) {
            printf("  提示：顾客%d耐心即将耗尽！\n", customer->customer_id);
        } else if (customer->patience <= customer->original_time * 0.5) {
            printf("  提示：顾客%d耐心已减少一半\n", customer->customer_id);
        }
        
        output_counter = 0;
    }
}

bool SubmitOrder(Customer customer, int wrapped_pancake[5][5], int cola_status, int chips_status) {
    int pancake_is_finished = 0;
    int cola_is_finished = 0;
    int chips_is_finished = 0;
    
    printf("订单提交中...\n");
    
    // 检查是否有卷饼需求
    int has_pancake_demand = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (customer.pancake_requirement[i][j] == 1) {
                has_pancake_demand = 1;
                break;
            }
        }
        if (has_pancake_demand) break;
    }
    
    // 如果有卷饼需求，检查卷饼订单完成情况
    if (has_pancake_demand) {
        pancake_is_finished = check_pancake_order_completion(customer.pancake_requirement, wrapped_pancake);
    } else {
        pancake_is_finished = 1;
    }
    
    // 判断饮料需求是否完全得到满足
    if (customer.cola_demand) {
        if (cola_status == 1) {
            cola_is_finished = 1;
        }
    } else {
        cola_is_finished = 1;
    }
    
    // 判断薯片需求是否完全得到满足
    if (customer.chips_demand) {
        if (chips_status == 1) {
            chips_is_finished = 1;
        }
    } else {
        chips_is_finished = 1;
    }
    
    // 综合判断订单是否完成
    if (pancake_is_finished && cola_is_finished && chips_is_finished) {
        customer.order_is_finished = true;
        return 1;
    } else {
        customer.order_is_finished = false;
        return 0;
    }
}

int check_pancake_order_completion(int pancake_requirement[5][5], int wrapped_pancake[5][5]) {
    int wrapped_used[5] = {0};
    
    /*遍历每个需求的卷饼*/
    for (int i = 0; i < 5; i++) {
        /*检查这一行是否有需求*/
        int has_requirement = 0;
        for (int k = 0; k < 5; k++) {
            if (pancake_requirement[i][k] == 1) {
                has_requirement = 1;
                break;
            }
        }
        
        /* 如果没有需求，跳过 */
        if (!has_requirement) {
            continue;
        }

        /* 寻找匹配的完成卷饼 */
        int found_match = 0;
        for (int j = 0; j < 5; j++) {
            if (wrapped_used[j]) {
                continue;
            }

            /* 检查这一行是否有完成的卷饼 */
            int has_wrapped = 0;
            for (int k = 0; k < 5; k++) {
                if (wrapped_pancake[j][k] == 1) {
                    has_wrapped = 1;
                    break;
                }
            }
            
            if (!has_wrapped) {
                continue;
            }

            /* 比较两个卷饼是否相同 */
            int is_same = 1;
            for (int k = 0; k < 5; k++) {
                if (pancake_requirement[i][k] != wrapped_pancake[j][k]) {
                    is_same = 0;
                    break;
                }
            }
            
            if (is_same) {
                wrapped_used[j] = 1;
                found_match = 1;
                break;
            }
        }
        
        if (!found_match) {
            return 0;
        }
    }
    
    return 1;
}

int CalculateEarnings(Customer customer) {
    // 计算卷饼数量
    int pancake_amount = 0;
    for (int i = 0; i < 5; i++) {
        int has_ingredient = 0;
        for (int j = 0; j < 5; j++) {
            if (customer.pancake_requirement[i][j] == 1) {
                has_ingredient = 1;
                break;
            }
        }
        if (has_ingredient) {
            pancake_amount++;
        }
    }
    
    int order_earnings = 0;
    
    // 根据顾客需求计算收益
    bool has_pancake = (pancake_amount > 0);
    bool has_cola = customer.cola_demand;
    bool has_chips = customer.chips_demand;
    
    if (has_pancake && has_cola && has_chips) {
        order_earnings = pancake_amount * 3 + 4 + 5;
    } else if (has_pancake && has_chips) {
        order_earnings = pancake_amount * 3 + 4;
    } else if (has_chips && has_cola) {
        order_earnings = 5 + 4;
    } else if (has_pancake && has_cola) {
        order_earnings = 5 + pancake_amount * 3;
    } else if (has_chips) {
        order_earnings = 4;
    } else if (has_pancake) {
        order_earnings = pancake_amount * 3;
    } else if (has_cola) {
        order_earnings = 5;
    } else {
        order_earnings = 0;
    }
    
    // 累加到总收益
    total_earnings += order_earnings;
    
    return order_earnings;
}

// 获取总收益
int GetTotalEarnings(void) {
    return total_earnings;
}

// 重置顾客系统
void ResetCustomerSystem(void) {
    total_earnings = 0;
    customer_type = 0;
    customer_id_counter = 1;
}
