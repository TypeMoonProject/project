#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 窗口设置
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000
#define FPS 60

// 游戏状态
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER
} GameState;

// 食材和耗材枚举
typedef enum {
    INGREDIENT_BREAD,
    INGREDIENT_MEAT,
    INGREDIENT_CUCUMBER,
    INGREDIENT_SAUCE,
    INGREDIENT_FRIES,
    INGREDIENT_KETCHUP,
    INGREDIENT_COKE,
    INGREDIENT_COUNT
} IngredientType;

typedef enum {
    CONSUMABLE_SHAWARMA_WRAP,
    CONSUMABLE_FRY_BOX,
    CONSUMABLE_COKE_CUP,
    CONSUMABLE_COUNT
} ConsumableType;

// 餐品枚举
typedef enum {
    FOOD_SHAWARMA,
    FOOD_FRIES,
    FOOD_COKE,
    FOOD_COUNT
} FoodType;

// 顾客需求结构
typedef struct {
    FoodType type;
    int ingredients[INGREDIENT_COUNT]; // 0 = 不需要, 1 = 需要
} CustomerRequest;

// 顾客结构
typedef struct {
    CustomerRequest requests[3]; // 最多3个需求
    int request_count;
    int patience; // 100 = 满耐心
    int x, y;
} Customer;

// 卷饼制作状态
typedef struct {
    int has_bread; // 0 = 没有, 1 = 有
    int ingredients[INGREDIENT_COUNT]; // 0 = 没有, 1 = 有
    int rolled; // 0 = 未卷起, 1 = 卷起
    int wrapped; // 0 = 未包装, 1 = 包装
} Shawarma;

// 游戏数据结构
typedef struct {
    GameState state;
    int coins; // 金币
    int day; // 当前天数
    int time_left; // 剩余时间（秒）
    int game_time; // 游戏总时间（秒）
    
    // 库存
    int ingredients[INGREDIENT_COUNT];
    int consumables[CONSUMABLE_COUNT];
    int max_stock;
    
    // 操作台
    Shawarma current_shawarma;
    int wrapped_shawarmas; // 已包装的卷饼数量
    int max_wrapped_shawarmas;
    
    // 顾客
    Customer customers[3]; // 最多3个顾客
    int customer_count;
    int max_customers;
    
    // 店铺升级
    int has_auto_meat_cutter;
    int has_gold_plate;
    int has_expanded_store;
    
    // 预处理状态
    int cutting_meat;
    int cutting_potato;
    int frying_fries;
    int preprocess_timer;
    
    // 游戏价值
    int food_values[FOOD_COUNT];
    int upgrade_costs[3]; // 自动切肉机, 金盘子, 扩充店面
} GameData;

// 初始化游戏数据
void init_game_data(GameData *game) {
    game->state = GAME_STATE_MENU;
    game->coins = 0;
    game->day = 0;
    game->time_left = 120; // 2分钟
    game->game_time = 0;
    
    game->max_stock = 20;
    game->ingredients[INGREDIENT_BREAD] = game->max_stock;
    game->ingredients[INGREDIENT_MEAT] = game->max_stock;
    game->ingredients[INGREDIENT_CUCUMBER] = game->max_stock;
    game->ingredients[INGREDIENT_SAUCE] = game->max_stock;
    game->ingredients[INGREDIENT_FRIES] = game->max_stock;
    game->ingredients[INGREDIENT_KETCHUP] = game->max_stock;
    game->ingredients[INGREDIENT_COKE] = game->max_stock;
    
    game->consumables[CONSUMABLE_SHAWARMA_WRAP] = game->max_stock;
    game->consumables[CONSUMABLE_FRY_BOX] = game->max_stock;
    game->consumables[CONSUMABLE_COKE_CUP] = game->max_stock;
    
    game->max_wrapped_shawarmas = 3;
    game->wrapped_shawarmas = 0;
    memset(&game->current_shawarma, 0, sizeof(Shawarma));
    
    game->max_customers = 1;
    game->customer_count = 0;
    memset(game->customers, 0, sizeof(Customer) * 3);
    
    game->has_auto_meat_cutter = 0;
    game->has_gold_plate = 0;
    game->has_expanded_store = 0;
    
    game->cutting_meat = 0;
    game->cutting_potato = 0;
    game->frying_fries = 0;
    game->preprocess_timer = 0;
    
    // 设置餐品价值
    game->food_values[FOOD_SHAWARMA] = 10;
    game->food_values[FOOD_FRIES] = 5;
    game->food_values[FOOD_COKE] = 3;
    
    // 设置升级价格
    game->upgrade_costs[0] = 50; // 自动切肉机
    game->upgrade_costs[1] = 100; // 金盘子
    game->upgrade_costs[2] = 150; // 扩充店面
}

