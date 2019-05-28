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
#include <stack>

using namespace std;

/* 不要修改这个标准输入函数 */

class Production {//产生式
public:
    string left;
    vector<string> right;

    Production(string t) {
        left = t;
        right.clear();
    };

    void addRight(string s) { right.push_back(s); };
};

class TokenSet {//关系式，用于存储first 和 follow
public:
    string left;
    set<string> sets;

    TokenSet(string t) {
        left = t;
        sets.clear();
    };

    void addSets(string t) { sets.insert(t); };
};

class TreeNode{
public:
    string val;
    int length;
    vector<TreeNode*> child;
    TreeNode(string s,int len){
        val = s;
        length = len;
        child.clear();
    }
    void showTreeNode(){
        for(int i = 0;i<length;i++)cout<<"\t";
        cout<<val;
    }
};

class Grammar {
public:
    bool debug;

    void getFirst();//通过产生式生成first

    void getFollow(string start);//生成follow集合

    bool isTerminate(string s);//判断是否是终止字符

    bool isNoterminate(string s);

    bool isDifferent(vector<TokenSet> t1, vector<TokenSet> t2);

    void show();//输出生成的productions，first，follow

    void init();//初始化工作

    void addProduction(string s);

    bool findProduction(string terminate,string no_terminate,Production& result);

    void getLLTable();

    void showLLTable();

    void showProduction(int index);

    void showProduction(Production p);

    bool addFirstAtoFirstBWithoutE(string A, string B);

    void addEtoFirstA(string A);

    bool addFirstAtoFollowBWithoutE(vector<string> A, string B);

    void addFollowAtoFollowB(string A, string B);

    vector<string> getSubFirst(vector<string> strings, int begin);

    set<string> getFirst(vector<string> strings);

    set<string> getFollowSet(string s);

    vector<string> findTerminatesByTable(string noterminate);

    void getTokens(string src);

    void analysis();

    void showAnalysisInput(int begin);

    Grammar();

private:
    vector<Production> productions;//产生式
    vector<TokenSet> first;//first集合
    vector<TokenSet> follow;//follow集合
    vector<string> terminates;//终止符
    vector<string> terminates_$;//带有$的终止符集合
    vector<string> no_terminates;//非终止符
    vector<vector<int>> LLTable;//LL分析表
    vector<string> tokens;//token
    vector<int> row;//token row
    stack<TreeNode> parsingStack;
    vector<TreeNode> parsingOutput;
    string startSymbol;
};

Grammar::Grammar() {
    productions.clear();
    first.clear();
    follow.clear();
    terminates.clear();
    terminates_$.clear();
    LLTable.clear();
    debug=true;
    tokens.clear();
    parsingOutput.clear();
    row.clear();
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
    terminates_$.push_back(string("="));
    terminates_$.push_back(string("NUM"));
    terminates_$.push_back(string("E"));
    for (unsigned int i = 1; i < terminates_$.size(); i++)terminates.push_back(terminates_$[i]);

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

    addProduction("program->compoundstmt");
    addProduction("stmt->ifstmt");
    addProduction("stmt->whilestmt");
    addProduction("stmt->assgstmt");
    addProduction("stmt->compoundstmt");
    addProduction("compoundstmt->{ stmts }");
    addProduction("stmts->stmt stmts");
    addProduction("stmts->E");
    addProduction("ifstmt->if ( boolexpr ) then stmt else stmt");
    addProduction("whilestmt->while ( boolexpr ) stmt");
    addProduction("assgstmt->ID = arithexpr ;");
    addProduction("boolexpr->arithexpr boolop arithexpr");
    addProduction("boolop-><");
    addProduction("boolop->>");
    addProduction("boolop-><=");
    addProduction("boolop->>=");
    addProduction("boolop->==");
    addProduction("arithexpr->multexpr arithexprprime");
    addProduction("arithexprprime->+ multexpr arithexprprime");
    addProduction("arithexprprime->- multexpr arithexprprime");
    addProduction("arithexprprime->E");
    addProduction("multexpr->simpleexpr multexprprime");
    addProduction("multexprprime->* simpleexpr multexprprime");
    addProduction("multexprprime->/ simpleexpr multexprprime");
    addProduction("multexprprime->E");
    addProduction("simpleexpr->ID");
    addProduction("simpleexpr->NUM");
    addProduction("simpleexpr->( arithexpr )");
}

