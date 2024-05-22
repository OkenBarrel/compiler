#include "syntax_anl.h"
#include "../lab1/lexi_anl.h"
#include <fstream>

void errorMsg(int e){
    cout<<"error: ";
    switch (e)
    {
    case ERROR1:
        cout<<"need relop (>, <, <= .etc)";
        break;
    case ERROR2:
        cout<<"( ) doesn't match, witout (";
        break;
    case ERROR3:
        cout<<"need assignment(id=E) or control(if, while)";
        break;
    case ERROR4:
        cout<<"need id or intx as oprand";
        break;
    case ERROR5:
        cout<<"need op (+, - .etc)";
        break;
    case ERROR6:
        cout<<"need = to finish assighment";
        break;
    case ERROR7:
        cout<<"while or if not finished";
        break;
    case ERROR8:
        cout<<"need ; to finish";
        break;
    case ERROR9:
        cout<<"( ) doesn't match, need )";
        break;
    default:
        break;
    }
    cout<<endl;
}

void printTree(TreeNode *root){
    if(root==nullptr){
        cout<<"root is empty"<<endl;
        return;
    }
    std::deque<TreeNode*> currentLevel;
    std::deque<TreeNode*> nextLevel;
    TreeNode *fa;
    fa=root;
    std::cout << root->syntaxType<<"po:"<<root->proId << ", prop("+root->places+") &";
    std::cout << root->code+"@"<<endl;
    for(TreeNode *it:root->children){
        currentLevel.push_back(it);
    }
    while (!currentLevel.empty()) {
        for (TreeNode* node : currentLevel) {
            if(fa!=node->father) {
                fa=node->father;
                cout<<"| father "<<fa->syntaxType<<": ";
            }
            std::cout << node->syntaxType<<"po:"<<node->proId << ", prop("+node->places+") &";
            std::cout << node->code+"@";
            for (TreeNode* child : node->children) {
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
map<int, Production> PredictTable::getProduction()
{
    return production;
}
void PredictTable::setProduction(map<int, Production> pr)
{
    production = pr;
}
int PredictTable::getFormulaNum(Production f)
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
        a.state = EMPTY;
        for (auto vt : actionHeader)
        {
            table[it.first][vt] = a;
        }
        for (auto vn : gotoheader)
        {
            table[it.first][vn] = a;
        }
    }
    // 赋值ACCET
    for (auto it : lr1.getItemset())
    {
        for (auto it0 : it.second)
        {
            if (it0.dot == -1 && it0.pro.left == lr1.getStartSymbol())
            {
                action i;
                i.state = ACCEPT;
                if (table[it.first]["#"].state != EMPTY)
                {
                    cout<<"conflict1 at "<<to_string(it.first)<<endl;
                    isLLR1 = false;
                }
                table[it.first]["#"] = i;

            }
        }
    }
    //移进项
    for (auto g : lr1.getGOfuction())
    {
        action a;
        a.state = STATE;
        a.num = g.next;
        
        if (table[g.first][g.sign].state != EMPTY)
        {
            isLLR1 = false;
            cout<<"conflict2 at "<<to_string(g.first)<<" "<<g.sign<<table[g.first][g.sign].state<<endl;
        }
        table[g.first][g.sign] = a;
    }
    //归约项 make action table
    for (auto its : lr1.getItemset())
    {
        for (auto it : its.second)
        {
            if (it.dot == -1 && it.pro.left != lr1.getStartSymbol())
            {
                action a;
                a.state = REVERSE;
                a.num = getFormulaNum(it.pro);
                map<string,set<string>> fo=lr1.getFollow();
                set<string> ss=fo[it.pro.left];
                for(auto &t:table[its.first]){
                    if(t.second.state == EMPTY) t.second = a;
                }

            }
        }
    }
    // 错误处理
    for(auto its:lr1.getItemset()){
        int state=its.first;
        for(item it:its.second){
            int dot=it.dot;
            if(dot==-1) continue;
            string sign=it.pro.right[dot];
            if(lr1.isVN(sign)) continue;
            if(table[state][")"].state==EMPTY) table[state][")"].state=ERROR2;
            if(sign=="<" || sign==">"|| sign=="<>"|| sign==">="|| sign=="<="||sign=="=="){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR1;
                }
                cout<<endl;
            }else if(sign=="if"||sign=="while"){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR3;
                }
            }else if(sign=="id"||sign.substr(0,3)=="int"||sign=="("){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR4;
                }
            }else if(sign=="+"||sign=="-"||sign=="*"||sign=="/"){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR5;
                }
            }else if(sign=="="){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR6;
                }
            }else if(sign=="else"||sign=="do"||sign=="then"){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR7;
                }
            }
            else if(sign==";"){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR8;
                }
            }else if(sign==")"){
                for(auto &t:table[state]){
                    if(t.second.state==EMPTY) t.second.state=ERROR9;
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


unordered_set<string> PredictTable_LR::getActionHeader()
{
    return actionHeader;
}

unordered_set<string> PredictTable_LR::getGotoHeader()
{
    return gotoheader;
}

bool PredictTable_LR::analyse(deque<symbolTableNode> toSyn){
    vector<symbolTableNode> sl;
    vector<TreeNode*> ll;
    string str;
    for(symbolTableNode it:toSyn){
        string syntaxType;
        string props;
        switch (it.typeCode)
        {
            
            case IDN:
                syntaxType="id";
                props=it.props;
                break;
            case DEC:
                props=to_string(it.prop);
                syntaxType="int10";
                break;
            case OCT:
                props=to_string(it.prop);
                syntaxType="int8";
                break;
            case HEX:
                props=to_string(it.prop);
                syntaxType="int16";
                    break;
            case ILDEC:
            case ILOCT:
            case ILHEX:
                break;
            case FUNC:
                props=it.type;
                syntaxType=it.type;
                break;
            default:
                props=it.props;
                syntaxType=it.props;
                break;
        }
        TreeNode *t=new TreeNode(syntaxType,props);
        ll.push_back(t);
    }

    cout<<"this is vec: ";

    for(TreeNode *t:ll){
        cout<<t->syntaxType<<" ";
    }

    TreeNode* temp=new TreeNode("#",-1);
    ll.push_back(temp);

    cout << endl;

    cout << "   procedure:" << endl;
    stack<int> st_state;
    stack<string> st_str;

    stack<TreeNode*> st_tree;

    st_tree.push(temp);


    st_str.push("#");
    st_state.push(0);
    cout << "[parsing]" << endl;
    cout << "top         input  loopup    action";
    cout << "                                    what" << endl;

    for (int i = 0; i < ll.size(); i++){
        int label=0;
        string s = ll[i]->syntaxType;
        int top = st_state.top();
        action a = table[top][s];
        string str_top = st_str.top();
        for (stack<int> dump = st_state; !dump.empty(); dump.pop()){
            std::cout <<right << setw(2) << dump.top() << " ";
        }
        cout<<endl;
        for (stack<string> dump = st_str; !dump.empty(); dump.pop()){
            std::cout <<right << setw(2) << dump.top() << " ";
        }
        cout<<endl;
        cout<<"read:  ";
        for(int c=i;c<ll.size();c++){
            cout<<right << setw(2)<<ll[c]->syntaxType<<" ";
        }
        cout<<endl;
        cout << s << "      ";
        if (a.state >= EMPTY){   
            a=table[top]["~"];
            str_top="~";
            s="~";
        }
        if (a.state >= EMPTY){
            cout << "error1, stopped " <<a.state<< endl;
            errorMsg(a.state);
            return false;
        }
        else if (a.state == STATE){
            cout << 's' << left << setw(2) << a.num << " ";
            st_str.push(s);
            TreeNode * tt=new TreeNode(s,"-");
            if(s!="~"){
                tt=ll[i];
            }
            st_tree.push(tt);
            st_state.push(a.num);
            if(s=="~") i--;
            cout << "push  " << left << setw(2) << a.num << ' ' << s << endl;
            continue;
        }else if (a.state == REVERSE)
        {
            cout << 'r' << left << setw(2) << a.num << " ";
            Production f = getProduction()[a.num];

            int proId;
            proId=a.num;

            TreeNode* dad=new TreeNode(f.left,a.num);
            dad->proId=a.num;


            cout << "pop  " << left << setw(2) << f.right.size() << "tokens and states";
            for (int k = 0; k < f.right.size(); k++){
                st_str.pop();

                TreeNode* kid=st_tree.top();
                kid->setfather(dad);

                st_tree.pop();
                
                st_state.pop();
            }
            string gen;
            switch (proId+1){
                case 9://C -> E > E
                    dad->code.append("if ");
                    for(TreeNode* tn:dad->children){
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 10://C -> E >= E
                    dad->code.append("if ");

                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 11://C -> E < E
                    dad->code+="if ";
                    for(TreeNode* tn:dad->children){
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 12://C -> E = E
                    dad->code+="if ";
                    for(TreeNode* tn:dad->children){
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 13://C -> E <= E
                dad->code+="if ";
                    for(TreeNode* tn:dad->children){
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 14://C -> E <> E
                    dad->code+="if ";
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 15://E -> T E'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 16://E' -> + T E'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 17://E' -> - T E'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->places!=""){

                        }
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            // dad->place=
                            dad->code.append(tn->places);
                        }
                    }
                    
                    break;
                case 18://E' -> ~
                    dad->code.clear();
                    break;
                case 19://T -> F T'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 20://T' -> * F T'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 21://T' -> / F T'
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 22://T' -> ~
                    dad->code.clear();
                    break;
                case 23://F -> ( E )

                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    break;
                case 24://F -> id
                    dad->places=dad->children[0]->places;
                    dad->code="";
                    break;
                case 25://F -> int8
                    dad->places=dad->children[0]->places;
                    dad->code="";
                    break;
                case 26://F -> int10
                    dad->places=dad->children[0]->places;
                    dad->code="";
                    break;
                case 27://F -> int16
                    dad->places=dad->children[0]->places;
                    dad->code="";
                    break;
                default:
                    break;
            }
            top = st_state.top();
            cout << " push:";
            int x = table[top][f.left].num;
            if (x == -1){   
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

                TreeNode *t=st_tree.top();
                deque<TreeNode*> stt;
                stt.push_back(t);
                int label=0;
                while(!stt.empty()){
                    TreeNode* ttop=stt.back();
                    string gen="";
                    if(ttop->time==0){//从上往下继承
                        switch (ttop->proId+1){
                            case 2://P -> L
                                if(ttop->begin!=-1){
                                    ttop->children[0]->begin=ttop->begin;
                                }
                                break;
                            case 3://P -> L P
                                ttop->children[0]->next=label;
                                ttop->children[1]->begin=label;
                                label++;
                                
                                if(ttop->begin!=-1){
                                    cout<<"not equal"<<endl;
                                    ttop->children[0]->begin=ttop->begin;
                                    ttop->begin=-1;
                                }
                                break;
                            case 4://L -> S ;
                                if(ttop->next!=-1) ttop->children[0]->next=ttop->next;
                                break;
                            case 6://S -> if C then S
                                ttop->children[1]->T=label;
                                label++;

                                (ttop->children)[1]->F=ttop->next;
                                (ttop->children)[3]->next=ttop->next;
                                break;
                            case 7://S -> if C then S else S
                                ttop->children[1]->T=label;
                                label++;
                                ttop->children[1]->F=label;
                                label++;

                                (ttop->children)[1]->next=ttop->next;
                                (ttop->children)[3]->next=(ttop->children)[1]->next;
                                break;
                            case 8://S -> while C do S
                                ttop->begin=label;
                                label++;
                                (ttop->children)[1]->T=label;
                                label++;
                                (ttop->children)[1]->F=ttop->next;
                                cout<<"8: "<<(ttop->children)[1]->F<<" "<<ttop->next<<endl;
                                (ttop->children)[3]->next=ttop->begin;
                                break;
                            default:
                                break;
                        }
                        ttop->time+=1;
                    }else if(ttop->time==1){//从下往上
                        switch (ttop->proId+1){
                            case 1://P' -> P
                                for(TreeNode* tn:ttop->children){
                                    ttop->code.append("");
                                    if(tn->code!=""){
                                        ttop->code.append(tn->code);
                                    }else{
                                        ttop->code.append(tn->places);
                                    }
                                }
                                break;
                            case 2://P -> L
                                for(TreeNode* tn:ttop->children){
                                    // 
                                    ttop->code.append("");
                                    if(tn->code!=""){
                                        ttop->code.append(tn->code);
                                    }else{
                                        ttop->code.append(tn->places);
                                    }
                                }
                                break;
                            case 3://P -> L P
                                
                                for(TreeNode* tn:ttop->children){
                                    ttop->code.append("");
                                    if(tn->code!=""){
                                        ttop->code.append(tn->code);
                                    }else{
                                        ttop->code.append(tn->places);
                                    }
                                }
                                break;
                            case 4://L -> S ;
                                if(ttop->begin!=-1){
                                    ttop->code.append(to_string(ttop->begin));
                                    ttop->code+=":11 ";
                                }
                                if(ttop->children[0]->code!=""){
                                        ttop->code.append(ttop->children[0]->code);
                                    }else{
                                        ttop->code.append(ttop->children[0]->places);
                                    }
                                break;
                            case 5://S -> id = E
                                
                                gen=((ttop->children)[0]->places)+" := ";
                                if(((ttop->children)[2]->code!="")){
                                    gen+=((ttop->children)[2]->code);
                                }else{
                                    gen+=((ttop->children)[2]->places);
                                }
                                ttop->code.append(gen);
                                break;
                            case 6://S -> if C then S
                                ttop->code.append((ttop->children)[1]->code);
                                
                                ttop->code.append("\n");
                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);
                                
                                ttop->code.append((ttop->children)[3]->code);
                                break;
                            case 7://S -> if C then S else S
                                ttop->code.append("\n");
                                ttop->code.append((ttop->children)[1]->code);

                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);


                                ttop->code.append((ttop->children)[3]->code);

                                gen=" goto "+to_string(ttop->next);
                                ttop->code.append(gen);

                                ttop->code.append("\n");
                                gen=to_string((ttop->children)[1]->F)+" : ";
                                ttop->code.append(gen);

                                ttop->code.append((ttop->children)[5]->code);
                                ttop->code.append("\n");
                                break;
                            case 8://S -> while C do S
                                ttop->code.append("\n");
                                gen=to_string(ttop->begin)+" : ";
                                ttop->code.append(gen);

                                ttop->code.append((ttop->children)[1]->code);

                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);

                                ttop->code.append((ttop->children)[3]->code);

                                ttop->code.append("\n");
                                gen=" goto "+to_string(ttop->begin);
                                ttop->code.append(gen);
                                ttop->code.append("\n");
                                break;
                            case 9://C -> E > E
                                ttop->code.append("goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 10://C -> E >= E
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 11://C -> E < E
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 12://C -> E = E
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 13://C -> E <= E
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 14://C -> E <> E
                                ttop->code.append(" goto "+to_string(ttop->T));

                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                break;
                            default:
                                break;
                        }
                        stt.pop_back();
                        ttop->time+=1;
                    }
                    if(ttop->time==1&&ttop->children.size()!=0){
                        for(TreeNode* kid:ttop->children){
                            stt.push_back(kid);
                        }
                    }
                    
                }
                printTree(t);
                st_tree.pop();
                cout<<"next"<<endl;
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
            else if(a.state>=ERROR1){
                string s = "e"+to_string(a.state-3);
                os.setf(ios::left);
                os.width(6);
                os << s;
            }else
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