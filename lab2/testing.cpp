// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include "syntax_anl.h"
// #include<storagewmi_provider.mof>
// #include<deque>

using namespace std;

int main()
{
    CFG_LR1 lr1("production.txt");
    // cin >> lr1;
    cout << lr1;
    lr1.createItem();
    lr1.outPutItem();
    PredictTable_LR lr1table(lr1);
    cout << lr1table;
    if (lr1table.analyse("testing.txt"))
        cout << "syntax is right!" << endl;
    else
        cout << "syntax is wrong!" << endl;

    // treeNode* root = new treeNode("root");
    // treeNode* child1 = new treeNode("child1");
    // treeNode* child2 = new treeNode("child2");
    // treeNode* child3 = new treeNode("child3");
    // treeNode* grandchild1 = new treeNode("grandchild1");
    // treeNode* grandchild2 = new treeNode("grandchild2");
    // treeNode* grandchild3 = new treeNode("grandchild2");

    // // Build the tree
    // child1->setfather(root);
    // child2->setfather(root);
    // child3->setfather(root);
    // grandchild1->setfather(child1);
    // grandchild1->setfather(child3);
    // grandchild2->setfather(child2);

    // // Print the tree structure
    // std::cout << "Tree structure:" << std::endl;
    // printTree(root);

    // // Clean up
    // delete root;
    // delete child1;
    // delete child2;
    // delete child3;
    // delete grandchild1;
    // delete grandchild2;
    return 0;
}