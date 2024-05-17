#include "syntax_anl.h"
#include <fstream>

bool inVec(vector<string> vec, string e){
    for(string i:vec){
        if(e==i) return true;
    }
    return false;
}

bool inMap(map<int,set<item>>itemSet,set<item> item){
    int notFoundFlag;
    for(auto it :itemSet){
        notFoundFlag=0;
        auto setit=it.second.begin();
        auto itemit=item.begin();
        if (item.size()!=it.second.size()) continue;
        for(auto sit:item){
            if(it.second.find(sit)==it.second.end()){
                notFoundFlag=1;
                break;
            }
        }
        if(notFoundFlag==0) continue;
        else return true;
        // while(setit!=it.second.end()||itemit!=item.end()){
        //     // if(setit!=it.second.end()^itemit!=item.end()) return false;
        //     if(((*setit)==(*itemit))==false) return false;
        //     setit++;
        //     itemit++;
        // }
    }
    return false;
}

bool insertSet(set<string> &seta, set<string> setb)
{
    int asize = seta.size();
    for (auto i : setb)
    {
        seta.insert(i);
    }
    return seta.size() != asize;
}

CFG_LR1::CFG_LR1(string path){
    cout<<"into con"<<endl;
    int num;
    ifstream infile;
    infile.open(path);
    infile >> num;
    for (int i = 0; i < num; i++)
    {

        string s;
        infile >> s;
        VN.insert(s);
    }
    infile>> num;
    for (int i = 0; i < num; i++)
    {
        string s;
        infile >> s;
        VT.insert(s);
    }
    infile >> num;
    string space;
    getline(infile,space);
    for (int i = 0; i < num; i++)
    {
        string str;
        string s;
        char ch;
        vector<string> vct;
        infile >> s;
        infile >> str;

        while (true)
        {
            ch = infile.get();
            if (ch != ' ')
                break;
            infile >> str;
            if(str=="~"){
                cout<<str<<endl;
            }
            vct.push_back(str);
        }
        Production f(s, vct);
        production[i] = f;
    }
    string s;
    infile >> s;
    setStartSymbol(s);
    infile.close();
    // return infile;
}

// CFG_LR1::~CFG_LR1(){}

map<string,set<string>> CFG_LR1::getFollow(){
    return followSet;
}

vector<Goto> CFG_LR1::getGOfuction(){
    return GOfuction;
}

