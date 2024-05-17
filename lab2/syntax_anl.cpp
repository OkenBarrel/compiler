#include "syntax_anl.h"


CFG::CFG(){}
// CFG::~CFG(){}

//是终结符
bool CFG::isVT(string str)
{
    return VT.find(str) != VT.end();
}

//是非终结符
bool CFG::isVN(string str)
{
    return VN.find(str) != VN.end();
}

string CFG::getStartSymbol()
{
    return startSymbol;
}
void CFG::setStartSymbol(string str)
{
    startSymbol = str;
}
unordered_set<string> CFG::getVT()
{
    return VT;
}
void CFG::setVT(unordered_set<string> vt)
{
    VT = vt;
}
unordered_set<string> CFG::getVN()
{
    return VN;
}
void CFG::setVN(unordered_set<string> vn)
{
    VN = vn;
}
map<int, Production> CFG::getProduction()
{
    return production;
}
void CFG::setProduction(map<int, Production> pr)
{
    production = pr;
}

//重载>>
istream &operator >>(istream &in, CFG &cfg)
{
    int num;
    in >> num;
    for (int i = 0; i < num; i++)
    {

        string s;
        in >> s;
        cfg.VN.insert(s);
    }
    in >> num;
    for (int i = 0; i < num; i++)
    {
        string s;
        in >> s;
        cfg.VT.insert(s);
    }
    in >> num;
    getchar();
    for (int i = 0; i < num; i++)
    {
        string str;
        string s;
        char ch;
        vector<string> vct;
        in >> s;
        in >> str;

        while (true)
        {
            ch = in.get();
            if (ch != ' ')
                break;
            in >> str;
            if(str=="~"){
                cout<<str<<endl;
            }
            vct.push_back(str);
        }
        Production f(s, vct);
        cfg.production[i] = f;
    }
    string s;
    in >> s;
    cfg.setStartSymbol(s);

    return in;
}
//重载<<
ostream &operator<<(ostream &os, CFG &cfg)
{
    os << " CFG=(VN,VT,P,S)";
    os << "\n  VN:";
    for (auto i : cfg.VN)
    {
        os << ' ' << i;
    }
    os << "\n  VT:";
    for (auto i : cfg.VT)
    {
        os << ' ' << i;
    }
    os << "\n  Production:" << endl;
    for (auto pr : cfg.production)
    {
        os << "     " << pr.first << ':' << ' ' << pr.second.left << " ->";
        for (auto j : pr.second.right)
        {
            os << ' ' << j;
        }
        os << endl;
    }
    os << "  StartSymbol:";
    os << ' ' << cfg.startSymbol << endl;
    return os;
}
