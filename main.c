#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/ui.h"
#include "SDL2/game_manager.h"
#include "SDL2/ExtensionModule.h"

int main(int argc, char *argv[]) {
    // 初始化随机数生成器
    srand(time(NULL));
    
    // 初始化游戏状态
    GameState gameState = initialize();
    
    // 尝试加载保存的游戏数据
    if (!LoadGame(&gameState, "game_save.dat")) {
        printf("No saved game found. Starting new game.\n");
        // 如果没有保存数据，使用默认值
        gameState = initialize();
    } else {
        printf("Loaded saved game.\n");
    }
    
    // 启动游戏UI
    action(position, rect, col, speed);
    
    // 游戏结束时保存数据
    SaveGame(&gameState, "game_save.dat");
    
    // 清理资源
    destroy_game_state();
    
    return 0;
}