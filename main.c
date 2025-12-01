#include"SDL.h"
#include<stdio.h>
#include"core_struct.h"

int main(int argc, char *argv[])
{
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL 初始化失败: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("SDL2 测试窗口",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("创建窗口失败: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        printf("创建渲染器失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 设置渲染颜色（红色）
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // 清除屏幕
    SDL_RenderClear(renderer);

    // 显示渲染内容
    SDL_RenderPresent(renderer);

    // 延迟5秒
    SDL_Delay(5000);

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
