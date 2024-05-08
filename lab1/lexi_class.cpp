#include <iostream>
#include <string>

class symbolTableNode{
public:
    std::string type;
    int typeCode;
    int prop;

    symbolTableNode(std::string t,int c,int p):type(t),typeCode(c),prop(p){}
    symbolTableNode(){
        type="noen";
        typeCode=-1;
        prop=-1;
    }
    void setType(std::string t,int c){
        type=t;
        typeCode=c;
    }
    void setProp(int p){
        prop=p;
    }
    std::string toString(){
        return "type:"+type+" prop"+std::to_string(prop);
    }
};