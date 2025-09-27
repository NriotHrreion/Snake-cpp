#define SDL_MAIN_USE_CALLBACKS 1

#include <ctime>
#include <random>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "snake.hpp"

#define SNAKE_BODY_PIXEL 24
#define SNAKE_SPEED 200
#define WINDOW_PADDING 10
#define WINDOW_SIZE MAP_SIZE * SNAKE_BODY_PIXEL + 2 * WINDOW_PADDING

std::vector<Point> initial_snake_body = {
  { 1, 0 },
  { 0, 0 }
};

typedef struct AppState {
  SDL_Window* window;
  SDL_Renderer* renderer;
  Snake snake;
  Uint64 last_ms = 0;
} AppState;

int point_to_coord(const int point_num) {
  return point_num * SNAKE_BODY_PIXEL + WINDOW_PADDING;
}

int get_window_size(const unsigned int map_size_add) {
  return WINDOW_SIZE + map_size_add * SNAKE_BODY_PIXEL;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  SDL_Log("Starting snake-cpp...");
  SDL_SetAppMetadata("Snake C++", "0.1.0", "net.nocpiun.snake-cpp");

  srand(time(nullptr));

  if(!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState* as = (AppState*) SDL_calloc(1, sizeof(AppState));
  if(!as) {
    SDL_Log("AppState is null");
    return SDL_APP_FAILURE;
  }
  *appstate = as;

  if(!SDL_CreateWindowAndRenderer(
    "Snake C++",
    get_window_size(as->snake.map_size_add),
    get_window_size(as->snake.map_size_add),
    0,
    &as->window,
    &as->renderer
  )) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  Snake snake;
  snake.body = &initial_snake_body;
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

  if(!snake->is_alive) {
    SDL_SetWindowTitle(as->window, "Snake C++ - You Died!");
    return SDL_APP_CONTINUE;
  }

  while((now - as->last_ms) >= SNAKE_SPEED) {
    snake_update(snake);

    // Update map size
    if(snake->score > MIN_MAP_SIZE_SCORE && snake->score <= MAX_MAP_SIZE_SCORE) {
      as->snake.map_size_add = snake->score - MIN_MAP_SIZE_SCORE;
      SDL_SetWindowSize(as->window, get_window_size(as->snake.map_size_add), get_window_size(as->snake.map_size_add));
    }

    as->last_ms = now;
  }

  // Reset canvas
  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(as->renderer);
  
  // Draw border
  SDL_FRect border;
  border.w = border.h = get_window_size(as->snake.map_size_add) - WINDOW_PADDING * 2;
  border.x = border.y = WINDOW_PADDING;
  SDL_SetRenderDrawColor(as->renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
  SDL_RenderRect(as->renderer, &border);

  // Draw candy
  SDL_FRect candy;
  candy.w = candy.h = SNAKE_BODY_PIXEL;
  candy.x = point_to_coord(snake->candy.x);
  candy.y = point_to_coord(snake->candy.y);
  SDL_SetRenderDrawColor(as->renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(as->renderer, &candy);

  // Draw snake head
  SDL_FRect head;
  head.w = head.h = SNAKE_BODY_PIXEL;
  head.x = point_to_coord(snake->head.x);
  head.y = point_to_coord(snake->head.y);
  SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(as->renderer, &head);

  // Draw snake body
  SDL_FRect body;
  body.w = body.h = SNAKE_BODY_PIXEL;
  SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  for(Point bp : *snake->body) {
    body.x = point_to_coord(bp.x);
    body.y = point_to_coord(bp.y);
    SDL_RenderFillRect(as->renderer, &body);
  }

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
