#include <iostream>
#include <string>
#include "lexi_class.h"

symbolTableNode::symbolTableNode(std::string t,int c,int p):type(t),typeCode(c),prop(p){}

symbolTableNode::symbolTableNode(std::string t,int c,std::string p):type(t),typeCode(c),props(p){}

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
void symbolTableNode::setProp(std::string p){
    props=p;
}
std::string symbolTableNode::toString(){
    if(typeCode>5){
        return type+" "+props;
    }
    return type+" "+std::to_string(prop);
}