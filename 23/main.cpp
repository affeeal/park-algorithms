/*
Постройте B-дерево минимального порядка t и выведите его по слоям. В качестве
ключа используются числа, лежащие в диапазоне [0..2^(32)-1]

Требования:
- B-дерево должно быть реализовано в виде шаблонного класса.
- Решение должно поддерживать передачу функции сравнения снаружи.

Формат ввода

Сначала вводится минимальный порядок дерева t. Затем вводятся элементы дерева.

Формат вывода

Программа должна вывести B-дерево по слоям. Каждый слой на новой строке,
элементы должны выводится в том порядке, в котором они лежат в узлах. 
*/

#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template <typename T, typename Comparator>
class BTree {
 public:
  BTree(std::size_t t) : root_(nullptr), t_(t), comparator_(Comparator()) {
  }
  
  ~BTree() {
    delete root_;
  }

  void Insert(const T& key) {
    if (!root_) {
      root_ = new Node(true);
    }

    if (IsNodeFull(root_)) {
      Node* new_root = new Node(false);
      new_root->children.push_back(root_);
      root_ = new_root;
      SplitChild(root_, 0);
    }
    
    InsertNonFull(root_, key);
  }

  void Print() {
    if (!root_) {
      return;
    }

    // пара узел-высота
    std::queue<std::pair<Node*, std::size_t>> to_print;
    to_print.push(std::make_pair(root_, 1));
    while (!to_print.empty()) {
      auto [node, height] = to_print.front();
      to_print.pop();
      for (const auto& key : node->keys) {
        std::cout << key << ' ';
      }
      if (to_print.empty() || to_print.front().second != height) {
        std::cout << std::endl;
      }
      for (auto child : node->children) {
        to_print.push(std::make_pair(child, height + 1));
      }
    }
  }
  
 private:
  struct Node {
    std::vector<T> keys;
    std::vector<Node*> children;
    bool is_leaf;

    Node (bool is_leaf) : is_leaf(is_leaf) {
    }

    ~Node() {
      for (Node* child : children) {
        delete child;
      }
    }
  };

  bool IsNodeFull(Node* node) {
    return node->keys.size() == 2 * t_ - 1;
  }

  void SplitChild(Node* node, std::size_t index) {
    Node* full_child = node->children[index];
    Node* new_child = new Node(full_child->is_leaf);

    new_child->keys.resize(t_ - 1);
    for (std::size_t i = 0; i < t_ - 1; i++) {
      new_child->keys[i] = full_child->keys[t_ + i];
    }
    T median_key = full_child->keys[t_ - 1];
    full_child->keys.resize(t_ - 1);

    if (!full_child->is_leaf) {
      new_child->children.resize(t_);
      for (std::size_t i = 0; i < t_; i++) {
        new_child->children[i] = full_child->children[t_ + i];
      }
      full_child->children.resize(t_);
    }

    node->children.resize(node->children.size() + 1);
    for (auto i = node->children.size() - 1; i > index + 1; i--) {
      node->children[i] = node->children[i - 1];
    }
    node->children[index + 1] = new_child;

    node->keys.resize(node->keys.size() + 1);
    for (auto i = node->keys.size() - 1; i > index; i--) {
      node->keys[i] = node->keys[i - 1];
    }
    node->keys[index] = median_key;
  }

  void InsertNonFull(Node* node, const T& key) {
    int pos = node->keys.size() - 1;

    if (node->is_leaf) {
      node->keys.resize(node->keys.size() + 1);
      while (pos >= 0 && comparator_(key, node->keys[pos])) {
        node->keys[pos + 1] = node->keys[pos];
        pos--;
      }
      node->keys[pos + 1] = key;
    } else {
      while (pos >= 0 && comparator_(key, node->keys[pos])) {
        pos--;
      }
      if (IsNodeFull(node->children[pos + 1])) {
        SplitChild(node, pos + 1);
        if (comparator_(node->keys[pos + 1], key)) {
          pos++;
        }
      }
      InsertNonFull(node->children[pos + 1], key);
    }
  }

  Node* root_;
  std::size_t t_;
  Comparator comparator_;
};

int main() {
  std::size_t t = 0;
  std::cin >> t;

  BTree<int, std::less<int>> tree(t);

  int key = 0;
  while (std::cin >> key) {
    tree.Insert(key);
  }

  tree.Print();
  
  return 0;
}
