#include <iostream>
#include <queue>
#include <vector>
using namespace std;
void printKey(int key, int value)
{
    cout << key << endl;
}
int max(int a, int b)
{
    return a > b ? a : b;
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
        typename AVLTree::Node *A = NULL;
        try
        {
            A = avl->add(new_entry);
        }
        catch (char const *e)
        {
            delete new_entry;
            throw e;
        }
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
        // I have to fixx this problem
        avl->remove_BKU(avl->root, key);
        splay->remove_BKU(splay->root, key);
        if (splay->root)
        {
            if (keys.size() == maxNumOfKeys)
            {
                keys.pop();
                keys.push(splay->root->entry->key);
            }
            else
            {
                keys.push(splay->root->entry->key);
            }
        }
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
        {
            avl->clear();
            delete avl;
            avl = nullptr;
        }

        if (splay)
        {
            splay->clear();
            delete splay;
            splay = nullptr;
        }

        while (!keys.empty())
        {
            keys.pop();
        }
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
            if (root == nullptr)
                return;
            clear(root->left);
            clear(root->right);
            if (root->entry)
            {
                delete root->entry;
                root->entry = nullptr;
            }
            delete root;
            this->root = nullptr;
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
        void remove_BKU(Node *root, K key)
        {
            if (root == nullptr)
                throw("Not Found");
            else
            {
                if (key > root->entry->key)
                    remove_BKU(root->right, key);
                else if (key < root->entry->key)
                    remove_BKU(root->left, key);
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
                        root = nullptr;
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
                        root = nullptr;
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
                        root = nullptr;
                    }
                    else
                    {
                        if (root->entry)
                        {
                            delete root->entry;
                            root->entry = nullptr;
                        }
                        delete root;
                        this->root = nullptr;
                    }
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
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
                        if (temp->entry)
                        {
                            delete temp->entry;
                            temp->entry = nullptr;
                        }
                        delete temp;
                        temp = nullptr;
                    }
                    else
                    {
                        if (root->entry)
                        {
                            delete root->entry;
                            root->entry = nullptr;
                        }
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
            int height;
            typename SplayTree::Node *corr;
            Node(Entry *entry = NULL, Node *left = NULL, Node *right = NULL)
            {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
                this->height = 1;
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
        void remove_BKU(Node *&root, K key)
        {
            if (root == nullptr)
                throw("Not Found");
            else
            {
                if (key < root->entry->key)
                {
                    remove_BKU(root->left, key);
                }
                else if (key > root->entry->key)
                {
                    remove_BKU(root->right, key);
                }
                else
                {
                    if (!root->right && !root->left)
                    {
                        Node *temp = root;
                        root = nullptr;
                        temp->entry = nullptr;
                        delete temp;
                        temp = nullptr;
                    }
                    else if (!root->right)
                    {
                        Node *temp = root;
                        root = root->left;
                        temp->entry = nullptr;
                        delete temp;
                        temp = nullptr;
                    }
                    else if (!root->left)
                    {
                        Node *temp = root;
                        root = root->right;
                        temp->entry = nullptr;
                        delete temp;
                        temp = nullptr;
                    }
                    else
                    {
                        Node *temp = (BiggestLeftNode(root->left));
                        root->entry = temp->entry;
                        root->corr = temp->corr;
                        temp->corr->corr = root;
                        remove_BKU(root->left, temp->entry->key);
                    }
                }
                if (root)
                    root->height = max(Height(root->left), Height(root->right)) + 1;
                SeflBalancing(root);
            }
        }
        void clear(Node *root)
        {
            if (root == nullptr)
                return;

            clear(root->left);
            clear(root->right);
            if (root->entry)
            {
                delete root->entry;
                root->entry = nullptr;
            }
            delete root;
            this->root = nullptr;
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
        Node *BiggestLeftNode(Node *root)
        {
            if (root->right)
                return BiggestLeftNode(root->right);
            else
            {
                return root;
            }
        }
        int Height(Node *_root)
        {
            if (_root == nullptr)
                return 0;
            else
            {
                return _root->height;
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
            _root->height = max(Height(_root->left), Height(_root->right)) + 1;
            temp->height = max(Height(temp->right), Height(temp->left)) + 1;
            _root = temp;
        }
        void RotateRight(Node *&_root)
        {
            Node *temp = _root->left;
            _root->left = temp->right;
            temp->right = _root;
            _root->height = max(Height(_root->left), Height(_root->right)) + 1;
            temp->height = max(Height(temp->right), Height(temp->left)) + 1;
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
                if (BalanceFactor(_root->left) >= 0)
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
                else if (BalanceFactor(_root->right) <= 0)
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
                if (root->entry)
                {
                    delete root->entry;
                    root->entry = nullptr;
                }
                root = nullptr;
                delete temp;
                temp = nullptr;
            }
            else if (!root->right)
            {
                Node *temp = root;
                root = root->left;
                if (root->entry)
                {
                    delete root->entry;
                    root->entry = nullptr;
                }
                delete temp;
                temp = nullptr;
            }
            else if (!root->left)
            {
                Node *temp = root;
                root = root->right;
                if (root->entry)
                {
                    delete root->entry;
                    root->entry = nullptr;
                }
                delete temp;
                temp = nullptr;
            }
            else
            {
                Entry *temp = (BiggestLeft(root->left));
                root->entry = temp;
                remove(root->left, temp->key);
            }
        }
        if (root)
            root->height = max(Height(root->left), Height(root->right)) + 1;
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
    root->height = max(Height(root->left), Height(root->right)) + 1;
    SeflBalancing(root);
}
int main()
{
    BKUTree<int, int> *tree = new BKUTree<int, int>(3);
    tree->add(5, 10);
    tree->add(4, 16);
    tree->add(3, 16);
    tree->add(2, 16);
    tree->add(1, 16);
    tree->add(50, 50);
    tree->add(110, 110);
    tree->add(150, 150);
    tree->add(200, 220);
    tree->add(7, 220);
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    tree->remove(110);
    cout << endl;
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    cout << endl;
    queue<int> Q1 = tree->copy_queue();
    cout << "\nThe queue at the moment is : \n";
    while (!Q1.empty())
    {
        cout << Q1.front() << "\t";
        Q1.pop();
    }
    cout << endl;
    cout << endl;
    vector<int> result;
    tree->search(150, result);
    tree->traverseNLROnAVL(printKey);
    // //tree->remove(3);
    // cout << endl;
    cout << endl;
    tree->traverseNLROnSplay(printKey);
    cout << "\n The vector is : \n";
    for (auto i = result.begin(); i != result.end(); i++)
    {
        cout << *i << "\t";
    }
    Q1 = tree->copy_queue();
    cout << "\nThe queue at the moment is : \n";
    while (!Q1.empty())
    {
        cout << Q1.front() << "\t";
        Q1.pop();
    }
    cout << "\nThe Search vector is : \n";
    vector<int> result2;
    tree->search(2, result2);
    for (auto i = result2.begin(); i != result2.end(); i++)
    {
        cout << *i << "\t";
    }
    cout << endl;
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    tree->add(205, 110);
    tree->add(204, 150);
    tree->add(203, 220);
    tree->add(202, 220);
    result2.clear();
    cout << endl;
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    tree->search(205, result2);
    cout << "\n The vecotr is : \n";
    for (auto i = result2.begin(); i != result2.end(); i++)
    {
        cout << *i << "\t";
    }
    cout << endl;
    cout << endl;
    tree->traverseNLROnSplay(printKey);
    cout << endl;
    tree->traverseNLROnAVL(printKey);
    delete tree;
    system("pause");
    return 0;
}