#include <iostream>
#include <string>
#include <sstream>
using namespace std;
template <class T>
class SplayTree
{
public:
    class Node;

private:
    Node *root;

protected:
    string inOrderRec(Node *root)
    {
        stringstream ss;
        if (root != nullptr)
        {
            ss << inOrderRec(root->pLeft);
            ss << root->data << " ";
            ss << inOrderRec(root->pRight);
        }
        return ss.str();
    }
    string preOrderRec(Node *root)
    {
        stringstream ss;
        if (root != nullptr)
        {
            ss << root->data << " ";
            ss << preOrderRec(root->pLeft);
            ss << preOrderRec(root->pRight);
        }
        return ss.str();
    }
    string postOrderRec(Node *root)
    {
        stringstream ss;
        if (root != nullptr)
        {
            ss << postOrderRec(root->pLeft);
            ss << postOrderRec(root->pRight);
            ss << root->data << " ";
        }
        return ss.str();
    }
    int Height(Node *root)
    {
        if (root == nullptr)
            return 0;
        else
        {
            int pLeft = 0;
            int pRight = 0;
            pLeft = 1 + Height(root->pLeft);
            pRight = 1 + Height(root->pRight);
            return pLeft < pRight ? pRight : pLeft;
        }
    }
    Node *Right_Rotation(Node *_root)
    {
        Node *p = _root->pParent;
        Node *temp = _root->pLeft;
        _root->pLeft = temp->pRight;
        if (temp->pRight != nullptr)
            temp->pRight->pParent = _root;
        temp->pRight = _root;
        _root->pParent = temp;
        temp->pParent = p;
        if (p != nullptr)
        {
            if (p->pLeft == _root)
                p->pLeft = temp;
            else
            {
                p->pRight = temp;
            }
        }
        return temp;
    }
    Node *Left_Rotation(Node *_root)
    {
        Node *p = _root->pParent;
        Node *temp = _root->pRight;
        _root->pRight = temp->pLeft;
        if (temp->pLeft != nullptr)
            temp->pLeft->pParent = _root;
        temp->pLeft = _root;
        _root->pParent = temp;
        temp->pParent = p;
        if (p != nullptr)
        {
            if (p->pLeft == _root)
                p->pLeft = temp;
            else
            {
                p->pRight = temp;
            }
        }
        return temp;
    }
    void Self_Splay3(Node *&root, Node *&new_node)
    {
        while (new_node->pParent != nullptr)
        {
            if (new_node->pParent == root)
            {
                if (new_node == new_node->pParent->pLeft)
                {
                    root = Right_Rotation(new_node->pParent);
                }
                else
                {
                    root = Left_Rotation(new_node->pParent);
                }
            }
            else
            {
                Node *p = new_node->pParent;
                Node *g = p->pParent;
                if (new_node == new_node->pParent->pLeft && p == p->pParent->pLeft)
                {
                    g = Right_Rotation(g);
                    p = Right_Rotation(p);
                    if (new_node->pParent == nullptr)
                        root = p;
                }
                else if (new_node == new_node->pParent->pRight && p == p->pParent->pRight)
                {
                    g = Left_Rotation(g);
                    p = Left_Rotation(p);
                    if (new_node->pParent == nullptr)
                        root = p;
                }
                else if (new_node == new_node->pParent->pLeft && p == p->pParent->pRight)
                {
                    p = Right_Rotation(p);
                    g = Left_Rotation(g);
                    if (new_node->pParent == nullptr)
                        root = g;
                }
                else
                {
                    p = Left_Rotation(p);
                    g = Right_Rotation(g);
                    if (new_node->pParent == nullptr)
                        root = g;
                }
            }
        }
    }
    void Self_Splay4(Node *&root, Node *new_node)
    {
        while (new_node->pParent != nullptr)
        {
            if (new_node->pParent == root)
            {
                if (new_node == new_node->pParent->pLeft)
                {
                    root = Right_Rotation(new_node->pParent);
                }
                else
                {
                    root = Left_Rotation(new_node->pParent);
                }
            }
            else
            {
                Node *p = new_node->pParent;
                Node *g = p->pParent;
                if (new_node == new_node->pParent->pLeft && p == p->pParent->pLeft)
                {
                    g = Right_Rotation(g);
                    p = Right_Rotation(p);
                    if (new_node->pParent == nullptr)
                        root = p;
                }
                else if (new_node == new_node->pParent->pRight && p == p->pParent->pRight)
                {
                    g = Left_Rotation(g);
                    p = Left_Rotation(p);
                    if (new_node->pParent == nullptr)
                        root = p;
                }
                else if (new_node == new_node->pParent->pLeft && p == p->pParent->pRight)
                {
                    p = Right_Rotation(p);
                    g = Left_Rotation(g);
                    if (new_node->pParent == nullptr)
                        root = g;
                }
                else
                {
                    p = Left_Rotation(p);
                    g = Right_Rotation(g);
                    if (new_node->pParent == nullptr)
                        root = g;
                }
            }
        }
        }

public:
    SplayTree() : root(nullptr) {}
    ~SplayTree()
    {
        clear();
    }
    void clear()
    {
        clear(this->root);
    }
    void clear(Node *&root)
    {
        if (root)
        {
            clear(root->pLeft);
            clear(root->pRight);
            delete root;
        }
    }
    void print2()
    {
        prettyPrintTree(root);
    }
    void prettyPrintTree(Node *node, string prefix = "", bool isLeft = true)
    {
        if (node == nullptr)
        {
            cout << "Empty tree";
            return;
        }

        if (node->pRight)
        {
            prettyPrintTree(node->pRight, prefix + (isLeft ? "│   " : "    "), false);
        }

        cout << prefix + (isLeft ? "└── " : "┌── ") + to_string(node->data) + "\n";

        if (node->pLeft)
        {
            prettyPrintTree(node->pLeft, prefix + (isLeft ? "    " : "│   "), true);
        }
    }
    int getHeight()
    {
        return this->Height(this->root);
    }
    class Node
    {
    private:
        T data;
        Node *pLeft, *pRight, *pParent;
        friend class SplayTree<T>;

