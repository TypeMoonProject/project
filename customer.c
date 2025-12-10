#include <stdio.h>
#include <stdlib.h>
#include <time.h>    
#include <stdbool.h>
#include "core_struct.h"
#include "customer.h"

static int customer_type = 0;
static int total_earnings = 0; /*总收益*/
static int system_initialized = 0;

// 初始化顾客系统（只需调用一次）
void InitCustomerSystem(void) {
    if (!system_initialized) {
        srand(time(NULL));
        system_initialized = 1;
    }
}

/*顾客生成与更新*/
Customer GenerateCustomer(void) {
    if (!system_initialized) {
        InitCustomerSystem();
    }
    
    Customer customer1;
    customer1.patience = 0;
    customer1.cola_demand = false;
    customer1.chips_demand = false;
    
    /*初始化顾客需求数组为0*/
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            customer1.pancake_requirement[i][j] = 0;
        }
    }

    customer_type = rand() % 7 + 1;
    
    /*排列组合顾客可能出现的7种食物种类方面的需求*/
    switch(customer_type) {
        case 1:
            /*只要cola*/
            customer1.patience = 30;
            customer1.cola_demand = true;
            break;
        case 2:
            /*只要pancake*/
            customer1.patience = 45;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 3:
            /*只要chips*/
            customer1.chips_demand = true;
            customer1.patience = 60;
            break;
        case 4:
            /*要pancake和cola*/
            customer1.patience = 30;
            customer1.cola_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 5:
            /*要chips和cola*/
            customer1.patience = 45;
            customer1.cola_demand = true;
            customer1.chips_demand = true;
            break;
        case 6:
            /*要pancake和chips*/
            customer1.patience = 60;
            customer1.chips_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
        case 7:
            /*要pancake、chips和cola*/
            customer1.patience = 75;
            customer1.cola_demand = true;
            customer1.chips_demand = true;
            Pancake_demand(customer1.pancake_requirement);
            break;
    }
    
    /*测试输出顾客需求*/
    printf("customer_type:%d\n", customer_type);
    printf("chips_demand:%d\n", customer1.chips_demand);
    printf("cola_demand:%d\n", customer1.cola_demand);
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d\t", customer1.pancake_requirement[i][j]);
        }
        printf("\n");
    }

    return customer1;    
}

/*顾客需求中卷饼部分的生成*/    
int Pancake_demand(int pancake_requirement[5][5]) {
    int pancake_amount;
    pancake_amount = rand() % 5 + 1;
    
    int initial_ingredient;
    for (int i = 0; i < pancake_amount; i++) {
        initial_ingredient = rand() % 5;
        pancake_requirement[i][initial_ingredient] = 1;
        
        for (int j = 0; j < 5; j++) {
            pancake_requirement[i][j] = bool_number_generator();
        }
    }
    
    printf("pancake_amount:%d\n", pancake_amount);
    return pancake_amount;
}

/*状态判断码生成*/
int bool_number_generator(void) {
    return rand() % 2;
}

void UpdateCustomer(Customer* customer, int elapsed_time) {
    
    
    /*顾客状态更新函数待完成*/
    /*这里可以更新顾客耐心值等*/
    
    // 示例：减少耐心值
    customer->patience -= elapsed_time;
    if (customer->patience < 0) {
        customer->patience = 0;
    }
    
    printf("顾客剩余耐心: %d秒\n", customer->patience);
}

