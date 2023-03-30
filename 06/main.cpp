#include <functional>
#include <iostream>
#include <cmath>

/*
Дано множество целых чисел из [0..10^9] размера n. Используя алгоритм поиска
k-ой порядковой статистики, требуется найти следующие параметры множества:
- 10% перцентиль
- медиана
- 90% перцентиль

Требования:
- Дополнительная память: O(n).
- Среднее время работы: O(n)
- Должна быть отдельно выделенная функция partition.
- Рекурсия запрещена.
- Решение должно поддерживать передачу функции сравнения снаружи.

Формат ввода

Сначала вводится кол-во элементов массива n. После сам массив.

Формат вывода

Параметры множества в порядке:
- 10% перцентиль
- медиана
- 90% перцентиль
*/

template <typename T, typename Comparator = std::less<T>>
void MedianOfThree(
    int left,
    int right,
    T* array,
    Comparator compare = Comparator()) {
  int middle = (left + right) / 2;
  if (compare(array[middle], array[left]))
    std::swap(array[middle], array[left]);

  if (compare(array[middle], array[right]))
    std::swap(array[middle], array[right]);
  
  if (compare(array[middle], array[left]))
    std::swap(array[middle], array[left]);
}

template <typename T, typename Comparator = std::less<T>>
int Partition(
    int left,
    int right,
    T* array,
    Comparator compare = Comparator()) {
  MedianOfThree<T, Comparator>(left, right, array, compare);
  
  auto pivot = array[right];
  auto i = right;
  for (auto j = right - 1; j >= left; j--) {
    if (compare(pivot, array[j]))
      std::swap(array[j], array[--i]);
  }
  std::swap(array[i], array[right]);

  return i;
}

template <typename T, typename Comparator = std::less<T>>
int KthStatistics(
    double percentile,
    T* array,
    int n,
    Comparator compare = Comparator()) {
  auto k = int(std::floor(n * percentile));
  auto left = 0;
  auto right = n - 1;
  while (true) {
    auto pivot_position = Partition<T, Comparator>(left, right, array, compare);
    if (pivot_position == k)
      return array[pivot_position];
    if (pivot_position > k)
      right = pivot_position - 1;
    else
      left = pivot_position + 1;
  }
}

int main() {
  constexpr int kPercentilesSize = 3;
  constexpr double kPercentiles[kPercentilesSize] { 0.1, 0.5, 0.9 };

  int n { 0 };
  std::cin >> n;
  
  int* array = new int[n];
  for (auto i = 0; i < n; i++)
    std::cin >> array[i];

  for (auto i = 0; i < kPercentilesSize; i++)
    std::cout << KthStatistics<int>(kPercentiles[i], array, n) << std::endl;

  delete[] array;
  return 0;
}
