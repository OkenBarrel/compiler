#include <iostream>
#include <deque>
#include "lexi_anl.h"
#include "lexi_class.h"


int main(){
    //结果储存在符号表中，符号表的结构需要便于检索
    //e.g. 频率高的在前面
    std::string in;
    std::string token;
    std::deque <symbolTableNode> tokenStream;
    int i=0;
    int type;
    symbolTableNode res;

    while(std::cin>>in){
        if(in=="#") break;        
        type=tellType(in[0]);
        switch(type){
            // token.erase();
            case IS_DIGIT:
                res=scan_digit(in);
                tokenStream.push_back(res);
                std::cout<<res.toString()<<std::endl;
            case IS_LETTER:
                scan_letter();
                break;
            //more case
            default: break;
        }
    
        std::cout<<"in:"<<in<<std::endl;
    }
    
    for(auto it=tokenStream.begin();it!=tokenStream.end();it++){
        std::cout<<"begin"<<std::endl;
        std::cout<<(*it).toString()<<std::endl;
    }
    // hello();
    return 0;
}