#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ExtensionModule.h"


GameState gameSaveData = {
    .money = 0,
    .time = 0,
    .running_or_not = false,
    .day = 0,
    .upgrades = {
        .hasAutoMeatCutter = false,
        .hasGoldPlate = false,
        .hasExpandedShop = false
    }
};


