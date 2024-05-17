#include "syntax_anl.h"
#include <fstream>

void printTree(treeNode *root){
    if(root==nullptr){
        cout<<"root is empty"<<endl;
        return;
    }
    std::deque<treeNode*> currentLevel;
    std::deque<treeNode*> nextLevel;
    treeNode *fa;
    // currentLevel.push_back(root);
    fa=root;
    cout<<root->name<<endl;
    for(treeNode *it:root->children){
        currentLevel.push_back(it);
    }
    while (!currentLevel.empty()) {
        // fa=currentLevel.front()->father;
        for (treeNode* node : currentLevel) {
            if(fa!=node->father) {
                fa=node->father;
                cout<<"| father: "<<fa->name<<" ";
            }
            std::cout << node->name << " ";
            for (treeNode* child : node->children) {
                nextLevel.push_back(child);
            }
        }
        std::cout << std::endl;
        currentLevel.swap(nextLevel);
        nextLevel.clear();
    }
}

PredictTable::PredictTable(){};
PredictTable::~PredictTable(){};
map<int, Formula> PredictTable::getProduction()
{
    return production;
}
void PredictTable::setProduction(map<int, Formula> pr)
{
    production = pr;
}
int PredictTable::getFormulaNum(Formula f)
{
    for (auto pr : production)
    {
        if (pr.second == f)
            return pr.first;
    }
    return -1; //未找到返回-1
}

PredictTable_LR::PredictTable_LR() {}
PredictTable_LR::~PredictTable_LR(){}

PredictTable_LR::PredictTable_LR(CFG_LR1 lr1)
{
    //是否是LR1
    bool isLLR1 = true;
    //初始化产生式
    setProduction(lr1.getProduction());
    //初始化表头
    actionHeader = lr1.getVT();
    actionHeader.insert("#");
    for (auto vn : lr1.getVN())
    {
        if (vn != lr1.getStartSymbol())
        {
            gotoheader.insert(vn);
        }
    }

    // 初始化表格
    for (auto it : lr1.getItemset())
    {
        action a;
        a.state = ERROR;
        for (auto vt : actionHeader)
        {
            table[it.first][vt] = a;
            // cout<<"init "<<it.first<<" "<<vt<<endl;
        }
        for (auto vn : gotoheader)
        {
            table[it.first][vn] = a;
            // cout<<"init "<<it.first<<" "<<vn<<endl;
        }
    }
    // 赋值ACCET
    for (auto it : lr1.getItemset())
    {
        for (auto it0 : it.second)
        {
            if (it0.dot == -1 && it0.formula.left == lr1.getStartSymbol())
            {
                action i;
                i.state = ACCEPT;
                if (table[it.first]["#"].state != ERROR)
                {
                    cout<<"error1 at "<<to_string(it.first)<<endl;
                    isLLR1 = false;
                }
                table[it.first]["#"] = i;
                // cout<<"creating accept action "<<it.first<<" "<<"#"<<endl;

            }
        }
    }
    //移进项
    for (auto g : lr1.getGOfuction())
    {
        action a;
        a.state = STATE;
        a.num = g.next;
        
        if (table[g.first][g.sign].state != ERROR)
        {
            isLLR1 = false;
            cout<<"error2 at "<<to_string(g.first)<<" "<<g.sign<<table[g.first][g.sign].state<<endl;
        }
        table[g.first][g.sign] = a;
        // cout<<"creating move in action "<<g.first<<" "<<g.sign<<endl;
    }
    //归约项 make action table
    for (auto its : lr1.getItemset())
    {
        for (auto it : its.second)
        {
            if (it.dot == -1 && it.formula.left != lr1.getStartSymbol())
            {
                action a;
                a.state = REVERSE;
                a.num = getFormulaNum(it.formula);
                map<string,set<string>> fo=lr1.getFollow();
                set<string> ss=fo[it.formula.left];
            //     if(lr1.isVN(*it.formula.right.rbegin())){
            //         for(auto i:ss){
            //             cout<<"creating action "<<its.first<<" "<<i<<endl;
            //             if (table[its.first][i].state != ERROR){
            //                 cout<<"error2 at "<<to_string(its.first)<<" "<<i<<endl;
            //                 isLLR1 = false;
            //             }
            //             table[its.first][i] = a;
            //         }
            //     }
            //     else{
            //         // cout<<"into setting vt"<<endl;
            //         unordered_set<string> vt=lr1.getVT();
            //         vt.insert("#");
            //         for(string str:vt){
            //             if (table[its.first][str].state != ERROR){
            //                 cout<<"error3 at "<<to_string(its.first)<<" "<<str<<endl;
            //                 isLLR1 = false;
            //             }
            //             table[its.first][str] = a;
            //         }
                // }
                // if(lr1.isVT(*it.formula.right.rbegin())){
                //     unordered_set<string> vt=lr1.getVT();
                //     vt.insert("#");
                //     for(string str:vt){
                //         if (table[its.first][str].state != ERROR){
                //             cout<<"error3 at "<<to_string(its.first)<<" "<<str<<endl;
                //             isLLR1 = false;
                //         }
                //         table[its.first][str] = a;
                //     }
                // }
                for (auto s : it.symbol)
                {
                    if (table[its.first][s].state != ERROR)
                    {
                        cout<<"error4 at "<<to_string(its.first)<<" "<<s<<table[its.first][s].state<<" "<<table[its.first][s].num<<endl;
                        isLLR1 = false;
                    }
                    table[its.first][s] = a;
                    // cout<<"creating back up action "<<its.first<<" "<<s<<endl;

                }
            }
        }
    }

    if (isLLR1)
    {
        cout << "\nsyntax is LR(1) !\n";
    }
    else
    {
        cout << "\nsyntax is not LR(1) !\n";
    }
}

