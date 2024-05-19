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
    int proId;
    int time;
    string syntaxType;
    vector<TreeNode*> children;
    TreeNode *father;
    int place;
    // string placeS;
    string code;
    int next;
    int begin;
    int T;
    int F;
    // int 

    TreeNode(){}
    TreeNode(string syn,int pro){
        syntaxType=syn;
        father=nullptr;
        place=-1;
        places="";
        code="";
        proId=pro;
        time=0;
        // father=father;
    }
    TreeNode(string syn,string ps){
        syntaxType=syn;
        father=nullptr;
        places=ps;
        place=-1;
        code="";
        time=0;
        proId=T=F=begin=next=-1;
        // father=father;
    }
    // TreeNode(string syn,int p){
    //     syntaxType=syn;
    //     father=nullptr;
    //     places=to_string(p);
    //     place=p;
    //     code="";
    //     proId=T=F=begin=next=-1;
    // }
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
    // void code(int label){
    //     string gen;
    //     switch (proId){
    //             case 1://P' -> P
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 2://P -> L
    //                 for(TreeNode* tn:father->children){
    //                     // 
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 3://P -> L P
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 4://L -> S ;
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 5://S -> id = E
    //                 gen=((father->children)[0]->places)+" := ";
    //                 if(((father->children)[2]->code!="")){
    //                     gen+=((father->children)[2]->code);
    //                 }else{
    //                     gen+=((father->children)[2]->places);
    //                 }
    //                 father->code.append(gen);
    //                 break;
    //             case 6://S -> if C then S
    //                 father->children[1]->T=label;
    //                 label++;

    //                 (father->children)[1]->F=father->next;
    //                 (father->children)[3]->next=father->next;

    //                 // father->code.append("if ");

    //                 // father->code.append("\n");
    //                 father->code.append((father->children)[1]->code);
                    
    //                 father->code.append("\n");
    //                 gen=to_string((father->children)[1]->T)+" : ";
    //                 father->code.append(gen);
                    
    //                 // father->code.append("\n");
    //                 father->code.append((father->children)[3]->code);
    //                 break;
    //             case 7://S -> if C then S else S
    //                 father->children[1]->T=label;
    //                 label++;
    //                 father->children[1]->F=label;
    //                 label++;

    //                 // father->code.append("if ");
    //                 (father->children)[1]->next=father->next;
    //                 // (father->children)[1]->T=
    //                 (father->children)[3]->next=(father->children)[1]->next;

    //                 // father->code.append("\n");
    //                 father->code.append((father->children)[1]->code);

    //                 gen=to_string((father->children)[1]->T)+" : ";
    //                 father->code.append(gen);

    //                 // father->code.append("then ");

    //                 father->code.append((father->children)[3]->code);

    //                 gen=" goto1 "+to_string(father->next);
    //                 father->code.append(gen);

    //                 // father->code.append("\n");
    //                 // gen=" goto "+to_string(father->next);
    //                 // father->code.append(gen);

    //                 father->code.append("\n");
    //                 gen=to_string((father->children)[1]->F)+" : ";
    //                 father->code.append(gen);

    //                 // father->code.append("\n");
    //                 father->code.append((father->children)[5]->code);
    //                 break;
    //             case 8://S -> while C do S
    //                 father->begin=label;
    //                 label++;
    //                 father->T=label;
    //                 label++;
    //                 (father->children)[1]->F=father->next;

    //                 // father->code.append("\n");
    //                 gen=to_string(father->begin)+" : ";
    //                 father->code.append(gen);

    //                 father->code.append((father->children)[1]->code);

    //                 // father->code.append("\n");
    //                 gen=to_string((father->children)[1]->T)+" : ";
    //                 father->code.append(gen);

    //                 father->code.append((father->children)[3]->code);

    //                 father->code.append("\n");
    //                 gen=" goto "+to_string(father->begin);
    //                 father->code.append(gen);
    //                 break;
    //             case 9://C -> E > E
    //                 father->code.append("if ");
    //                 for(TreeNode* tn:father->children){
    //                     // if(tn->code=="") continue;
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto2 "+to_string(father->T));
    //                 father->code.append("\n");
    //                 father->code.append("goto3 "+to_string(father->F));
    //                 break;
    //             case 10://C -> E >= E
    //                 father->code.append("if ");

    //                 for(TreeNode* tn:father->children){
    //                     // if(tn->code=="") continue;
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto "+to_string(father->T));
    //                 father->code.append("\n");
    //                 father->code.append("goto "+to_string(father->F));
    //                 break;
    //             case 11://C -> E < E
    //                 father->code+="if ";
    //                 for(TreeNode* tn:father->children){
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto "+to_string(father->T));
    //                 father->code.append("\n");
    //                 father->code.append("goto "+to_string(father->F));
    //                 break;
    //             case 12://C -> E = E
    //             father->code+="if ";
    //                 for(TreeNode* tn:father->children){
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto "+to_string(father->T));
    //                 father->code.append("\n");
    //                 father->code.append("goto "+to_string(father->F));
    //                 break;
    //             case 13://C -> E <= E
    //             father->code+="if ";
    //                 for(TreeNode* tn:father->children){
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto "+to_string(father->T));
    //                 father->code.append("\n");
    //                 father->code.append("goto "+to_string(father->F));
    //                 break;
    //             case 14://C -> E <> E
    //             father->code+="if ";
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 father->code.append(" goto "+to_string(father->T));
    //                 // gen="if "+(father->children)[0]->places+" > "+((father->children)[2]->places)+" goto "+to_string(father->T);
    //                 // father->code.append(gen);
    //                 // father->code.append("\n");
    //                 father->code.append("goto "+to_string(father->F));
    //                 break;
    //             case 15://E -> T E'
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 16://E' -> + T E'
    //                 // father->code.append("+");

    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 17://E' -> - T E'
    //                 // father->code.append("-");
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 18://E' -> ~
    //                 father->code.clear();
    //                 break;
    //             case 19://T -> F T'
    //                 father->code.append(father->children[0]->places);
    //                 if(father->children[1]->code!=""){
    //                     father->code.append(father->children[0]->code);
    //                 }else{
    //                     father->code.append(father->children[0]->places);
    //                 }
    //                 // for(TreeNode* tn:father->children){
    //                 //     father->code.append("\n");
    //                 //     father->code.append(tn->code);
    //                 // }
    //                 break;
    //             case 20://T' -> * F T'
    //                 // father->code.append("*");
    //                 for(TreeNode* tn:father->children){
    //                     father->code.append("!");
    //                     if(tn->code!=""){
    //                         father->code.append(tn->code);
    //                     }else{
    //                         father->code.append(tn->places);
    //                     }
    //                 }
    //                 break;
    //             case 21://T' -> / F T'
    //                 // father->code.append("/");
    //                 father->code.append(father->children[1]->places);
    //                 if(father->children[2]->code!="") father->code.append(father->children[2]->code);
    //                 // for(TreeNode* tn:father->children){
    //                 //     father->code.append("\n");
    //                 //     father->code.append(tn->code);
    //                 // }
    //                 break;
    //             case 22://T' -> ~
    //                 father->code.clear();
    //                 break;
    //             case 23://F -> ( E )
    //                 // father->code.append("(");
    //                 father->code.append(father->children[1]->code);
    //                 // for(TreeNode* tn:father->children){
    //                 //     father->code.append("\n");
    //                 //     father->code.append(tn->code);
    //                 // }
    //                 father->code.append(")");
    //                 break;
    //             case 24://F -> id
    //                 father->places=father->children[0]->places;
    //                 father->code="";
    //                 break;
    //             case 25://F -> int8
    //                 father->places=father->children[0]->places;
    //                 father->code="";
    //                 break;
    //             case 26://F -> int10
    //                 father->places=father->children[0]->places;
    //                 father->code="";
    //                 break;
    //             case 27://F -> int16
    //                 father->places=father->children[0]->places;
    //                 father->code="";
    //                 break;
    //             default:
    //                 break;
    //         }
    // }
};
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
    bool analyse(deque<symbolTableNode> tosyn);
};