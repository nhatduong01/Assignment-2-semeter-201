template <typename E>
class BinNode
{
public:
    virtual ~BinNode() {}
    virtual E &element() = 0;
    virtual void setElement(const E &) = 0;
    virtual BinNode *left() const = 0;
    virtual BinNode *right() const = 0;
    virtual void setLeft(BinNode *) = 0;
    virtual void setRight(BinNode *) = 0;
    virtual bool isLeaf() = 0;
};

// Simple binary tree node implementation
template <typename Key, typename E>
class BSTNode : public BinNode<E>
{
private:
    Key k;
    E it;
    BSTNode *lc;
    BSTNode *rc;

public:
    BSTNode()
    {
        lc = rc = nullptr;
    }
    BSTNode(Key K, E e, BSTNode *l = nullptr, BSTNode *r = nullptr) : k(K), it(e), lc(l), rc(r) {}
    ~BSTNode() {}
    E &element()
    {
        return it;
    }
    void setElement(const E &e)
    {
        it = e;
    }
    Key &key()
    {
        return k;
    }
    void setKey(const Key &K)
    {
        k = K;
    }
    inline BSTNode *left() const { return lc; }
    void setLeft(BinNode<E> *b) { lc = (BSTNode *)b; }
    inline BSTNode *right() const { return rc; }
    void setRight(BinNode<E> *b) { rc = (BSTNode *)b; }
    bool isLeaf() { return (lc == NULL) && (rc == NULL); }
};
template <typename Key, typename E>
class Dictionary
{
private:
    void operator=(const Dictionary &) {}
    Dictionary(const Dictionary &) {}

public:
    Dictionary() {}
    virtual ~Dictionary() {}
    virtual void clear() = 0;
    // Insert a record
    // k : the key for the record being inserted
    // e : the record being inserted
    virtual void insert(const Key &k, const E &e) = 0;
    // Remove and return a record
    // k : the key of the record to be removed
    // Return : A matching record. If multiple records match
    // return an arbitrary one. Return Null if no record
    virtual E remove(const Key &k) = 0;
    // Remove and return an arbitrary record from dictionary
    // REturn : the record removed, or NULL if none exists
    virtual E removeAny() = 0;
    // Return matching "k" (NULL if none exists)
    virtual E find(const Key &k) const = 0;
    virtual int size() = 0;
};
// Binary Search Tree implementation for the Dictionary
template <typename Key, typename E>
class BST : public Dictionary<Key, E>
{
private:
    BSTNode<Key, E> *root;
    int nodecount;
    void clearHelper(BSTNode<Key, E> *);
    BSTNode<Key, E> *inserthelp(BSTNode<Key, E> *,
                                const Key &, const E &);
    BSTNode<Key, E> *deleteMin(BSTNode<Key, E> *);
    BSTNode<Key, E> *getMin(BSTNode<Key, E> *);
    BSTNode<Key, E> *removeHelp(BSTNode<Key, E> *, const Key &);
    E findHelp(BSTNode<Key, E> *, const Key &) const;
    void printHelp(BSTNode<Key, E> *, int) const;

public:
    BST()
    {
        root = NULL;
        nodecount = 0;
    }
    ~BST()
    {
        clearHelper(root);
    }
    void clear()
    {
        clearHelper(root);
        root = NULL;
        nodecount = 0;
    }
    void insert(const Key &k, const E &e)
    {
        root = inserthelp(root, k, e);
        nodecount++;
    }
    E remove(const Key &k)
    {
        E temp = findHelp(root, k);
        if (temp != NULL)
        {
            root = removeHelp(root, k);
            nodecount--;
        }
        return temp;
    }
    E removeAny()
    {
        if (root == NULL)
        {
            E temp = root->element();
            root = removeHelp(root, root->key());
            nodecount--;
            return temp;
        }
        else
            return NULL;
    }
    E find(const Key &k) const
    {
        return findHelp(root, k);
    }
    int size()
    {
        return nodecount;
    }
    void print() const
    {
        if (root == NULL)
            std::cout << "The BST is empty \n";
        else
        {
            printHelp(root, 0);
        }
    }
};
template <typename Key, typename E>
E BST<Key, E>::findHelp(BSTNode<Key, E> *root, const Key &k) const
{
    if (root)
    {
        if (root->key() == k)
        {
            return root->element();
        }
        else if (root->key() > k)
        {
            return findHelp(root->left(), k);
        }
        else
            return findHelp(root->right(), k);
    }
    else
        return NULL;
}
template <typename Key, typename E>
BSTNode<Key, E> *BST<Key, E>::inserthelp(
    BSTNode<Key, E> *root, const Key &k, const E &it)
{
    if (root)
    {
        if (root->key() >= k)
            inserthelp(root->left(), k, it);
        else
        {
            inserthelp(root->right(), k, it);
        }
    }
    else
    {
        root = new BSTNode<Key, E>(k, it);
    }
    return root;
}
template <typename Key, typename E>
BSTNode<Key, E> *BST<Key, E>::deleteMin(BSTNode<Key, E> *root) // just cut the path to that node
// We havent delete it
{
    if (root->left() == NULL)
    {
        return root->right();
    }
    else
    {
        root->setLeft(deleteMin(root->left()));
        return root;
    }
}
template <typename Key, typename E>
BSTNode<Key, E> *BST<Key, E>::getMin(BSTNode<Key, E> *root)
{
    if (root->left())
        return getMin(root->left());
    else
    {
        return root;
    }
}
template <typename Key, typename E>
BSTNode<Key, E> *BST<Key, E>::removeHelp(BSTNode<Key, E> *root, const Key &k)
{
    if (root)
    {
        if (root->key() > k)
            return removeHelp(root->left(), k);
        else if (root->key() < k)
        {
            removeHelp(root->right(), k);
        }
        else
        {
            BSTNode<Key, E> *temp = root;
            if (root->left() && root->right())
            {
                temp = getMin(root->right());
                root->setKey(temp->key());
                root->setElement(temp->element());
                root->setRight(deleteMin(root->right())); // to cut the path to that node
                delete temp;
            }
            else if (root->left())
            {
                root = root->left();
                delete temp;
            }
            else
            {
                root = root->right();
                delete temp;
            }
        }
    }
    else
    {
        return NULL;
    }
}
template <typename Key, typename E>
void BST<Key, E>::clearHelper(BSTNode<Key, E> *root)
{
    if (root == NULL)
        return;
    clearHelper(root->left());
    clearHelper(root->right());
    delete root;
}
template <typename Key, typename E>
void BST<Key, E>::printHelp(BSTNode<Key, E> *root, int level) const
{
    if (root == NULL)
        return;
    printHelp(root->left(), level + 1);
    for (int i = 0; i < level; i++) // indent to level of the sub tree
        cout << "\t";
    cout << root->key()<<"\n";
    printHelp(root->right(),level+1);
    //This perfrom inorder traversal  it will print ascending order
}