#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "core_struct.h"
Customer GenerateCustomer();
void UpdateCustomer(Customer* customer, int elapsed_time);
bool SubmitOrder(Customer customer, int wrapped_pancake[5][5], int cola_status, int chips_status);
int check_pancake_order_completion(int pancake_requirement[5][5], int wrapped_pancake[5][5]);//linked to "SubmitOrder"
int CalculateEarnings(Customer customer);
void ResetCustomerSystem(void);
int GetTotalEarnings(void);

extern int customer_type;
extern bool cola_demand;
extern bool chips_demand;
extern int pancake_amount;
extern int cola_status;
extern int chips_status;
extern int total_earnings;
extern int customer_id_counter;
#endif//CUSTOMER_H