void Grammar::addProduction(string s){
    int start = 0;
    int end = 0;
    for(int i = 0;i<s.size()-1;i++){
        if(s[i]=='-'&&s[i+1]=='>'){
            end = i-1;
            break;
        }
    }
    string string1;
    string1 = s.substr(start,end-start+1);
    Production p = Production(string1);
    start=end+3;
    while (start<s.size()){
        for(end = start+1;end<s.size();end++){
            if(s[end]==' '){
                end = end-1;
                break;
            }
        }
        string1 = s.substr(start,end-start+1);
        p.addRight(string1);
        start = end+2;
    }
    productions.push_back(p);
    if(debug){
        cout<<"[addProduction]:: "<<p.left<<"->";
        for(string s2:p.right){
            cout<<s2<<",";
        }
        cout<<endl;
    }
}

void Grammar::show() {
    cout << "terminates:" << endl;
    for (string s :terminates)cout << s << endl;
    cout << endl;

    cout << "noterminates:" << endl;
    for (const string &s:no_terminates)cout << s << endl;
    cout << endl;

    cout << "productions:" << endl;
    for (Production p:productions) {
        cout << p.left << "  ->  ";
        for (const string &s:p.right)cout << s << " ";
        cout << endl;
    }
    cout << endl;

    cout << "first sets:" << endl;
    for (TokenSet tokenSet:first) {
        cout << tokenSet.left << ":[ ";
        for (string s:tokenSet.sets)cout << s << " ";
        cout << "]" << endl;
    }
    cout << endl;

    cout << "follow sets:" << endl;
    for (TokenSet tokenSet:follow) {
        cout << tokenSet.left << ":[ ";
        for (string s:tokenSet.sets)cout << s << " ";
        cout << "]" << endl;
    }

    showLLTable();
}

void Grammar::getFirst() {
    first.clear();
    for (string ter: terminates) {//
        TokenSet tokenSet = TokenSet(ter);
        tokenSet.addSets(ter);
        first.push_back(tokenSet);
    }

    for (string no_terminate: no_terminates) {
        first.push_back(TokenSet(no_terminate));
    }

    vector<TokenSet> temp = first;
    do {
        temp = first;
        for (Production p:productions) {
            int k = 0;
            bool Continue = true;
            while (Continue && k < p.right.size()) {
                if (!addFirstAtoFirstBWithoutE(p.right[k], p.left))Continue = false;
                k++;
            }
            if (Continue)addEtoFirstA(p.left);
        }
    } while (isDifferent(temp, first));
}

set<string> Grammar::getFirst(vector<string> strings) {//stmts
    int k = 0;
    bool Continue = true;
    set<string> result;
    result.clear();
    int index = 0;

    while (Continue && k < strings.size()) {//first(XiX2....Xn)->follow(xi-1)//add follow(A)->follow(xi-1)
        for (int i = 0; i < first.size(); i++) {
            if (first[i].left == strings[k]) {
                index = i;
                break;
            }
        }
        for (string s:first[index].sets) {
            if (s != string("E"))result.insert(s);
        }
        bool hasE = false;
        for(string s:first[index].sets){
            if(s == string("E")){
                hasE = true;
            }
        }
        if (!hasE) Continue = false;
        k++;
    }
    if (Continue)result.insert(string("E"));

    if(debug){
        cout << "[getFirst]::first set( ";
        for (string s:strings)cout << s << " ";
        cout << ") " << ":[ ";
        for (string s:result)cout << s << " ";
        cout << "] " << endl;
        if (Continue)cout << "[getFirst]::has E" << endl;
    }
    return result;
}

vector<string> Grammar::getSubFirst(vector<string> strings, int begin) {
    vector<string> result;
    result.clear();
    if (begin == strings.size()) {
        result.push_back(string("E"));
    } else {
        for (int i = begin; i < strings.size(); i++)result.push_back(strings[i]);
    }
    if(debug){
        cout << "[getSubFirst]::handle vector<string>:[ ";
        for (string s:result)cout << s << " ";
        cout << "] " << begin << " [ ";
        for (string s:strings)cout << s << " ";
        cout << "]" << endl;
    }
    return result;
}

