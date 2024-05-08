#include <iostream>
#include <tuple>
#include <string>
#include "lexi_class.h"
// #include <lexi_anl.h>
enum TOKEN{
    DEC,
    ILEDC,
    OCT,
    ILOCT,
    HEX,
    ILHEX,
};
#define IS_LETTER 0
#define IS_DIGIT 1
#define DEC 2
#define ILDEC -2
#define OCT 3
#define ILOCT -3
#define HEX 4
#define ILHEX -4



void hello(){
    std::cout<<"heeeeellooooo!!"<<std::endl;
}
symbolTableNode scan_digit(std::string in){
    int state=0;
    int i=0;
    // std::tuple<TOKEN,int> result;
    std::string t;
    int code;
    int prop;
    while(state>=0){
        // std::cout<<"state="<<state<<"letter="<<in[i]<<std::endl;
        if(state==0){
            if(in[i]=='0'){
                state=1;
                i++;
                continue;
            }else if(in[i]>='1' && in[1]<='9'){
                state=2;
                i++;
                continue;
            }
            std::cout<<"error1"<<std::endl;
        }else if(state==1){
            if(i>=in.size()){
                state=3;
            }else if(in[i]=='x' || in[i]=='X'){
                state=6;
                i++;
                continue;
            }else if(in[i]>='0' && in[i]<='7'){
                state=4;
                i++;
                continue;
            }else if(in[i]>='8'&&in[i]<='9'){
                state=9;
                continue;
            }
            std::cout<<"error2"<<std::endl;
        }else if(state==2){
            if(i==in.size()){
                state=3;
                continue;
            }
            if(in[i]>='0'&&in[i]<='9'){
                i++;
                continue;
            }else if(in[i]>='a'&&in[i]<='z'||in[i]>='A'&&in[i]<='Z'){
                state=11;
                continue;
            }
            std::cout<<"error5"<<std::endl;
        }
        else if(state==3){
            t="DEC";
            code=DEC;
            prop=std::stoi(in.substr(0,i));
            // return DEC;
        }else if(state==4){
            
            if(i==in.size()){
                state=5;
            }else if(in[i]>='0' && in[i]<='7'){
                i++;
                continue;
            }else if(in[i]>='8'&&in[i]<='9'||in[i]>='a'&&in[i]<='z'||in[i]>='A'&&in[i]<='Z'){
                state=9;
                continue;
            }
            std::cout<<"error3"<<std::endl;
        }else if(state==6){
            if(i==in.size()){
                state=10;
                continue;
            }
            if(in[i]>='0'&&in[i]<='9'||in[i]>='a'&&in[i]<='z'||in[i]>='A'&&in[i]<='Z'){
                state=7;
                i++;
            }
        }else if(state==5){
            t="OCT";
            code=OCT;
            prop=std::stoi(in.substr(0,i),0,8);
            // return OCT;
        }
        else if(state==7){
            if(i==in.size()){
                state=8;
            }
            else if(in[i]>='0'&&in[i]<='9'||in[i]>='a'&&in[i]<='f'||in[i]>='A'&&in[i]<='F'){
                i++;
            }
            else if(in[i]>='g'&&in[i]<='z'||in[i]>='G'&&in[i]<='Z'){
                state=10;
            }
        }else if(state==8){
            t="HEX";
            code=HEX;
            prop=std::stoi(in.substr(0,i),0,16);
            // return OCT;
            // return HEX;
        }else if(state==9){
            t="ILOCT";
            code=ILOCT;
            // prop=std::stoi(in.substr(0,i),0,16);
            // return ILOCT;
        }
        else if(state==10){
            t="ILHEX";
            code=ILHEX;
            // return ILHEX;
        }else if(state==11){
            t="ILDEX";
            code=ILDEC;
            // return ILDEC;
        }
    }
    symbolTableNode res=symbolTableNode(t,code,prop);
    return res;
    // std::string token("");
    // return result;
    // std::cout<<"error4"<<std::endl;
}

void scan_letter(){
    return;
}

int tellType(char ch){
    if(ch>='0' && ch <='9'){
        return IS_DIGIT;
    }else if(ch>='a' && ch<='z'){
        return IS_LETTER;
    }
    return -1;
}

