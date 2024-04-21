#include <iostream>
#include "lexi_anl.h"


int main(){
    //结果储存在符号表中，符号表的结构需要便于检索
    //e.g. 频率高的在前面
    std::string in;
    int i=0;
    // while(std::cin>>in){
        
    // }
    // std::cout<<"testing for cpp"<<std::endl;
    // hello();

    while(std::cin>>in){
        while (i<in.length())
        {
            switch(in[0]){
                case IS_DIGIT:
                    scan_digit();
                case IS_LETTER:
                    scan_letter();
                    break;
                //more case
                default: break;
            }
        
        std::cout<<in<<std::endl;

        }
    }
    return 0;
}