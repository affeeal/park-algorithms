#include <cassert>
#include <iostream>

/*
Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с
помощью динамического буфера. Обрабатывать команды push back и pop front.

Формат ввода

В первой строке количество команд n. n ≤ 1000000. Каждая команда задаётся как
2 целых числа: a b.

a = 2 - pop front
a = 3 - push back

Если дана команда pop front, то число b - ожидаемое значение. Если команда pop
front вызвана для пустой структуры данных, то ожидается “-1”.

Формат вывода

Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если
хотя бы одно ожидание не оправдалось, то напечатать NO. 
*/

enum class Command {
  kPopFront = 2,
  kPushBack
};

constexpr int kInitialStackSize = 1;

class Stack {
public:
  explicit Stack();
  ~Stack();

  void Push(int value);
  int Pop();
  
  bool IsEmpty() const;
private:
  int* buffer_;
  int size_;
  int top_;

  void ExpandBuffer();
};

Stack::Stack()
  : top_(-1), size_(kInitialStackSize) {
  buffer_ = new int[kInitialStackSize];
}

Stack::~Stack() {
  delete[] buffer_;
}

int Stack::Pop() {
  assert(!IsEmpty());

  return buffer_[top_--];
}

void Stack::Push(int value) {
  if (top_ + 1 == size_)
    ExpandBuffer();

  buffer_[++top_] = value;
}

bool Stack::IsEmpty() const {
  return (top_ == -1);
}

void Stack::ExpandBuffer() {
  int* new_buffer = new int[size_ << 1];
  for (auto i = 0; i < size_; i++)
    new_buffer[i] = buffer_[i];
  
  delete[] buffer_;
  buffer_ = new_buffer;
  size_ <<= 1;
}

class Queue {
public:
  int PopFront();
  void PushBack(int value);

  bool IsEmpty() const;
private:
  Stack push_stack_;
  Stack pop_stack_;
};

int Queue::PopFront() {
  if (pop_stack_.IsEmpty())
    while (!push_stack_.IsEmpty())
      pop_stack_.Push(push_stack_.Pop());
  
  assert(!pop_stack_.IsEmpty());
  return pop_stack_.Pop();
}

void Queue::PushBack(int value) {
  push_stack_.Push(value);
}

bool Queue::IsEmpty() const {
  return (push_stack_.IsEmpty() && pop_stack_.IsEmpty());
}
    
int main() {
  int n = 0;
  std::cin >> n;

  Queue queue;
  for (auto i = 0; i < n; i++) {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    
    switch (a) {
    case static_cast<int>(Command::kPopFront):
      if (queue.IsEmpty() && b != -1 ||
          !queue.IsEmpty() && queue.PopFront() != b) {
        std::cout << "NO" << std::endl;
        return 0;
      }
      break;
    case static_cast<int>(Command::kPushBack):
      queue.PushBack(b);
      break;
    }
  }
  
  std::cout << "YES" << std::endl;
  
  return 0;
}
