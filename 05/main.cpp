#include <cstddef>
#include <cstring>
#include <iostream>

template <typename T, typename Comparator = std::less<T>>
void Merge(std::size_t left, std::size_t middle, std::size_t right,
    T* array, Comparator compare = Comparator()) {

  std::size_t temp_array_size = right - left + 1;
  T* temp_array = new T[temp_array_size];

  std::size_t i = left;
  std::size_t j = middle + 1;

  for (std::size_t k = 0; k < temp_array_size; k++) {
    if (j <= right && (i == middle + 1 || compare(array[j], array[i])))
      temp_array[k] = array[j++];
    else
      temp_array[k] = array[i++];
  }

  std::memcpy(array + left, temp_array, temp_array_size * sizeof(T));
  delete[] temp_array;
}

template <typename T, typename Comparator = std::less<T>>
void MergeSortRec(std::size_t left, std::size_t right, T* array,
    Comparator compare = Comparator()) {
  if (left < right) {
    std::size_t middle = (left + right) / 2; 
    MergeSortRec(left, middle, array, compare);
    MergeSortRec(middle + 1, right, array, compare);
    Merge(left, middle, right, array, compare);
  }
}

template <typename T, typename Comparator = std::less<T>>
void MergeSort(T* array, std::size_t size, Comparator compare = Comparator()) {
  MergeSortRec(0, size - 1, array, compare);
}

int main() {
  // ...

  return 0;
}
