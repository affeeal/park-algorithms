#include <iostream>
#include <vector>

/*
Дан отсортированный по возрастанию массив попарно различных целых чисел
A[0..n-1]. На вход также подается некоторое целое число k. Необходимо вернуть
индекс элемента k в массиве A, если он там присутствует, либо вернуть позицию,
куда его следовало бы вставить, чтобы массив остался отсортированным.

Требование: сложность O(logN)

Формат ввода

В первой строчке записано число n – размерность массива A. Во второй строчке
перечислены элементы массива A. В третьей строчке записано значение k.

Формат вывода

Целое число — позиция k в A. 
*/

int BinarySearch(int k, const std::vector<int>& v) {
  int left = 0;
  int right = v.size();
  while (left < right) {
    int middle = (left + right) / 2;
    if (v[middle] < k)
      left = middle + 1;
    else
      right = middle;
  }
  return left;
}

int main() {
  int n { 0 };
  std::cin >> n;

  std::vector<int> v(n);
  for (auto& elem : v)
    std::cin >> elem;

  int k { 0 };
  std::cin >> k;

  std::cout << BinarySearch(k, v) << std::endl;

  return 0;
}
