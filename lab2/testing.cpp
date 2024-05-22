#include <iostream>
#include <string>
#include "syntax_anl.h"

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
    return 0;
}