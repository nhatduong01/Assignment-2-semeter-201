#include <iostream>
#include <utility>
#include <functional>
template <typename T>
class BSTree
{
protected:
    struct Node
    {
        T data;
        Node *pLeft, *pRight;
        Node(const T &a, Node *pL = nullptr, Node *pR = nullptr) : data(a), pLeft(pL), pRight(pR) {}
        Node(const T &&a, Node *pL = nullptr, Node *pR = nullptr) : data(std::move(a)), pLeft(pL), pRight(pR) {}
    };
    void clear(Node *pR)
    {
        if (pR)
        {
            clear(pR->pLeft);
            clear(pR->pRight);
            delete pR;
        }
    }
    void NLT_traserse(std::function<void(T &)> op, Node *pR)
    {
        op(pR->data);
        if (pR->pLeft)
            NLR_traverse(op, pR->pLeft);
        if (pR->pRight)
            NLR_traverse(op, pR->pRight);
    }

public:
    Node *pRoot;
    BSTree() : pRoot(nullptr)
    {
    }
    ~BSTree()
    {
        clear();
    }
    void clear()
    {
        clear(pRoot);
    }
    void insert(const T &a, Node *&pR)
    {
        if (pR)
        {
            if (a < pR->data)
                insert(a, pR->pLeft);
            else
            {
                insert(a, pR->pRight);
            }
        }
        else
        {
            pR = new Node(a);
        }
    }
    void BFTraserse(std::function<void(T &)> op)
    {
    }
    void NLP_Traverse(std::function<void(T &)> op, Node *pR)
    {
        op(pR->data);
        if (pR->pLeft)
            NLP_Traverse(op, pR->pLeft);
        if (pR->pRight)
            NLP_Traverse(op, pR->pRight);
    }
    // Now we come to delete a node of a binary tree
    // Maximum on the left or minimum on the right ????
    /*

    */
    void remove(const T &a, Node *&pR)
    {
        if (pR)
        {
            if (pR->data == a)
            {
                if (pR->pLeft && pR->pRight)
                {
                    Node *p = pR->pRight;
                    while (p->pLeft)
                    {
                        p = p->pLeft;
                    }
                    pR->data = p->data;
                    remove(p->data, pR->right);
                }
                else if (pR->pLeft)
                {
                    Node *p = pR;
                    pR = pR->pLeft;
                    delete p;
                }
                else if (pR->pRight)
                {
                    Node *p = pR;
                    pR = pR->pRight;
                    delete p;
                }
                else
                {
                    delete pR;
                }
            }
            else if (pR->data > a)
            {
                remove(a, pR->pLeft);
            }
            else
            {
                remove(a, pR->pRight);
            }
        }
        else
        {
            pR = new Node(a);
        }
    }
};
/*

AVL tree
Balance Factor:
EH, LH, RH

*/
enum BalanceFactor
{
    EH,
    LH,
    RH
};
template <typename T>
class AVLTree
{
protected:
    Node *pRoot;
    struct Node
    {
        T data;
        BalanceFactor b;
        Node *pLeft, *pRight;
        Node(const T &a, BalanceFactor _b, Node *pL = nullptr, Node *pR = nullptr) : data(a), pLeft(pL), pRight(pR), b(_b) {}
        Node(const T &&a, Node *pL = nullptr, Node *pR = nullptr) : data(std::move(a)), pLeft(pL), pRight(pR) {}
    };
    void clear(Node *pR)
    {
        if (pR)
        {
            clear(pR->pLeft);
            clear(pR->pRight);
            delete pR;
        }
    }
    void rotL(Node *&pR)
    {
        Node *p = pR->pRight;
        pR->pRight = p->pLeft;
        p->pLeft = pR;
        pR = p;
    }
    void rotR(Node *&pR)
    {
        Node *p = pR->pLeft;
        pR->pLeft = p->pRight;
        p->pRight = pR;
        pR = p;
    }
    bool balanceLeft(Node *&pR)
    //insert to the left and the left get higher
    // this will balance the tree in this context
    {
        if (pR == nullptr)
        {
            pR = new Node(a);
            return true;
        }
        if (pR->data > a)
        {
            if (insert(a, pR->pLeft))
            {
                if (pR->b == LH)
                {
                    // WE have 3 case
                    if (pR->pLeft->b == LH)
                    {
                        rotR(pR);
                        pR->b = pR->pRight->b = EH;
                        return false;
                    }
                    else if (pR->pLeft->b == EH)
                    {
                        rotR(pR);
                        pR->b = RH;
                        pR->pRight->b = LH;
                        return true;
                    }
                    else
                    {
                        rotL(pR->pLeft);
                        rotR(pR);
                        if (pR->b == LH)
                        {
                            //pr->b == pR->pLeft->b =
                        }
                    }

                }                     // difficult case
                else if (pR->b == EH) //modify only the status
                {
                    pR->b = LH;
                    return true;
                }
                else
                {
                    pR->b = EH;
                    return false;
                }
            }
            else
                return 0;
        }
        else
        {
            if (insert(a, pR->pRight))
            {
            }
            else
                return 0;
        }
    }
    void NLT_traserse(std::function<void(T &)> op, Node *pR)
    {
        op(pR->data);
        if (pR->pLeft)
            NLR_traverse(op, pR->pLeft);
        if (pR->pRight)
            NLR_traverse(op, pR->pRight);
    }
    bool insert(const T &a, Node *&pR) // REturn true if the tree get higher
    {
        if (pR == nullptr)
        {
            pR = new Node(a);
            return true;
        }
        if (pR->data > a)
        {
            if (insert(a, pR->pLeft))
            {
                if (pR->b == LH)
                {
                    // WE have 3 case
                    if (pR->pLeft->b == LH)
                    {
                        rotR(pR);
                        pR->b = pR->pRight->b = EH;
                        return false;
                    }
                    else if (pR->pLeft->b == EH)
                    {
                        rotR(pR);
                        pR->b = RH;
                        pR->pRight->b = LH;
                        return true;
                    }
                    else
                    {
                        rotL(pR->pLeft);
                        rotR(pR);
                        if (pR->b == LH)
                        {
                            //pr->b == pR->pLeft->b =
                        }
                    }
                }                     // difficult case
                else if (pR->b == EH) //modify only the status
                {
                    pR->b = LH;
                    return true;
                }
                else
                {
                    pR->b = EH;
                    return false;
                }
            }
            else
                return 0;
        }
        else
        {
            if (insert(a, pR->pRight))
            {
                
            }
            else
                return 0;
        }
    }

public:
    AVLTree() : pRoot(nullptr)
    {
    }
    ~AVLTree()
    {
        clear();
    }
    void clear()
    {
        clear(pRoot);
    }
    void BFTraserse(std::function<void(T &)> op)
    {
    }
    void NLP_Traverse(std::function<void(T &)> op, Node *pR)
    {
        op(pR->data);
        if (pR->pLeft)
            NLP_Traverse(op, pR->pLeft);
        if (pR->pRight)
            NLP_Traverse(op, pR->pRight);
    }
    // Now we come to delete a node of a binary tree
    // Maximum on the left or minimum on the right ????
    /*

    */
    void remove(const T &a, Node *&pR)
    {
        if (pR)
        {
            if (pR->data == a)
            {
                if (pR->pLeft && pR->pRight)
                {
                    Node *p = pR->pRight;
                    while (p->pLeft)
                    {
                        p = p->pLeft;
                    }
                    pR->data = p->data;
                    remove(p->data, pR->right);
                }
                else if (pR->pLeft)
                {
                    Node *p = pR;
                    pR = pR->pLeft;
                    delete p;
                }
                else if (pR->pRight)
                {
                    Node *p = pR;
                    pR = pR->pRight;
                    delete p;
                }
                else
                {
                    delete pR;
                }
            }
            else if (pR->data > a)
            {
                remove(a, pR->pLeft);
            }
            else
            {
                remove(a, pR->pRight);
            }
        }
        else
        {
            pR = new Node(a);
        }
    }
};