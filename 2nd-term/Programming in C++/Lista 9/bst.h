#include <iostream>
#include <initializer_list>
using namespace std;

namespace struktury
{
    template <typename T> class asc_trait
    {
         public: static int compare(T first, T second)
        {
            if(first==second)
                return 0;
            if(first<second)
                return -1;
            return 1;
        }
    };

    template <typename T> class desc_trait
    {
        public:
        static int compare(T first, T second)
        {
            if(first==second)
                return 0;
            if(first<second)
                return 1;
            return -1;
        }
    };


    template <typename T, class comparison_trait> class BST //tu mogłoby być napisane class T (?)
    {
        class Node
        {
                public:
                T value;
                Node* left_son;
                Node* right_son;

                Node(T value) : value(value)
                {
                    left_son=nullptr;
                    right_son=nullptr;
                }

                Node(Node* node)
                {
                    if(node==nullptr)
                    {
                        //TODO - exception
                    }
                    value=node->value;
                    if(node->left_son==nullptr)
                        this->left_son=nullptr;
                    else
                        this->left_son=new Node(node->left_son);
                    if(node->right_son==nullptr)
                        this->right_son=nullptr;
                    else
                        this->right_son=new Node(node->right_son);
                }

                bool exists(T value)
                {
                    if(comparison_trait::compare(value, this->value)==0)
                        return true;
                    if(comparison_trait::compare(value, this->value)==-1)//(comparison_trait::compare(value, this->value)==-1)
                    {
                        if(this->left_son==nullptr)
                            return false;
                        return this->left_son->exists(value);
                    }
                    else
                    {
                        if(this->right_son==nullptr)
                            return false;
                        return this->right_son->exists(value);
                    }
                }

                void insert (T value)
                {
                    if(comparison_trait::compare(value, this->value)==-1)
                    {
                        if(left_son==nullptr)
                            left_son=new Node(value);
                        else
                            left_son->insert(value);
                    }
                    else
                    {
                        if(right_son==nullptr)
                            right_son=new Node(value);
                        else
                            right_son->insert(value);
                    }
                }

                T minimum()
                {
                    if(left_son==nullptr)
                        return this->value;
                    else
                        return left_son->minimum();
                }

                T maximum()
                {
                    if(right_son==nullptr)
                        return this->value;
                    else
                        return right_son->maximum();
                }

                Node* remove (T value)
                {
                    if(comparison_trait::compare(value, this->value)==0)
                    {
                        if(left_son==nullptr && right_son==nullptr)
                            return nullptr;
                        if(left_son==nullptr)
                            return right_son;
                        if(right_son==nullptr)
                            return left_son;
                        //elsewise
                        T right_son_minimum=(this->right_son)->minimum();
                        this->remove(right_son_minimum);
                        this->value=right_son_minimum;
                    }
                    if(comparison_trait::compare(value, this->value)==-1)
                    {
                        if(left_son==nullptr)
                        {
                            //exception
                        }
                        Node* new_left_son=left_son->remove(value);
                        if(new_left_son!=left_son)
                        {
                            delete left_son;
                            left_son=new_left_son;
                        }
                        return this;
                    }
                    else//(this->value<value)
                    {
                        if(right_son==nullptr)
                        {
                            //TODO - exception
                        }
                        Node* new_right_son=right_son->remove(value);
                        if(new_right_son!=right_son)
                        {
                            delete right_son;
                            right_son=new_right_son;
                        }
                        return this;
                    }
                }
                friend ostream& operator<<(ostream& output, const Node &n)
                {
                    if(n.left_son!=nullptr)
                        output << *(n.left_son);
                    output << n.value << " ";
                    if(n.right_son!=nullptr)
                        output << *(n.right_son);
                    return output;
                }

//                void operator delete(void* node_pointer)
//                {
//
//                }

        };

        Node* root;

        public:
        BST()
        {
            root=nullptr;
        }

        BST(T value)
        {
            root=new Node(value);
        }

        BST(const BST& bst)
        {
            this->root=new Node(bst.root);
        }

        BST(const BST* bst)
        {
            this->root=new Node(bst->root);
        }

        BST(initializer_list<T> l)
        {
            //if(l.begin()==l.end)
//            initializer_list<T>::iterator it;
            for(auto object : l)
            {
                this->insert(object);
            }
        }

        bool exists(T value)
        {
            return this->root->exists(value);
        }

        void insert(T value)
        {
            if(root==nullptr)
                root=new Node(value);
            else
                root->insert(value);
        }

        void remove(T value)
        {
//                    ifcomparison_trait::compare(value, this->value)==0
//                    {
//                        T right_son_minimum=*(this->right_son).minimum();
//                        this->remove(right_son_minimum);
//                        this->value=right_son_minimum;
//                    }
            if(root==nullptr)
            {
                //TODO exception
            }
            if(root->value==value)
            {
                if(root->left_son==nullptr)
                {
                    root=root->right_son;
                    return;
                }
                if(root->right_son==nullptr)
                {
                    root=root->left_son;
                    return;
                }
                T right_son_minimum=root->right_son->minimum();
                root->right_son->remove(right_son_minimum);
                root->value=right_son_minimum;
            }
            else
            {
                root->remove(value);
            }
        }

        friend ostream& operator<< (ostream& output, const BST& bst)
        {
            return output << *(bst.root) << endl;
        }
    };

}
