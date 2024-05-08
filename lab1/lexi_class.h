#pragma once
#include <iostream>


class symbolTableNode{
public:
    std::string type;
    int prop;

    symbolTableNode(std::string t,int c,int p);
    symbolTableNode();
    void setType(std::string t,int c);
    void setProp(int p);
    std::string toString();
};