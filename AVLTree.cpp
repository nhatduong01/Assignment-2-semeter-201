#include <iostream>
#include <queue>
using namespace std;
class Node
{
public:
    int key;
    Node *left;
    Node *right;
    int balance;
    int height;

public:
    Node()
    {
        left = nullptr;
        right = nullptr;
    }
    Node(int _key, Node *_left = nullptr, Node *_right = nullptr)
    {
        key = _key;
        left = _left;
        right = _right;
    }
};
class AVLTree
{
protected:
    Node *root;
    void clear(Node *root)
    {
        if (root)
        {
            clear(root->left);
            clear(root->right);
            delete root;
        }
    }
    /*void rotateRight(Node *root)
    {
        Node *temp = root->left;
        root->left = temp->right;
        temp->right = root;
        root = temp;
    }
    void rotateLeft(Node *root)
    {
        Node *temp = root->right;
        root->right = temp->left;
        temp->left = root;
        root = temp;
    }*/

public:
    AVLTree();
    ~AVLTree();
    int calculateHeight(Node *root);
    int calculateBalance(Node *root);
    Node *RotateLeftLeft(Node *root);
    Node *RotateRightRight(Node *root);
    Node *RotateRightLeft(Node *root);
    Node *RotateLeftRight(Node *root);
    Node *insert(Node *, int);
    void printTree(Node *, int) const;
    Node *getRoot() const
    {
        return root;
    }
};

AVLTree::AVLTree()
{
    root = nullptr;
}

AVLTree::~AVLTree()
{
    clear(root);
}
int AVLTree::calculateHeight(Node *root)
{
    if (root->left && root->right)
    {
        if (root->left->height > root->right->height)
            return root->left->height + 1;
        else
            return root->right->height + 1;
    }
    else if (root->left)
    {
        return root->left->height + 1;
    }
    else if (root->right)
    {
        return root->right->height + 1;
    }
}
int AVLTree::calculateBalance(Node *root)
{
    if (root->left && root->right)
    {
        int temp = root->left->height - root->right->height;
        root->balance = temp;
        return temp;
    }
    else if (root->left)
    {
        int temp = root->left->height;
        root->balance = temp;
        return temp;
    }
    else if (root->right)
    {
        int temp = -root->right->height;
        root->balance = temp;
        return temp;
    }
}
Node *AVLTree::RotateLeftLeft(Node *root)
{
    Node *temp;
    Node *result;
    result = root;
    temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}
Node *AVLTree::RotateRightRight(Node *root)
{
    Node *temp;
    Node *result;
    result = root;
    temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}
Node *AVLTree::RotateRightLeft(Node *root)
{
    Node *temp;
    Node *result;
    result = root;
    //Rotate Right
    Node *curr = root->right;
    temp = curr->left;
    curr->left = temp->right;
    temp->right = curr;
    temp = curr;
    //Rotate Left
    temp = result->right;
    result->right = temp->left;
    temp->left = result;
    return temp;
}
Node *AVLTree::RotateLeftRight(Node *root)
{
    Node *temp;
    Node *result;
    result = root;
    //Rotate Right
    Node *curr = root->left;
    temp = curr->right;
    curr->right = temp->left;
    temp->left = curr;
    temp = curr;
    //Rotate Left
    temp = result->left;
    result->left = temp->right;
    temp->right = result;
    return temp;
}
Node *AVLTree::insert(Node *root, int _key)
{
    if (root == nullptr)
    {
        root = new Node(_key);
        root->height = 1;
        return root;
    }
    else
    {
        if (_key < root->key)
        {
            insert(root->left, _key);
        }
        else
        {
            insert(root->right, _key);
        }
    }
    root->height = calculateHeight(root);
    if (calculateBalance(root) == -2 && calculateBalance(root->right) == -1)
        root = RotateRightRight(root);
    else if (calculateBalance(root) == -2 && calculateBalance(root->right) == 1)
        root = RotateRightLeft(root);
    else if (calculateBalance(root) == 2 && calculateBalance(root->left) == -1)
        root = RotateLeftRight(root);
    else if (calculateBalance(root) == 2 && calculateBalance(root->left) == 1)
        root = RotateLeftLeft(root);
    return root;
}
void levelorder_newline(Node *v)
{
    queue<struct Node *> q;
    Node *cur;
    q.push(v);
    q.push(NULL);

    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        if (cur == NULL && q.size() != 0)
        {
            cout << "\n";

            q.push(NULL);
            continue;
        }
        if (cur != NULL)
        {
            cout << " " << cur->key;

            if (cur->left != NULL)
            {
                q.push(cur->left);
            }
            if (cur->right != NULL)
            {
                q.push(cur->right);
            }
        }
    }
}
int main()
{
    AVLTree test_tree;
    for (int i = 0; i < 15; i++)
        test_tree.insert(test_tree.getRoot(), rand() % 200);
    test_tree.printTree(test_tree.getRoot(), 0);
}