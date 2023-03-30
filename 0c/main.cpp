#include <iostream>
#include <vector>

/*
На склад привезли много пустых ящиков. Все ящики пронумерованы по порядку
поступления от 0. Известно, что их все можно сложить один в один (то есть так,
что каждый следующий помещается в предыдущий). Один ящик можно вложить в другой,
если его можно перевернуть так, что размеры одного ящика по всем осям станут
строго меньше размеров другого ящика по соответствующим осям.

Требуется определить, в какой последовательности они будут вложены друг в друга.
Вывести номера ящиков. Для сортировки точек реализуйте алгоритм сортировки
вставками. Максимальное кол-во 10000. 
*/

struct Box {
  int x;
  int y;
  int z;
  int i;

  Box() : x(0), y(0), z(0), i(0) {}
  Box(int x, int y, int z, int i) : x(x), y(y), z(z), i(i) {}

  friend bool operator<(const Box& box_1, const Box& box_2);
};

bool operator<(const Box& box_1, const Box& box_2) {
  return box_1.x < box_2.x && box_1.y < box_2.y && box_1.z < box_2.z
      || box_1.x < box_2.x && box_1.y < box_2.z && box_1.z < box_2.y
      || box_1.x < box_2.y && box_1.y < box_2.x && box_1.z < box_2.z
      || box_1.x < box_2.y && box_1.y < box_2.z && box_1.z < box_2.x
      || box_1.x < box_2.z && box_1.y < box_2.x && box_1.z < box_2.y
      || box_1.x < box_2.z && box_1.y < box_2.y && box_1.z < box_2.x;
}

template <typename T>
void InsertionSort(std::vector<T>& v) {
  for (auto i = 1; i < v.size(); i++) {
    auto tmp = v[i];
    auto j = i - 1;
    while (j >= 0 && tmp < v[j]) {
      v[j + 1] = v[j];
      j--;
    }
    v[j + 1] = tmp;
  }
}

int main() {
  int n { 0 };
  std::cin >> n;

  std::vector<Box> boxes { };
  for (auto i = 0; i < n; i++) {
    int x { 0 }, y { 0 }, z { 0 };
    std::cin >> x >> y >> z;
    boxes.push_back(Box(x, y, z, i));
  }

  InsertionSort<Box>(boxes);

  for (const auto& box : boxes)
    std::cout << box.i << ' ';
  std::cout << std::endl;

  return 0;
}
