#include <iostream>
#include <queue>
#include <utility>

class BinaryTree {
 public:
  BinaryTree() : root_(nullptr) {}
  
  ~BinaryTree() {
    DeleteRec(root_);
  }

  void Insert(int data) {
    InsertRec(root_, data);
  }
  
  int MinHeight() {
    if (!root_) {
      return 0;
    }
    
    // пара узел-высота
    std::queue<std::pair<Node*, int>> q;
    q.push(std::make_pair(root_, 1));
    
    while (!q.empty()) {
      auto [node, height] = q.front();
      q.pop();
      
      bool has_children = false;
      if (node->left) {
        has_children = true;
        q.push(std::make_pair(node->left, height + 1));
      }
      if (node->right) {
        has_children = true;
        q.push(std::make_pair(node->right, height + 1));
      }
      if (!has_children) {
        return height;
      }
    }
    
    // не должно произойти:
    return 0;
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
    if (node->data > data) {
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
  std::cout << binary_tree.MinHeight() << std::endl;
  return 0;
}
