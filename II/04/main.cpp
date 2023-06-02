/*
В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была
далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе
приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в
процессе прихода и ухода должны были всегда быть выстроены по росту – сначала
самые высокие, а в конце – самые низкие. За расстановку солдат отвечал
прапорщик, который заметил интересную особенность – все солдаты в части разного
роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять
солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом
в строе он должен становится. Требуемая скорость выполнения команды - O(log n). 

Формат ввода

Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000). В каждой
следующей строке содержится описание команды: число 1 и X если солдат приходит
в строй (X – рост солдата, натуральное число до 100 000 включительно) и число
2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в
строе нумеруются с нуля.

Формат вывода

На каждую команду 1 (добавление в строй) вы должны выводить число K – номер
позиции, на которую должен встать этот солдат (все стоящие за ним двигаются
назад).
*/

#include <functional>
#include <iostream>
#include <optional>

// #define DEBUG

#ifdef DEBUG
#include <cassert>
#endif

template <typename T, typename Comparator>
class AvlTree {
 public:
  AvlTree() : root_(nullptr), comparator_(Comparator()) {
  }
  
  ~AvlTree() {
    DestroyTree(root_);
  }

  void Add(const T& data) {
#ifdef DEBUG
    assert(!Has(data));
#endif
    root_ = AddInternal(root_, data);
  }

  // возвращает порядок k переданной статистики
  std::size_t Order(const T& data) {
    Node* node = root_;
    std::size_t upper = GetCount(root_) - 1;
    std::size_t lower = 0;
    while (node) {
      if (comparator_(node->data, data)) {
        lower += GetCount(node->left) + 1;
        node = node->right;
      } else if (comparator_(data, node->data)) {
        upper -= GetCount(node->right) + 1;
        node = node->left;
      } else {
#ifdef DEBUG
        assert(upper >= GetCount(node->right));
#endif
        return upper - GetCount(node->right);
        // или, что то же самое,
        // return lower + GetCount(node->left);
      }
    }
    return std::string::npos;
  }

  // возвращает k-ю порядковую статистику
  std::optional<T> Statistics(std::size_t k) {
    Node* node = root_;
    std::size_t tmp = k;
    while (node) {
      if (tmp < GetCount(node->left)) {
        node = node->left;
      } else if (tmp > GetCount(node->left)) {
#ifdef DEBUG
        assert(tmp >= GetCount(node->left) + 1);
#endif 
        tmp -= GetCount(node->left) + 1;
        node = node->right;
      } else {
        return node->data;
      }
    }
    return std::nullopt;
  }

  bool Has(const T& data) {
    Node* node = root_;
    while (node) {
      if (node->data == data) {
        return true;
      }
      if (comparator_(node->data, data)) {
        node = node->right;
      } else {
        node = node->left;
      }
    }
    return false;
  }

  void Delete(const T& data) {
#ifdef DEBUG
    assert(Has(data));
#endif
    root_ = DeleteInternal(root_, data);
  }

 private:
  struct Node {
    T data;
    Node* left;
    Node* right;
    std::size_t height;
    std::size_t count;

    Node(const T& data)
      : data(data), left(nullptr), right(nullptr), height(1), count(1) {
    }
  };

  void DestroyTree(Node* node) {
    if (!node) {
      return;
    }

    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node;
  }

  Node* AddInternal(Node* node, const T& data) {
    if (!node) {
      return new Node(data);
    }

    if (comparator_(data, node->data)) {
      node->left = AddInternal(node->left, data);
    } else {
      node->right = AddInternal(node->right, data);
    }

    return DoBalance(node);
  }

  Node* DoBalance(Node* node) {
    FixHeight(node);
    FixCount(node);
    switch(GetBalance(node)) {
     case 2:
      if (GetBalance(node->right) < 0) {
        node->right = RotateRight(node->right);
      }
      return RotateLeft(node);
     case -2:
      if (GetBalance(node->left) > 0) {
        node->left = RotateLeft(node->left);
      }
      return RotateRight(node);
     default:
      return node;
    }
  }

  void FixHeight(Node* node) {
    node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
  }

  std::size_t GetHeight(Node* node) {
    return (node ? node->height : 0);
  }

  void FixCount(Node* node) {
    node->count = 1 + GetCount(node->left) + GetCount(node->right);
  }

  std::size_t GetCount(Node* node) {
    return (node ? node->count : 0);
  }

  int GetBalance(Node* node) {
    return static_cast<int>(GetHeight(node->right) - GetHeight(node->left));
  }

  Node* RotateLeft(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    FixHeight(node);
    FixCount(node);
    FixHeight(tmp);
    FixCount(tmp);
    return tmp;
  }
  
  Node* RotateRight(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    FixHeight(node);
    FixCount(node);
    FixHeight(tmp);
    FixCount(tmp);
    return tmp;
  }

  Node* DeleteInternal(Node* node, const T& data) {
    if (!node) {
      return node;
    }
    
    if (comparator_(node->data, data)) {
      node->right = DeleteInternal(node->right, data);
    } else if (comparator_(data, node->data)) {
      node->left = DeleteInternal(node->left, data);
    } else {
      Node* left = node->left;
      Node* right = node->right;

      delete node;

      if (!right) {
        return left;
      }

      Node* min = FindMin(right);
      min->right = RemoveMin(right);
      min->left = left;

      return DoBalance(min);
    }

    return DoBalance(node);
  }

  Node* FindMin(Node* node) {
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  Node* RemoveMin(Node* node) {
    if (!node->left) {
      return node->right;
    }
    node->left = RemoveMin(node->left);
    return DoBalance(node);
  }
  
  Node* root_;
  Comparator comparator_;
};

int main() {
  std::size_t n = 0;
  std::cin >> n;
  AvlTree<std::size_t, std::greater<std::size_t>> tree;
  for (std::size_t i = 0; i < n; i++) {
    std::size_t operation = 0;
    std::size_t data = 0;
    std::cin >> operation >> data;
    switch (operation) {
      case 1: {
        tree.Add(data);
        std::cout << tree.Order(data) << std::endl;;
        break;
      }
      case 2: {
        std::optional<std::size_t> s = tree.Statistics(data);
#ifdef DEBUG
        assert(s.has_value());
#endif
        tree.Delete(s.value());
        break;
      }
      default: {
        std::cerr << "undefined operation " << operation << std::endl;
        return -1;
      }
    }
  }
  return 0;
}
