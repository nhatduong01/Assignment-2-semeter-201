#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <class K, class V>
class BKUTree
{
public:
    class AVLTree;
    class SplayTree;

    class Entry
    {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };

private:
    AVLTree *avl;
    SplayTree *splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5)
    {
    }
    ~BKUTree()
    { /*this->clear();*/
    }

    void add(K key, V value);
    void remove(K key);
    V search(K key, vector<K> &traversedList);

    void traverseNLROnAVL(void (*func)(K key, V value));
    void traverseNLROnSplay(void (*func)(K key, V value));
    void clear();
    class SplayTree
    {
    public:
        class Node
        {
        public:
            Entry *entry;
            Node *left;
            Node *right;
            typename AVLTree::Node *corr;
            Node *parent;
            Node(Entry *entry = NULL, Node *left = NULL, Node *right = NULL)
            {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
        };

    protected:
        Node *BiggestLeft(Node *root)
        {
            if (root->right)
                return BiggestLeft(root->right);
            else
            {
                return root;
            }
        }
        void traverseNLR(void (*func)(K key, V value), Node *root)
        {
            if (root)
            {
                func(root->entry->key, root->entry->value);
                traverseNLR(func, root->left);
                traverseNLR(func, root->right);
            }
        }
        void clear(Node *root)
        {
            if (root)
            {
                clear(root->left);
                clear(root->right);
                delete root;
            }
        }
        Node *Right_Rotation(Node *_root)
        {
            Node *p = _root->parent;
            Node *temp = _root->left;
            _root->left = temp->right;
            if (temp->right != nullptr)
                temp->right->parent = _root;
            temp->right = _root;
            _root->parent = temp;
            temp->parent = p;
            if (p != nullptr)
            {
                if (p->left == _root)
                    p->left = temp;
                else
                {
                    p->right = temp;
                }
            }
            return temp;
        }
        Node *Left_Rotation(Node *_root)
        {
            Node *p = _root->parent;
            Node *temp = _root->right;
            _root->right = temp->left;
            if (temp->left != nullptr)
                temp->left->parent = _root;
            temp->left = _root;
            _root->parent = temp;
            temp->parent = p;
            if (p != nullptr)
            {
                if (p->left == _root)
                    p->left = temp;
                else
                {
                    p->right = temp;
                }
            }
            return temp;
        }
        void Self_Splay1(Node *&root, Node *&new_node)
        {
            while (new_node->parent != nullptr)
            {
                if (new_node->parent == root)
                {
                    if (new_node == new_node->parent->left)
                    {
                        root = Right_Rotation(new_node->parent);
                    }
                    else
                    {
                        root = Left_Rotation(new_node->parent);
                    }
                }
                else
                {
                    Node *p = new_node->parent;
                    Node *g = p->parent;
                    if (new_node == new_node->parent->left && p == p->parent->left)
                    {
                        g = Right_Rotation(g);
                        p = Right_Rotation(p);
                        if (new_node->parent == nullptr)
                            root = p;
                    }
                    else if (new_node == new_node->parent->right && p == p->parent->right)
                    {
                        g = Left_Rotation(g);
                        p = Left_Rotation(p);
                        if (new_node->parent == nullptr)
                            root = p;
                    }
                    else if (new_node == new_node->parent->left && p == p->parent->right)
                    {
                        p = Right_Rotation(p);
                        g = Left_Rotation(g);
                        if (new_node->parent == nullptr)
                            root = g;
                    }
                    else
                    {
                        p = Left_Rotation(p);
                        g = Right_Rotation(g);
                        if (new_node->parent == nullptr)
                            root = g;
                    }
                }
            }
        }
        void add(Node *root, Entry *entry)
        {
            if (root == nullptr)
            {
                Node *temp = new Node(entry);
                root = temp;
                Self_Splay1(this->root, temp);
                return;
            }
            else
            {
                if (entry->key < root->entry->key)
                {
                    add(root->left, entry);
                }
                else if (entry->key > root->entry->key)
                {
                    add(root->right, entry);
                }
                {
                    throw("Duplicate key");
                }
            }
        }
        void remove(Node *root, K key)
        {
            if (root == nullptr)
                throw("Not Found");
            else
            {
                if (key > root->entry->key)
                    remove(root->right, key);
                else if (key < root->entry->key)
                    remove(root->left, key);
                else
                {
                    Node *temp1 = root;
                    Self_Splay1(this->root, temp1);
                    if (root->left && root->right)
                    {
                        Node *temp = this->root;
                        Node *temp_left = temp->left;
                        temp_left->parent = nullptr;
                        Node *temp_right = temp->right;
                        temp_right->parent = nullptr;
                        delete temp;
                        Node *biggestLeft = BiggestLeft(temp_left);
                        Self_Splay1(temp_left, biggestLeft);
                        temp_left->right = temp_right;
                        temp_right->parent = temp_left;
                        this->root = temp_left;
                    }
                    else if (root->left)
                    {
                        Node *temp = this->root;
                        Node *temp_left = temp->left;
                        temp_left->parent = nullptr;
                        delete temp;
                        Node *biggestLeft = BiggestLeft(temp_left);
                        Self_Splay1(temp_left, biggestLeft);
                        this->root = temp_left;
                    }
                    else if (root->right)
                    {
                        Node *temp = this->root;
                        Node *temp_right = temp->right;
                        temp_right->parent = nullptr;
                        this->root = temp_right;
                        delete temp;
                    }
                    else
                    {
                        delete root;
                        this->root = nullptr;
                    }
                }
            }
        }
        V search(Node *root, K key)
        {
            if (root == nullptr)
                throw("Not Found");
            else
            {
                if (key > root->entry->key)
                    return search(root->right, key);
                else if (key < root->entry->key)
                    return search(root->left, key);
                else
                {
                    Self_Splay3(this->root, root);
                    return root->entry->value;
                }
            }
        }

    public:
        Node *root;

        SplayTree() : root(NULL){};
        ~SplayTree() { this->clear(); };
        void add(K key, V value)
        {
            Entry *temp = new Entry(key, value);
            add(this->root, temp);
        }
        void add(Entry *entry)
        {
            add(this->root, entry);
        }
        void remove(K key)
        {
            remove(this->root, key);
        }
        V search(K key)
        {
            return search(this->root, key);
        }
        void traverseNLR(void (*func)(K key, V value))
        {
            traverseNLR(func, this->root);
        }
        void clear()
        {
            clear(this->root);
        }
    };
    class AVLTree
    {
    public:
        class Node
        {
        public:
            Entry *entry;
            Node *left;
            Node *right;
            int balance;
            typename SplayTree::Node *corr;
            Node(Entry *entry = NULL, Node *left = NULL, Node *right = NULL)
            {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }
        };
        Node *root;

    protected:
        void traverseNLR(void (*func)(K key, V value), Node *root)
        {
            if (root)
            {
                func(root->entry->key, root->entry->value);
                traverseNLR(func, root->left);
                traverseNLR(func, root->right);
            }
        }
        void clear(Node *root)
        {
            if (root)
            {
                clear(root->left);
                clear(root->right);
                delete root;
            }
        }
        void remove(Node *, K);
        Entry *BiggestLeft(Node *root)
        {
            if (root->right)
                return BiggestLeft(root->right);
            else
            {
                return root->entry;
            }
        }
        int Height(Node *_root)
        {
            if (_root == nullptr)
                return 0;
            else
            {
                int left = 0;
                int right = 0;
                left = 1 + Height(_root->left);
                right = 1 + Height(_root->right);
                return left < right ? right : left;
            }
        }
        V search(Node *root, K key)
        {
            if (root == nullptr)
                throw("Not Found");
            else
            {
                if (key > root->entry->key)
                    return search(root->right, key);
                else if (key < root->entry->key)
                    return search(root->left, key);
                else
                {
                    return root->entry->value;
                }
            }
        }
        void RotateLeft(Node *&_root)
        {
            Node *temp = _root->right;
            _root->right = temp->left;
            temp->left = _root;
            _root = temp;
        }
        void RotateRight(Node *&_root)
        {
            Node *temp = _root->left;
            _root->left = temp->right;
            temp->right = _root;
            _root = temp;
        }
        int BalanceFactor(Node *_root)
        {
            if (_root)
            {
                return Height(_root->left) - Height(_root->right);
            }
            return 0;
        }
        void SeflBalancing(Node *&_root)
        {
            if (!_root)
                return;
            if (BalanceFactor(_root) > 1)
            {
                if (BalanceFactor(_root->left) >= 1)
                {
                    RotateRight(_root);
                }
                else if (BalanceFactor(_root->left) <= -1)
                {
                    RotateLeft(_root->left);
                    RotateRight(_root);
                }
            }
            else if (BalanceFactor(_root) < -1)
            {
                if (BalanceFactor(_root->right) >= 1)
                {
                    RotateRight(_root->right);
                    RotateLeft(_root);
                }
                else if (BalanceFactor(_root->right) <= -1)
                {
                    RotateLeft(_root);
                }
            }
        }

    public:
        AVLTree() : root(NULL){};
        ~AVLTree() { this->clear(); };
        void add(K key, V value);
        void add(Entry *entry);
        void add(Node *, Entry *);
        void remove(K key)
        {
            remove(this->root, key);
        }
        V search(K key)
        {
            return search(this->root, key);
        }
        void traverseNLR(void (*func)(K key, V value))
        {
            traverseNLR(func, this->root);
        }
        void clear()
        {
            clear(this->root);
        }
        int getHeight()
        {
            return Height(this->root);
        }
    };
};
template <class K, class V>
void BKUTree<K, V>::AVLTree::remove(Node *root, K key)
{
    if (root == nullptr)
        throw("Not Found");
    else
    {
        if (key < root->entry->key)
        {
            remove(root->left, key);
        }
        else if (key > root->entry->key)
        {
            remove(root->right, key);
        }
        else
        {
            if (!root->right && !root->left)
            {
                Node *temp = root;
                root = nullptr;
                delete temp;
            }
            else if (!root->right)
            {
                Node *temp = root;
                root = root->left;
                delete temp;
            }
            else if (!root->left)
            {
                Node *temp = root;
                root = root->right;
                delete temp;
            }
            else
            {
                Entry *temp = (BiggestLeft(root->left));
                root->entry = temp;
                remove(root->left, temp->key);
            }
        }
        SeflBalancing(root);
    }
}
template <class K, class V>
void BKUTree<K, V>::AVLTree::add(K key, V value)
{
    Entry *temp = new Entry(key, value);
    add(this->root, temp);
}
template <class K, class V>
void BKUTree<K, V>::AVLTree::add(Entry *entry)
{
    add(this->root, entry);
}
template <class K, class V>
void BKUTree<K, V>::AVLTree::add(Node *root, Entry *entry)
{
    if (root == nullptr)
    {
        Node *temp = new Node(entry);
        root = temp;
        return;
    }
    else
    {
        if (entry->key < root->entry->key)
        {
            add(root->left, entry);
        }
        else if (entry->key > root->entry->key)
        {
            add(root->right, entry);
        }
        {
            throw("Duplicate key");
        }
    }
    SeflBalancing(root);
}
int main()
{
    BKUTree<int, int> test;
    BKUTree<int, int>::AVLTree test_avl;
    int arr[] = {10, 52, 98, 32, 68, 92, 40, 13, 42, 63, 99, 100};
    for (int i = 0; i < 12; i++)
    {
        test_avl.add(arr[i], i);
    }
    for (int i = 0; i < 12; i++)
    {
        test_avl.remove(arr[i]);
    }
    system("pause");
    return 0;
}