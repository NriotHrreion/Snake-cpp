#include <vector>
#include "utils.hpp"

#define MAP_SIZE 30

enum SnakeDirection {
  UP, DOWN, LEFT, RIGHT
};

typedef struct Point {
  int x;
  int y;
} Point;

typedef struct Snake {
  int score = 0;
  bool is_alive = true;
  SnakeDirection dir = RIGHT;
  Point head = { 2, 0 };
  std::vector<Point> body = {
    { 1, 0 },
    { 0, 0 }
  };
  Point candy = {
    get_random(1, MAP_SIZE - 1), // to avoid being generated at the snake head
    get_random(1, MAP_SIZE - 1)
  };
} Snake;

void snake_update(Snake* snake);
