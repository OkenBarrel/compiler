#include <iostream>
#include <string>
#include "lexi_class.h"

// class symbolTableNode{
// public:
//     std::string type;
//     int typeCode;
//     int prop;

//     symbolTableNode(std::string t,int c,int p):type(t),typeCode(c),prop(p){}
//     symbolTableNode(){
//         type="noen";
//         typeCode=-1;
//         prop=-1;
//     }
//     void setType(std::string t,int c){
//         type=t;
//         typeCode=c;
//     }
//     void setProp(int p){
//         prop=p;
//     }
//     std::string toString(){
//         return "type:"+type+" prop"+std::to_string(prop);
//     }
// };
symbolTableNode::symbolTableNode(std::string t,int c,int p):type(t),typeCode(c),prop(p){}
symbolTableNode::symbolTableNode(){
    type="noen";
    typeCode=-1;
    prop=-1;
}
void symbolTableNode::setType(std::string t,int c){
    type=t;
    typeCode=c;
}
void symbolTableNode::setProp(int p){
    prop=p;
}
std::string symbolTableNode::toString(){
    return "type:"+type+" prop:"+std::to_string(prop);
}