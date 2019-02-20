#include "BTree.h"

int main()
{
    int add2[22] = {105, 99, 104, 80, 16, 74, 112, 230, 71, 33, 86, 399, 33, 120, 51, 67, 90, 84, 45, 405, 257, 110};
    for(int i = 0; i<22; i++)
        insertion(add2[i]);

    traversal(root);
    cout<"\n\n";

    int del[10] = {399, 80, 105, 84, 86, 51, 67, 33, 112, 104};

    deletion(399, root);
    deletion(80, root);
    deletion(105, root);
    deletion(84, root);
    deletion(86, root);
    deletion(51, root);
    deletion(67, root);
    deletion(33, root);
    deletion(112, root);
    deletion(104, root);

    traversal(root);
    cout<"\n\n";
}
