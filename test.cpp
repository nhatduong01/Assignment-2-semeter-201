#include <iostream>
#include <string>
#include <sstream>
#include <queue>
using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"
template <class T>
class BinarySearchTree
{
public:
    class Node;

private:
    Node *root;

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree()
    {
        // You have to delete all Nodes in BinaryTree. However in this task, you can ignore it.
        clear();
    }

    //Helping function

    void add(T value)
    {
        /*if (!root)
        {
            root = new Node(value);
            return;
        }
        Node *temp = root;
        Node *prev = nullptr;
        while (temp)
        {
            if (value > temp->value)
            {
                prev = temp;
                temp = temp->pRight;
            }
            else
            {
                prev = temp;
                temp = temp->pLeft;
            }
        }
        temp = new Node(value);
        if (value > prev->value)
            prev->pRight = temp;
        else
            prev->pLeft = temp;*/
        Node *new_node = new Node(value);
        add(this->root, new_node);
    }
    void add(Node *&root, Node *new_node)
    {
        Node *temp = root;
        Node *prev = NULL;
        while (temp != nullptr)
        {
            prev = temp;
            if (new_node->value < temp->value)
                temp = temp->pLeft;
            else
                temp = temp->pRight;
        }
        if (prev == NULL)
            root = new_node;
        else
        {
            if (new_node->value < prev->value)
                prev->pLeft = new_node;
            else
                prev->pRight = new_node;
        }
    }
    void deleteNode(T value)
    {
        deleteNode(this->root, value);
    }
    void deleteNode(Node *&root, T value)
    {
        if (root)
        {
            if (value < root->value)
            {
                deleteNode(root->pLeft, value);
            }
            else if (value > root->value)
            {
                deleteNode(root->pRight, value);
            }
            else
            {
                if (!root->pRight && !root->pLeft)
                {
                    Node *temp = root;
                    root = nullptr;
                    delete temp;
                }
                else if (!root->pRight)
                {
                    Node *temp = root;
                    root = root->pLeft;
                    delete temp;
                }
                else if (!root->pLeft)
                {
                    Node *temp = root;
                    root = root->pRight;
                    delete temp;
                }
                else
                {
                    T temp = getSmallestRight(root->pRight);
                    root->value = temp;
                    deleteNode(root->pRight, temp);
                }
            }
        }
    }
    T getSmallestRight(Node *root)
    {
        if (root->pLeft)
            return getSmallestRight(root->pLeft);
        else
        {
            return root->value;
        }
    }
    Node *getDeletedNode(Node *root)
    {
        if (root->pLeft)
            return getDeletedNode(root->pLeft);
        else
        {
            return root;
        }
    }

    string inOrderRec(Node *root)
    {
        stringstream ss;
        if (root != nullptr)
        {
            ss << inOrderRec(root->pLeft);
            ss << root->value << " ";
            ss << inOrderRec(root->pRight);
        }
        return ss.str();
    }

    string inOrder()
    {
        return inOrderRec(this->root);
    }
    void clear()
    {
        clear(this->root);
    }
    void clear(Node *root)
    {
        if (root)
        {
            clear(root->pLeft);
            clear(root->pRight);
            delete root;
        }
    }
    class Node
    {
    private:
        T value;
        Node *pLeft, *pRight;
        friend class BinarySearchTree<T>;

    public:
        Node(T value) : value(value), pLeft(NULL), pRight(NULL) {}
        ~Node() {}
    };
};
int main()
{

    BinarySearchTree<int> bst;
    bst.add(9);
    bst.add(2);
    bst.add(10);
    bst.add(100);
    bst.add(1000);
    bst.add(200);
    for (int i = 0; i <= 5; i++)
        bst.add(rand() % 500);
    bst.deleteNode(1000);
    bst.deleteNode(9);
    cout << bst.inOrder();
    system("pause");
    return 0;
}