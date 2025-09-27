#include <vector>
#include "utils.hpp"

#define MAP_SIZE 30
#define MIN_MAP_SIZE_SCORE 40
#define MAX_MAP_SIZE_SCORE 60

enum SnakeDirection {
  UP, DOWN, LEFT, RIGHT
};

typedef struct Point {
  int x = 0;
  int y = 0;
} Point;

typedef struct Snake {
  int score = 0;
  bool is_alive = true;
  SnakeDirection dir = RIGHT;
  Point head = { 2, 0 };
  std::vector<Point>* body = nullptr; // to be initialized
  Point candy = {
    get_random(1, MAP_SIZE - 1), // to avoid being generated at the snake head
    get_random(1, MAP_SIZE - 1)
  };
  unsigned int map_size_add = 0;
} Snake;

void snake_update(Snake* snake);
