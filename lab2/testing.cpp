// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include "syntax_anl.h"
// #include<storagewmi_provider.mof>
// #include<deque>

using namespace std;

int main()
{
    CFG_LR1 lr1;
    cin >> lr1;
    cout << lr1;
    lr1.createItem();
    lr1.outPutItem();
    PredictTable_LR lr1table(lr1);
    cout << lr1table;
    if (lr1table.analyse())
        cout << "syntax is right!" << endl;
    else
        cout << "syntax is wrong!" << endl;
    return 0;
}