#include <iostream>
#include <tuple>
#include <string>
#include <queue>
#include "lexi_class.h"
#include "lexi_anl.h"
enum TOKEN{
    DEC,
    ILDEC,
    OCT,
    ILOCT,
    HEX,
    ILHEX,
    IDN,//6
    ADD,
    SUB,
    MUL,
    DIV,
    GT,
    LT,
    EQ,
    GE,
    LE,
    NEQ,
    SLP,
    SRP,
    SEMI,
    FUNC,
    ERROR
};

void hello(){
    std::cout<<"heeeeellooooo!!"<<std::endl;
}
symbolTableNode scan_digit(std::string in,int* error){
    int state=0;
    int i=0;
    std::string t;
    int code;
    int prop;
    while(state>=0){
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
                continue;
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
            break;
            // return DEC;
        }else if(state==4){
            
            if(i==in.size()){
                state=5;
                continue;
            }else if(in[i]>='0' && in[i]<='7'){
                i++;
                continue;
            }else if(in[i]>='8'&&in[i]<='9'||in[i]>='a'&&in[i]<='z'||in[i]>='A'&&in[i]<='Z'){
                state=9;
                continue;
            }
            std::cout<<"error3: "<<in<<" "<<i<<" "<<in[i]<<std::endl;
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
            break;
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
            break;
            // return OCT;
            // return HEX;
        }else if(state==9){
            t="ILOCT";
            code=ILOCT;
            break;
            // return ILOCT;
        }
        else if(state==10){
            t="ILHEX";
            code=ILHEX;
            break;
            // return ILHEX;
        }else if(state==11){
            t="ILDEX";
            code=ILDEC;
            break;
            // return ILDEC;
        }
    }
    symbolTableNode res=symbolTableNode(t,code,prop);
    return res;
}

symbolTableNode scan_letter(std::string in){
    std::string t;
    int state=0;
    int i=0;
    int code=-1;
    std::string prop;
    switch(in.length()){
        case 2:
            if(in=="if"){
                prop="";
                t="if";
                code=FUNC;
            }else if(in=="do"){
                prop="";
                t="do";
                code=FUNC;
            }
            break;
        case 3:
            if(in=="end"){
                prop="";
                t="end";
                code=FUNC;
            }
            break;
        case 4:
            if(in=="then"){
                prop="";
                t="then";
                code=FUNC;
            }else if(in=="else"){
                prop="";
                t="else";
                code=FUNC;
            }
            break;
        case 5:
            if(in=="while"){
                prop="";
                t="while";
                code=FUNC;
            }else if(in=="begin"){
                prop="";
                t="while";
                code=FUNC;
            }
            break;
        default: ;
    }
    if(code==-1){
        t="IDN";
        prop=in;
        code=IDN;
    }

    symbolTableNode res=symbolTableNode(t,code,prop);
    return res;
}

symbolTableNode scan_else(std::string in,int* error){
    std::string t;
    int code;
    std::string prop;

    if(in.length()==1){
        if(in=="+"){
            t="ADD";
            code=ADD;
        }else if(in=="-"){
            t="SUB";
            code=SUB;
        }else if(in=="*"){
            t="MUL";
            code=MUL;
        }else if(in=="/"){
            t="DIV";
            code=DIV;
        }else if(in==">"){
            t="GT";
            code=GT;
        }else if(in=="<"){
            t="LT";
            code=LT;
        }else if(in=="="){
            t="EQ";
            code=EQ;
        }else if(in=="("){
            t="SLP";
            code=SLP;
        }else if(in==")"){
            t="SRP";
            code=SRP;
        }else if(in==";"){
            t="SEMI";
            code=SEMI;
        }else{
            t="ERROR SYMBOL:";
            code=ERROR;
            std::cout<<"error6"<<std::endl;
            *error=1;
        }
    }else if(in.length()==2){
        if(in=="<="){
            t="LE";
            code=LE;
        }else if(in==">="){
            t="GE";
            code=GE;
        }else if(in=="<>"){
            t="NEQ";
            code=NEQ;
        }else{
            t="ERROR SYMBOL:";
            code=ERROR;
            std::cout<<"error6"<<std::endl;
            *error=1;    
        }
    }else{
        t="ERROR SYMBOL:";
        code=ERROR;
        std::cout<<"error6"<<std::endl;
        *error=1;
    }
    symbolTableNode res=symbolTableNode(t,code,in);
    return res;
}

std::queue<std::string> sep_words(std::string in){
    int type=tellType(in[0]);
    int i=0;
    bool FLAG = false;
    std::string t;
    std::queue <std::string> tmpQ;

    while(in[i]!='\0'){
        int curtype=tellType(in[i]);
        
        if(in[i]=='#'){
            tmpQ.push(t);
            t.clear();
            t.push_back(in[i]);
            break;
        }
        if(curtype==type && (type!= IS_SYMBOL)){
            t.push_back(in[i]);
        }else{
            if(in[i-1]=='0'&&(in[i]=='x'||in[i]=='X')){
                FLAG=true;
            }
            if(tellType(in[i-1])==IS_LETTER && (tellType(in[i])==IS_DIGIT)){
                FLAG=true;
            }
            if(in[i-1]=='<'){
                if(in[i]=='=' || in[i]=='>'){
                    t.push_back(in[i]);
                    i++;
                    continue;
                }
            }else if(in[i-1]=='>'){
                if(in[i]=='='){
                    t.push_back(in[i]);
                    i++;
                    continue;
                }
            }
            if((tellType(in[i-1])==IS_LETTER || tellType(in[i-1])==IS_DIGIT) && (tellType(in[i])==IS_SYMBOL)){
                FLAG=false;
            }


            if(FLAG==false){
                type=curtype;
                tmpQ.push(t);
                t.clear();
                t.push_back(in[i]);
            }else{
                t.push_back(in[i]);
            }

        }
        i++;

    }
    FLAG=false;
    tmpQ.push(t);
    

    return tmpQ;
}

int tellType(char ch){
    if(ch>='0' && ch <='9'){
        return IS_DIGIT;
    }else if( (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') ){
        return IS_LETTER;
    }else{
        return IS_SYMBOL;
    }
    return -1;
}

