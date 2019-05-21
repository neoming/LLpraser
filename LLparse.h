//
// Created by ASUS on 2019/5/20.
//

#ifndef LLPARSER_LLPARSER_H
#define LLPARSER_LLPARSER_H

#endif //LLPARSER_LLPARSERMAIN_H

// C语言词法分析器
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;
/* 不要修改这个标准输入函数 */

class Production{//产生式
public:
    string left;
    vector<string> right;
    Production(string& t){
        left=t;
        right.clear();
    };
};

class TokenSet{//关系式，用于存储first 和 follow
public:
    string left;
    set<string> sets;
    TokenSet(string t){ left = t;sets.clear(); };

    void addSets(string t){ sets.insert(t); };
};

class Grammar{
public:
    void getFirst();//通过产生式生成first
    void getFollow();//生成follow集合
    bool isTerminate(string s);//判断是否是终止字符
    void show();//输出生成的productions，first，follow
    void init();//初始化工作
    Grammar();
private:
    vector<Production> productions;//产生式
    vector<TokenSet> first;//first集合
    vector<TokenSet> follow;//follow集合
    vector<string> terminates;//终止符
    vector<string> terminates_$;//带有$的终止符集合
    vector<string> no_terminates;//非终止符
    vector<vector<int>> LLTable;//LL分析表
};

Grammar::Grammar() {
    productions.clear();
    first.clear();
    follow.clear();
    terminates.clear();
    terminates_$.clear();
    LLTable.clear();
}

void Grammar::init() {
    terminates_$.push_back(string("$"));
    terminates_$.push_back(string("{"));
    terminates_$.push_back(string("}"));
    terminates_$.push_back(string("if"));
    terminates_$.push_back(string("("));
    terminates_$.push_back(string(")"));
    terminates_$.push_back(string("then"));
    terminates_$.push_back(string("else"));
    terminates_$.push_back(string("while"));
    terminates_$.push_back(string("ID"));
    terminates_$.push_back(string("<"));
    terminates_$.push_back(string("<="));
    terminates_$.push_back(string(">"));
    terminates_$.push_back(string(">="));
    terminates_$.push_back(string("=="));
    terminates_$.push_back(string("+"));
    terminates_$.push_back(string("-"));
    terminates_$.push_back(string("*"));
    terminates_$.push_back(string("/"));
    terminates_$.push_back(string("NUM"));
    terminates_$.push_back(string("E"));
    for(unsigned int i = 1;i<terminates_$.size();i++)terminates.push_back(terminates_$[i]);

    no_terminates.push_back(string("program"));
    no_terminates.push_back(string("compoundstmt"));
    no_terminates.push_back(string("stmt"));
    no_terminates.push_back(string("ifstmt"));
    no_terminates.push_back(string("whilestmt"));
    no_terminates.push_back(string("assgstmt"));
    no_terminates.push_back(string("stmts"));
    no_terminates.push_back(string("boolexpr"));
    no_terminates.push_back(string("arithexpr"));
    no_terminates.push_back(string("boolop"));
    no_terminates.push_back(string("arithexprprime"));
    no_terminates.push_back(string("multexpr"));
    no_terminates.push_back(string("simpleexpr"));
    no_terminates.push_back(string("multexprprime"));
}

void Grammar::show(){
    cout<<"terminates:";
    for(string  s :terminates)cout<<s<<endl;

    cout<<"noterminates:";
    for(string s:no_terminates)cout<<s<<endl;

    cout<<"first sets:"<<endl;
    for(TokenSet tokenSet:first){
        cout<<tokenSet.left<<":{";
        for(string s:tokenSet.sets)cout<<s<<",";
        cout<<"}"<<endl;
    }

    cout<<"follow sets:"<<endl;
    for(TokenSet tokenSet:follow){
        cout<<tokenSet.left<<":{";
        for(string s:tokenSet.sets)cout<<s<<",";
        cout<<"}"<<endl;
    }
}

void Grammar::getFirst() {
    first.clear();
    for(string ter: terminates){//
        TokenSet tokenSet = TokenSet(ter);
        tokenSet.addSets(ter);
        first.push_back(tokenSet);
    }
    for(string no_terminate: no_terminates){
        first.push_back(TokenSet(no_terminate));
    }
}

void Grammar::getFollow() {

}



bool Grammar::isTerminate(string s) {
    for(string t:terminates){
        if(t==s)return true;
    }
    return false;
}

void read_prog(string& prog)
{
    char c;
    while(scanf("%c",&c)!=EOF){
        prog += c;
    }
}
/* 你可以添加其他函数 */

void Analysis()
{
    string prog;
    read_prog(prog);
    /* 骚年们 请开始你们的表演 */
    /********* Begin *********/
    Grammar g;
    g.init();
    g.show();
    /********* End *********/
}