#include<ui.h>

//监听行为
void action(char *position,SDL_Rect *rect,color *col,int speed,char *f_p){
    SDL_Window *win=init_creat();
    SDL_Renderer *ren=create_renderer(win);
    SDL_Event event;
    first_ui(ren,win,&rect[1],&col[0],&col[4],position,f_p,10);
    draw_ui(ren);
    int id=back_music();
    bool run=true;

    //
    while(run){
        while(SDL_PollEvent(&event)!=0){
            switch(event.type){
                case SDL_QUIT:                            //退出事件
                    run=false;
                    break;
                case SDL_WINDOWEVENT:                     //窗口事件
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_SIZE_CHANGED://窗口大小变化事件
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
                case SDL_KEYUP:                           //按键抬起事件
                    break;
            }
        }
    }
    //quit必须放在事件循环外
    QUIT: quit(win,ren);
}

int main(int argc, char *argv[]) {
   action(position,rect,col,speed,f_p);
   return 0;
}
