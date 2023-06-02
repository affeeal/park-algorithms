#include <cassert>
#include <iostream>

/*
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:
- приоритетом P;
- временем, которое он уже отработал t
- временем, которое необходимо для завершения работы процесса T

Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
выполняет его время P и кладет обратно в очередь процессов. Если выполняется
условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.

Требования:

- В качестве очереди с приоритетом нужно использовать кучу.
- Куча должна быть реализована в виде шаблонного класса.
- Решение должно поддерживать передачу функции сравнения снаружи.
- Куча должна быть динамической.

Формат ввода

Сначала вводится кол-во процессов. После этого процессы в формате P T

Формат вывода

Кол-во переключений процессора.
*/

constexpr int kInitialSize = 1;

template<typename T>
class DynamicArray {
public:
  DynamicArray();
  ~DynamicArray();

  int Size() const;
  bool Empty() const;
  T Front() const;
  T& Front();
  T Back() const;
  T& Back();
  void PushBack(T element);
  T PopBack();
  T& operator[](int index);
  T operator[](int index) const;
private:
  T* buffer_;
  int buffer_size_;
  int real_size_;

  void Grow();
};

template <typename T>
DynamicArray<T>::DynamicArray()
  : buffer_(nullptr), buffer_size_(0), real_size_(0) {}

template <typename T>
DynamicArray<T>::~DynamicArray() {
  delete[] buffer_;
}

template <typename T>
int DynamicArray<T>::Size() const {
  return real_size_;
}

template <typename T>
bool DynamicArray<T>::Empty() const {
  return (real_size_ == 0);
}

template <typename T>
T DynamicArray<T>::Front() const {
  assert(!Empty());
  return buffer_[0];
}

template <typename T>
T& DynamicArray<T>::Front() {
  assert(!Empty());
  return buffer_[0];
}

template <typename T>
T DynamicArray<T>::Back() const {
  assert(!Empty());
  return buffer_[real_size_ - 1];
}

template <typename T>
T& DynamicArray<T>::Back() {
  assert(!Empty());
  return buffer_[real_size_ - 1];
}

template <typename T>
void DynamicArray<T>::PushBack(T element) {
  if (real_size_ == buffer_size_)
    Grow();
  assert(real_size_ < buffer_size_);
  buffer_[real_size_++] = element;
}

template <typename T>
T DynamicArray<T>::PopBack() {
  assert(!Empty());
  auto element = buffer_[real_size_- 1];
  real_size_--;
  return element;
}

template<typename T>
T& DynamicArray<T>::operator[](int i) {
  assert(0 <= i && i < real_size_);
  return buffer_[i];
}

template<typename T>
T DynamicArray<T>::operator[](int i) const {
  assert(0 <= i && i < real_size_);
  return buffer_[i];
}

template<typename T>
void DynamicArray<T>::Grow() {
  T* new_buffer = new T[std::max(kInitialSize, buffer_size_ << 1)];
  for (auto i = 0; i < buffer_size_; i++)
    new_buffer[i] = buffer_[i];
  delete[] buffer_;
  buffer_ = new_buffer;
  buffer_size_ = std::max(kInitialSize, buffer_size_ << 1);
}

template<typename T, typename Comparator = std::less<T>>
class Heap {
public:
  bool Empty() const;
  void Insert(T element);
  T ExtractMin();
  T PeekMin() const;
private:
  DynamicArray<T> array;
  Comparator comparator;

  void SiftDown(int i);
  void SiftUp(int i);
};

template<typename T, typename Comparator>
bool Heap<T, Comparator>::Empty() const {
  return array.Empty();
}

template<typename T, typename Comparator>
void Heap<T, Comparator>::Insert(T element) {
  array.PushBack(element);
  SiftUp(array.Size() - 1);
}

template<typename T, typename Comparator>
T Heap<T, Comparator>::ExtractMin() {
  assert(!array.Empty());
  T min_element = array.Front();
  array.Front() = array.Back();
  array.PopBack();
  if (!array.Empty())
    SiftDown(0);
  return min_element;
}

template<typename T, typename Comparator>
T Heap<T, Comparator>::PeekMin() const {
  assert(!array.Empty());
  return array.Front();
}

// !
template <typename T, typename Comparator>
void Heap<T, Comparator>::SiftUp(int i) {
  assert(0 <= i && i < array.Size());
  int parent = (i - 1) / 2;
  while (parent >= 0 && comparator(array[i], array[parent])) {
    T saver = array[i];
    array[i] = array[parent];
    array[parent] = saver;

    i = parent;
    parent = (i - 1) / 2;
  }
}

// !
template <typename T, typename Comparator>
void Heap<T, Comparator>::SiftDown(int i) {
  assert(0 <= i && i < array.Size());
  while (true) {
    int left = 2 * i + 1;
    int right = left + 1;
    int j = i;
    if (left < array.Size() && comparator(array[left], array[i]))
      i = left;
    if (right < array.Size() && comparator(array[right], array[i]))
      i = right;
    if (i == j)
      break;
    T saver = array[i];
    array[i] = array[j];
    array[j] = saver;
  }
}

struct Process {
 public:
   int P;
   int t;
   int T;

   Process();
   Process(int P, int T);

   friend std::istream& operator>>(std::istream& in, Process& process);
   friend std::ostream& operator<<(std::ostream& out, const Process& process);

   friend bool operator<(const Process& process_1, const Process& process_2);
};

Process::Process()
  : P(0), t(0), T(0) {}

Process::Process(int P, int T)
  : P(P), t(0), T(T) {}

std::istream& operator>>(std::istream& in, Process& process) {
  in >> process.P >> process.T;
  process.t = 0;
  return in;
}

std::ostream& operator<<(std::ostream& out, const Process& process) {
  out << '(' << process.P << ", " << process.t << ", " << process.T << ")\n";
  return out;
}

class ProcessComparator {
 public:
   bool operator()(const Process& process_1, const Process& process_2) const;
};

bool operator<(const Process& process_1, const Process& process_2) {
 return ProcessComparator{}(process_1, process_2); 
}

bool ProcessComparator::operator()(const Process &process_1,
                                   const Process &process_2) const {
  return (process_1.P * (process_1.t + 1) < process_2.P * (process_2.t + 1));
}

int main() {
  int n { 0 };
  std::cin >> n;
  assert(n >= 0);

  Heap<Process, ProcessComparator> heap;
  // по идее, также должно работать следующее:
  // Heap<Process> heap;
  for (auto i = 0; i < n; i++) {
    Process process {};
    std::cin >> process;
    heap.Insert(process);
  }

  int processor_switches { 0 };
  while(!heap.Empty()) {
    auto process = heap.ExtractMin();
    process.t += process.P;
    if (process.t < process.T)
      heap.Insert(process);
    processor_switches++;
  }
  
  std::cout << processor_switches << std::endl;

  return 0;
}