bool Grammar::addFirstAtoFirstBWithoutE(string A, string B) {
    int IndexA, IndexB;
    bool hasE = false;
    for (int i = 0; i < first.size(); i++) {
        if (first[i].left == A) {/*cout<<first[i].left<<"is equal to "<<A<<endl;*/IndexA = i;
            break;
        }
    }
    for (int i = 0; i < first.size(); i++) {
        if (first[i].left == B) {/*cout<<first[i].left<<"is equal to "<<B<<endl;*/IndexB = i;
            break;
        }
    }
    for (string s:first[IndexA].sets) {
        if (s == string("E")) {
            hasE = true;
        }
        else { first[IndexB].addSets(s); }
    }
    if(debug){
        cout << "[addFirstAtoFirstBWithoutE]::first("<<first[IndexA].left<<"):[ ";
        for (const string &s:first[IndexA].sets)cout << s << " ";
        cout << "] to first("<<first[IndexB].left<<"):[ ";
        for (string s:first[IndexB].sets)cout << s << " ";
        cout << "]" << endl;
    }
    return hasE;
}

void Grammar::addEtoFirstA(string A) {
    int IndexA;
    for (int i = 0; i < first.size(); i++) {
        if (first[i].left == A) {
            IndexA = i;
            break;
        }
    }
    first[IndexA].addSets(string("E"));
}

bool Grammar::addFirstAtoFollowBWithoutE(vector<string> A, string B) {
    set<string> result = getFirst(A);
    int index = 0;
    bool hasE = false;
    for (int i = 0; i < follow.size(); i++) {
        if (follow[i].left == B) {
            index = i;
            break;
        }
    }
    string e = string("E");
    for (string s :result) {
        if (s == e)hasE = true;
        else {
            follow[index].addSets(s);
        }
    }

    if(debug){
        cout << "[addFirtstoFollowWithoutE]::add First( ";
        for (string s:A)cout << s << " ";
        cout << "):[ ";
        for (string s:result)cout << s << " ";
        cout << "]" << "to Follow(" << follow[index].left << "):" << "[ ";
        for (string s:follow[index].sets)cout << s << " ";
        cout << "]" << endl;
        if (hasE)cout << "[addFirstToFollowWithoutE]::has E" << endl;
    }
    return hasE;
}

void Grammar::addFollowAtoFollowB(string A, string B) {
    int IndexA=0;
    int IndexB=0;
    for (int i = 0; i < follow.size(); i++) {
        if (follow[i].left == A) {
            IndexA = i;
            break;
        }
    }
    for (int i = 0; i < follow.size(); i++) {
        if (follow[i].left == B) {
            IndexB = i;
            break;
        }
    }
    for (string s:follow[IndexA].sets) {
        follow[IndexB].addSets(s);
    };
    if(debug){
        cout << "[addFollowToFollow]::add follow(" << follow[IndexA].left << "):[ ";
        for (string s:follow[IndexA].sets)cout << s << " ";
        cout << "] to follow(" << follow[IndexB].left << "):[ ";
        for (string s:follow[IndexB].sets)cout << s << " ";
        cout << "]" << endl;
    }
}

void Grammar::getFollow(string start) {
    follow.clear();
    startSymbol = start;
    for (string s:no_terminates) {
        follow.push_back(TokenSet(s));
        if (s == startSymbol) {
            follow.back().addSets(terminates_$.front());
        }
    }
    vector<TokenSet> temp;
    do {
        temp = follow;
        for (Production p:productions) {
            for (int i = 0; i < p.right.size(); i++) {
                string s = p.right[i];
                if (!isTerminate(s)) {
                    bool hasE = false;
                    hasE = addFirstAtoFollowBWithoutE(getSubFirst(p.right, i + 1), s);
                    if (hasE)addFollowAtoFollowB(p.left, s);
                }
            }
        }
    } while (isDifferent(temp, follow));
}

set<string> Grammar::getFollowSet(string s){
    int index = 0;
    for(int i = 0;i<follow.size();i++){
        if(follow[i].left==s){
            index = i;
            break;
        }
    }
    return follow[index].sets;
}

bool Grammar::isDifferent(vector<TokenSet> t1, vector<TokenSet> t2) {
    for (unsigned int i = 0; i < t1.size(); i++) {
        if (t1[i].sets != t2[i].sets)return true;
    }
    return false;
}

