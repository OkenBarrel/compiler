#pragma once
#include <iostream>

class symbolTableNode{
public:
    std::string type;
    int typeCode;
    std::string props;
    int prop;


    symbolTableNode(std::string t,int c,int p);
    symbolTableNode(std::string t,int c,std::string p);
    symbolTableNode();
    void setType(std::string t,int c);
    void setProp(int p);
    void setProp(std::string p);
    std::string toString();
};
