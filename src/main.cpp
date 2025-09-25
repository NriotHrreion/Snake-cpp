#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "snake.hpp"

#define SNAKE_BODY_PIXEL 24
#define SNAKE_SPEED 500
#define WINDOW_PADDING 10
#define WINDOW_SIZE MAP_SIZE * SNAKE_BODY_PIXEL + 2 * WINDOW_PADDING

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  Snake snake;
  Uint64 last_ms;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_Log("Starting snake-cpp...");
  SDL_SetAppMetadata("Snake C++", "0.1.0", "net.nocpiun.snake-cpp");

  if(!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));
  if(!as) {
    return SDL_APP_FAILURE;
  }
  *appstate = as;

  if(!SDL_CreateWindowAndRenderer("Snake C++", WINDOW_SIZE, WINDOW_SIZE, 0, &as->window, &as->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  Snake snake;
  as->snake = snake;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  AppState* as = (AppState*) appstate;
  Snake* snake = &as->snake;

  if(event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  } else if(event->type == SDL_EVENT_KEY_DOWN) {
    switch(event->key.key) {
      case SDLK_ESCAPE: return SDL_APP_SUCCESS;
    }
    switch(event->key.scancode) {
      case SDL_SCANCODE_W:
        if(snake->dir != DOWN) {
          snake->dir = UP;
        }
        break;
      case SDL_SCANCODE_A:
        if(snake->dir != RIGHT) {
          snake->dir = LEFT;
        }
        break;
      case SDL_SCANCODE_S:
        if(snake->dir != UP) {
          snake->dir = DOWN;
        }
        break;
      case SDL_SCANCODE_D:
        if(snake->dir != LEFT) {
          snake->dir = RIGHT;
        }
        break;
    }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  AppState* as = (AppState*) appstate;
  Snake* snake = &as->snake;
  const Uint64 now = SDL_GetTicks();

  while((now - as->last_ms) >= SNAKE_SPEED) {
    // Update snake
    move_head(snake);
    if(snake->x == snake->candy.x && snake->y == snake->candy.y) {
      snake->score++;
      new_candy(snake);
    }

    as->last_ms = now;
  }

  // Reset canvas
  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(as->renderer);
  
  // Draw border
  SDL_FRect border;
  border.w = border.h = WINDOW_SIZE - WINDOW_PADDING * 2;
  border.x = border.y = WINDOW_PADDING;
  SDL_SetRenderDrawColor(as->renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
  SDL_RenderRect(as->renderer, &border);

  // Draw candy
  SDL_FRect candy;
  candy.w = candy.h = SNAKE_BODY_PIXEL;
  candy.x = snake->candy.x * SNAKE_BODY_PIXEL + WINDOW_PADDING;
  candy.y = snake->candy.y * SNAKE_BODY_PIXEL + WINDOW_PADDING;
  SDL_SetRenderDrawColor(as->renderer, 128, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(as->renderer, &candy);

  // Draw body
  SDL_FRect body;
  body.w = body.h = SNAKE_BODY_PIXEL;
  body.x = snake->x * SNAKE_BODY_PIXEL + WINDOW_PADDING;
  body.y = snake->y * SNAKE_BODY_PIXEL + WINDOW_PADDING;
  SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(as->renderer, &body);
  SDL_RenderPresent(as->renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  SDL_Log("Stopping snake-cpp...");

  if(appstate != NULL) {
    AppState* as = (AppState*) appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}
