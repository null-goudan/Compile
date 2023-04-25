#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define Debug_readini 0
#define Debug_analysis 1

using namespace std;

const string cateName[] = { "关键字", "界符", "操作符", "标识符", "数字", "浮点数", "错误"};

enum Catetory{
    KEYWORD,    // 关键字
    DELIMITERS, // 界符
    OPERATOR,   // 操作符 
    CHARACTER,  // 标识符
    NUMBERS,    // 数字
    FLOAT,      // 浮点数
    ERROR,      // 错误
};

class Lexical_Machine{
public:
    Lexical_Machine()=default;
    Lexical_Machine(const string filename, bool _isfile);
    Lexical_Machine(string& text);
    // void init(const string filename);
    void analysis();
    void output_res();

private:
    struct Token
    {
        string      str;        //编码本身
        Catetory    cate;       //分类
        int         num_code;   //种别码
    };

    string target_text;
    int now_idx = 0;
    vector<Token> res_code;
    vector<string> keyword;
    vector<char> delimiters;
    vector<string> operators;

    void initTables();

    int findKeywordCode(const string& str){
        for(int i = 0;i<keyword.size(); ++i){
            if(str == keyword[i]) return i;
        }
        return -1;
    }

    int findOperatorCode(const string& str){
        for(int i = 0;i<operators.size(); ++i){
            if(str == operators[i]) return i;
        }
        return -1;
    }

    int findDelimitersCode(const char ch){
        for(int i = 0;i<delimiters.size(); ++i){
            if(ch == delimiters[i]) return i;
        }
        return -1;
    }

    inline const string getCateName(Catetory cate){ return cateName[cate]; }
    inline char get_next(){ return (now_idx >= target_text.length()) ? '\0' : target_text[now_idx++];}
    inline bool isAlpha(char ch){ return  (ch >= 'A' && ch <= 'z') || (ch >= 'a'&& ch <= 'z'); }
    inline bool isNumber(char ch){ return  (ch >='0' &&  ch <= '9'); }
    inline bool isOperator(string s){ 
        for(auto i : operators) if(i == s) return true;
        return false;
    }
    inline bool isDelimiter(char ch){
        for(auto i : delimiters) if(i == ch) return true;
        return false;
    }
    inline bool isKeyword(string& s){
        for(auto i : keyword) if( i == s) return true;
        return false;
    }
};

Lexical_Machine::Lexical_Machine(string& text){
    this->target_text = text;
    initTables();
    now_idx = 0;
}

Lexical_Machine::Lexical_Machine(const string filename, bool _isfile){
    ifstream textfile;
    textfile.open(filename);
    string s;
    while(getline(textfile, s)) this->target_text += s;
    initTables();
    now_idx = 0;
}

// 初始化依赖表
void Lexical_Machine::initTables(){
    ifstream keyword_ini, delimiters_ini, operator_ini;
    keyword_ini.open("./keyword.ini");
    string s; stringstream ss;
    while(getline(keyword_ini, s)) {
        this->keyword.push_back(s);
        #if Debug_readini
            cout << s << endl;
        #endif
    }
    delimiters_ini.open("./delimiters.ini");
    while(getline(delimiters_ini, s)) { this->delimiters.push_back(s[0]); }
    operator_ini.open("./operator.ini");
    while(getline(operator_ini, s)) { this->operators.push_back(s); }

    # if Debug_readini // 查看读取情况
        for(auto i: keyword){ cout << "item " << i <<endl; }
        for(auto i: delimiters){ cout << "item " << i <<endl; }
        for(auto i: operators){ cout << "item " << i <<endl; }
    # endif
}

