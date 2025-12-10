#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "core_struct.h"
void UpdateCustomer();
int SubmitOrder();
int Pancake_demand();
int bool_number_generator();
int check_pancake_order_completion();//linked to "SubmitOrder"
int CalculateEarnings();
extern int customer_type;
extern bool cola_demand;
extern bool chips_demand;
extern int pancake_amount;
extern int cola_status;
extern int chips_status;
#endif
