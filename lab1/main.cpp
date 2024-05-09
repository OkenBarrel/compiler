#include <iostream>
#include "lexi_anl.h"
#include "lexi_class.h"


int main(){
    //结果储存在符号表中，符号表的结构需要便于检索
    //e.g. 频率高的在前面
    std::string in;
    std::string token;
    int i=0;
    int type;
    symbolTableNode res;

    while(std::cin>>in){
        
        type=tellType(in[0]);
        switch(type){
            // token.erase();
            case IS_DIGIT:
                res=scan_digit(in);
                std::cout<<res.toString()<<std::endl;
            case IS_LETTER:
                scan_letter();
                break;
            //more case
            default: break;
        }
    
        std::cout<<"in:"<<in<<std::endl;
    }
    
    // hello();
    return 0;
}