#include <functional>
#include <iostream>
#include <stack>

/*
2_1. Обход дерева в порядке in-order

Дано число N ≤ 10^4 и последовательность целых чисел из [-2^(31)..2^(31)]
длиной N. Требуется построить бинарное дерево, заданное наивным порядком
вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если
root->Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое
поддерево root. Выведите элементы в порядке in-order (слева направо).

Рекурсия запрещена.
*/

template <typename T, typename Comparator>
class BinaryTree {
  struct Node {
    T data;
    Node* left;
    Node* right;

    Node(T data) : data(data), left(nullptr), right(nullptr) {}
  };

 public:
  BinaryTree() : root_(nullptr), comparator_(Comparator()) {
  }
  
  ~BinaryTree() {
    if (!root_) {
      return;
    }

    std::stack<Node*> unvisited, visited;
    unvisited.push(root_);

    while (!unvisited.empty()) {
      Node* node = unvisited.top();
      unvisited.pop();
      visited.push(node);
      
      if (node->left) {
        unvisited.push(node->left);
      }
      if (node->right) {
        unvisited.push(node->right);
      }
    }

    while (!visited.empty()) {
      delete visited.top();
      visited.pop();
    }
  }

  void Insert(T data) {
    Node** node = &root_;
    while (*node) {
      if (comparator_(data, (*node)->data)) {
        node = &(*node)->left;
      } else {
        node = &(*node)->right;
      }
    }
    *node = new Node(data);
  }

  void InOrder() {
    std::stack<Node*> unvisited;
    Node* node = root_;

    while (true) {
      if (node) {
        unvisited.push(node);
        node = node->left;
      } else if (!unvisited.empty()) {
        node = unvisited.top();
        unvisited.pop();
        Visit(node);
        node = node->right;
      } else {
        break;
      }
    }
  }

 private:
  void Visit(Node* node) {
    std::cout << node->data << ' ';
  }
  
  Node* root_;
  Comparator comparator_;
};

int main() {
  int n = 0;
  std::cin >> n;

  BinaryTree<int, std::less<int>> binary_tree;
  for (auto i = 0; i < n; i++) {
    int data = 0;
    std::cin >> data;
    binary_tree.Insert(data);
  }

  binary_tree.InOrder();
  std::cout << std::endl;
}
