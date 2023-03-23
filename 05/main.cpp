#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

constexpr int kDynamicArrayInitialSize = 1;

template<typename T>
class DynamicArray {
public:
  DynamicArray();
  DynamicArray(const DynamicArray& other);
  DynamicArray& operator=(const DynamicArray& other);
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
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
  buffer_size_ = other.buffer_size_;
  real_size_ = other.real_size_;
  buffer_ = new T[buffer_size_];
  std::memcpy(buffer_, other.buffer_, real_size_ * sizeof(T));
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T> &other) {
  if (this == other)
    return *this;

  buffer_size_ = other.buffer_size_;
  real_size_ = other.real_size_;
  T* new_buffer = new T[other.buffer_size_];
  std::memcpy(new_buffer, other.buffer_, real_size_ * sizeof(T));
  delete[] buffer_;
  buffer_ = new_buffer;
  return *this;
}

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
  T* new_buffer = new T[std::max(kDynamicArrayInitialSize, buffer_size_ << 1)];
  for (auto i = 0; i < buffer_size_; i++)
    new_buffer[i] = buffer_[i];
  delete[] buffer_;
  buffer_ = new_buffer;
  buffer_size_ = std::max(kDynamicArrayInitialSize, buffer_size_ << 1);
}

template <typename T,
          typename Comparator = std::less<T>>
void Merge(
    int left,
    int middle,
    int right,
    DynamicArray<T>& dynamic_array,
    Comparator compare = Comparator()) {
  assert(0 <= left && left <= middle && middle < right);

  auto temp_array_size = right - left + 1;
  T* temp_array = new T[temp_array_size];

#ifdef DEBUG
  std::cout << "before: ";
  for (auto i = 0; i < dynamic_array.Size(); i++)
    std::cout << dynamic_array[i] << ' ';
  std::cout << std::endl;
#endif

  int i = left;
  int j = middle + 1;

  for (int k = 0; k < temp_array_size; k++) {
    if (j <= right
        && (i == middle + 1 || compare(dynamic_array[j], dynamic_array[i])))
      temp_array[k] = dynamic_array[j++];
    else
      temp_array[k] = dynamic_array[i++];
  }

  std::memcpy(&dynamic_array[left], temp_array, temp_array_size * sizeof(T));
  delete[] temp_array;

#ifdef DEBUG
  std::cout << "after:  ";
  for (auto i = 0; i < dynamic_array.Size(); i++)
    std::cout << dynamic_array[i] << ' ';
  std::cout << std::endl << std::endl;
#endif
}

template <typename T,
          typename Comparator = std::less<T>>
void MergeSortRec(
    int left,
    int right,
    DynamicArray<T>& dynamic_array,
    Comparator compare = Comparator()) {
  assert(left >= 0 && right >= 0);

  if (left < right) {
    int middle = (left + right) / 2; 
    MergeSortRec(left, middle, dynamic_array, compare);
    MergeSortRec(middle + 1, right, dynamic_array, compare);
    Merge(left, middle, right, dynamic_array, compare);
  }
}

template <typename T,
          typename Comparator = std::less<T>>
void MergeSort(
    DynamicArray<T>& dynamic_array,
    Comparator compare = Comparator()) {
  MergeSortRec(0, dynamic_array.Size() - 1, dynamic_array, compare);
}


struct Date {
 public:
  int day;
  int month;
  int year;

  Date();
  explicit Date(int day, int month, int year);

  friend std::istream& operator>>(std::istream& in, Date& date);
  friend std::ostream& operator<<(std::ostream& out, const Date& date);

  friend bool operator<(const Date& date_1, const Date& date_2);
};

Date::Date()
  : day(0), month(0), year(0) {}

Date::Date(int day, int month, int year)
  : day(day), month(month), year(year) {}
  
std::istream& operator>>(std::istream& in, Date& date) {
  in >> date.day >> date.month >> date.year;
  return in;
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
  out << '(' << date.day << ' ' << date.month << ' '<< date.year << ')';
  return out;
}

bool operator<(const Date& date_1, const Date& date_2) {
#ifdef DEBUG
  std::cout << "comparing: " << date_1 << ' ' << date_2 << std::endl;
#endif

  if (date_1.year < date_2.year) {
    return true;
  } else if (date_1.year == date_2.year) {
    if (date_1.month < date_2.month) {
      return true;
    } else if (date_1.month == date_2.month) {
      if (date_1.day < date_2.day)
        return true;
    }
  }

  return false;
}

bool CheckForLegalAge(const Date& birth, const Date& death) {
  return death.year - birth.year >= 18
    && death.month - birth.month >= 0
    && death.day - birth.day > 0;
}


struct Meeting {
 public:
  Date date;
  int delta;

  Meeting();
  explicit Meeting(Date date, int delta);

  friend std::ostream& operator<<(std::ostream& out, const Meeting& meeting);

  friend bool operator<(const Meeting& meeting_1, const Meeting& meeting_2);
};

Meeting::Meeting()
  : date(Date()), delta(0) {} 

Meeting::Meeting(Date date, int delta)
  : date(date), delta(delta) {} 

std::ostream& operator<<(std::ostream& out, const Meeting& meeting) {
  out << '[' << meeting.date << ' ' << meeting.delta << ']';
  return out;
}

bool operator<(const Meeting& meeting_1, const Meeting& meeting_2) {
  return meeting_1.date < meeting_2.date;
}

int main() {
  int n { 0 };
  std::cin >> n;
  assert(n >= 0);

  DynamicArray<Meeting> meeting_bounds { };
  for (auto i = 0; i < n; i++) {
    Date birth { };
    Date death { };
    std::cin >> birth >> death;

    if (bool could_participate_in_the_meeting = CheckForLegalAge(birth, death);
        could_participate_in_the_meeting == true) {
    }

    meeting_bounds.PushBack(Meeting(
        Date(birth.day, birth.month, birth.year + 18),
        1));

    meeting_bounds.PushBack(Meeting(
        std::min(death, Date(birth.day, birth.month, birth.year + 80)),
        -1));
  }

  MergeSort<Meeting>(meeting_bounds);

  int max_overlap { 0 };
  int current_overlap { 0 };
  for (auto i = 0; i < meeting_bounds.Size(); i++) {
    current_overlap += meeting_bounds[i].delta;
    max_overlap = std::max(max_overlap, current_overlap);
  }

  std::cout << max_overlap << std::endl;

  return 0;
}