    public:
        Node(T value) : data(value), pLeft(nullptr), pRight(nullptr), pParent(nullptr) {}
        ~Node() {}
    };
    void add(const T &);
    void add(Node *&, Node *);
    string inOrder()
    {
        return inOrderRec(this->root);
    }
    string preOrder()
    {
        return preOrderRec(this->root);
    }
    string postOrder()
    {
        return postOrderRec(this->root);
    }
    T search(const T &key)
    {
        return search(this->root, key);
    }
    T search(Node *node, const T &key)
    {
        if (node == nullptr)
        {
            throw("Not Found");
        }
        else
        {
            if (key == node->data)
            {
                Node *temp = node;
                Self_Splay4(this->root, node);
                return key;
            }
            else if (key > node->data)
                return search(node->pRight, key);
            else
            {
                return search(node->pLeft, key);
            }
        }
    }
};
template <class T>
void SplayTree<T>::add(const T &new_value)
{
    Node *new_node = new Node(new_value);
    add(root, new_node);
}
template <class T>
void SplayTree<T>::add(Node *&root, Node *new_node)
{
    Node *temp = root;
    Node *prev = nullptr;
    while (temp != nullptr)
    {
        prev = temp;
        if (new_node->data < temp->data)
            temp = temp->pLeft;
        else
            temp = temp->pRight;
    }
    if (prev == nullptr)
    {
        root = new_node;
        root->pParent = nullptr;
    }
    else
    {
        if (new_node->data < prev->data)
        {
            prev->pLeft = new_node;
            new_node->pParent = prev;
        }
        else
        {
            prev->pRight = new_node;
            new_node->pParent = prev;
        }
    }
    //Self_Splay2(root, new_node->data);
    Self_Splay3(root, new_node);
}

int main()
{
    SplayTree<int> test_tree;
    test_tree.add(383);
    test_tree.add(886);
    test_tree.add(100);
    test_tree.add(915);
    test_tree.add(793);
    //test_tree.add(100);
    test_tree.print2();
    cout << test_tree.search(383) << endl;
    test_tree.print2();
    system("pause");
    return 0;
}