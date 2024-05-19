#include <iostream>
#include <string>
#include "./lab2/syntax_anl.h"

#include <fstream>
#include <deque>
#include <queue>
#include <string>
#include "./lab1/lexi_anl.h"
#include "./lab1/lexi_class.h"

using namespace std;

int parsing(string proPath,string testPath,std::deque<symbolTableNode> toSyn){
    CFG_LR1 lr1(proPath);
    // cin >> lr1;
    cout << lr1;
    lr1.createItem();
    lr1.outPutItem();
    PredictTable_LR lr1table(lr1);
    cout << lr1table;
    system("pause");
    if (lr1table.analyse(testPath,toSyn))
        cout << "syntax is right!" << endl;
    else
        cout << "syntax is wrong!" << endl;
    return 0;
}
std::deque<symbolTableNode> lexi(string path){
    //结果储存在符号表中，符号表的结构需要便于检索
    //e.g. 频率高的在前面
    std::string in;
    std::string token;
    int i=0;
    int type;
    symbolTableNode res;
    std::queue<std::string> linkQ;
    std::ofstream f(path,std::ios::trunc);

    std::deque<symbolTableNode> toSyn;

    while(!std::cin.eof()){
        std::cin>>in;
        std::queue<std::string> tmpQ;   
        tmpQ = sep_words(in);
        std::string STOP="#";
        
        while(!tmpQ.empty()){
            linkQ.push(tmpQ.front());
            tmpQ.pop();
        }

        if(linkQ.back()==STOP){
            break;
        }
    }

    for(int i=0;i<linkQ.size();i++){
        std::cout<<linkQ.front()<<"_";
        linkQ.push(linkQ.front());
        linkQ.pop();
    }
    system("pause");
    
    while(!linkQ.empty()){
        in=linkQ.front();
        int error=0;
        if(in=="#") break;
        type=tellType(in[0]);
        switch(type){ 
            case IS_DIGIT:  
                res=scan_digit(in,&error);
                if(!error){
                    f<<res.toString()<<std::endl;
                    toSyn.push_back(res);
                }
                break;
            case IS_LETTER:
                res=scan_letter(in);
                if(!error){
                    f<<res.toString()<<std::endl;
                    toSyn.push_back(res);
                }
                break;
            case IS_SYMBOL:
                res=scan_else(in,&error);
                if(!error){
                    f<<res.toString()<<std::endl;
                    toSyn.push_back(res);
                }
                break;
            default: break;
        }
        linkQ.pop();
    }
    cout<<"deque"<<endl;
    for(symbolTableNode it:toSyn){
        cout<<it.toString()<<endl;
    }
    f.close();
    return toSyn;
}

int main(){

    std::deque<symbolTableNode> toSyn;
    toSyn=lexi("testing.txt");
    system("pause");
    parsing("lab2/production.txt","testing.txt",toSyn);
    return 0;
}