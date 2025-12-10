#include <stdio.h>
#include <stdlib.h>
#include <time.h>    
#include "core_struct.h"
#include <stdbool.h>
#include "production_statemachine.h"
Customer GenerateCustomer();
void UpdateCustomer();
int SubmitOrder(bool cola_demand, bool chips_demand, int pancake_requirement[5][5], int wrapped_pancake[5][5], int cola_status, int chips_status);
int Pancake_demand(int pancake_requirement[5][5]);
int bool_number_generator();
int check_pancake_order_completion(int pancake_requirement[5][5], int wrapped_pancake[5][5]);
int CalculateEarnings(int pancake_amount, int customer_type);
int customer_type;
int order_status = 0; /*订单状态码，0未完成，1已完成*/
int earnings = 0; /*总收益*/
int pancake_amount;
/*for test*/
/*int wrapped_pancake[5][5] = {
    {1,0,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
};*/


int main() {   
srand(time(NULL));
Customer customer1 = GenerateCustomer();
UpdateCustomer();
order_status = SubmitOrder(customer1.cola_demand, customer1.chips_demand, customer1.pancake_requirement, customer1.pancake_requirement, 1, 1);
if (order_status) {
    earnings = CalculateEarnings(pancake_amount, customer_type);
    printf("Total earnings: $%d\n", earnings);
}
return 0;
}

/*顾客生成与更新*/
Customer GenerateCustomer(){
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

customer_type = rand()%7+1;
    /*排列组合顾客可能出现的7种食物种类方面的需求*/
switch(customer_type){
    case 1:
        /*只要cola*/
        customer1.patience = 30;
        customer1.cola_demand = true;
        break;
    case 2:
        /*只要pancake*/
        customer1.patience = 45;
        pancake_amount = Pancake_demand(customer1.pancake_requirement);
            
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
        pancake_amount = Pancake_demand(customer1.pancake_requirement);
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
        pancake_amount = Pancake_demand(customer1.pancake_requirement);

        break;
    case 7:
        /*要pancake、chips和cola*/
       
        customer1.patience = 75;
        customer1.cola_demand = true;
        customer1.chips_demand = true;
        pancake_amount = Pancake_demand(customer1.pancake_requirement);
        break;
    }
    /*测试输出顾客需求*/
    printf("customer_type:%d\n",customer_type);
    printf("chips_demand:%d\n",customer1.chips_demand);
    printf("cola_demand:%d\n",customer1.cola_demand);
    for (int i=0; i<5; i++){
        for (int j=0;j<5;j++){
            printf("%d\t",customer1.pancake_requirement[i][j]);
            }
            printf("\n");
        }

    return customer1;    
    
    }

/*顾客需求中卷饼部分的生成*/    
int Pancake_demand(int pancake_requirement[5][5]){
    int pancake_amount;
    pancake_amount = rand()%5+1;
    int initial_ingredient;
    for (int i=0;i<pancake_amount;i++){
        initial_ingredient = rand()%5;
        pancake_requirement[i][initial_ingredient] = 1;
        for (int j=0;j<5;j++){
            pancake_requirement[i][j] = bool_number_generator();
        }
    }
    printf("pancake_amount:%d\n",pancake_amount);
    return pancake_amount;
}

/*状态判断码生成*/
int bool_number_generator(){
    int random_number = rand()%2;
    return random_number;
}

void UpdateCustomer(){
    /*顾客状态更新函数待完成*/
}

int SubmitOrder(bool cola_demand, bool chips_demand, int pancake_requirement[5][5], int wrapped_pancake[5][5], int cola_status, int chips_status) {
    int pancake_is_finished = 0;
    int cola_is_finished = 0;
    int chips_is_finished = 0;
    printf("Order submitted.\n");

    /*调用检查卷饼订单完成情况的函数*/
    pancake_is_finished = check_pancake_order_completion(pancake_requirement, wrapped_pancake);

    if (pancake_is_finished) {
    printf("所有卷饼需求已满足！\n");
    } else {
    printf("卷饼需求未完全满足！\n");
    }

    /*判断饮料需求是否完全得到满足*/
    if (cola_demand) {
        if (cola_status == 1) {
            cola_is_finished = 1;
            printf("Cola order fulfilled.\n");
        }
    } else {
        cola_is_finished = 1; /*不需要可乐视为已完成*/
    }

    /*判断薯片需求是否完全得到满足*/
    if (chips_demand) {
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
    
    //printf("\n=== 卷饼匹配详情 ===\n");
    
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
        
        //printf("\n正在匹配需求卷饼[行%d]: ", i);
        for (int k = 0; k < 5; k++) {
            //printf("%d ", pancake_requirement[i][k]);
        }
        //printf("\n");

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
                //printf("  匹配到完成卷饼[行%d]\n", j);
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
    int matched_count = 0;
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
            // 如果这一行有需求，在循环中应该已经匹配
            // 这里我们假设匹配函数已经正确工作
        }
    }
    
    printf("\n卷饼需求总数: %d\n", requirement_count);
    printf("卷饼匹配完成: 是\n");
    
    return 1;  /* 所有需求都找到匹配 */
}
int CalculateEarnings(int pancake_amount, int customer_type) {
    /*根据顾客类型和需求计算收益*/
    switch(customer_type){
        case 1:
            earnings += 5; /*只要cola*/
            break;
        case 2:
            earnings += pancake_amount * 3; /*只要pancake*/
            break;
        case 3:
            earnings += 4; /*只要chips*/
            break;
        case 4:
            earnings += 5 + pancake_amount * 3; /*要pancake和cola*/
            break;
        case 5:
            earnings += 5 + 4; /*要chips和cola*/
            break;
        case 6:
            earnings += pancake_amount * 3 + 4; /*要pancake和chips*/
            break;
        case 7:
            earnings += pancake_amount * 3 + 4 + 5; /*要pancake、chips和cola*/
            break;
    }
    
    printf("Earnings from this order: $%d\n", earnings);
    return earnings;
    
}
