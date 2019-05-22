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
    Production(string t){
        left=t;
        right.clear();
    };

    void addRight(string s){right.push_back(s);};
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
    bool isDifferent(vector<TokenSet> t1,vector<TokenSet> t2);
    void show();//输出生成的productions，first，follow
    void init();//初始化工作
    void initTest();

    bool addFirstAtoFirstBWithoutE(string A,string B);
    void addEtoFirstA(string A);
    bool addFirstAtoFollowBWithoutE(vector<string> A,string B);
    void addFollowAtoFollowB(string A,string B);
    vector<string> getSubFirst(vector<string> strings,int begin);
    set<string> getFirst(vector<string> strings);

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
    terminates_$.push_back(string(";"));
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

    Production p = Production(string("program"));
    p.addRight(string("compoundstmt"));
    productions.push_back(p);

    p = Production(string("stmt"));
    p.addRight(string("ifstmt"));
    productions.push_back(p);

    p = Production(string("stmt"));
    p.addRight(string("whilestmt"));
    productions.push_back(p);

    p = Production(string("stmt"));
    p.addRight(string("assgstmt"));
    productions.push_back(p);

    p = Production(string("stmt"));
    p.addRight(string("compoundstmt"));
    productions.push_back(p);

    p = Production(string("compoundstmt"));
    p.addRight(string("{"));
    p.addRight(string("stmts"));
    p.addRight(string("}"));
    productions.push_back(p);

    p = Production(string("stmts"));
    p.addRight(string("stmt"));
    p.addRight(string("stmts"));
    productions.push_back(p);

    p = Production(string("stmts"));
    p.addRight(string("E"));
    productions.push_back(p);

    p = Production(string("ifstmt"));
    p.addRight(string("if"));
    p.addRight(string("("));
    p.addRight(string("boolexpr"));
    p.addRight(string(")"));
    p.addRight(string("then"));
    p.addRight(string("stmt"));
    p.addRight(string("else"));
    p.addRight(string("stmt"));
    productions.push_back(p);

    p = Production(string("whilestmt"));
    p.addRight(string("while"));
    p.addRight(string("("));
    p.addRight(string("boolexpr"));
    p.addRight(string(")"));
    p.addRight(string("stmt"));
    productions.push_back(p);

    p = Production(string("assgstmt"));
    p.addRight(string("ID"));
    p.addRight(string("="));
    p.addRight(string("arithexpr"));
    p.addRight(string(";"));
    productions.push_back(p);

    p = Production(string("boolexpr"));
    p.addRight(string("arithexpr"));
    p.addRight(string("boolop"));
    p.addRight(string("arithexpr"));
    productions.push_back(p);

    p = Production(string("boolop"));
    p.addRight(string("<"));
    productions.push_back(p);

    p = Production(string("boolop"));
    p.addRight(string(">"));
    productions.push_back(p);

    p = Production(string("boolop"));
    p.addRight(string("<="));
    productions.push_back(p);

    p = Production(string("boolop"));
    p.addRight(string(">="));
    productions.push_back(p);

    p = Production(string("boolop"));
    p.addRight(string("=="));
    productions.push_back(p);

    p = Production(string("arithexpr"));
    p.addRight(string("multexpr"));
    p.addRight(string("arithexprprime"));
    productions.push_back(p);

    p = Production(string("arithexprprime"));
    p.addRight(string("+"));
    p.addRight(string("multexpr"));
    p.addRight(string("arithexprprime"));
    productions.push_back(p);

    p = Production(string("arithexprprime"));
    p.addRight(string("-"));
    p.addRight(string("multexpr"));
    p.addRight(string("arithexprprime"));
    productions.push_back(p);

    p = Production(string("arithexprprime"));
    p.addRight(string("E"));
    productions.push_back(p);

    p = Production(string("multexpr"));
    p.addRight(string("simpleexpr"));
    p.addRight(string("multexprprime"));
    productions.push_back(p);

    p = Production(string("multexprprime"));
    p.addRight(string("*"));
    p.addRight(string("simpleexpr"));
    p.addRight(string("multexprprime"));
    productions.push_back(p);

    p = Production(string("multexprprime"));
    p.addRight(string("/"));
    p.addRight(string("simpleexpr"));
    p.addRight(string("multexprprime"));
    productions.push_back(p);

    p = Production(string("multexprprime"));
    p.addRight(string("E"));
    productions.push_back(p);

    p = Production(string("simpleexpr"));
    p.addRight("ID");
    productions.push_back(p);

    p = Production(string("simpleexpr"));
    p.addRight("NUM");
    productions.push_back(p);

    p = Production(string("simpleexpr"));
    p.addRight("(");
    p.addRight("arithexpr");
    p.addRight(")");
    productions.push_back(p);
}

