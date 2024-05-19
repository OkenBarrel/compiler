#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <queue>
#include <stack>
#include "../lab1/lexi_class.h"

using namespace std;

// seta=seta U setb seta未改变返回值为false，否则为true
struct TreeNode{
    string places;
    string syntaxType;
    vector<TreeNode*> children;
    TreeNode *father;
    int place;
    string code;
    TreeNode(){}
    TreeNode(string syn){
        syntaxType=syn;
        father=nullptr;
        place=-1;
        places="";
        code="";
        // father=dad;
    }
    TreeNode(string syn,string ps){
        syntaxType=syn;
        father=nullptr;
        places=ps;
        place=-1;
        code="";
        // father=dad;
    }
    TreeNode(string syn,int p){
        syntaxType=syn;
        father=nullptr;
        places="";
        place=p;
        code="";
        // father=dad;
    }
    bool setfather(TreeNode* f){
        if(f==NULL) return false;
        f->children.insert(f->children.begin(),this);
        father=f;
        // if(res.second){
        //     father=f;
        //     return true;
        // }
        return false;
    }
};

// void printTree(treeNode *root){
//     if(root==nullptr){
//         cout<<"root is empty"<<endl;
//         return;
//     }
//     std::deque<treeNode*> currentLevel;
//     std::deque<treeNode*> nextLevel;
//     treeNode *fa;
//     currentLevel.push_back(root);

//     while (!currentLevel.empty()) {
//         fa=currentLevel.front()->father;
//         for (treeNode* node : currentLevel) {
//             if(fa!=node->father) cout<<"| father: "<<fa->name<<" ";
//             std::cout << node->name << " ";
//             for (treeNode* child : node->children) {
//                 nextLevel.push_back(child);
//             }
//         }
//         std::cout << std::endl;
//         currentLevel.swap(nextLevel);
//         nextLevel.clear();
//     }
// }

struct Production{
    string left;
    vector<string> right;
    Production() {}
    Production(string l, vector<string> r){
        left = l;
        right = r;
    }
    bool operator<(Production it) const{ //重载<,放入map.key{
        if (left != it.left){
            return (left < it.left);
        }else{
            if (right.size() != it.right.size())
                return right.size() < it.right.size();
            else{
                for (int i = 0; i < right.size(); i++){
                    if (right[i] != it.right[i])
                        return right[i] < it.right[i];
                }
            }
        }
        return false;
    }
    bool operator==(Production f) const {//重载==比较
        return (left == f.left && right == f.right);
    }
    string toString(){
        string res=left+" -> ";
        for(auto it=right.begin();it!=right.end();it++){
            res=*it+" ";
        }
        return res;

    }
};

struct item{
    Production pro;    //产生式
    int dot;            //点的位置,-1表示到达最后
    set<string> symbol; //展望串
    item() {}
    item(Production f, int d)
    {
        pro = f;
        dot = d;
    }

    bool operator<(item it) const //重载<,放入set
{
        if (symbol != it.symbol)
        {
            return symbol < it.symbol;
        }
        if (dot != it.dot)
            return dot < it.dot;
        return pro < it.pro;
    }
    bool operator==(item it) const //重载==比较
    {
        return (pro == it.pro && dot == it.dot&& symbol == it.symbol);//&& symbol == it.symbol
    }
};

enum ActionState{
    STATE,
    REVERSE,
    ACCEPT,
    EMPTY,
    ERROR1,//等待比较符号
    ERROR2,//只有)
    ERROR3,//等待赋值或语法结构
    ERROR4,//等待运算数或标识符
    ERROR5,//等待运算符 + -
    ERROR6,//等待赋值=
    ERROR7,//while if语法不全
    ERROR8,//没有;
    ERROR9,//没有）
};

struct Goto{
    int first;
    string sign;
    int next;
};
struct action{
    ActionState state;
    int num = -1;
};
class CFG{
protected:
    unordered_set<string> VT;     //终结符集
    unordered_set<string> VN;     //非终结符集
    string startSymbol;           //开始符号
    map<int, Production> production; //产生式集

public:
    CFG();
    // ~CFG();

    //是终结符
    bool isVT(string str);

    //是非终结符
    bool isVN(string str);

    string getStartSymbol();

    void setStartSymbol(string str);
    unordered_set<string> getVT();
   
    void setVT(unordered_set<string> vt);
    
    unordered_set<string> getVN();
    void setVN(unordered_set<string> vn);
    map<int, Production> getProduction();
    void setProduction(map<int, Production> pr);
    //重载>>
    friend istream &operator>>(istream &in, CFG &cfg);
    //重载<<
    friend ostream &operator<<(ostream &os, CFG &cfg);
};

class CFG_LR1 : public CFG{
private:
    map<string, set<string>> followSet;
    unordered_set<string> emptySet;
    map<string, set<string>> firstSet; // first集
    map<int, set<item>> itemset;       //项目集
    vector<Goto> GOfuction;         // GO映射
public:
    CFG_LR1(string path);
    // ~CFG_LR1();
    map<int, set<item>> getItemset();
    vector<Goto> getGOfuction();
    //计算First集合
    void makeFirst();
    void makeFollow();
    map<string,set<string>> getFollow();

    void getEmptySet(string s);
    //建立项目集
    void createItem();
    //输出项目集
    void outPutItem();
    //求闭包
    void Closure(set<item> &ist);
};

class PredictTable{
private:
    map<int, Production> production; //产生式集
public:
    PredictTable();
    ~PredictTable();
    map<int, Production> getProduction();
    void setProduction(map<int, Production> pr);
    int getFormulaNum(Production f);
};

class PredictTable_LR : public PredictTable{
private:
    unordered_set<string> actionHeader; // action表头
    unordered_set<string> gotoheader;   // goto表头
    map<int, map<string, action>> table;

public:
    PredictTable_LR();
    PredictTable_LR(CFG_LR1 lr1);
    ~PredictTable_LR();
    unordered_set<string> getActionHeader();
    unordered_set<string> getGotoHeader();
    //分析程序
    // bool analyse(vector<string> l);
    //重载<<输出分析表
    friend ostream &operator<<(ostream &os, PredictTable_LR &lrtable);
    //分析串
    bool analyse(string path,deque<symbolTableNode> tosyn);
};