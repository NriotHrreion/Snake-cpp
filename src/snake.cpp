#include<ctime>
#include<random>
#include "snake.hpp"

int get_random(int min, int max) {
  srand(time(nullptr));
  return (rand() % (max - min + 1)) + min;
}

void move_head(Snake* snake) {
  switch(snake->dir) {
    case UP:
      snake->y--;
      break;
    case DOWN:
      snake->y++;
      break;
    case LEFT:
      snake->x--;
      break;
    case RIGHT:
      snake->x++;
      break;
  }

  if(snake->x < 0) {
    snake->x = MAP_SIZE - 1;
  }
  if(snake->x >= MAP_SIZE) {
    snake->x = 0;
  }
  if(snake->y < 0) {
    snake->y = MAP_SIZE - 1;
  }
  if(snake->y >= MAP_SIZE) {
    snake->y = 0;
  }
}

void new_candy(Snake* snake) {
  snake->candy.x = get_random(0, 39);
  snake->candy.y = get_random(0, 39);
}
