#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/ExtensionModule.h"
#include "SDL2/core_struct.h"

// 测试升级系统
void TestUpgradeSystem() {
    printf("\n=== 测试升级系统 ===\n");
    
    GameState gameState;
    gameState.money = 1000;
    gameState.upgrades.hasAutoMeatCutter = false;
    gameState.upgrades.hasGoldPlate = false;
    gameState.upgrades.hasExpandedShop = false;
    
    // 测试获取升级信息
    printf("升级信息：\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s - 成本: %d金币 - %s\n", 
               i + 1, 
               GetUpgradeName(i), 
               GetUpgradeCost(i), 
               GetUpgradeDescription(i));
    }
    
    // 测试购买升级
    printf("\n初始金币: %d\n", gameState.money);
    printf("尝试购买自动切肉机...\n");
    
    if (PurchaseUpgrade(UPGRADE_AUTO_MEAT_CUTTER, &gameState.money, &gameState.upgrades)) {
        printf("购买成功！剩余金币: %d\n", gameState.money);
        printf("自动切肉机状态: %s\n", gameState.upgrades.hasAutoMeatCutter ? "已拥有" : "未拥有");
    } else {
        printf("购买失败！\n");
    }
    
    // 测试重复购买
    printf("\n尝试再次购买自动切肉机...\n");
    if (PurchaseUpgrade(UPGRADE_AUTO_MEAT_CUTTER, &gameState.money, &gameState.upgrades)) {
        printf("购买成功！剩余金币: %d\n", gameState.money);
    } else {
        printf("购买失败（已拥有该升级）！\n");
    }
    
    // 测试金币不足
    printf("\n尝试购买扩充店面（成本800金币）...\n");
    if (PurchaseUpgrade(UPGRADE_EXPAND_SHOP, &gameState.money, &gameState.upgrades)) {
        printf("购买成功！剩余金币: %d\n", gameState.money);
    } else {
        printf("购买失败（金币不足）！\n");
    }
}

// 测试数据保存和读取
void TestDataPersistence() {
    printf("\n=== 测试数据保存和读取 ===\n");
    
    // 创建测试游戏状态
    GameState saveState;
    saveState.money = 5000;
    saveState.time = 3600;
    saveState.running_or_not = true;
    saveState.day = 5;
    saveState.upgrades.hasAutoMeatCutter = true;
    saveState.upgrades.hasGoldPlate = true;
    saveState.upgrades.hasExpandedShop = false;
    
    printf("保存前游戏状态：\n");
    printf("金币: %d\n", saveState.money);
    printf("时间: %d\n", saveState.time);
    printf("天数: %d\n", saveState.day);
    printf("自动切肉机: %s\n", saveState.upgrades.hasAutoMeatCutter ? "已拥有" : "未拥有");
    printf("金盘子: %s\n", saveState.upgrades.hasGoldPlate ? "已拥有" : "未拥有");
    printf("扩充店面: %s\n", saveState.upgrades.hasExpandedShop ? "已拥有" : "未拥有");
    
    // 保存游戏数据
    const char* saveFilePath = "test_save.dat";
    if (SaveGame(&saveState, saveFilePath)) {
        printf("\n游戏数据保存成功！\n");
    } else {
        printf("\n游戏数据保存失败！\n");
        return;
    }
    
    // 读取游戏数据
    GameState loadState;
    if (LoadGame(&loadState, saveFilePath)) {
        printf("\n游戏数据读取成功！\n");
        printf("读取后游戏状态：\n");
        printf("金币: %d\n", loadState.money);
        printf("时间: %d\n", loadState.time);
        printf("天数: %d\n", loadState.day);
        printf("自动切肉机: %s\n", loadState.upgrades.hasAutoMeatCutter ? "已拥有" : "未拥有");
        printf("金盘子: %s\n", loadState.upgrades.hasGoldPlate ? "已拥有" : "未拥有");
        printf("扩充店面: %s\n", loadState.upgrades.hasExpandedShop ? "已拥有" : "未拥有");
        
        // 验证数据一致性
        if (saveState.money == loadState.money &&
            saveState.time == loadState.time &&
            saveState.day == loadState.day &&
            saveState.upgrades.hasAutoMeatCutter == loadState.upgrades.hasAutoMeatCutter &&
            saveState.upgrades.hasGoldPlate == loadState.upgrades.hasGoldPlate &&
            saveState.upgrades.hasExpandedShop == loadState.upgrades.hasExpandedShop) {
            printf("\n数据一致性验证通过！\n");
        } else {
            printf("\n数据一致性验证失败！\n");
        }
    } else {
        printf("\n游戏数据读取失败！\n");
    }
}

// 测试乞丐机制
void TestBeggarMechanism() {
    printf("\n=== 测试乞丐机制 ===\n");
    
    srand(time(NULL));
    Beggar beggar;
    beggar.isPresent = false;
    
    // 测试生成乞丐
    printf("尝试生成乞丐...\n");
    if (SpawnBeggar(&beggar)) {
        printf("乞丐生成成功！\n");
        printf("乞丐状态: %s\n", beggar.isPresent ? "存在" : "不存在");
        printf("耐心时间: %d秒\n", beggar.patienceTimer);
        printf("金币奖励: %d\n", beggar.coinReward);
    } else {
        printf("本次未生成乞丐（这是正常现象，生成概率约10%%）\n");
    }
    
    // 如果生成了乞丐，测试状态更新
    if (beggar.isPresent) {
        printf("\n测试乞丐状态更新...\n");
        printf("初始状态: WAITING\n");
        
        // 模拟时间流逝
        for (int i = 0; i < 20; i++) {
            UpdateBeggar(&beggar);
            if (beggar.state == BEGGAR_STATE_RUNNING) {
                printf("经过%d秒后，乞丐开始逃跑！\n", i + 1);
                break;
            }
        }
        
        // 测试提供食物
        printf("\n测试为乞丐提供食物...\n");
        ServeBeggar(&beggar);
        printf("乞丐状态: %s\n", beggar.state == BEGGAR_STATE_SERVED ? "已获得食物" : "未获得食物");
    }
    
    // 测试抓住逃跑的乞丐
    printf("\n测试抓住逃跑的乞丐...\n");
    Beggar runningBeggar;
    runningBeggar.isPresent = true;
    runningBeggar.state = BEGGAR_STATE_RUNNING;
    runningBeggar.coinReward = 100;
    
    int coins = 0;
    if (CatchBeggar(&runningBeggar, &coins)) {
        printf("抓住成功！获得奖励: %d金币\n", coins);
        printf("乞丐状态: %s\n", runningBeggar.isPresent ? "存在" : "已离开");
    } else {
        printf("抓住失败！\n");
    }
}

int main() {
    printf("===== 扩展模块功能测试 =====\n");
    
    // 运行所有测试
    TestUpgradeSystem();
    TestDataPersistence();
    TestBeggarMechanism();
    
    printf("\n===== 测试完成 =====\n");
    return 0;
}