#include "syntax_anl.h"
#include "../lab1/lexi_anl.h"
#include <fstream>

// void doThis(int proId,){
//     switch (proId)
//     {
//     case 1:
        
//         break;
    
//     default:
//         break;
//     }
// }
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
    // currentLevel.push_back(root);
    fa=root;
    std::cout << root->syntaxType<<"po:"<<root->proId << ", prop("+root->places+") &";
    std::cout << root->code+"@"<<endl;
    for(TreeNode *it:root->children){
        currentLevel.push_back(it);
    }
    while (!currentLevel.empty()) {
        // fa=currentLevel.front()->father;
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
        
        if (table[g.first][g.sign].state != EMPTY)
        {
            isLLR1 = false;
            cout<<"conflict2 at "<<to_string(g.first)<<" "<<g.sign<<table[g.first][g.sign].state<<endl;
        }
        table[g.first][g.sign] = a;
        // cout<<"creating move in action "<<g.first<<" "<<g.sign<<endl;
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
            //     if(lr1.isVN(*it.pro.right.rbegin())){
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
                // if(lr1.isVT(*it.pro.right.rbegin())){
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

                for(auto &t:table[its.first]){
                    if(t.second.state == EMPTY) t.second = a;
                }

                // for (auto s : it.symbol)
                // {
                //     if (table[its.first][s].state != EMPTY)
                //     {
                //         cout<<"conflict4 at "<<to_string(its.first)<<" "<<s<<table[its.first][s].state<<" "<<table[its.first][s].num<<endl;
                //         isLLR1 = false;
                //     }
                //     table[its.first][s] = a;
                //     // cout<<"creating back up action "<<its.first<<" "<<s<<endl;

                // }
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
                // cout<<"state"<<state<<" creating error1"<<endl;
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR1;
                }
                cout<<endl;
            }else if(sign=="if"||sign=="while"){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR3;
                }
            }else if(sign=="id"||sign.substr(0,3)=="int"||sign=="("){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR4;
                }
            }else if(sign=="+"||sign=="-"||sign=="*"||sign=="/"){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR5;
                }
            }else if(sign=="="){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR6;
                }
            }else if(sign=="else"||sign=="do"||sign=="then"){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR7;
                }
            }
            else if(sign==";"){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
                    if(t.second.state==EMPTY) t.second.state=ERROR8;
                }
            }else if(sign==")"){
                for(auto &t:table[state]){
                    // cout<<t.second.state<<" ";
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

// PredictTable_LR::~PredictTable_LR() {}

unordered_set<string> PredictTable_LR::getActionHeader()
{
    return actionHeader;
}

unordered_set<string> PredictTable_LR::getGotoHeader()
{
    return gotoheader;
}

bool PredictTable_LR::analyse(deque<symbolTableNode> toSyn){
    // vector<string> l;
    vector<symbolTableNode> sl;
    vector<TreeNode*> ll;
    string str;
    // ifstream infile;
    // infile.open(path);
    for(symbolTableNode it:toSyn){
        string syntaxType;
        string props;
        // ll.push_back();
        switch (it.typeCode)
        {
            
            case IDN:
                syntaxType="id";
                props=it.props;
                // l.push_back("id");
                break;
            case DEC:
                props=to_string(it.prop);
                syntaxType="int10";
                // l.push_back("int10");
                break;
            case OCT:
                props=to_string(it.prop);
                syntaxType="int8";
                // l.push_back("int8");
                break;
            case HEX:
                props=to_string(it.prop);
                syntaxType="int16";
                // l.push_back("int16");
                    break;
            case ILDEC:
            case ILOCT:
            case ILHEX:
                break;
            case FUNC:
                props=it.type;
                syntaxType=it.type;
                // l.push_back(it.type);
                break;
            default:
                props=it.props;
                syntaxType=it.props;
                // l.push_back(it.props);
                break;
        }
        TreeNode *t=new TreeNode(syntaxType,props);
        ll.push_back(t);
    }
    // if(infile.is_open()) cout<<"yes"<<endl;
    // do{
    //     infile >> str;
    //     cout<<str<<endl;
    //     l.push_back(str);

    //     TreeNode* temp=new TreeNode(str);
    //     ll.push_back(temp);

    // } while (!infile.eof());
    // l.push_back("#");
    cout<<"this is vec: ";

    for(TreeNode *t:ll){
        cout<<t->syntaxType<<" ";
    }

    TreeNode* temp=new TreeNode("#",-1);
    ll.push_back(temp);

    // infile.close();
    cout << endl;

    // for (auto i : l){
    //     cout << i;
    // }
    cout << "   procedure:" << endl;
    stack<int> st_state;
    stack<string> st_str;

    stack<TreeNode*> st_tree;

    // stack<string> st_prop;

    st_tree.push(temp);

    // st_prop.push("-");

    st_str.push("#");
    st_state.push(0);
    cout << "[parsing]" << endl;
    cout << "top         input  loopup    action";
    cout << "                                    what" << endl;
    // cout << "------------+--------+----+";
    // cout << "-------------------------------------+-----------" << endl;

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
        // cout<< right << setw(2) <<
        // cout << right << setw(2) << top << " " << str_top << "            ";
        for (stack<string> dump = st_str; !dump.empty(); dump.pop()){
            std::cout <<right << setw(2) << dump.top() << " ";
        }
        cout<<endl;
        cout<<"read:  ";
        for(int c=i;c<ll.size();c++){
            cout<<right << setw(2)<<ll[c]->syntaxType<<" ";
        }
        cout<<endl;
        // cout<<str_top<<"            ";
        cout << s << "      ";
        if (a.state >= EMPTY){   
            a=table[top]["~"];
            str_top="~";
            s="~";
            // i--;
            // l.push_back("~");//i--;
            // st_str.push("~");
            // cout << "doing~"<<a.state <<" "<<table[top]["~"].num<< endl;
            // return false;
        }
        if (a.state >= EMPTY){
            cout << "error1, stopped " <<a.state<< endl;
            errorMsg(a.state);
            return false;
        }
        else if (a.state == STATE){
            cout << 's' << left << setw(2) << a.num << " ";
            st_str.push(s);
            // TreeNode * tt=new TreeNode(s);
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
            // vector<TreeNode*> kids;
            proId=a.num;

            TreeNode* dad=new TreeNode(f.left,a.num);
            dad->proId=a.num;


            cout << "pop  " << left << setw(2) << f.right.size() << "tokens and states";
            for (int k = 0; k < f.right.size(); k++){
                // cout<<"str_tpo "<<str_top<<endl;
                st_str.pop();

                TreeNode* kid=st_tree.top();
                kid->setfather(dad);
                // kids.insert(kids.begin(),kid);

                st_tree.pop();
                
                st_state.pop();
            }
            string gen;
            switch (proId+1){
                case 1://P' -> P
                    // for(TreeNode* tn:dad->children){
                    //     dad->code.append("");
                    //     if(tn->code!=""){
                    //         dad->code.append(tn->code);
                    //     }else{
                    //         dad->code.append(tn->places);
                    //     }
                    // }
                    break;
                case 2://P -> L
                    // for(TreeNode* tn:dad->children){
                    //     // 
                    //     dad->code.append("");
                    //     if(tn->code!=""){
                    //         dad->code.append(tn->code);
                    //     }else{
                    //         dad->code.append(tn->places);
                    //     }
                    // }
                    break;
                case 3://P -> L P
                    // for(TreeNode* tn:dad->children){
                    //     dad->code.append("");
                    //     if(tn->code!=""){
                    //         dad->code.append(tn->code);
                    //     }else{
                    //         dad->code.append(tn->places);
                    //     }
                    // }
                    break;
                case 4://L -> S ;
                    // for(TreeNode* tn:dad->children){
                    //     dad->code.append("");
                    //     if(tn->code!=""){
                    //         dad->code.append(tn->code);
                    //     }else{
                    //         dad->code.append(tn->places);
                    //     }
                    // }
                    break;
                case 5://S -> id = E
                    // gen=((dad->children)[0]->places)+" := ";
                    // if(((dad->children)[2]->code!="")){
                    //     gen+=((dad->children)[2]->code);
                    // }else{
                    //     gen+=((dad->children)[2]->places);
                    // }
                    // dad->code.append(gen);
                    break;
                case 6://S -> if C then S
                    // dad->children[1]->T=label;
                    // label++;

                    // (dad->children)[1]->F=dad->next;
                    // (dad->children)[3]->next=dad->next;

                    // dad->code.append((dad->children)[1]->code);
                    
                    // dad->code.append("\n");
                    // gen=to_string((dad->children)[1]->T)+" : ";
                    // dad->code.append(gen);
                    
                    // dad->code.append((dad->children)[3]->code);
                    break;
                case 7://S -> if C then S else S
                    // dad->children[1]->T=label;
                    // label++;
                    // dad->children[1]->F=label;
                    // label++;

                    // (dad->children)[1]->next=dad->next;
                    // // (dad->children)[1]->T=
                    // (dad->children)[3]->next=(dad->children)[1]->next;

                    // dad->code.append("\n");
                    // dad->code.append((dad->children)[1]->code);

                    // gen=to_string((dad->children)[1]->T)+" : ";
                    // dad->code.append(gen);


                    // dad->code.append((dad->children)[3]->code);

                    // gen=" goto1 "+to_string(dad->next);
                    // dad->code.append(gen);

                    // dad->code.append("\n");
                    // gen=to_string((dad->children)[1]->F)+" : ";
                    // dad->code.append(gen);

                    // dad->code.append("\n");
                    // dad->code.append((dad->children)[5]->code);
                    break;
                case 8://S -> while C do S
                    // dad->begin=label;
                    // label++;
                    // dad->T=label;
                    // label++;
                    // (dad->children)[1]->F=dad->next;

                    // dad->code.append("\n");
                    // gen=to_string(dad->begin)+" : ";
                    // dad->code.append(gen);

                    // dad->code.append((dad->children)[1]->code);

                    // // dad->code.append("\n");
                    // gen=to_string((dad->children)[1]->T)+" : ";
                    // dad->code.append(gen);

                    // dad->code.append((dad->children)[3]->code);

                    // dad->code.append("\n");
                    // gen=" goto "+to_string(dad->begin);
                    // dad->code.append(gen);
                    break;
                case 9://C -> E > E
                    dad->code.append("if ");
                    for(TreeNode* tn:dad->children){
                        if(tn->code!=""){
                            dad->code.append(tn->code);
                        }else{
                            dad->code.append(tn->places);
                        }
                    }
                    // dad->code.append(" goto2 "+to_string(dad->T));
                    // dad->code.append("\n");
                    // dad->code.append("goto3 "+to_string(dad->F));
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
                    // dad->code.append(" goto "+to_string(dad->T));
                    // dad->code.append("\n");
                    // dad->code.append("goto "+to_string(dad->F));
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
                    // dad->code.append(" goto "+to_string(dad->T));
                    // dad->code.append("\n");
                    // dad->code.append("goto "+to_string(dad->F));
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
                //     dad->code.append(" goto "+to_string(dad->T));
                //     dad->code.append("\n");
                //     dad->code.append("goto "+to_string(dad->F));
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
                    // dad->code.append(" goto "+to_string(dad->T));
                    // dad->code.append("\n");
                    // dad->code.append("goto "+to_string(dad->F));
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
                    // dad->code.append(" goto "+to_string(dad->T));

                    // dad->code.append("\n");
                    // dad->code.append("goto "+to_string(dad->F));
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
                    // dad->code.append("+");
                    // if(dad->children[1]->code==""&&dad->children[0]->code==""){

                    // }
                    for(TreeNode* tn:dad->children){
                        dad->code.append("");
                        // if(tn->places!=""){
                        //     dad->code.append(tn->places);
                        // }
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
                    // for(TreeNode* tn:dad->children){
                    //     dad->code.append("");
                    //     dad->code.append(tn->places);
                    //     // if(tn->code!=""){
                    //     //     dad->code.append(tn->code);
                    //     // }else{
                    //     //     dad->code.append(tn->places);
                    //     // }
                    // }
                    // if(dad->children[0]->code==""||dad->children[1]->code==""){
                    //     dad->places=dad->places=
                    // }
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

            // for(int ii=0;ii<st_tree.size();ii++){
                TreeNode *t=st_tree.top();
                // stack<TreeNode*> stt;
                deque<TreeNode*> stt;
                stt.push_back(t);
                int label=0;
                while(!stt.empty()){
                    TreeNode* ttop=stt.back();
                    string gen="";
                    // cout<<"doing "<<ttop->syntaxType<<" pro"<<ttop->proId<<" time:"<<ttop->time;
                    if(ttop->time==0){//从上往下继承
                        // if(ttop->proId+1==7) cout<<"this is S!!"<<endl;
                        switch (ttop->proId+1){
                            case 1://P' -> P
                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 2://P -> L
                                if(ttop->begin!=-1){
                                    ttop->children[0]->begin=ttop->begin;
                                }
                                // for(TreeNode* tn:ttop->children){
                                //     // 
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 3://P -> L P
                                ttop->children[0]->next=label;
                                ttop->children[1]->begin=label;
                                label++;
                                
                                if(ttop->begin!=-1){
                                    cout<<"not equal"<<endl;
                                    ttop->children[0]->begin=ttop->begin;
                                }
                                break;
                            case 4://L -> S ;
                                if(ttop->next!=-1) ttop->children[0]->next=ttop->next;
                                break;
                            case 5://S -> id = E
                                // gen=((ttop->children)[0]->places)+" := ";
                                // if(((ttop->children)[2]->code!="")){
                                //     gen+=((ttop->children)[2]->code);
                                // }else{
                                //     gen+=((ttop->children)[2]->places);
                                // }
                                // ttop->code.append(gen);
                                break;
                            case 6://S -> if C then S
                                // ttop->next=label;
                                // label++;
                                ttop->children[1]->T=label;
                                label++;

                                (ttop->children)[1]->F=ttop->next;
                                (ttop->children)[3]->next=ttop->next;

                                // ttop->code.append((ttop->children)[1]->code);
                                
                                // ttop->code.append("\n");
                                // gen=to_string((ttop->children)[1]->T)+" : ";
                                // ttop->code.append(gen);
                                
                                // ttop->code.append((ttop->children)[3]->code);
                                break;
                            case 7://S -> if C then S else S
                                // ttop->next=label;
                                // label++;
                                ttop->children[1]->T=label;
                                label++;
                                ttop->children[1]->F=label;
                                label++;

                                (ttop->children)[1]->next=ttop->next;
                                (ttop->children)[3]->next=(ttop->children)[1]->next;

                                // ttop->code.append("\n");
                                // ttop->code.append((ttop->children)[1]->code);
                                // cout<<"appending S1: "<<ttop->code<<endl;

                                // gen=to_string((ttop->children)[1]->T)+" : ";
                                // ttop->code.append(gen);


                                // ttop->code.append((ttop->children)[3]->code);

                                // gen=" goto1 "+to_string(ttop->next);
                                // ttop->code.append(gen);

                                // ttop->code.append("\n");
                                // gen=to_string((ttop->children)[1]->F)+" : ";
                                // ttop->code.append(gen);

                                // ttop->code.append("\n");
                                // ttop->code.append((ttop->children)[5]->code);
                                break;
                            case 8://S -> while C do S
                                // ttop->next=label;
                                // label++;
                                ttop->begin=label;
                                label++;
                                (ttop->children)[1]->T=label;
                                label++;
                                (ttop->children)[1]->F=ttop->next;
                                (ttop->children)[3]->next=ttop->begin;
                                break;
                            case 9://C -> E > E
                                break;
                            case 10://C -> E >= E
                                break;
                            case 11://C -> E < E
                                break;
                            case 12://C -> E = E
                                break;
                            case 13://C -> E <= E
                                break;
                            case 14://C -> E <> E
                                break;
                            case 15://E -> T E'
                                break;
                            case 16://E' -> + T E'
                                break;
                            case 17://E' -> - T E'
                                break;
                            case 18://E' -> ~
                                // ttop->code.clear();
                                break;
                            case 19://T -> F T'
                                break;
                            case 20://T' -> * F T'
                                break;
                            case 21://T' -> / F T'
                                break;
                            case 22://T' -> ~
                                // ttop->code.clear();
                                break;
                            case 23://F -> ( E )
                                break;
                            case 24://F -> id
                                break;
                            case 25://F -> int8
                                break;
                            case 26://F -> int10
                                break;
                            case 27://F -> int16
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
                                
                                // ttop->next=ttop->children[0]->next;
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
                                // ttop->next=ttop->children[0]->next;
                                if(ttop->children[0]->code!=""){
                                        ttop->code.append(ttop->children[0]->code);
                                    }else{
                                        ttop->code.append(ttop->children[0]->places);
                                    }
                                break;
                            case 5://S -> id = E
                                // if(ttop->begin!=-1){
                                //     ttop->code.append(to_string(ttop->begin));
                                //     ttop->code+=" :";
                                // }
                                gen=((ttop->children)[0]->places)+" := ";
                                if(((ttop->children)[2]->code!="")){
                                    gen+=((ttop->children)[2]->code);
                                }else{
                                    gen+=((ttop->children)[2]->places);
                                }
                                ttop->code.append(gen);
                                break;
                            case 6://S -> if C then S
                                // ttop->next=label;
                                // label++;
                                // ttop->children[1]->T=label;
                                // label++;

                                // (ttop->children)[1]->F=ttop->next;
                                // (ttop->children)[3]->next=ttop->next;

                                ttop->code.append((ttop->children)[1]->code);
                                
                                ttop->code.append("\n");
                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);
                                
                                ttop->code.append((ttop->children)[3]->code);
                                break;
                            case 7://S -> if C then S else S
                                // ttop->next=label;
                                // label++;
                                // ttop->children[1]->T=label;
                                // label++;
                                // ttop->children[1]->F=label;
                                // label++;

                                // (ttop->children)[1]->next=ttop->next;
                                // // (ttop->children)[1]->T=
                                // (ttop->children)[3]->next=(ttop->children)[1]->next;

                                ttop->code.append("\n");
                                ttop->code.append((ttop->children)[1]->code);
                                // cout<<"appending S2: "<<ttop->code<<endl;

                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);


                                ttop->code.append((ttop->children)[3]->code);

                                gen=" goto "+to_string(ttop->next);
                                ttop->code.append(gen);

                                ttop->code.append("\n");
                                gen=to_string((ttop->children)[1]->F)+" : ";
                                ttop->code.append(gen);

                                // ttop->code.append("\n");
                                ttop->code.append((ttop->children)[5]->code);
                                ttop->code.append("\n");
                                break;
                            case 8://S -> while C do S
                                // ttop->next=label;
                                // label++;
                                // ttop->begin=label;
                                // label++;
                                // ttop->T=label;
                                // label++;
                                // (ttop->children)[1]->F=ttop->next;

                                ttop->code.append("\n");
                                gen=to_string(ttop->begin)+" : ";
                                ttop->code.append(gen);

                                ttop->code.append((ttop->children)[1]->code);

                                // ttop->code.append("\n");
                                gen=to_string((ttop->children)[1]->T)+" : ";
                                ttop->code.append(gen);

                                ttop->code.append((ttop->children)[3]->code);

                                ttop->code.append("\n");
                                gen=" goto "+to_string(ttop->begin);
                                ttop->code.append(gen);
                                ttop->code.append("\n");
                                break;
                            case 9://C -> E > E
                                // ttop->code.append("if ");
                                // for(TreeNode* tn:ttop->children){
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                ttop->code.append("goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 10://C -> E >= E
                                // ttop->code.append("if ");

                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 11://C -> E < E
                                // ttop->code+="if ";
                                // for(TreeNode* tn:ttop->children){
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 12://C -> E = E
                                // ttop->code+="if ";
                                // for(TreeNode* tn:ttop->children){
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 13://C -> E <= E
                            // ttop->code+="if ";
                            //     for(TreeNode* tn:ttop->children){
                            //         if(tn->code!=""){
                            //             ttop->code.append(tn->code);
                            //         }else{
                            //             ttop->code.append(tn->places);
                            //         }
                            //     }
                                ttop->code.append(" goto "+to_string(ttop->T));
                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                ttop->code.append("\n");
                                break;
                            case 14://C -> E <> E
                            // ttop->code+="if ";
                            //     for(TreeNode* tn:ttop->children){
                            //         ttop->code.append("");
                            //         if(tn->code!=""){
                            //             ttop->code.append(tn->code);
                            //         }else{
                            //             ttop->code.append(tn->places);
                            //         }
                            //     }
                                ttop->code.append(" goto "+to_string(ttop->T));

                                ttop->code.append("\n");
                                ttop->code.append("goto "+to_string(ttop->F));
                                break;
                            case 15://E -> T E'
                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 16://E' -> + T E'

                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 17://E' -> - T E'
                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 18://E' -> ~
                                // ttop->code.clear();
                                break;
                            case 19://T -> F T'
                                // ttop->code.append(ttop->children[0]->places);
                                // if(ttop->children[1]->code!=""){
                                //     ttop->code.append(ttop->children[1]->code);
                                // }else{
                                //     ttop->code.append(ttop->children[1]->places);
                                // }
                                break;
                            case 20://T' -> * F T'
                                // for(TreeNode* tn:ttop->children){
                                //     ttop->code.append("");
                                //     if(tn->code!=""){
                                //         ttop->code.append(tn->code);
                                //     }else{
                                //         ttop->code.append(tn->places);
                                //     }
                                // }
                                break;
                            case 21://T' -> / F T'
                                // ttop->code.append(ttop->children[1]->places);
                                // if(ttop->children[2]->code!=""){ 
                                //     ttop->code.append(ttop->children[2]->code);
                                // }else{
                                //     ttop->code.append(ttop->children[2]->places);
                                // }
                                break;
                            case 22://T' -> ~
                                // ttop->code.clear();
                                break;
                            case 23://F -> ( E )
                                // ttop->code.append("(");
                                // ttop->code.append(ttop->children[1]->code);
                                // // for(TreeNode* tn:ttop->children){
                                // //     ttop->code.append("\n");
                                // //     ttop->code.append(tn->code);
                                // // }
                                // ttop->code.append(")");
                                break;
                            case 24://F -> id
                                // ttop->places=ttop->children[0]->places;
                                // ttop->code="";
                                break;
                            case 25://F -> int8
                                // ttop->places=ttop->children[0]->places;
                                // ttop->code="";
                                break;
                            case 26://F -> int10
                                // ttop->places=ttop->children[0]->places;
                                // ttop->code="";
                                break;
                            case 27://F -> int16
                                // ttop->places=ttop->children[0]->places;
                                // ttop->code="";
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
            // }
            
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