void Lexical_Machine::analysis(){
    cout << target_text <<endl;
    char now_ch = ' ';
    string nowStr;
    nowStr.clear();
    now_idx = 0;
    const int len = this->target_text.length();
    while(now_idx != len){
        int syn = 0;    // 种别码
        now_ch = get_next();
        if(isNumber(now_ch)){
            if(nowStr.empty()){
                nowStr.clear();
                bool isFloat = false;
                bool isERROR = false;
                while(isNumber(now_ch)){
                    nowStr += now_ch;
                    now_ch = get_next();
                    if(now_ch == '.'){
                        nowStr += now_ch;
                        isFloat = true;
                        if(!isNumber(target_text[now_idx])){
                            Token new_item = {nowStr, ERROR, ERROR*100};
                            res_code.push_back(new_item);
                            nowStr.clear();
                            isFloat = false;
                            isERROR = true;
                            break;
                        }
                        now_ch = get_next();
                    }
                }
                if(!isERROR){
                    Token new_item = {nowStr, isFloat?FLOAT:NUMBERS, (isFloat?FLOAT:NUMBERS)*100};
                    res_code.push_back(new_item);
                    now_idx--;
                    nowStr.clear();
                }
            }
        }
        else if(isAlpha(now_ch)){
            if(nowStr.empty()){
                nowStr.clear();
                while(isAlpha(now_ch)|| isNumber(now_ch)){
                    nowStr += now_ch;
                    now_ch = get_next();
                }
                syn = isKeyword(nowStr) ? 100 * KEYWORD + findKeywordCode(nowStr) : 100 * CHARACTER;
                Token new_item = {nowStr, (isKeyword(nowStr))? KEYWORD:CHARACTER, syn};
                res_code.push_back(new_item);
                now_idx--;
                nowStr.clear();
            }
        }else if (isDelimiter(now_ch)){
            char next = (now_idx >= target_text.length()) ? '\0' : target_text[now_idx];
            // 第二个符号为界符 可能为运算符： >> <<
            if(isDelimiter(next)){
                nowStr = string(1, now_ch) + string(1, next);
                if(isOperator(nowStr)){
                    ++now_idx;
                    Token new_item = {nowStr, OPERATOR, findOperatorCode(nowStr)+ 100 * OPERATOR};
                    res_code.push_back(new_item);
                    nowStr.clear();
                }
                else{
                    nowStr = string(1, now_ch);
                    Token new_item = {nowStr, DELIMITERS, findDelimitersCode(now_ch)+ DELIMITERS * 100};
                    res_code.push_back(new_item);
                    nowStr.clear();
                }
            }
            else{
                nowStr = string(1, now_ch);
                Token new_item = {nowStr, DELIMITERS, findDelimitersCode(now_ch)+ DELIMITERS * 100};
                res_code.push_back(new_item);
                nowStr.clear();
            }
        }else if (isOperator(string(1, now_ch))) {
            nowStr += now_ch;
            // 第二个符号也可能是运算符的 :: == >= <= 
            char next = (now_idx >= target_text.length()) ? '\0' : target_text[now_idx];
            if(isOperator(string(1, next))){
                nowStr += next;
                if(!isOperator(nowStr)){
                    ++now_idx;
                    nowStr.clear();
                    nowStr += now_ch;
                }else{
                    ++now_idx;
                }
                Token new_item = {nowStr, OPERATOR, findOperatorCode(nowStr)+ 100*OPERATOR};
                res_code.push_back(new_item);
                // now_idx--;
                nowStr.clear();
            }else{
                Token new_item = {nowStr, OPERATOR, findOperatorCode(nowStr)+ 100*OPERATOR};
                res_code.push_back(new_item);
                // now_idx--;
                nowStr.clear();
            }
        }
    }
}

void Lexical_Machine::output_res(){
    printf("%-10s  |  %-10s  |  %-6s\n", "字符", "类别", "种别码");
    printf("%-10s|%-10s  |  %-6s\n", "---------", "---------", "------");
    for(auto i : this->res_code){
        printf("%-10s|  %-10s  |  %-6d\n", i.str.c_str(), getCateName(i.cate).c_str(),i.num_code);
    }
    cout<<"共计"<<res_code.size()<<"个单词"<<endl;
}

int main(){
    // string text;
    // cin>>text;
    Lexical_Machine* lexical = new Lexical_Machine("./test.cpp", true);
    lexical->analysis();
    lexical->output_res();
    delete lexical;
    // system("pause");
    return 0;
}