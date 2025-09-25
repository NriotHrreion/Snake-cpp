#include "utils.hpp"

#define MAP_SIZE 30

typedef enum SnakeDirection {
  UP, DOWN, LEFT, RIGHT
} SnakeDirection;

typedef struct Candy {
  unsigned int x = get_random(1, MAP_SIZE - 1); // to avoid being generated at the snake head
  unsigned int y = get_random(1, MAP_SIZE - 1);
} Candy;

typedef struct Snake {
  unsigned int score = 0;
  int x = 0;
  int y = 0;
  SnakeDirection dir = RIGHT;
  Candy candy;
} Snake;

void snake_update(Snake* snake);
