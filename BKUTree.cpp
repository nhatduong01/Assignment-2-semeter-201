#include <iostream>
#include <queue>
#include <vector>
using namespace std;
void printKey(int key, int value)
{
    cout << key << endl;
}
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
    unsigned int maxNumOfKeys;

public:
    BKUTree(unsigned int maxNumOfKeys = 5)
    {
        this->maxNumOfKeys = maxNumOfKeys;
        avl = new AVLTree();
        splay = new SplayTree();
    }
    queue<K> copy_queue()
    {
        queue<int> Q2 = this->keys;
        return Q2;
    }
    ~BKUTree()
    {
        this->clear();
    }
    void add(K key, V value)
    {
        if (keys.size() == maxNumOfKeys)
        {
            keys.pop();
            keys.push(key);
        }
        else
        {
            keys.push(key);
        }
        Entry *new_entry = new Entry(key, value);
        typename AVLTree::Node *A = avl->add(new_entry);
        typename SplayTree::Node *B = splay->add(new_entry);
        A->corr = B;
        B->corr = A;
    }
    void remove(K key)
    {
        int size_queue = keys.size();
        while (size_queue && !keys.empty())
        {
            K temp_key = keys.front();
            keys.pop();
            if (temp_key != key)
            {
                keys.push(temp_key);
            }
            size_queue--;
        }
        splay->remove(key);
        avl->remove(key);
        if (splay->root)
            keys.push(splay->root->entry->key);
    }
    V search(K key, vector<K> &traversedList)
    {
        if (splay->root->entry->key == key)
        {
            if (keys.size() == maxNumOfKeys)
            {
                keys.pop();
                keys.push(key);
            }
            else
            {
                keys.push(key);
            }
            return splay->root->entry->value;
        }
        else
        {
            traversedList.push_back(splay->root->entry->key);
            bool found = false;
            int size = keys.size();
            while (size)
            {
                K temp = keys.front();
                keys.pop();
                keys.push(temp);
                if (temp == key)
                    found = true;
                size--;
            }
            if (found == true)
            {
                if (keys.size() == maxNumOfKeys)
                {
                    keys.pop();
                    keys.push(key);
                }
                else
                {
                    keys.push(key);
                }
                return splay->search(splay->root, key, traversedList);
            }
            else
            {
                typename AVLTree::Node *A = splay->root->corr;
                typename AVLTree::Node *B = avl->search_AVL(A, key, traversedList);
                if (B == nullptr)
                {
                    B = avl->search_AVL2(avl->root, key, A, traversedList);
                    typename SplayTree::Node *temp_splay = B->corr;
                    splay->Self_Splay_only_one(splay->root, temp_splay);
                    if (keys.size() == maxNumOfKeys)
                    {
                        keys.pop();
                        keys.push(key);
                    }
                    else
                    {
                        keys.push(key);
                    }
                    return B->entry->value;
                }
                else
                {
                    typename SplayTree::Node *temp_splay = B->corr;
                    splay->Self_Splay_only_one(splay->root, temp_splay);
                    if (keys.size() == maxNumOfKeys)
                    {
                        keys.pop();
                        keys.push(key);
                    }
                    else
                    {
                        keys.push(key);
                    }
                    return B->entry->value;
                }
            }
        }
    }
    void traverseNLROnAVL(void (*func)(K key, V value))
    {
        avl->traverseNLR(func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value))
    {
        splay->traverseNLR(func);
    }
    void clear()
    {
        if (avl)
            avl->clear();
        if (splay)
            splay->clear();
        while (!keys.empty())
        {
            keys.pop();
        }
        delete avl;
        delete splay;
    }
    class SplayTree
    {
    public:
        friend class BKUTree;
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
        V search(Node *root, K key, vector<K> &traversedList)
        {

            if (key > root->entry->key)
            {
                if (traversedList[traversedList.size() - 1] != root->entry->key)
                    traversedList.push_back(root->entry->key);
                return search(root->right, key, traversedList);
            }
            else if (key < root->entry->key)
            {
                if (traversedList[traversedList.size() - 1] != root->entry->key)
                    traversedList.push_back(root->entry->key);
                return search(root->left, key, traversedList);
            }
            else
            {
                Self_Splay_only_one(this->root, root);
                return root->entry->value;
            }
        }
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
                delete root->entry;
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
        void Self_Splay_only_one(Node *&root, Node *&new_node)
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
        Node *add(Node *&root, Node *&new_node)
        {
            Node *temp = root;
            Node *prev = nullptr;
            while (temp != nullptr)
            {
                prev = temp;
                if (new_node->entry->key < temp->entry->key)
                    temp = temp->left;
                else
                    temp = temp->right;
            }
            if (prev == nullptr)
            {
                root = new_node;
                root->parent = nullptr;
            }
            else
            {
                if (new_node->entry->key < prev->entry->key)
                {
                    prev->left = new_node;
                    new_node->parent = prev;
                }
                else
                {
                    prev->right = new_node;
                    new_node->parent = prev;
                }
            }
            Self_Splay1(this->root, new_node);
            return new_node;
        }
        Node *add(Node *&root, Entry *entry)
        {
            Node *new_node = new Node(entry);
            return add(this->root, new_node);
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
                        ;
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
                        ;
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
                    Self_Splay1(this->root, root);
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
        Node *add(Entry *entry)
        {
            return add(this->root, entry);
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
            if (this->root)
                clear(this->root);
        }
    };
    class AVLTree
    {
    public:
        friend class BKUTree;
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
                delete root->entry;
                delete root;
            }
        }
        void remove(Node *&, K);
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
        void add(Node *&, Entry *, Node *);
        Node *search_AVL(Node *root, K key, vector<K> &traversedList)
        {
            if (root == nullptr)
            {
                return nullptr;
            }
            else
            {
                if (key > root->entry->key)
                {
                    if (traversedList[traversedList.size() - 1] != root->entry->key)
                        traversedList.push_back(root->entry->key);
                    return search_AVL(root->right, key, traversedList);
                }
                else if (key < root->entry->key)
                {
                    if (traversedList[traversedList.size() - 1] != root->entry->key)
                        traversedList.push_back(root->entry->key);
                    return search_AVL(root->left, key, traversedList);
                }
                else
                {
                    return root;
                }
            }
        }
        Node *search_AVL2(Node *root, K key, Node *temp, vector<K> &traversedList)
        {
            if (root == nullptr || root == temp)
            {
                throw("Not Found");
            }
            else
            {
                if (key > root->entry->key)
                {
                    if (traversedList[traversedList.size() - 1] != root->entry->key)
                        traversedList.push_back(root->entry->key);
                    return search_AVL2(root->right, key, temp, traversedList);
                }
                else if (key < root->entry->key)
                {
                    if (traversedList[traversedList.size() - 1] != root->entry->key)
                        traversedList.push_back(root->entry->key);
                    return search_AVL2(root->left, key, temp, traversedList);
                }
                else
                {
                    return root;
                }
            }
        }

    public:
        AVLTree() : root(NULL){};
        ~AVLTree() { this->clear(); };
        void add(K key, V value);
        Node *add(Entry *entry);
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
            if (this->root)
                clear(this->root);
        }
        int getHeight()
        {
            return Height(this->root);
        }
    };
};
template <class K, class V>
void BKUTree<K, V>::AVLTree::remove(Node *&root, K key)
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
                //;
                delete temp;
            }
            else if (!root->right)
            {
                Node *temp = root;
                root = root->left;
                //;
                delete temp;
            }
            else if (!root->left)
            {
                Node *temp = root;
                root = root->right;
                //;
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
    Node *new_node = new Node(temp);
    add(this->root, temp, new_node);
}
template <class K, class V>
typename BKUTree<K, V>::AVLTree::Node *BKUTree<K, V>::AVLTree::add(Entry *entry)
{
    Node *new_node = new Node(entry);
    add(this->root, entry, new_node);
    return new_node;
}
template <class K, class V>
void BKUTree<K, V>::AVLTree::add(Node *&root, Entry *entry, Node *new_node)
{
    if (root == nullptr)
    {
        root = new_node;
        return;
    }
    else
    {
        if (entry->key < root->entry->key)
        {
            add(root->left, entry, new_node);
        }
        else if (entry->key > root->entry->key)
        {
            add(root->right, entry, new_node);
        }
        else
        {
            throw("Duplicate key");
        }
    }
    SeflBalancing(root);
}
int main()
{
    /*BKUTree<int, int> test;
    BKUTree<int, int>::AVLTree test_avl;
    int arr[] = {10, 52, 98, 32, 68, 92, 40, 13, 42, 63, 99, 100};
    for (int i = 0; i < 12; i++)
    {
        test_avl.add(arr[i], i);
    }
    test_avl.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    for (int i = 0; i < 12; i++)
    {
        test_avl.remove(arr[i]);
    }
    test_avl.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << "\n Test splay tree \n";
    BKUTree<int, int>::SplayTree test_tree;
    test_tree.add(383, 1);
    test_tree.add(886, 1);
    test_tree.add(100, 1);
    test_tree.add(915, 1);
    test_tree.add(793, 1);
    test_tree.add(150, 1);
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    test_tree.search(383);
    cout << "\nTest Searching \n ";
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    test_tree.add(700, 1);
    test_tree.add(701, 1);
    test_tree.add(702, 1);
    test_tree.add(703, 1);
    test_tree.add(704, 1);
    test_tree.add(705, 1);
    cout << endl;
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    test_tree.search(793);
    cout << endl;
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    test_tree.add(500, 1);
    test_tree.search(702);
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    test_tree.remove(500);
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    test_tree.remove(703);
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    int arr1[] = {100, 105, 102, 99, 103, 106, 107, 111, 999, 123, 145, 165, 250, 270, 108};
    for (int i = 0; i < 7; i++)
    {
        test_tree.add(arr1[i], 1);
    }
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    for (int i = 0; i < 3; i++)
    {
        test_tree.search(arr1[i]);
    }
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    for (int i = 0; i < 7; i++)
    {
        test_tree.remove(arr1[i]);
    }
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    test_tree.add(10, 1);
    test_tree.add(12, 1);
    test_tree.add(11, 1);
    test_tree.remove(10);
    test_tree.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    cout << endl;
    cout << "Test tree 2 \n";
    BKUTree<int, int>::SplayTree test_tree1;
    for (int i = 0; i < 15; i++)
    {
        test_tree1.add(arr1[i], 1);
    }
    test_tree1.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });
    for (int i = 0; i < 15; i++)
    {
        test_tree1.remove(arr1[i]);
    }
    cout << "\nTest removing all\n";
    test_tree1.traverseNLR([](int a, int b) {
        cout << a << "  ";
    });*/
    BKUTree<int, int> *tree = new BKUTree<int, int>(3);
    int keys[] = {1, 3, 5, 7, 9, 2, 4};
    for (int i = 0; i < 7; i++)
        tree->add(keys[i], keys[i]);
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    cout << "\n Testing Search \n";
    vector<int> traversedList;
    cout << "\n Testing the queue \n";
    queue<int> test_queue = tree->copy_queue();
    cout << "The size is : " << test_queue.size() << endl;
    while (test_queue.size() != 0)
    {
        cout << test_queue.front() << "\t";
        test_queue.pop();
    }
    cout << endl;
    cout << tree->search(7, traversedList);
    cout << "\n Print the vector \n";
    for (auto i = traversedList.begin(); i != traversedList.end(); i++)
    {
        cout << *i << "\t";
    }
    /*for (int i = 0; i < 7; i++)
        tree->remove(keys[i]);
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);*/
    cout << endl;
    cout << "The slay tree is : \n";
    tree->traverseNLROnSplay(printKey);
    system("pause");
    return 0;
}