void Grammar::initTest() {
    terminates_$.push_back(string("$"));
    terminates_$.push_back(string("+"));
    terminates_$.push_back(string("-"));
    terminates_$.push_back(string("("));
    terminates_$.push_back(string(")"));
    terminates_$.push_back(string("number"));
    terminates_$.push_back(string("*"));
    for(int i = 1;i<terminates_$.size();i++)terminates.push_back(terminates_$[i]);

    no_terminates.push_back(string("exp"));
    no_terminates.push_back(string("term"));
    no_terminates.push_back(string("addop"));
    no_terminates.push_back(string("mulop"));
    no_terminates.push_back(string("factor"));

    Production p = Production(string("exp"));
    p.addRight(string("exp"));
    p.addRight(string("addop"));
    p.addRight(string("term"));
    productions.push_back(p);

    p = Production(string("exp"));
    p.addRight(string("term"));
    productions.push_back(p);

    p = Production(string("addop"));
    p.addRight(string("+"));
    productions.push_back(p);

    p = Production(string("addop"));
    p.addRight(string("-"));
    productions.push_back(p);

    p = Production(string("term"));
    p.addRight(string("term"));
    p.addRight(string("mulop"));
    p.addRight(string("factor"));
    productions.push_back(p);

    p = Production(string("term"));
    p.addRight(string("factor"));
    productions.push_back(p);

    p = Production(string("mulop"));
    p.addRight(string("*"));
    productions.push_back(p);

    p = Production(string("factor"));
    p.addRight(string("("));
    p.addRight(string("exp"));
    p.addRight(string(")"));
    productions.push_back(p);

    p = Production(string("factor"));
    p.addRight("number");
    productions.push_back(p);
}