int SubmitOrder(Customer customer, int wrapped_pancake[5][5], int cola_status, int chips_status) {
    
    
    int pancake_is_finished = 0;
    int cola_is_finished = 0;
    int chips_is_finished = 0;
    
    printf("Order submitted.\n");

    /*调用检查卷饼订单完成情况的函数*/
    pancake_is_finished = check_pancake_order_completion(customer.pancake_requirement, wrapped_pancake);

    if (pancake_is_finished) {
        printf("所有卷饼需求已满足！\n");
    } else {
        printf("卷饼需求未完全满足！\n");
    }

    /*判断饮料需求是否完全得到满足*/
    if (customer.cola_demand) {
        if (cola_status == 1) {
            cola_is_finished = 1;
            printf("Cola order fulfilled.\n");
        }
    } else {
        cola_is_finished = 1; /*不需要可乐视为已完成*/
    }

    /*判断薯片需求是否完全得到满足*/
    if (customer.chips_demand) {
        if (chips_status == 1) {
            chips_is_finished = 1;
            printf("Chips order fulfilled.\n");
        }
    } else {
        chips_is_finished = 1; /*不需要薯片视为已完成*/
    }

    /*综合判断订单是否完成*/
    if (pancake_is_finished && cola_is_finished && chips_is_finished) {
        printf("Order completed successfully!\n");
        return 1;
    } else {
        printf("Order not completed.\n");
        return 0;
    }
}

int check_pancake_order_completion(int pancake_requirement[5][5], int wrapped_pancake[5][5]) {
    
    int wrapped_used[5] = {0};  /* 标记完成的卷饼是否已使用*/
    
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
                continue;  /* 这个完成的卷饼已使用 */
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
                continue;  /* 这一行没有卷饼 */
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
                wrapped_used[j] = 1;  /* 标记为已使用 */
                found_match = 1;
                break;
            }
        }
        
        if (!found_match) {
            printf("没有找到匹配的完成卷饼\n");
            return 0;  /* 有需求没找到匹配 */
        } else {
            printf("匹配成功\n");
        }
    }
    
    // 检查是否所有需求行都被处理
    int requirement_count = 0;
    for (int i = 0; i < 5; i++) {
        int has_req = 0;
        for (int k = 0; k < 5; k++) {
            if (pancake_requirement[i][k] == 1) {
                has_req = 1;
                break;
            }
        }
        if (has_req) {
            requirement_count++;
        }
    }
    
    printf("\n卷饼需求总数: %d\n", requirement_count);
    printf("卷饼匹配完成: 是\n");
    
    return 1;  /* 所有需求都找到匹配 */
}

int CalculateEarnings(const Customer* customer) {
    if (!customer) return 0;
    
    // 计算卷饼数量
    int pancake_amount = 0;
    for (int i = 0; i < 5; i++) {
        int has_ingredient = 0;
        for (int j = 0; j < 5; j++) {
            if (customer->pancake_requirement[i][j] == 1) {
                has_ingredient = 1;
                break;
            }
        }
        if (has_ingredient) {
            pancake_amount++;
        }
    }
    
    int order_earnings = 0;
    
    /*根据顾客类型和需求计算收益*/
    int current_customer_type = GetCustomerType(customer);
    
    switch(current_customer_type) {
        case 1:
            order_earnings = 5; /*只要cola*/
            break;
        case 2:
            order_earnings = pancake_amount * 3; /*只要pancake*/
            break;
        case 3:
            order_earnings = 4; /*只要chips*/
            break;
        case 4:
            order_earnings = 5 + pancake_amount * 3; /*要pancake和cola*/
            break;
        case 5:
            order_earnings = 5 + 4; /*要chips和cola*/
            break;
        case 6:
            order_earnings = pancake_amount * 3 + 4; /*要pancake和chips*/
            break;
        case 7:
            order_earnings = pancake_amount * 3 + 4 + 5; /*要pancake、chips和cola*/
            break;
    }
    
    // 累加到总收益
    total_earnings += order_earnings;
    
    printf("Earnings from this order: $%d\n", order_earnings);
    printf("Total earnings: $%d\n", total_earnings);
    
    return order_earnings;
}


int GetCustomerType(const Customer* customer) {
    return customer_type;
}

// 获取总收益
int GetTotalEarnings(void) {
    return total_earnings;
}

// 重置顾客系统
void ResetCustomerSystem(void) {
    total_earnings = 0;
    customer_type = 0;
}
