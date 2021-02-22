#include <iostream>
#include <string>
#include "bst.h"
using namespace std;
using namespace struktury;

int main()
{
    cout << "Hello world!" << endl;
    BST<string, desc_trait<string> >* bst=new BST<string, desc_trait<string> >({"Bob", "Alice", "David", "Carol", "Eve"});
    cout << *bst;

    cout << bst->exists("Bob") << endl;

    bst->remove("Carol");
    cout << *bst;

//    BST<string>* bst2=new BST<string>(bst);
//    cout << bst2;

    delete bst;
//    delete bst2;
    return 0;
}
