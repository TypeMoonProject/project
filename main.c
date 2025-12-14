#include<ui.h>
<<<<<<< Updated upstream
=======
#include<Inputmanagement.h>
#include<ExtensionModule.h>
#include<game_manager.h>
#include<inventory_manager.h>
#include<customer.h>

// 定义game_manager.h中声明的全局变量
GameState* state1 = NULL;
Inventory* state2 = NULL;
Customer* state3 = NULL;

// 初始化游戏状态
void init_game_state() {
    // 动态分配内存
    state1 = (GameState*)malloc(sizeof(GameState));
    state2 = (Inventory*)malloc(sizeof(Inventory));
    state3 = (Customer*)malloc(sizeof(Customer));
    
    // 初始化库存
    InitInventory(state2);
    
    // 初始化游戏状态
    if (state1) {
        state1->running_or_not = 1;
        state1->money = 100;
        state1->day = 1;
        state1->time = 0;
    }
}

// 释放游戏状态内存
void free_game_state() {
    if (state1) {
        free(state1);
        state1 = NULL;
    }
    if (state2) {
        free(state2);
        state2 = NULL;
    }
    if (state3) {
        free(state3);
        state3 = NULL;
    }
}

// 游戏状态枚举
typedef enum {
    GAME_STATE_START,      // 开始界面
    GAME_STATE_PLAYING,    // 游戏界面
    GAME_STATE_PAUSED      // 游戏暂停
} GameStateType;

//监听行为
void action(char *position,SDL_Rect *rect,color *col,int speed,char *f_p){
    // 初始化游戏状态
    init_game_state();
    
    // 生成第一个顾客
    *state3 = GenerateCustomer();
    state3->customer_id = 1;
    
    SDL_Window *win=init_creat();
    SDL_Renderer *ren=create_renderer(win);
    SDL_Event event;
    
    // 当前游戏状态
    GameStateType current_state = GAME_STATE_START;
    
    // 绘制开始界面
    first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,10);
    draw_ui(ren);
    int id=back_music();
    bool run=true;
    
    // 游戏主循环
    while(run){
        while(SDL_PollEvent(&event)!=0){
            switch(event.type){
                case SDL_QUIT:                            //退出事件
                    run=false;
                    break;
                case SDL_WINDOWEVENT:                     //窗口事件
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_SIZE_CHANGED://窗口大小变化事件
                            if (current_state == GAME_STATE_START) {
                                first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,72);
                            } else {
                                game_ui(ren, win, f_p, state2, state3);
                            }
                            draw_ui(ren);
                            break;
                    }break;
                case SDL_MOUSEMOTION:                     //鼠标移动事件
                    //矩形跟随鼠标
                    rect[0].x=event.motion.x-5;
                    rect[0].y=event.motion.y-5;
                    
                    // 根据当前状态绘制相应界面
                    if (current_state == GAME_STATE_START) {
                        first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,72);
                    } else {
                        game_ui(ren, win, f_p, state2, state3);
                    }
                    
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
                    
                    // 检查是否点击了begin按键（只有在开始界面时有效）
                    if (current_state == GAME_STATE_START &&
                        event.button.x >= rect[1].x && event.button.x <= rect[1].x + rect[1].w &&
                        event.button.y >= rect[1].y && event.button.y <= rect[1].y + rect[1].h) {
                        // 点击了begin按键，切换到游戏界面
                        printf("点击了begin按键，进入游戏主界面\n");
                        current_state = GAME_STATE_PLAYING;
                        game_ui(ren, win, f_p, state2, state3);
                        draw_ui(ren);
                    }
                    break;
                case SDL_KEYDOWN:                         //按键按下事件
                    switch(event.key.keysym.sym){
                        //按esc退出
                        case SDLK_ESCAPE:                 //按下esc
                            goto QUIT;
                            break;
                        default:
                            // 处理其他按键，调用Inputmanagement模块的handle_key_input函数
                            handle_key_input(event.key.keysym.sym);
                            // 重新绘制游戏界面，显示状态变化
                            if (current_state == GAME_STATE_PLAYING) {
                                game_ui(ren, win, f_p, state2, state3);
                                draw_ui(ren);
                            }
                            break;
                    }
                    break;
                case SDL_KEYUP:                           //按键抬起事件
                    break;
            }
        }
        
        // 游戏状态更新（仅在游戏界面时）
        if (current_state == GAME_STATE_PLAYING) {
            // 更新顾客状态
            UpdateCustomer(state3, 1); // 假设每次循环代表1秒
            
            // 检查顾客是否离开，需要生成新顾客
            if (state3->patience <= 0) {
                *state3 = GenerateCustomer();
                state3->customer_id++;
                // 重新绘制游戏界面
                game_ui(ren, win, f_p, state2, state3);
                draw_ui(ren);
            }
            
            // 更新游戏时间
            if (state1) {
                state1->time++;
            }
        }
    }
    //quit必须放在事件循环外
    QUIT: 
        // 释放游戏状态内存
        free_game_state();
        quit(win,ren);
}
>>>>>>> Stashed changes

int main(int argc, char *argv[]) {
   action(position,rect,col,speed);
   return 0;
}