map<int, set<item>> CFG_LR1::getItemset(){
    return itemset;
}
void CFG_LR1::getEmptySet(string s){
    unordered_set<int> trace;
    // unordered_set<string> res;
    stack<string> lookup;

    for(auto pr:getProduction()){
        if(inVec(pr.second.right,"~")){
            lookup.push(pr.second.left);
            emptySet.insert(pr.second.left);
        }
    }
    while(!lookup.empty()){
        string target=lookup.top();
        lookup.pop();
        if(target==getStartSymbol()) continue;
        for(auto pr:getProduction()){
            
            if(!inVec(pr.second.right,target)) continue;
            trace.insert(pr.first);
            emptySet.insert(pr.second.left);
            lookup.push(pr.second.left);
        }
    }
    // lookup.push(s);
    // while(!lookup.empty()){
    //     string tsrget=lookup.top();
    //     lookup.pop();
    //     for(auto pr:getProduction()){
    //         if(pr.second.left==s){
    //             trace.insert(pr.first);
    //             if(inVec(pr.second.right,"~")){
    //                 return true;
    //             }
    //             for(string l :pr.second.right){
    //                 if(isVN(l)){
    //                     lookup.push(l);
    //                 }
    //             }
    //         }
    //     }
    // }
}
void CFG_LR1::makeFollow(){
    set<string> f;
    int flag=0;
    bool changed;
    // f.insert("#");
    for(auto i:getVN()){
        followSet[i]=f;
    }
    followSet[getStartSymbol()].insert("#");
    while(1){
        changed = false;
        for(auto pr:getProduction()){
            for (int i = 0; i < pr.second.right.size(); i++){
                string symbol = pr.second.right[i];
                if (isVN(symbol)) {
                    if (i + 1 < pr.second.right.size()) {
                        auto nextSym = pr.second.right[i + 1];
                        if (isVN(nextSym)) {
                            auto firstNext = firstSet[nextSym];
                            bool hadEpsilon = firstNext.erase("~") > 0;
                            if (insertSet(followSet[symbol], firstNext)) {
                                changed = true;
                            }
                            if (hadEpsilon) {
                                if (insertSet(followSet[symbol], followSet[pr.second.left])) {
                                    changed = true;
                                }
                            }
                        } else {
                            if (followSet[symbol].insert(nextSym).second) {
                                changed = true;
                            }
                        }
                    } else {
                        if (insertSet(followSet[symbol], followSet[pr.second.left])) {
                            changed = true;
                        }
                    }
                }
                // string nextSym=pr.second.right[i+1];
                // if((isVN(symbol) && i==pr.second.right.size()-1||firstSet[nextSym].find("~")!=firstSet[nextSym].end())&&pr.second.left!=symbol){
                //     insertSet(followSet[symbol],firstSet[pr.second.left]);
                //     changed = true;
                // }else if(isVN(symbol) && isVN(nextSym)){
                //     insertSet(followSet[symbol],firstSet[nextSym]);
                //     changed=true;
                //     followSet[symbol].erase("~");
                // }else if(isVN(symbol) && !isVN(nextSym)){
                //     followSet[symbol].insert(nextSym);
                //     changed=true;
                // }
            }
        }
        if(!changed) break;
    }
}
void CFG_LR1::makeFirst(){
    //初始化空集合
    set<string> us;
    for (auto i : getVN())
    {
        firstSet[i] = us;
    }
    //遍历表达式
    while (true)
    {
        bool changeFlag = false;
        for (auto pr : getProduction())
        {
            //如果X->~,把~加入first(X)
            if (pr.second.right[0] == "~")
            {
                if (firstSet[pr.second.left].find("~") != firstSet[pr.second.left].end())
                    continue;
                firstSet[pr.second.left].insert("~");
                changeFlag = true;
            }
            else // X->Y1Y2...
            {
                for (int i = 0; i < pr.second.right.size(); i++)
                {
                    string symbol = pr.second.right[i];
                    if (getVT().find(symbol) != getVT().end())
                    {
                        if (firstSet[pr.second.left].find(symbol) != firstSet[pr.second.left].end())
                            break;
                        firstSet[pr.second.left].insert(symbol);
                        changeFlag = true;
                        break;
                    }
                    else
                    {
                        if (insertSet(firstSet[pr.second.left], firstSet[symbol]))
                            changeFlag = true;
                        if (firstSet[symbol].find("~") == firstSet[symbol].end())
                        {
                            break;
                        }
                        else
                        {
                            if (i == pr.second.right.size() - 1) //最后一个非终结符含有~
                            {
                                if (firstSet[pr.second.left].find("~") != firstSet[pr.second.left].end())
                                    continue;
                                firstSet[pr.second.left].insert("~");
                                changeFlag = true;
                            }
                        }
                    }
                }
            }
        }

        if (changeFlag == false)
            break;
    }
    // for(auto mit:firstSet){
    //     cout<<mit.first<<endl;
    //     cout<<"first: ";
    //     for(auto sit:mit.second){
    //         cout<<string(sit)<<" ";
    //     }
    // }
    // cout<<"\n"<<"first end"<<endl;
}

