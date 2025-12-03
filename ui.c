#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<SDL.h>
#include<SDL_ttf.h>

//初始化界面(成功返回0，失败返回-1)
int init_ui(){
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO)){
        SDL_Log("init_ui failed:%s\n",SDL_GetError);
        return -1;
    }
    return 0;
}

//创建窗口
SDL_Window *creat_ui(){
    const int width=1000;
    const int hight=1200;
    char title[]="Saweima Legend";
    SDL_Window *window=SDL_CreateWindow(
                                title,                 //窗口标题
                                SDL_WINDOWPOS_CENTERED,//居中
                                SDL_WINDOWPOS_CENTERED,//居中
                                width,                 //窗口宽度
                                hight,                 //窗口高度
                                0                      //flag
                                );
    //检验是否成功
    if(window==NULL){
        SDL_Log("creat_ui failed:%s\n",SDL_GetError);
    }
        return window;
}

//退出界面并销毁窗口(输入creat_ui的返回值)
void quit_ui(SDL_Window *window){
    SDL_DestroyWindow(window);
    SDL_QUIT;
}