// PredictTable_LR::~PredictTable_LR() {}

unordered_set<string> PredictTable_LR::getActionHeader()
{
    return actionHeader;
}

unordered_set<string> PredictTable_LR::getGotoHeader()
{
    return gotoheader;
}

bool PredictTable_LR::analyse(string path){
    vector<string> l;
    vector<treeNode*> ll;
    string str;
    ifstream infile;
    infile.open(path);
    if(infile.is_open()) cout<<"yes"<<endl;
    do{
        infile >> str;
        cout<<str<<endl;
        l.push_back(str);

        treeNode* temp=new treeNode(str);
        ll.push_back(temp);

    } while (!infile.eof());
    l.push_back("#");

    treeNode* temp=new treeNode("#");
    ll.push_back(temp);

    infile.close();
    cout << endl;

    for (auto i : l){
        cout << i;
    }
    cout << "   procedure:" << endl;
    stack<int> st_state;
    stack<string> st_str;

    stack<treeNode*> st_tree;

    st_tree.push(temp);

    st_str.push("#");
    st_state.push(0);
    cout << "[parsing]" << endl;
    cout << "top         input  loopup    action";
    cout << "                                    what" << endl;
    // cout << "------------+--------+----+";
    // cout << "-------------------------------------+-----------" << endl;

    for (int i = 0; i < l.size(); i++){
        string s = l[i];
        int top = st_state.top();
        action a = table[top][s];
        string str_top = st_str.top();
        for (stack<int> dump = st_state; !dump.empty(); dump.pop()){
            std::cout <<right << setw(2) << dump.top() << " ";
        }
        cout<<endl;
        // cout<< right << setw(2) <<
        // cout << right << setw(2) << top << " " << str_top << "            ";
        for (stack<string> dump = st_str; !dump.empty(); dump.pop()){
            std::cout <<right << setw(2) << dump.top() << " ";
        }
        cout<<endl;
        cout<<"read:  ";
        for(int c=i;c<l.size();c++){
            cout<<right << setw(2)<<l[c]<<" ";
        }
        cout<<endl;
        // cout<<str_top<<"            ";
        cout << s << "      ";
        if (a.state == ERROR){   
            a=table[top]["~"];
            str_top="~";
            s="~";
            // i--;
            // l.push_back("~");//i--;
            // st_str.push("~");
            // cout << "doing~"<<a.state <<" "<<table[top]["~"].num<< endl;
            // return false;
        }
        if (a.state == ERROR){
            cout << "error1, stopped" << endl;
            return false;
        }
        else if (a.state == STATE){
            cout << 's' << left << setw(2) << a.num << " ";
            st_str.push(s);
            treeNode * tt=new treeNode(s);
            st_tree.push(tt);
            st_state.push(a.num);
            if(s=="~") i--;
            cout << "push  " << left << setw(2) << a.num << ' ' << s << endl;
            continue;
        }else if (a.state == REVERSE)
        {
            cout << 'r' << left << setw(2) << a.num << " ";
            Formula f = getProduction()[a.num];

            treeNode* dad=new treeNode(f.left);


            cout << "pop  " << left << setw(2) << f.right.size() << "tokens and states";
            for (int k = 0; k < f.right.size(); k++){
                // cout<<"str_tpo "<<str_top<<endl;
                st_str.pop();

                treeNode* kid=st_tree.top();
                kid->setfather(dad);

                st_tree.pop();
                
                st_state.pop();
            }
            // reverse(dad->children.begin(),dad->children.end());
            top = st_state.top();
            cout << " push:";
            int x = table[top][f.left].num;
            if (x == -1){   
                // x=table[top]["~"].num
                cout << "error2, stopped" << endl;
                return false;
            }
            st_state.push(x);
            cout << left << setw(2) << x << ' ';
            st_str.push(f.left);

            st_tree.push(dad);

            cout << f.left << "       ";
            cout << f.left << " ->";
            for (auto i : f.right){
                cout << ' ' << i;
            }
            i--;
            cout << endl;
        }
        else if (a.state == ACCEPT){
            cout << "acc"
                 << " ";
            cout << "successfully accept!" << endl;

            for(int ii=0;ii<st_tree.size();ii++){
                treeNode *t=st_tree.top();
                printTree(t);
                st_tree.pop();
                cout<<"next"<<endl;
            }
            
            // cout << "------------+--------+----+";
            // cout << "-------------------------------------+-----------" << endl;
            // cout << " end!" << endl;
            return true;
        }
    }

    return false;
}

