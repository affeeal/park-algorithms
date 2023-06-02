#include <iostream>

class BinaryTree {
 public:
  BinaryTree() : root_(nullptr) {}
  
  ~BinaryTree() {
    DeleteRec(root_);
  }

  void Insert(int data) {
    InsertRec(root_, data);
  }

  bool CheckEqualityOfNodes() {
    if (!root_) {
      return true;
    }
    
    Node* node = root_;
    int expected_data = root_->data;
    while (node) {
      // значение левого ребёнка строго меньше значения текущего узла,
      // поэтому если такой ребёнок имеется, то мы возвращаем false.
      if (node->left) {
        return false;
      }
      if (node->data != expected_data) {
        return false;
      }
      node = node->right;
    }
    return true;
  }

 private:
  struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int data) : data(data), left(nullptr), right(nullptr) {}
  };

  void DeleteRec(Node* node) {
    if (!node) {
      return;
    }
    DeleteRec(node->left);
    DeleteRec(node->right);
    delete node;
  }

  void InsertRec(Node*& node, int data) {
    if (!node) {
      node = new Node(data);
      return;
    }
    if (data < node->data) {
      InsertRec(node->left, data);
    } else {
      InsertRec(node->right, data);
    }
  }
  
  Node* root_;
};

int main() {
  BinaryTree binary_tree;
  int data = 0;
  while (std::cin >> data) {
    binary_tree.Insert(data);
  }
  std::cout << binary_tree.CheckEqualityOfNodes() << std::endl;
  return 0;
}