// 渲染文本
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    int width = surface->w;
    int height = surface->h;
    SDL_Rect rect = { x, y, width, height };
    
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// 渲染按钮
int render_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int width, int height, int hover) {
    SDL_Color button_color;
    SDL_Color border_color;
    SDL_Color text_color = (SDL_Color){255, 255, 255, 255};
    
    // 根据按钮文本设置不同的主题色
    if (strcmp(text, "Place Bread") == 0 || strcmp(text, "Add Cucumber") == 0 || strcmp(text, "Add Sauce") == 0) {
        // 绿色主题 - 食材
        button_color = hover ? (SDL_Color){70, 130, 70, 255} : (SDL_Color){50, 100, 50, 255};
    } else if (strcmp(text, "Add Meat") == 0) {
        // 红色主题 - 肉
        button_color = hover ? (SDL_Color){178, 34, 34, 255} : (SDL_Color){139, 0, 0, 255};
    } else if (strcmp(text, "Add Fries") == 0 || strcmp(text, "Make Fries") == 0) {
        // 棕色主题 - 薯条
        button_color = hover ? (SDL_Color){210, 180, 140, 255} : (SDL_Color){222, 184, 135, 255};
    } else if (strcmp(text, "Add Ketchup") == 0) {
        // 红色主题 - 番茄酱
        button_color = hover ? (SDL_Color){255, 99, 71, 255} : (SDL_Color){220, 20, 60, 255};
    } else if (strcmp(text, "Make Coke") == 0) {
        // 蓝色主题 - 可乐
        button_color = hover ? (SDL_Color){100, 149, 237, 255} : (SDL_Color){0, 191, 255, 255};
    } else if (strcmp(text, "Roll Wrap") == 0 || strcmp(text, "Package") == 0) {
        // 橙色主题 - 制作
        button_color = hover ? (SDL_Color){255, 165, 0, 255} : (SDL_Color){205, 133, 63, 255};
    } else if (strcmp(text, "Restock") == 0) {
        // 紫色主题 - 补货
        button_color = hover ? (SDL_Color){147, 112, 219, 255} : (SDL_Color){138, 43, 226, 255};
    } else {
        // 默认蓝色主题
        button_color = hover ? (SDL_Color){100, 150, 255, 255} : (SDL_Color){50, 100, 200, 255};
    }
    
    // 设置边框颜色
    border_color = hover ? (SDL_Color){255, 255, 255, 255} : (SDL_Color){200, 200, 200, 255};
    
    // 绘制按钮背景
    SDL_Rect rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, border_color.a);
    SDL_RenderDrawRect(renderer, &rect);
    
    // 绘制内边框（3D效果）
    if (hover) {
        // 悬停时添加发光效果
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        SDL_RenderDrawRect(renderer, &(SDL_Rect){x - 2, y - 2, width + 4, height + 4});
    }
    
    // 计算文本居中位置
    int text_width, text_height;
    TTF_SizeText(font, text, &text_width, &text_height);
    int text_x = x + (width - text_width) / 2;
    int text_y = y + (height - text_height) / 2;
    
    render_text(renderer, font, text, text_x, text_y, text_color);
    
    return hover;
}

