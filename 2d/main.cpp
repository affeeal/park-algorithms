#include <iostream>
#include <sstream>
#include <utility>

class AvlTree {
  struct Node {
    int data;
    Node* left;
    Node* right;
    std::size_t height;

    Node(int data)
      : data(data), left(nullptr), right(nullptr), height(1) {}
  };
  
 public:
  AvlTree() : root_(nullptr) {}
  
  ~AvlTree() {
    DestroyTree(root_);
  }

  void Add(int data) {
    if (!Has(data)) {
      root_ = AddInternal(root_, data);
    }
  }

  bool Has(int data) {
    Node* tmp = root_;
    while (tmp) {
      if (tmp->data == data) {
        return true;
      }
      if (tmp->data < data) {
        tmp = tmp->right;
      } else {
        tmp = tmp->left;
      }
    }
    return false;
  }

  void Delete(int data) {
    if (Has(data)) {
      root_ = DeleteInternal(root_, data);
    }
  }

  std::pair<bool, int> Next(int data) {
    Node* tmp = root_;
    Node* last_upper = nullptr;
    while (tmp) {
      if (tmp->data < data) {
        tmp = tmp->right;
      } else if (tmp->data > data) {
        last_upper = tmp;
        tmp = tmp->left;
      } else {
        if (tmp->right) {
          Node* found = FindMin(tmp->right);
          if (found) {
            return std::make_pair(true, found->data);
          }
        }
        return std::make_pair(false, 0);
      }
    }
    if (last_upper) {
      return std::make_pair(true, last_upper->data);
    } else {
      return std::make_pair(false, 0);
    }
  }
  
  std::pair<bool, int> Prev(int data) {
    Node* tmp = root_;
    Node* last_lower = nullptr;
    while (tmp) {
      if (tmp->data < data) {
        last_lower = tmp;
        tmp = tmp->right;
      } else if (tmp->data > data) {
        tmp = tmp->left;
      } else {
        if (tmp->left) {
          Node* found = FindMax(tmp->left);
          if (found) {
            return std::make_pair(true, found->data);
          }
        }
        return std::make_pair(false, 0);
      }
    }
    if (last_lower) {
      return std::make_pair(true, last_lower->data);
    } else {
      return std::make_pair(false, 0);
    }
  }
 private:
  void DestroyTree(Node* node) {
    if (!node) {
      return;
    }
    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node;
  }

  Node* AddInternal(Node* node, int data) {
    if (!node) {
      return new Node(data);
    }
    if (node->data <= data) {
      node->right = AddInternal(node->right, data);
    } else {
      node->left = AddInternal(node->left, data);
    }
    return DoBalance(node);
  }

  Node* DoBalance(Node* node) {
    FixHeight(node);
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

  int GetBalance(Node* node) {
    return GetHeight(node->right) - GetHeight(node->left);
  }

  Node* RotateLeft(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    FixHeight(node);
    FixHeight(tmp);
    return tmp;
  }
  
  Node* RotateRight(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    FixHeight(node);
    FixHeight(tmp);
    return tmp;
  }

  Node* DeleteInternal(Node* node, int data) {
    if (!node) {
      return node;
    }
    
    if (node->data < data) {
      node->right = DeleteInternal(node->right, data);
    } else if (node->data > data) {
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

  Node* Find(int data) {
    Node* tmp = root_;
    while (tmp) {
      if (tmp->data == data) {
        return tmp;
      }
      if (tmp->data < data) {
        tmp = tmp->right;
      } else {
        tmp = tmp->left;
      }
    }
    return nullptr;
  }
  
  Node* FindMax(Node* node) {
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  Node* root_;
};

int main() {
  AvlTree tree;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::stringstream ss(line);
    std::string operation = "";
    int data = 0;
    ss >> operation >> data;

    if (operation == "insert") {
      tree.Add(data);
    } else if (operation == "delete") {
      tree.Delete(data);
    } else if (operation == "exists") {
      if (tree.Has(data)) {
        std::cout << "true" << std::endl;
      } else {
        std::cout << "false" << std::endl;
      }
    } else if (operation == "next") {
      auto [is_found, value] = tree.Next(data);
      if (is_found) {
        std::cout << value << std::endl;
      } else {
        std::cout << "none" << std::endl;
      }
    } else if (operation == "prev") {
      auto [is_found, value] = tree.Prev(data);
      if (is_found) {
        std::cout << value << std::endl;
      } else {
        std::cout << "none" << std::endl;
      }
    } else {
      return -1;
    }
  }
  return 0;
}