void CFG_LR1::createItem()
{
    //计算First集合
    makeFirst();
    makeFollow();
    //项目编号
    int index = 0;
    //初始化符号集合
    unordered_set<string> sign = getVT();
    for(auto sit:sign){
        cout<<string(sit)<<" ";
        // cout<<"first: ";
    }
    // cout<<"end"<<endl;
    for (auto vn : getVN())
    {
        sign.insert(vn);
    }
    queue<int> deal; //项目集处理队列
    //开始项目入队列
    set<item> beginist;
    string start = getStartSymbol();
    for (auto pr : getProduction())
    {
        if (pr.second.left == start)
        {
            item i(pr.second, 0);
            i.symbol.insert("#");
            beginist.insert(i);
        }
    }
    Closure(beginist);
    // cout<<"ending Closure 1"<<endl;
    if(inMap(itemset,beginist)==false){
        itemset[index] = beginist;
    }
    deal.push(index); //初始项目集编号入队
    index++;
    while (!deal.empty()) //处理队列不空
    {
        // cout<<"into while"<<endl;
        int num = deal.front();
        set<item> ist = itemset[num];
        //遍历符号
        for (auto v : sign)
        {
            // cout<<"into for1"<<endl;
            set<item> st;
            for (auto it : ist)
            {
                // cout<<"into for2"<<endl;

                if (it.dot != -1) //未到末尾
                {
                    if (it.pro.right[it.dot] == v) //下一个读入的符号是v
                    {
                        item i(it.pro, it.dot + 1);
                        if (i.dot >= i.pro.right.size())
                            i.dot = -1;
                        i.symbol = it.symbol;
                        st.insert(i);
                    }
                }
            }
            // cout<<"after for 2"<<endl;
            Closure(st);
            // cout<<"after closure2"<<endl;
            if (!st.empty())
            {
                int flag = -1;
                for (auto it : itemset)
                {
                    if (st == it.second) flag = it.first;
                }

                if (flag == -1)
                {
                    itemset[index] = st;
                    Goto r;
                    r.first = num;//状态数
                    r.sign = v;
                    r.next = index;
                    GOfuction.push_back(r);
                    deal.push(index);
                    index++;
                }
                else
                {
                    Goto r;
                    r.first = num;
                    r.sign = v;
                    r.next = flag;
                    GOfuction.push_back(r);
                }
            }
        }
        deal.pop();
    }
    cout<<"ending create Item"<<endl;

}

void CFG_LR1::outPutItem()
{
    cout << "\n[LR(1) item set cluster]" << endl;
    for (auto it0 : itemset)
    {
        cout << "  I" << it0.first << ':' << endl;

        for (auto it1 : it0.second)
        {
            cout << "    ";
            cout << it1.pro.left << ' ' << "->";
            for (int i = 0; i < it1.pro.right.size(); i++)
            {
                if (i == it1.dot)
                {
                    cout << " .";
                }
                cout << ' ' << it1.pro.right[i];
            }
            if (it1.dot == -1)
                cout << " .";
            cout << " , ";
            for (auto s : it1.symbol)
            {
                cout << s << ' ';
            }
            cout << endl;
        }
    }
}

void CFG_LR1::Closure(set<item> &ist)
{
    queue<item> que; //处理队列
    for (auto it : ist)
    {
        que.push(it);
    }
    while (!que.empty())
    {
        // cout<<"in closure while"<<endl;
        // int cnt=0;
        // int s=que.size();
        // while(cnt<s){
        //     item ii=que.front();
        //     que.pop();
        //     cout<<ii.pro.toString()<<" "<<ii.dot<<endl;
        //     que.push(ii);
        //     cnt++;
        // }
        item it = que.front();
        if (it.dot != -1)
        {
            // dot后面是非终结符
            string str = it.pro.right[it.dot];
            if (isVN(str))
            {
                for (auto pr : getProduction()){
                    if (pr.second.left == str){
                        item i(pr.second, 0);
                        //如果待归约符号后面为空
                        if (it.dot == it.pro.right.size() - 1)
                        {
                            i.symbol = it.symbol;
                        }
                        else //如果待归约符号后面不为空
                        {
                            string next = it.pro.right[it.dot + 1];
                            if (isVN(next))
                            {
                                if (firstSet[next].find("~") == firstSet[next].end())
                                {
                                    i.symbol = firstSet[next];
                                }
                                else
                                {
                                    for (auto a : firstSet[next])
                                    {
                                        // if (a != "~")
                                            i.symbol.insert(a);
                                    }
                                }
                            }
                            else
                            {
                                i.symbol.insert(next);
                            }
                        }
                        que.push(i);
                        ist.insert(i);
                    }
                }
            }
        }
        que.pop();
    }
    // cout<<"ending Closure"<<endl;
}