void Grammar::show(){
    cout<<"terminates:"<<endl;
    for(string  s :terminates)cout<<s<<endl;
    cout<<"\n\n"<<endl;

    cout<<"noterminates:"<<endl;
    for(string s:no_terminates)cout<<s<<endl;
    cout<<"\n\n"<<endl;

    cout<<"productions:"<<endl;
    for(Production p:productions){
        cout<<p.left<<"  ->  ";
        for(string s:p.right)cout<<s<<" ";
        cout<<endl;
    }
    cout<<"\n\n"<<endl;

    cout<<"first sets:"<<endl;
    for(TokenSet tokenSet:first){
        cout<<tokenSet.left<<":[ ";
        for(string s:tokenSet.sets)cout<<s<<" ";
        cout<<"]"<<endl;
    }
    cout<<"\n\n"<<endl;

    cout<<"follow sets:"<<endl;
    for(TokenSet tokenSet:follow){
        cout<<tokenSet.left<<":[ ";
        for(string s:tokenSet.sets)cout<<s<<" ";
        cout<<"]"<<endl;
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

    vector<TokenSet> temp = first;
    do{
        temp = first;
        for(Production p:productions){
                int k = 0;
                bool Continue = true;
                while (Continue&&k<p.right.size()){
                    if(!addFirstAtoFirstBWithoutE(p.right[k],p.left))Continue = false;
                    k++;
                }
                if(Continue)addEtoFirstA(p.left);
        }
    }while (isDifferent(temp,first));
}

set<string> Grammar::getFirst(vector<string> strings){
    int k = 0;
    bool Contione = true;
    set<string> result;
    result.clear();
    int index=0;
    while (Contione&&k<strings.size()){
        for(int i = 0;i<first.size();i++){if(first[i].left==strings[k]){index = i;break;}}
        for(string s:first[index].sets){
            if(s == string("E")){
                Contione = true;
            }else{
                Contione = false;
                result.insert(s);
            }
        }
        k++;
    }
    if(Contione)result.insert(string("E"));
    return result;
}

vector<string> Grammar::getSubFirst(vector<string> strings,int begin) {
    vector<string> result;
    result.clear();
    if(begin==strings.size()){
        result.push_back(string("E"));
        return result;
    }
    for(int i = begin;i<strings.size();i++)result.push_back(strings[i]);
    return result;
}

bool Grammar::addFirstAtoFirstBWithoutE(string A,string B){
    int IndexA,IndexB;
    bool hasE = false;
    for(int i = 0;i<first.size();i++){if(first[i].left==A){/*cout<<first[i].left<<"is equal to "<<A<<endl;*/IndexA=i;break;}}
    for(int i = 0;i<first.size();i++){if(first[i].left==B){/*cout<<first[i].left<<"is equal to "<<B<<endl;*/IndexB=i;break;}}
    for(string s:first[IndexA].sets){
        if(s==string("E"))hasE=true;
        else{first[IndexB].addSets(s);}
    }
    return hasE;
}

void Grammar::addEtoFirstA(string A){
    int IndexA;
    for(int i  = 0;i<first.size();i++){if(first[i].left==A){IndexA=i;break;}}
    first[IndexA].addSets(string("E"));
}

bool Grammar::addFirstAtoFollowBWithoutE(vector<string> A,string B){
    set<string> result = getFirst(A);
    int index = 0;
    bool hasE = false;
    for(int i = 0;i<follow.size();i++){if(follow[i].left==B){index = i;break;}}
    for(string s :result){
        if(s==string("E"))hasE = true;
        else{
            follow[index].addSets(s);
        }
    }
    return hasE;
}

void Grammar::addFollowAtoFollowB(string A,string B){
    int IndexA,IndexB;
    for(int i = 0;i<follow.size();i++){if(follow[i].left==A){IndexA=i;break;}}
    for(int i = 0;i<follow.size();i++){if(follow[i].left==B){IndexB=i;break;}}
    for(string s:first[IndexA].sets){
        first[IndexB].addSets(s);
    }
}

void Grammar::getFollow() {
    follow.clear();
    for(string s:no_terminates){
        follow.push_back(TokenSet(s));
        if(s==string("exp")){
            follow.back().addSets(terminates_$.front());
        }
    }
    vector<TokenSet> temp;
    do{
        temp = follow;
        for(Production p:productions){
            for(int i = 0;i<p.right.size();i++){
                string s = p.right[i];
                if(!isTerminate(s)){
                    bool hasE = false;
                    hasE = addFirstAtoFollowBWithoutE(getSubFirst(p.right,i+1),s);
                    if(hasE)addFollowAtoFollowB(p.left,s);
                }
            }
        }
    }while (isDifferent(temp,follow));
}

bool Grammar::isDifferent(vector<TokenSet> t1, vector<TokenSet> t2) {
    for(unsigned int i = 0;i<t1.size();i++){
        if(t1[i].sets!=t2[i].sets)return true;
    }
    return false;
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
    while(scanf("%c",&c)!=EOF){
        prog += c;
    }
}
/* 你可以添加其他函数 */

void Analysis()
{
    string prog;
    //read_prog(prog);
    /* 骚年们 请开始你们的表演 */
    /********* Begin *********/
    Grammar g;
    g.initTest();
    g.getFirst();
    g.getFollow();
    g.show();
    /********* End *********/
}