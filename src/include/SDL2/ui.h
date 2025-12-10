#ifndef UI
#define UI

#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<stdbool.h>
#include<stdio.h>
#include<core_struct.h>

typedef struct color{
    int r;
    int g;
    int b;
    int a;
}color;

char position[]="./sourse/picture/begin.png";
char f_p[]="./font/Arial.ttf";
//(X,Y)起始点(W,H)区域宽高
//                 X   Y   W   H
SDL_Rect rect[]={{200,200,10 ,10 },//鼠标
                 {750,500,200,50 },//封面按键
                 {0  ,210,76 ,76 },
                 {76 ,210,76 ,76 },
                 {152,210,76 ,76 },
                 {228,210,76 ,76 },
                 {304,210,76 ,76 },
                 {380,210,76 ,76 },
                 {456,210,76 ,76 },
                 {532,210,76 ,76 },
                 {608,210,76 ,76 },
                 {684,210,76 ,76 },
             };
//            R   G   B   A
color col[]={{0  ,0  ,0  ,255},//黑
             {255,0  ,0  ,255},//红
             {0  ,255,0  ,255},//绿
             {0  ,0  ,255,255},//蓝
             {255,255,255,255},//白
             {0  ,0  ,0  ,50 },//半透明
        };
int speed=10;
char begin[]="begin";

SDL_Window* init_creat();
TTF_Font *open_font(char *font_position,int size);
void print(TTF_Font *font,char *word,color *col,SDL_Renderer *ren,SDL_Rect *rect);
SDL_Renderer *create_renderer(SDL_Window *win);
SDL_Surface *load_image(char *position);
void draw_rectangle(SDL_Renderer *ren,SDL_Rect *rect,color *col);
void callback(void *userdata,Uint8 *stream,int len);
int back_music();
void into_image(SDL_Window *win,SDL_Renderer *ren,SDL_Surface *ima);
void move_rect(SDL_Renderer *ren,SDL_Rect *rect,color *col,int speed);
void quit(SDL_Window *win,SDL_Renderer *ren);
void draw_ui(SDL_Renderer *ren);
void first_ui(SDL_Renderer *ren,SDL_Window *win,SDL_Rect *rect,color *col1,color *col2,char *position,char *f_p,int size);
void game_ui(SDL_Renderer *ren,SDL_Window *win,char *f_p,Inventory *inventory,Customer *customer);
void print_number(TTF_Font *font,Inventory *num,color *col,SDL_Renderer *ren,SDL_Rect *rect);
void print_req(TTF_Font *font,Customer *customer,color *col,SDL_Renderer *ren);


#endif