bool Grammar::isTerminate(string s) {
    for (string t:terminates_$) {
        if (t == s)return true;
    }
    return false;
}

bool Grammar::isNoterminate(string s){
    for(string t:no_terminates){
        if(t == s)return true;
    }
    return false;
}

void Grammar::getLLTable(){
    vector<vector<int>> vector1(terminates_$.size(),vector<int>(no_terminates.size(),-1));

    for(int i = 0;i<productions.size();i++){
        Production p = productions[i];
        set<string> first_set;
        first_set = getFirst(p.right);
        bool hasE = false;
        for(string s:first_set){
            if(s == "E")hasE = true;
            int TerminatesIndex=0;
            int NoterminatesIndex=0;

            for(int j = 0;j<terminates_$.size();j++){
                if(terminates_$[j]==s){
                    TerminatesIndex = j;
                    break;
                }
            }

            for(int j = 0;j<no_terminates.size();j++){
                if(no_terminates[j]==p.left){
                    NoterminatesIndex = j;
                    break;
                }
            }
            vector1[TerminatesIndex][NoterminatesIndex] = i;
            if(debug){
                cout<<"[getLLTable]:: M["<<no_terminates[NoterminatesIndex]<<","<<terminates_$[TerminatesIndex]<<"] ";
                showProduction(i);
                cout<<endl;
            }
        }

        if(hasE){
            set<string> follow_set;
            follow_set = getFollowSet(p.left);
            int NoterminatesIndex=0;

            for(int j = 0;j<no_terminates.size();j++){
                if(no_terminates[j]==p.left){
                    NoterminatesIndex = j;
                    break;
                }
            }

            for(string s:follow_set){
                int TerminatesIndex;
                for(int j = 0;j<terminates_$.size();j++){
                    if(terminates_$[j]==s){
                        TerminatesIndex = j;
                        break;
                    }
                }
                vector1[TerminatesIndex][NoterminatesIndex]=i;
                if(debug){
                    cout<<"[getLLTable]:: M["<<no_terminates[NoterminatesIndex]<<","<<terminates_$[TerminatesIndex]<<"]";
                    showProduction(i);
                    cout<<endl;
                }
            }
        }

        LLTable = vector1;
    }
}

void Grammar::showLLTable() {
    for(int i = 0;i < terminates_$.size();i++){
        for(int j = 0;j < no_terminates.size();j++){
            cout<<"M[ "<<terminates_$[i]<<", "<<no_terminates[j]<<" ]:";
            if(LLTable[i][j]!=-1){
                showProduction(LLTable[i][j]);
            }
            cout<<endl;
        }
    }
}

bool Grammar::findProduction(string terminate,string no_terminate,Production& result){
    int terminateIndex=0;
    int no_terminateIndex=0;
    for(int i = 0;i<terminates_$.size();i++){
        if(terminates_$[i]==terminate){
            terminateIndex = i;
            break;
        }
    }

    for(int i = 0;i<no_terminates.size();i++){
        if(no_terminates[i]==no_terminate){
            no_terminateIndex = i;
            break;
        }
    }
    if(LLTable[terminateIndex][no_terminateIndex]==-1){
        return false;
    }else{
        result = productions[LLTable[terminateIndex][no_terminateIndex]];
        return true;
    }
}

void Grammar::showProduction(int index){
    cout<<productions[index].left<<" -> ";
    for(string s:productions[index].right){
        cout<<s<<" ";
    }
}

void Grammar::showProduction(Production p){
    cout<<p.left<<"->";
    for(string s:p.right){
        cout<<s<<" ";
    }
}

void passWhiteAndSpace(string s,int& begin){
    while (begin<s.size()){
        if(s[begin]==' '||s[begin]=='\n'){
            begin++;
        }else{
            break;
        }
    }
}

void Grammar::getTokens(string src){
    int begin = 0;
    int length = 1;
    vector<string> result;
    result.clear();
    string target;
    int r = 0;
    while (begin<src.size()){
        passWhiteAndSpace(src,begin);
        length = 1;
        while (begin+length<src.size()){
            if(src[begin+length]!=' '&&src[begin+length]!='\n'){
                length++;
            }else{
                break;
            }
        }
        target = src.substr(begin,length);
        r = 0;
        for(int i = begin;i>=0;i--){
            if(src[i]=='\n')r++;
        }

        if(debug){
            cout<<"getTokne:"<<target<<endl;
        }
        result.push_back(target);
        row.push_back(r+1);
        begin = begin+length;
    }
    result.push_back("$");
    tokens=result;
}

