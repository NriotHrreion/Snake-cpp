#include<ctime>
#include<random>

int get_random(int min, int max) {
  srand(time(nullptr));
  return (rand() % (max - min + 1)) + min;
}
