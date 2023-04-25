#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

/*
Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
вычисления значения многочлена методом Горнера. Начальный размер таблицы должен
быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна
поддерживать операции добавления строки в множество, удаления строки из
множества и проверки принадлежности данной строки множеству.

Вариант 2. Для разрешения коллизий используйте двойное хеширование.
*/

const std::size_t kHashtableInitialSize = 8;
const double kMaxFillFactor = 0.75;

template <std::size_t Factor>
class StringHasher {
 public:
  std::size_t operator()(const std::string& str) const;
};

template <std::size_t Factor>
std::size_t StringHasher<Factor>::operator()(const std::string& str) const {
  std::size_t hash = 0;
  for (auto ch : str) {
    hash = hash * Factor + std::size_t(ch);
  }
  return hash;
}

template <typename FirstHasher, typename SecondHasher>
class Hashtable {
 public:
  explicit Hashtable();

  bool Has(const std::string& key) const;
  bool Add(const std::string& key);
  bool Delete(const std::string& key);
 private:
  void Grow();
  
  enum class Status {
    kInitialized = 0,
    kNotInitialized,
    kDeleted,
  };

  struct Cell {
    Status status;
    std::string data;

    Cell() : status(Status::kNotInitialized) {}
  };

  std::vector<Cell> buffer_;
  std::size_t size_;
  FirstHasher first_hasher_;
  SecondHasher second_hasher_;
};

template <typename FirstHasher, typename SecondHasher>
Hashtable<FirstHasher, SecondHasher>::Hashtable()
  : buffer_(kHashtableInitialSize), size_(0),
    first_hasher_(FirstHasher()), second_hasher_(SecondHasher()) {
}

template <typename FirstHasher, typename SecondHasher>
bool Hashtable<FirstHasher, SecondHasher>::Has(const std::string& key) const {
  std::size_t first_hash = first_hasher_(key);
  std::size_t second_hash = 1 + (second_hasher_(key) << 1);

  for (std::size_t i = 0; i < buffer_.size(); i++) {
    std::size_t hash = (first_hash + i * second_hash) % buffer_.size();

    if (buffer_[hash].status == Status::kNotInitialized) {
      return false;
    } else if (buffer_[hash].status == Status::kInitialized) {
      if (buffer_[hash].data == key) {
        return true;
      }
    }
  }

  return false;
}

template <typename FirstHasher, typename SecondHasher>
bool Hashtable<FirstHasher, SecondHasher>::Add(const std::string& key) {
  if (static_cast<double>(size_) / static_cast<double>(buffer_.size())
      >= kMaxFillFactor) {
    Grow();
  }

  std::size_t first_hash = first_hasher_(key);
  std::size_t second_hash = 1 + (second_hasher_(key) << 1);

  std::size_t first_deleted = 0;
  bool is_first_deleted_set = false;

  for (std::size_t i = 0; i < buffer_.size(); i++) {
    std::size_t hash = (first_hash + i * second_hash) % buffer_.size();

    if (buffer_[hash].status == Status::kNotInitialized) {
      buffer_[hash].status = Status::kInitialized;
      buffer_[hash].data = key;
      size_++;
      return true;
    } else if (buffer_[hash].status == Status::kInitialized) {
      if (buffer_[hash].data == key) {
        return false;
      }
    } else if (buffer_[hash].status == Status::kDeleted) {
      if (!is_first_deleted_set) {
        first_deleted = hash;
        is_first_deleted_set = true;
      }
    }
  }
  
  buffer_[first_deleted].status = Status::kInitialized;
  buffer_[first_deleted].data = key;
  size_++;

  return true;
}

template <typename FirstHasher, typename SecondHasher>
bool Hashtable<FirstHasher, SecondHasher>::Delete(const std::string& key) {
  std::size_t first_hash = first_hasher_(key);
  std::size_t second_hash = 1 + (second_hasher_(key) << 1);

  for (std::size_t i = 0; i < buffer_.size(); i++) {
    std::size_t hash = (first_hash + i * second_hash) % buffer_.size();

    if (buffer_[hash].status == Status::kNotInitialized) {
      return false;
    } else if (buffer_[hash].status == Status::kInitialized) {
      if (buffer_[hash].data == key) {
        buffer_[hash].status = Status::kDeleted;
        size_--;
        return true;
      }
    }
  }

  return false;
}

template <typename FirstHasher, typename SecondHasher>
void Hashtable<FirstHasher, SecondHasher>::Grow() {
  std::vector<Cell> new_buffer(buffer_.size() << 1);

  for (std::size_t i = 0; i < buffer_.size(); i++) {
    if (buffer_[i].status == Status::kInitialized) {
      std::size_t first_hash = first_hasher_(buffer_[i].data);
      std::size_t second_hash = 1 + (second_hasher_(buffer_[i].data) << 1);

      for (std::size_t j = 0; j < new_buffer.size(); j++) {
        std::size_t hash = (first_hash + j * second_hash) % new_buffer.size();

        if (new_buffer[hash].status == Status::kNotInitialized) {
          new_buffer[hash].status = Status::kInitialized;
          new_buffer[hash].data = buffer_[i].data;
          break;
        }
      }
    }    
  }
  
  buffer_ = std::move(new_buffer);
}

const std::size_t kOperationPos = 0;
const std::size_t kOperandPos = 2;

enum Operation {
  kHas = '?',
  kAdd = '+',
  kDelete = '-',
};

void Run(std::istream& in, std::ostream& out) {
  Hashtable<StringHasher<71>, StringHasher<131>> hashtable;
  std::string line;
  while (std::getline(in, line)) {
    switch (line[kOperationPos]) {
     case Operation::kHas:
      out << (hashtable.Has(line.substr(kOperandPos)) ? "OK" : "FAIL")
          << std::endl;
      break;
     case Operation::kAdd:
      out << (hashtable.Add(line.substr(kOperandPos)) ? "OK" : "FAIL")
          << std::endl;
      break;
     case Operation::kDelete:
      out << (hashtable.Delete(line.substr(kOperandPos)) ? "OK" : "FAIL")
          << std::endl;
      break;
     default:
      return;
    }
  }
}

int main() {
  Run(std::cin, std::cout);
  return 0;
}
