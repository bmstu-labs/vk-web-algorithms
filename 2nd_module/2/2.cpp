
// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

// Выведите элементы в порядке pre-order (сверху вниз).

#include <vector>
#include <stack>
#include <iostream>
#include <functional>

template<class TYPE>
struct Node {
public:
    TYPE value;
    Node *left;
    Node *right;
public:
    Node(
        TYPE val,
        Node *leftNode = nullptr,
        Node *rightNode = nullptr
    ) : value(val), left(leftNode), right(rightNode) {}
};

template<class TYPE, class COMPARATOR>
class BinaryTree {
private:
    Node<TYPE> *root;
    COMPARATOR compare;
    std::size_t size;
public:
    BinaryTree() : root(nullptr), size(0), compare() {}
public:
    void insert(TYPE value) {
        if (root == nullptr) {
            root = new Node<TYPE>(value);
            size++;
            return;
        }

        Node<TYPE> *current = root;
        Node<TYPE> *currentParent = root;
        while (current != nullptr) {
            currentParent = current;

            if (compare(value, current->value)) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        auto n = new Node<TYPE>(value);
        if (compare(value, currentParent->value)) {
            currentParent->left = n;
        }
        else {
            currentParent->right = n;
        }
        size++;
        n = nullptr;
    }

    std::vector<TYPE> toArray() const noexcept {
        std::vector<TYPE> resultArray;
        if (root == nullptr) {
            return resultArray;
        }

        std::stack<Node<TYPE> *> stack;
        stack.push(root);
        
        while (stack.empty() == false) {
            Node<TYPE> *n = stack.top();
            stack.pop();

            resultArray.push_back(n->value);

            if (n->right) {
                stack.push(n->right);
            }
            if (n->left) {
                stack.push(n->left);
            }
        }

        return resultArray;
    }
};

int main() {
    BinaryTree<int, std::less<int>> btree;
    
    std::size_t s;
    std::cin >> s;

    while (s--) {
        int value;
        std::cin >> value;
        btree.insert(value);
    }

    auto arr = btree.toArray();
    for (auto &el : arr) {
        std::cout << el << " ";
    }

    return 0;
}