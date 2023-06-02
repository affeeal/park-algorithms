#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

struct Node {
  std::string product_name;
  int color;
  int size;

  friend std::istream& operator>>(std::istream& in, Node& node);

  bool operator==(const Node& other) const {
    return product_name == other.product_name
           && color == other.color
           && size == other.size;
  }
};

std::istream& operator>>(std::istream& in, Node& node) {
  in >> node.product_name >> node.color >> node.size;
  return in;
}

template<>
struct std::hash<Node> {
  std::size_t operator()(const Node& node) const {
    return std::hash<string>()(node.product_name)
           ^ std::hash<int>()(node.color)
           ^ std::hash<int>()(node.size);
  }
};

int main() {
  std::unordered_set<Node> us;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::stringstream ss(line);
    char operation = 0;
    Node node;
    ss >> operation >> node;
    switch (operation) {
     case '+':
      if (us.find(node) == us.end()) {
        us.insert(node);
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "FAIL" << std::endl;
      }
      break;
     case '-':
      if (us.find(node) == us.end()) {
        std::cout << "FAIL" << std::endl;
      } else {
        us.erase(node);
        std::cout << "OK" << std::endl;
      }
      break;
     case '?':
      if (us.find(node) == us.end()) {
        std::cout << "FAIL" << std::endl;
      } else {
        std::cout << "OK" << std::endl;
      }
      break;
     default:
      return -1;
    }
  }
  return 0;
}