// 检查按钮点击
int check_button_click(int mouse_x, int mouse_y, int button_x, int button_y, int button_width, int button_height) {
    return (mouse_x >= button_x && mouse_x <= button_x + button_width && 
            mouse_y >= button_y && mouse_y <= button_y + button_height);
}

// 添加食材到卷饼
int add_ingredient(GameData *game, IngredientType ingredient) {
    if (!game->current_shawarma.has_bread) {
        // 需要先放置面饼
        return 0;
    }
    
    if (game->current_shawarma.rolled) {
        // 已卷起，不能添加食材
        return 0;
    }
    
    if (game->ingredients[ingredient] <= 0) {
        // 食材不足
        return 0;
    }
    
    // 添加食材
    game->current_shawarma.ingredients[ingredient] = 1;
    game->ingredients[ingredient]--;
    
    return 1;
}

// 补货
int restock(GameData *game, IngredientType ingredient) {
    if (game->ingredients[ingredient] >= game->max_stock) {
        // 库存已满
        return 0;
    }
    
    // 检查是否需要预处理
    if (ingredient == INGREDIENT_MEAT && !game->has_auto_meat_cutter) {
        // 需要切肉
        if (game->cutting_meat) {
            return 0;
        }
        game->cutting_meat = 1;
        game->preprocess_timer = 5; // 5秒
        return 1;
    }
    
    if (ingredient == INGREDIENT_FRIES) {
        // 需要切土豆和炸薯条
        if (game->cutting_potato || game->frying_fries) {
            return 0;
        }
        game->cutting_potato = 1;
        game->preprocess_timer = 5; // 5秒
        return 1;
    }
    
    // 直接补货
    int amount = game->max_stock - game->ingredients[ingredient];
    game->ingredients[ingredient] += amount;
    
    return 1;
}

// 生成随机顾客需求
void generate_customer_requests(Customer *customer) {
    // 随机生成1-3个需求
    customer->request_count = rand() % 3 + 1;
    
    for (int i = 0; i < customer->request_count; i++) {
        CustomerRequest *req = &customer->requests[i];
        
        // 随机选择餐品类型
        req->type = rand() % FOOD_COUNT;
        
        // 初始化食材需求
        memset(req->ingredients, 0, sizeof(req->ingredients));
        
        if (req->type == FOOD_SHAWARMA) {
            // 卷饼必须至少有一个食材
            int ingredient_count = rand() % 4 + 1;
            int added = 0;
            
            while (added < ingredient_count) {
                IngredientType ing = rand() % INGREDIENT_COUNT;
                if (ing != INGREDIENT_BREAD && ing != INGREDIENT_COKE) {
                    req->ingredients[ing] = 1;
                    added++;
                }
            }
        }
    }
    
    customer->patience = 100;
}

// 检查卷饼是否符合需求
int check_shawarma_match(Shawarma *shawarma, CustomerRequest *request) {
    if (request->type != FOOD_SHAWARMA) {
        return 0;
    }
    
    // 检查所有食材
    for (int i = 0; i < INGREDIENT_COUNT; i++) {
        if (i == INGREDIENT_BREAD) {
            continue; // 不检查面饼
        }
        
        if (shawarma->ingredients[i] != request->ingredients[i]) {
            return 0;
        }
    }
    
    return 1;
}

// 处理顾客
void process_customers(GameData *game) {
    // 更新顾客耐心
    for (int i = 0; i < game->customer_count; i++) {
        game->customers[i].patience -= 1;
        
        if (game->customers[i].patience <= 0) {
            // 顾客离开
            for (int j = i; j < game->customer_count - 1; j++) {
                game->customers[j] = game->customers[j + 1];
            }
            game->customer_count--;
            i--;
        }
    }
    
    // 随机生成新顾客
    if (game->customer_count < game->max_customers && rand() % 100 < 5) {
        Customer *customer = &game->customers[game->customer_count];
        generate_customer_requests(customer);
        customer->x = rand() % (WINDOW_WIDTH - 100) + 50;
        customer->y = rand() % (WINDOW_HEIGHT - 200) + 100;
        game->customer_count++;
    }
}

