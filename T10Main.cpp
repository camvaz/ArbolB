#include "BTree.h"

int main()
{
//    btree Arbol;
//    int add[18] = {95, 10, 34, 87, 56, 99, 12, 23, 50, 40, 60, 54, 33, 20, 91, 17, 18, 94};
//
//    Arbol.insert(95);
//    Arbol.insert(10);
//    Arbol.insert(34);
//    Arbol.insert(87);
//    Arbol.insert(56);
//    Arbol.insert(99);
//    Arbol.insert(12);
//    Arbol.insert(23);
//    Arbol.insert(50);
//    Arbol.insert(40);
//    Arbol.insert(60);
//    Arbol.insert(54);
//    Arbol.insert(33);
//    Arbol.insert(20);
//    Arbol.insert(91);
//    Arbol.insert(17);
//    Arbol.insert(18);
//    Arbol.insert(94);
//
//    Arbol.show();
    insertion(95);
    insertion(10);
    insertion(34);
    insertion(87);
    insertion(56);
    insertion(99);
    insertion(12);
    insertion(23);
    insertion(50);
    insertion(40);
    insertion(60);
    insertion(54);
    insertion(33);
    insertion(20);
    insertion(91);
    insertion(17);
    insertion(18);
    insertion(94);

    traversal(root);
    cout<"\n\n";

    int del[6] = {99, 60, 23, 12, 95, 40};
    deletion(99, root);
    deletion(60, root);
    deletion(23, root);
    deletion(12, root);
    deletion(95, root);
    deletion(40, root);

    traversal(root);
    cout<"\n\n";
}
