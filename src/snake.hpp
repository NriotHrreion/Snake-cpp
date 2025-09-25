#define MAP_SIZE 30

typedef enum SnakeDirection {
  UP, DOWN, LEFT, RIGHT
} SnakeDirection;

typedef struct {
  unsigned int x;
  unsigned int y;
} Candy;

typedef struct Snake {
  unsigned int score = 0;
  int x = 0;
  int y = 0;
  SnakeDirection dir = RIGHT;
  Candy candy;
} Snake;

void move_head(Snake* snake);
void new_candy(Snake* snake);
