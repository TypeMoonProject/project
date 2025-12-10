#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<stdbool.h>
#include<stdio.h>

const int WIDTH=1000;
const int HIGHT=750;
Uint8 *buf=NULL;
Uint32 audio_len;
Uint32 pos=0;
SDL_AudioDeviceID id;
char b[]="begin";

typedef struct color{
    int r;
    int g;
    int b;
    int a;
}color;

//初始化和创建窗口
SDL_Window* init_creat(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0){
        SDL_Log("INIT UI FAILED:%s",SDL_GetError());
    }
    if(TTF_Init()<0){
        SDL_Log("INIT TTF FAILED:%s",TTF_GetError());
    }
    SDL_Window *win=SDL_CreateWindow("game",
                                    SDL_WINDOWPOS_CENTERED,//x轴居中
                                    SDL_WINDOWPOS_CENTERED,//y轴居中
                                    WIDTH,                 //宽
                                    HIGHT,                 //高
                                    SDL_WINDOW_RESIZABLE   //大小可变
                                );
    if(win==NULL){
        SDL_Log("CREATE UI FAILED:%s",SDL_GetError());
    }
    return win;
}

//打开字体
TTF_Font *open_font(char *font_position,int size){
    TTF_Font *font=TTF_OpenFont(font_position,size);
    if(font==NULL){
        SDL_Log("OPEN TTF FAILED:%s",TTF_GetError());
    }
    return font;
}

//渲染文字
void print(TTF_Font *font,char *word,color *col,SDL_Renderer *ren,SDL_Rect *rect){
    SDL_Color co={col->r,col->g,col->b,col->a};
    SDL_Surface *wo=TTF_RenderUTF8_Blended(font,word,co);
    SDL_Texture* text=SDL_CreateTextureFromSurface(ren,wo);
    if(wo==NULL||text==NULL){
        SDL_Log("LODA WORD FAILED:%s",TTF_GetError());
    }
    SDL_FreeSurface(wo);
    SDL_RenderCopy(ren,text,NULL,rect);
}


//创建渲染器
SDL_Renderer *create_renderer(SDL_Window *win){
    SDL_Renderer *ren=SDL_CreateRenderer(win,                   //创建渲染器的窗口
                                        -1,                     //自动寻找
                                        SDL_RENDERER_ACCELERATED//使用硬件加速
                                    );
    if(ren==NULL){
        SDL_Log("CREATE RENDERER FAILED:%s",SDL_GetError());
    }
    //设置混合模式
    SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_BLEND);
    return ren;
}

//下载图片
SDL_Surface *load_image(char *position){
    SDL_Surface *ima=IMG_Load(position);
    if(ima==NULL){
        SDL_Log("LOAD JPG FAILED:%s",SDL_GetError());
    }
    return ima;
}

//导入图片至渲染器
 void into_image(SDL_Window *win,SDL_Renderer *ren,SDL_Surface *ima){
    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *text=SDL_CreateTextureFromSurface(ren,ima);
    SDL_FreeSurface(ima);
    if(text==NULL){
        SDL_Log("CREATE TEXTURE FAILED:%s",SDL_GetError());
    }
    //导入渲染器
    SDL_RenderCopy(ren,
                text,
                NULL,//复制图片的区域
                NULL //目标区域
                );
    SDL_DestroyTexture(text);
}

//绘制矩形
void draw_rectangle(SDL_Renderer *ren,SDL_Rect *rect,color *col){
    SDL_SetRenderDrawColor(ren,col->r,col->g,col->b,col->a);
    SDL_RenderFillRect(ren,rect);
}

//回调
void callback(void *userdata,Uint8 *stream,int len){
    int remain=audio_len-pos;
    if(remain>len){
        SDL_memcpy(stream,buf+pos,len);
        pos+=len;
    }else{
        SDL_memcpy(stream,buf+pos,remain);
        pos=0;
    }
}

//播放背景音乐
int back_music(){
    SDL_AudioSpec spec;
    if(SDL_LoadWAV("./sourse/music/money.wav",&spec,&buf,&audio_len)==NULL){
        SDL_Log("LOAD MUSIC FAILED:%s",SDL_GetError());
    }
    spec.callback=callback;
    id=SDL_OpenAudioDevice(NULL,//默认设备
                        0,      //播放
                        &spec,
                        NULL,
                        0
                    );
    if(id==0){
        SDL_Log("OPEN AUDIO FAILED:%s",SDL_GetError());
    }
    SDL_PauseAudioDevice(id,0);
    return id;
}

//使矩形沿x轴移动（需搭配循环使用）
void move_rect(SDL_Renderer *ren,SDL_Rect *rect,color *col,int speed){
    draw_rectangle(ren,rect,col);
    rect->x+=speed;
    SDL_Delay(100);
    SDL_RenderPresent(ren);
}

//退出
void quit(SDL_Window *win,SDL_Renderer *ren){
    SDL_DestroyRenderer(ren);
    SDL_CloseAudioDevice(id);
    SDL_FreeWAV(buf);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

//绘制界面
void draw_ui(SDL_Renderer *ren){
    SDL_RenderPresent(ren);
}

//开始界面
void first_ui(SDL_Renderer *ren,SDL_Window *win,SDL_Rect *rect,color *col1,color *col2,char *position,char *f_p,int size){
    SDL_Surface *ima=load_image(position);
    into_image(win,ren,ima);
    draw_rectangle(ren,rect,col1);
    TTF_Font *font=open_font(f_p,size);
    print(font,b,col2,ren,rect);
}

//游戏界面
void game_ui(SDL_Renderer *ren,SDL_Window *win,SDL_Rect *rect,color *col1,color *col2,char *position,char *f_p,int size){
}

