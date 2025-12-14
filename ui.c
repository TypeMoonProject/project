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
char f_p[]="./font/Arial.ttf";

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
void back_music(int id){
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
<<<<<<< Updated upstream
void game_ui(SDL_Renderer *ren,SDL_Window *win,SDL_Rect *rect,color *col1,color *col2,char *position,char *f_p,int size){
=======
void game_ui(SDL_Renderer *ren,SDL_Window *win,char *f_p,Inventory *inventory,Customer *customer){
    char g_p[]="./sourse/picture/kitchen.png";
    char g_p1[]="./sourse/picture/data.png";
    char g_p2[]="./sourse/picture/head.png";
    char g_p3[]="./sourse/picture/man.png";
    SDL_Rect re1={0,0,210,760};
    SDL_Rect re2={439,500,61,100};
    SDL_Rect re3={700,400,100,150}; // 顾客图像位置和大小
    
    // 加载并显示背景图片
    SDL_Surface *ima = load_image(g_p);
    if (ima != NULL) {
        into_image(win, ren, ima);
        // into_image内部会处理表面，不需要在这里销毁
    }
    
    // 加载并显示data.png
    SDL_Surface *ima1 = load_image(g_p1);
    if (ima1 != NULL) {
        SDL_Texture *text = SDL_CreateTextureFromSurface(ren, ima1);
        SDL_FreeSurface(ima1);
        
        if (text != NULL) {
            SDL_RenderCopy(ren, text, NULL, &re1);
            SDL_DestroyTexture(text);
        } else {
            SDL_Log("CREATE TEXTURE FAILED:%s", SDL_GetError());
        }
    }
    
    // 加载并显示head.png
    SDL_Surface *ima2 = load_image(g_p2);
    if (ima2 != NULL) {
        SDL_Texture *text2 = SDL_CreateTextureFromSurface(ren, ima2);
        SDL_FreeSurface(ima2);
        
        if (text2 != NULL) {
            SDL_RenderCopy(ren, text2, NULL, &re2);
            SDL_DestroyTexture(text2);
        } else {
            SDL_Log("CREATE TEXTURE FAILED:%s", SDL_GetError());
        }
    }
    
    // 加载并显示顾客图像
    SDL_Surface *ima3 = load_image(g_p3);
    if (ima3 != NULL) {
        SDL_Texture *text3 = SDL_CreateTextureFromSurface(ren, ima3);
        SDL_FreeSurface(ima3);
        
        if (text3 != NULL) {
            SDL_RenderCopy(ren, text3, NULL, &re3);
            SDL_DestroyTexture(text3);
        } else {
            SDL_Log("CREATE TEXTURE FAILED:%s", SDL_GetError());
        }
    }
}
//打印数字
void print_number(TTF_Font *font,Inventory *num,color *col,SDL_Renderer *ren,SDL_Rect *rect){
    char text[11][4]={'\0'};
    sprintf(text[0],"%d",num->flatbread);
    sprintf(text[1],"%d",num->meat);
    sprintf(text[2],"%d",num->cucumber);
    sprintf(text[3],"%d",num->sauce);
    sprintf(text[4],"%d",num->chips);
    sprintf(text[5],"%d",num->ketchup);
    sprintf(text[6],"%d",num->cola);
    sprintf(text[7],"%d",num->wrapper);
    sprintf(text[8],"%d",num->box);
    sprintf(text[9],"%d",num->bottle);
    print(font,text[0],col,ren,&rect[2]);
    print(font,text[1],col,ren,&rect[3]);
    print(font,text[2],col,ren,&rect[4]);
    print(font,text[3],col,ren,&rect[5]);
    print(font,text[4],col,ren,&rect[6]);
    print(font,text[5],col,ren,&rect[7]);
    print(font,text[6],col,ren,&rect[8]);
    print(font,text[7],col,ren,&rect[9]);
    print(font,text[8],col,ren,&rect[10]);
    print(font,text[9],col,ren,&rect[11]);
>>>>>>> Stashed changes
}

//监听行为
void action(char *position,SDL_Rect *rect,color *col,int speed){
    SDL_Window *win=init_creat();
    SDL_Renderer *ren=create_renderer(win);
    SDL_Event event;
    first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,10);
    draw_ui(ren);
    back_music(id);
    bool run=true;
    while(run){
        while(SDL_PollEvent(&event)!=0){
            switch(event.type){
                case SDL_QUIT:                            //退出事件
                    run=false;
                    break;
                case SDL_WINDOWEVENT:                     //窗口事件
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_SIZE_CHANGED://窗口大小变化事件
                            //窗口大小变化后，原有纹理失效
                            first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,72);
                            draw_ui(ren);
                            break;
                    }break;
                case SDL_MOUSEMOTION:                     //鼠标移动事件
                    //矩形跟随鼠标
                    rect[0].x=event.motion.x-5;
                    rect[0].y=event.motion.y-5;
                    first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,72);
                    draw_rectangle(ren,&rect[0],&col[0]);
                    draw_ui(ren);
                    break;
                case SDL_MOUSEBUTTONDOWN:                 //鼠标按下事件
                    draw_rectangle(ren,&rect[0],&col[4]);
                    draw_ui(ren);
                    break;
                case SDL_MOUSEBUTTONUP:                   //鼠标抬起事件
                    draw_rectangle(ren,&rect[0],&col[0]);
                    draw_ui(ren);
                    break;
                case SDL_KEYDOWN:                         //按键按下事件
                    switch(event.key.keysym.sym){
                        //按esc退出
                        case SDLK_ESCAPE:                 //按下esc
                            goto QUIT;
                            break;
                    }
                    break;
                case SDL_KEYUP:                           //案件抬起事件
                    break;
            }
        }
    }
    //quit必须放在事件循环外
    QUIT: quit(win,ren);
}