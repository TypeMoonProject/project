#ifndef UI
#define UI

#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<SDL.h>
#include<SDL_ttf.h>

//初始化界面(成功返回0，失败返回-1)
int init_ui();

//创建窗口
SDL_Window *creat_ui();

//退出界面并销毁窗口(输入creat_ui的返回值)
void quit_ui(SDL_Window *window);
#endif;