ostream &operator<<(ostream &os, PredictTable_LR &lrtable)
{
    os << "\nLR(1) Tabl" << endl;
    os << "  Action:" << endl;
    os << "      ";

    for (auto vt : lrtable.actionHeader){
        os.setf(ios::left);
        os.width(6);
        os << vt;
    }
    os << endl;

    for (int i = 0; i < lrtable.table.size(); i++){
        os.setf(ios::left);
        os.width(5);
        os << i << ':';
        for (auto ah : lrtable.actionHeader)
        {
            action a = lrtable.table[i][ah];
            if (a.state == ACCEPT)
            {
                os.setf(ios::left);
                os.width(6);
                os << "acc";
            }
            else if (a.state == REVERSE)
            {
                string s = "r" + to_string(a.num);
                os.setf(ios::left);
                os.width(6);
                os << s;
            }
            else if (a.state == STATE)
            {
                string s = "s" + to_string(a.num);
                os.setf(ios::left);
                os.width(6);
                os << s;
            }
            else
            {
                os << "      ";
            }
        }
        os << endl;
    }
    os << "\n  Goto:" << endl;
    os << "      ";
    for (auto gh : lrtable.gotoheader)
    {
        os.setf(ios::left);
        os.width(6);
        os << gh;
    }
    os << endl;
    for (int i = 0; i < lrtable.table.size(); i++)
    {
        os.setf(ios::left);
        os.width(5);
        os << i << ':';
        for (auto gh : lrtable.gotoheader)
        {
            action a = lrtable.table[i][gh];
            if (a.num != -1)
            {
                os.setf(ios::left);
                os.width(6);
                os << a.num;
            }
            else
            {
                os << "      ";
            }
        }
        os << endl;
    }
    return os;
}