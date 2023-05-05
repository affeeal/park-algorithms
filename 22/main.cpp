#include <iostream>
#include <string>
#include <vector>

template <typename Comparator>
class BinaryTree {
 public:
  BinaryTree();
  ~BinaryTree();

  void InOrderTraverse();

 private:
  Comparator comparator_;
};

int main() {
  return 0;
}