void Grammar::showAnalysisInput(int begin){
    for(int i = begin;i<tokens.size();i++){
        cout<<tokens[i]<<" ";
    }
}

vector<string> Grammar::findTerminatesByTable(string noterminate){
    int index = 0;
    vector<string> result;
    result.clear();
    for(int i = 0;i<no_terminates.size();i++){
        if(no_terminates[i]==noterminate){
            index = i;
            break;
        }
    }
    for(int i = 0;i<terminates_$.size();i++){
        if(LLTable[i][index]!=-1){
            result.push_back(terminates_$[i]);
        }
    }
    return result;
}

void Grammar::analysis() {
    parsingStack.push(TreeNode(terminates_$[0],0));
    parsingStack.push(TreeNode(startSymbol,0));
    int tokenIndex = 0;
    string nextToken = tokens[tokenIndex];
    bool accept = true;

    while (parsingStack.top().val!=terminates_$[0]&&nextToken!=terminates_$[0]){
        TreeNode top = parsingStack.top();
        Production p = Production("$");
        if(debug){
            cout<<"[analysis]::";
            showAnalysisInput(tokenIndex);
            cout<<"stack top:"<<top.val<<"\t nextToken:"<<nextToken<<"\t";
        }
        if(top.val=="E"){
            parsingOutput.push_back(top);
            parsingStack.pop();
        }
        else if(isTerminate(top.val)&&nextToken==top.val){
            parsingOutput.push_back(top);
            parsingStack.pop();
            if(nextToken==tokens[tokenIndex]){
                tokenIndex++;
                nextToken = tokens[tokenIndex];
            } else{
                nextToken = tokens[tokenIndex];
            }
            if(debug){ cout<<"match"<<endl; }
        } else if(isNoterminate(top.val)){
            if(isTerminate(nextToken)){
                if(findProduction(nextToken,top.val,p)){
                    int length = top.length;
                    parsingOutput.push_back(top);
                    parsingStack.pop();
                    for(int i = p.right.size()-1;i>=0;i--){
                        parsingStack.push(TreeNode(p.right[i],length+1));
                    }
                    if(debug){
                        cout<<"generate: ";
                        showProduction(p);
                        cout<<endl;
                    }
                } else{
                    vector<string> words = findTerminatesByTable(top.val);
                    stack<TreeNode> temp;
                    string target;
                    bool done = false;
                    while (parsingStack.size()>0){
                        temp.push(parsingStack.top());
                        parsingStack.pop();
                    }
                    while (temp.size()>0){
                        if(!done){
                            for(string s:words){
                                if(s==temp.top().val){
                                    target = s;
                                    done = true;
                                    //cout<<"find "<<s<<" = "<<target<<endl;
                                    break;
                                }
                            }
                        }
                        parsingStack.push(temp.top());
                        temp.pop();
                    }
                    int r = row[tokenIndex-1];
                    cout<<"语法错误,第"<<r<<"行,缺少\""<<target<<"\""<<endl;
                    //cout<<"row: "<<r<<" lost "<<target<<endl;
                    nextToken = target;
                }
            }
        }else{
            break;
        }
    }
    if (parsingStack.top().val==terminates_$[0]&&nextToken==terminates_$[0]){
        accept = true;
    }else{
        accept = false;
        cout<<"error"<<endl;
    }

    if(accept){
        for(int i = 0;i<parsingOutput.size();i++){
            parsingOutput[i].showTreeNode();
            if(i!=parsingOutput.size()-1){
                cout<<endl;
            }
        }
    }
}

void read_prog(string &prog) {
    char c;
    while (scanf("%c", &c) != EOF) {
        prog += c;
    }
}

/* 你可以添加其他函数 */

void Analysis() {
    string prog = "{\n ID = NUM ;\n}";
    //read_prog(prog);
    /* 骚年们 请开始你们的表演 */
    /********* Begin *********/
    Grammar g;
    g.debug= true;
    g.init();
    //g.initTest1();
    g.getFirst();
    g.getFollow("program");
    g.getLLTable();
    g.getTokens(prog);
    //g.show();
    g.analysis();
    /********* End *********/
}