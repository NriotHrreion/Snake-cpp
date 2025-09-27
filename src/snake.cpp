#include <vector>
#include <SDL3/SDL.h>
#include "utils.hpp"
#include "snake.hpp"

using namespace std;

Point last_point = { -1, -1 };

void move_head(Snake* snake) {
  last_point = snake->head;

  switch(snake->dir) {
    case UP:
      snake->head.y--;
      break;
    case DOWN:
      snake->head.y++;
      break;
    case LEFT:
      snake->head.x--;
      break;
    case RIGHT:
      snake->head.x++;
      break;
  }

  // Go through the border
  if(snake->head.x < 0) {
    snake->head.x = MAP_SIZE + snake->map_size_add - 1;
  }
  if(snake->head.x >= MAP_SIZE + snake->map_size_add) {
    snake->head.x = 0;
  }
  if(snake->head.y < 0) {
    snake->head.y = MAP_SIZE + snake->map_size_add - 1;
  }
  if(snake->head.y >= MAP_SIZE + snake->map_size_add) {
    snake->head.y = 0;
  }

  // No eating self
  for(Point bp : *snake->body) {
    if(snake->head.x == bp.x && snake->head.y == bp.y) {
      SDL_Log("You died");
      snake->is_alive = false;
      break;
    }
  }
}

void move_body(Snake* snake) {
  std::vector<Point>* body = snake->body;
  for(size_t i = 0; i < body->size(); i++) {
    Point* bp = &(*body)[i];
    Point temp = *bp;
    (*bp).x = last_point.x;
    (*bp).y = last_point.y;
    last_point = temp;
  }
  last_point = { -1, -1 };
}

void new_candy(Snake* snake) {
  snake->candy.x = get_random(0, MAP_SIZE + snake->map_size_add - 1);
  snake->candy.y = get_random(0, MAP_SIZE + snake->map_size_add - 1);
}

void snake_update(Snake* snake) {
  if(!snake->is_alive) return;
  std::vector<Point>* body = snake->body;

  move_head(snake);
  move_body(snake);

  if(snake->head.x == snake->candy.x && snake->head.y == snake->candy.y) {
    snake->score++;
    body->push_back(body->back());
    new_candy(snake);
  }
}