// 游戏主循环
int main(int argc, char *argv[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // 初始化TTF
    if (TTF_Init() < 0) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    // 创建窗口
    SDL_Window *window = SDL_CreateWindow(
        "Shawarma Legend",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 加载字体
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 24);
    if (font == NULL) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 游戏数据
    GameData game;
    init_game_data(&game);
    
    // 随机种子
    srand(time(NULL));
    
    // 游戏循环
    int quit = 0;
    SDL_Event e;
    int mouse_x, mouse_y;
    int mouse_clicked = 0;
    
    Uint32 last_time = SDL_GetTicks();
    Uint32 last_second = SDL_GetTicks();
    
    while (!quit) {
        // 处理事件
        mouse_clicked = 0;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_MOUSEMOTION) {
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                mouse_x = e.button.x;
                mouse_y = e.button.y;
                mouse_clicked = 1;
            }
        }
        
        // 更新游戏逻辑
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_second >= 1000) {
            // 每秒更新
            last_second = current_time;
            
            if (game.state == GAME_STATE_PLAYING) {
                game.time_left--;
                game.game_time++;
                
                if (game.time_left <= 0) {
                    // 游戏结束
                    game.state = GAME_STATE_MENU;
                }
                
                // 处理预处理
                if (game.cutting_meat || game.cutting_potato || game.frying_fries) {
                    game.preprocess_timer--;
                    
                    if (game.preprocess_timer <= 0) {
                        if (game.cutting_meat) {
                            // 切肉完成
                            game.ingredients[INGREDIENT_MEAT] = game.max_stock;
                            game.cutting_meat = 0;
                        }
                        else if (game.cutting_potato) {
                            // 切土豆完成，开始炸薯条
                            game.cutting_potato = 0;
                            game.frying_fries = 1;
                            game.preprocess_timer = 5;
                        }
                        else if (game.frying_fries) {
                            // 炸薯条完成
                            game.ingredients[INGREDIENT_FRIES] = game.max_stock;
                            game.frying_fries = 0;
                        }
                    }
                }
                
                // 自动切肉机
                if (game.has_auto_meat_cutter && game.ingredients[INGREDIENT_MEAT] <= 0) {
                    game.ingredients[INGREDIENT_MEAT] = game.max_stock;
                }
                
                // 处理顾客
                process_customers(&game);
            }
        }
        
        // 清空屏幕
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        // 渲染游戏
        SDL_Color text_color = {0, 0, 0, 255};
        
        if (game.state == GAME_STATE_MENU) {
            // 入口界面
            render_text(renderer, font, "Shawarma Legend", 280, 50, text_color);
            
            char day_text[50];
            char coins_text[50];
            sprintf(day_text, "Days Played: %d", game.day);
            sprintf(coins_text, "Total Coins: %d", game.coins);
            
            render_text(renderer, font, day_text, 300, 150, text_color);
            render_text(renderer, font, coins_text, 300, 200, text_color);
            
            // 按钮
            int new_day_hover = render_button(renderer, font, "Start New Day", 280, 300, 240, 60, 
                                             (mouse_x >= 280 && mouse_x <= 520 && mouse_y >= 300 && mouse_y <= 360));
            
            int upgrade_hover = render_button(renderer, font, "Upgrade Shop", 280, 390, 240, 60, 
                                             (mouse_x >= 280 && mouse_x <= 520 && mouse_y >= 390 && mouse_y <= 450));
            
            if (mouse_clicked) {
                if (check_button_click(mouse_x, mouse_y, 280, 300, 240, 60)) {
                    // Start new day
                    game.state = GAME_STATE_PLAYING;
                    game.day++;
                    game.time_left = 120;
                    
                    // Reset counter
                    memset(&game.current_shawarma, 0, sizeof(Shawarma));
                    game.wrapped_shawarmas = 0;
                    
                    // Reset customers
                    game.customer_count = 0;
                    memset(game.customers, 0, sizeof(Customer) * 3);
                }
                else if (check_button_click(mouse_x, mouse_y, 280, 390, 240, 60)) {
                    // Upgrade shop
                    // Upgrade interface can be added here
                }
            }
        }
        else if (game.state == GAME_STATE_PLAYING) {
            // 主界面
            char buffer[100];
            
            // Status Bar
            sprintf(buffer, "Day: %d", game.day);
            render_text(renderer, font, buffer, 20, 20, text_color);
            
            sprintf(buffer, "Coins: %d", game.coins);
            render_text(renderer, font, buffer, 20, 50, text_color);
            
            sprintf(buffer, "Time Left: %d seconds", game.time_left);
            render_text(renderer, font, buffer, 20, 80, text_color);
            
            // Inventory Section
            render_text(renderer, font, "Inventory:", 20, 120, text_color);
            
            sprintf(buffer, "Bread: %d", game.ingredients[INGREDIENT_BREAD]);
            render_text(renderer, font, buffer, 20, 150, text_color);
            
            sprintf(buffer, "Meat: %d", game.ingredients[INGREDIENT_MEAT]);
            render_text(renderer, font, buffer, 20, 180, text_color);
            
            sprintf(buffer, "Cucumber: %d", game.ingredients[INGREDIENT_CUCUMBER]);
            render_text(renderer, font, buffer, 20, 210, text_color);
            
            sprintf(buffer, "Sauce: %d", game.ingredients[INGREDIENT_SAUCE]);
            render_text(renderer, font, buffer, 20, 240, text_color);
            
            sprintf(buffer, "Fries: %d", game.ingredients[INGREDIENT_FRIES]);
            render_text(renderer, font, buffer, 20, 270, text_color);
            
            sprintf(buffer, "Ketchup: %d", game.ingredients[INGREDIENT_KETCHUP]);
            render_text(renderer, font, buffer, 20, 300, text_color);
            
            sprintf(buffer, "Coke: %d", game.ingredients[INGREDIENT_COKE]);
            render_text(renderer, font, buffer, 20, 330, text_color);
            
            // Shawarma Station
            render_text(renderer, font, "Shawarma Station:", 600, 20, text_color);
            
            sprintf(buffer, "Current Shawarma: %s", game.current_shawarma.has_bread ? "With Bread" : "No Bread");
            render_text(renderer, font, buffer, 600, 50, text_color);
            
            sprintf(buffer, "Wrapped Shawarmas: %d/%d", game.wrapped_shawarmas, game.max_wrapped_shawarmas);
            render_text(renderer, font, buffer, 600, 80, text_color);
            
            // Render Customers
            for (int i = 0; i < game.customer_count; i++) {
                Customer *customer = &game.customers[i];
                sprintf(buffer, "Customer %d", i + 1);
                render_text(renderer, font, buffer, customer->x, customer->y, text_color);
                
                sprintf(buffer, "Patience: %d%%", customer->patience);
                render_text(renderer, font, buffer, customer->x, customer->y + 30, text_color);
                
                // Render customer requests
                render_text(renderer, font, "Order:", customer->x, customer->y + 60, text_color);
                for (int j = 0; j < customer->request_count; j++) {
                    CustomerRequest *req = &customer->requests[j];
                    const char *food_name;
                    switch (req->type) {
                        case FOOD_SHAWARMA:
                            food_name = "Shawarma";
                            break;
                        case FOOD_FRIES:
                            food_name = "Fries";
                            break;
                        case FOOD_COKE:
                            food_name = "Coke";
                            break;
                        default:
                            food_name = "Unknown";
                            break;
                    }
                    sprintf(buffer, "- %s", food_name);
                    render_text(renderer, font, buffer, customer->x, customer->y + 90 + (j * 30), text_color);
                }
            }
            
            // 渲染按钮
            int button_y = 350;
            int button_width = 150;
            int button_height = 40;
            int button_spacing = 20;
            
            // Place Bread
            int place_bread_hover = render_button(renderer, font, "Place Bread", 10, button_y, button_width, button_height, 
                                                 (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                  mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Add Meat
            button_y += button_height + button_spacing;
            int add_meat_hover = render_button(renderer, font, "Add Meat", 10, button_y, button_width, button_height, 
                                              (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                               mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Add Cucumber
            button_y += button_height + button_spacing;
            int add_cucumber_hover = render_button(renderer, font, "Add Cucumber", 10, button_y, button_width, button_height, 
                                                  (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                   mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Add Sauce
            button_y += button_height + button_spacing;
            int add_sauce_hover = render_button(renderer, font, "Add Sauce", 10, button_y, button_width, button_height, 
                                              (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                               mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Add Fries
            button_y += button_height + button_spacing;
            int add_fries_hover = render_button(renderer, font, "Add Fries", 10, button_y, button_width, button_height, 
                                              (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                               mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Add Ketchup
            button_y += button_height + button_spacing;
            int add_ketchup_hover = render_button(renderer, font, "Add Ketchup", 10, button_y, button_width, button_height, 
                                                 (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                  mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Roll Wrap
            button_y += button_height + button_spacing;
            int roll_shawarma_hover = render_button(renderer, font, "Roll Wrap", 10, button_y, button_width, button_height, 
                                                  (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                   mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Package
            button_y += button_height + button_spacing;
            int wrap_shawarma_hover = render_button(renderer, font, "Package", 10, button_y, button_width, button_height, 
                                                  (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                   mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Make Fries
            button_y += button_height + button_spacing;
            int make_fries_hover = render_button(renderer, font, "Make Fries", 10, button_y, button_width, button_height, 
                                                (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                 mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Make Coke
            button_y += button_height + button_spacing;
            int make_coke_hover = render_button(renderer, font, "Make Coke", 10, button_y, button_width, button_height, 
                                               (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                                mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // Restock
            button_y += button_height + button_spacing;
            int restock_hover = render_button(renderer, font, "Restock", 10, button_y, button_width, button_height, 
                                             (mouse_x >= 10 && mouse_x <= 10 + button_width && 
                                              mouse_y >= button_y && mouse_y <= button_y + button_height));
            
            // 响应按钮点击
            if (mouse_clicked) {
                if (check_button_click(mouse_x, mouse_y, 10, 350, button_width, button_height)) {
                    // 放置面饼
                    if (!game.current_shawarma.has_bread && game.ingredients[INGREDIENT_BREAD] > 0) {
                        game.current_shawarma.has_bread = 1;
                        game.ingredients[INGREDIENT_BREAD]--;
                    }
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 1, button_width, button_height)) {
                    // 添加肉
                    add_ingredient(&game, INGREDIENT_MEAT);
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 2, button_width, button_height)) {
                    // 添加黄瓜
                    add_ingredient(&game, INGREDIENT_CUCUMBER);
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 3, button_width, button_height)) {
                    // 添加沙司
                    add_ingredient(&game, INGREDIENT_SAUCE);
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 4, button_width, button_height)) {
                    // 添加薯条
                    add_ingredient(&game, INGREDIENT_FRIES);
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 5, button_width, button_height)) {
                    // 添加番茄酱
                    add_ingredient(&game, INGREDIENT_KETCHUP);
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 6, button_width, button_height)) {
                    // 卷起
                    if (game.current_shawarma.has_bread && !game.current_shawarma.rolled) {
                        game.current_shawarma.rolled = 1;
                    }
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 7, button_width, button_height)) {
                    // 包装
                    if (game.current_shawarma.rolled && !game.current_shawarma.wrapped && game.wrapped_shawarmas < game.max_wrapped_shawarmas) {
                        game.current_shawarma.wrapped = 1;
                        game.wrapped_shawarmas++;
                        
                        // 消耗包装纸
                        if (game.consumables[CONSUMABLE_SHAWARMA_WRAP] > 0) {
                            game.consumables[CONSUMABLE_SHAWARMA_WRAP]--;
                        }
                        
                        // 重置当前卷饼
                        memset(&game.current_shawarma, 0, sizeof(Shawarma));
                    }
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 8, button_width, button_height)) {
                    // 制作薯条
                    if (game.ingredients[INGREDIENT_FRIES] > 0 && game.consumables[CONSUMABLE_FRY_BOX] > 0) {
                        // 简单实现，直接给顾客
                        game.ingredients[INGREDIENT_FRIES]--;
                        game.consumables[CONSUMABLE_FRY_BOX]--;
                        
                        // 查找需要薯条的顾客
                        for (int i = 0; i < game.customer_count; i++) {
                            for (int j = 0; j < game.customers[i].request_count; j++) {
                                if (game.customers[i].requests[j].type == FOOD_FRIES) {
                                    // 满足需求
                                    game.coins += game.food_values[FOOD_FRIES];
                                    
                                    // 移除需求
                                    for (int k = j; k < game.customers[i].request_count - 1; k++) {
                                        game.customers[i].requests[k] = game.customers[i].requests[k + 1];
                                    }
                                    game.customers[i].request_count--;
                                    
                                    // 如果没有需求了，顾客离开
                                    if (game.customers[i].request_count == 0) {
                                        for (int k = i; k < game.customer_count - 1; k++) {
                                            game.customers[k] = game.customers[k + 1];
                                        }
                                        game.customer_count--;
                                    }
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 9, button_width, button_height)) {
                    // 制作可乐
                    if (game.ingredients[INGREDIENT_COKE] > 0 && game.consumables[CONSUMABLE_COKE_CUP] > 0) {
                        // 简单实现，直接给顾客
                        game.ingredients[INGREDIENT_COKE]--;
                        game.consumables[CONSUMABLE_COKE_CUP]--;
                        
                        // 查找需要可乐的顾客
                        for (int i = 0; i < game.customer_count; i++) {
                            for (int j = 0; j < game.customers[i].request_count; j++) {
                                if (game.customers[i].requests[j].type == FOOD_COKE) {
                                    // 满足需求
                                    game.coins += game.food_values[FOOD_COKE];
                                    
                                    // 移除需求
                                    for (int k = j; k < game.customers[i].request_count - 1; k++) {
                                        game.customers[i].requests[k] = game.customers[i].requests[k + 1];
                                    }
                                    game.customers[i].request_count--;
                                    
                                    // 如果没有需求了，顾客离开
                                    if (game.customers[i].request_count == 0) {
                                        for (int k = i; k < game.customer_count - 1; k++) {
                                            game.customers[k] = game.customers[k + 1];
                                        }
                                        game.customer_count--;
                                    }
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (check_button_click(mouse_x, mouse_y, 10, 350 + (button_height + button_spacing) * 10, button_width, button_height)) {
                    // 补货
                    for (int i = 0; i < INGREDIENT_COUNT; i++) {
                        if (i != INGREDIENT_MEAT && i != INGREDIENT_FRIES) {
                            restock(&game, i);
                        }
                    }
                    
                    // 补货耗材
                    for (int i = 0; i < CONSUMABLE_COUNT; i++) {
                        game.consumables[i] = game.max_stock;
                    }
                }
            }
        }
        
        // 渲染到屏幕
        SDL_RenderPresent(renderer);
        
        // 控制帧率
        Uint32 delta_time = SDL_GetTicks() - last_time;
        if (delta_time < 1000 / FPS) {
            SDL_Delay(1000 / FPS - delta_time);
        }
        last_time = SDL_GetTicks();
    }
    
    // 清理资源
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}