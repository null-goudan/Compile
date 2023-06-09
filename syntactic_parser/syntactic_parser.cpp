#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

/* 
根据第一次实验以及C语言的部分语法
定义语法:
    <程序>      -> <代码块>
    <代码块>    -> { <多个语句> }
    <多个语句>  -> <语句><多个语句> | ε
    <语句>      ->   标识符 = <表达式>; 
                    | if (<布尔表达式>) <语句>
                    | if (<布尔表达式>) <语句> else <语句> 
                    | while(<布尔表达式>) <语句>
                    | do <语句> while (<布尔表达式>)
                    | break
                    | <代码块>
    <布尔表达式  ->    <表达式> < <表达式>
                    | <表达式> <= <表达式>
                    | <表达式> > <表达式>
                    | <表达式> >= <表达式>
                    | <表达式>
    <表达式>    ->     <表达式> + <式项>
                    |  <表达式> - <式项>
                    | <式项>
    <式项>      ->      <式项> * <因子>
                    |  <式项> / <因子>
                    |  <因子>
    <因子>      ->  (表达式) | 标识符 | 数字

进行消除左递归 和 提取左公因子得：
    
    <程序>      -> <代码块>
    <代码块>    -> { <多个语句> }
    <多个语句>  -> <语句><多个语句> | null
    <语句>      ->   标识符 = <表达式>; 
                    | if (<布尔表达式>) <语句> <语句>'
                    | while(<布尔表达式>) <语句>
                    | do <语句> while (<布尔表达式>)
                    | break | <代码块>
    <语句>'     ->   null | else <语句>
    <布尔表达式>  ->  <表达式> <布尔表达式>'
    <布尔表达式>' ->     < <表达式> | <= <表达式> |
                        > <表达式> | >= <表达式> |
                        null
    <表达式>    ->     <式项> <表达式>'
    <表达式>'   ->      + <式项> <表达式>' | - <式项> <表达式>' |
                        null
    <式项>      ->      <因子> <式项>'
    <式项>'     ->   * <因子> <式项>' | / <因子>  <式项>'|null
    <因子>      ->  (表达式) | 标识符 | 数字

预测分析法：
First 集：
    First(<程序>) = { ‘{’ }
    First(<代码块>)　=　｛ '{' ｝
    First(<多个语句>) = { 标识符, if, while, do, break, null }
    First(<语句>) = { 标识符，if, while, do, break, '{' }
    First(<语句>’) = { else, null }
    First(<布尔表达式>) = { '{', 标识符, 数字  }
    First(<布尔表达式>’) = { '<', '<=' ,'>', '>=', null }
    First(<表达式>) = { '(', 标识符, 数字 }
    First(<表达式>’) = { '+', '-', null }
    First(<式项>) = { '(', 标识符, 数字 }
    First(<式项>’) = { '*', '/', null }
    First(<因子>) = { '(', 标识符, 数字 }

Follow 集:
    Follow(<程序>) = { $ }
    Follow(<代码块>) =　{  $,  else, while, '}' ｝
    Follow(<多个语句>) =  { '}', $ }
    Follow(<语句>) = { else, while, '}', $ }
    Follow(<语句>’) = { else, while, '}', $ }
    Follow(<布尔表达式>) = { ')', $ }
    Follow(<布尔表达式>’) = { ')', $ }
    Follow(<表达式>) = { ';', '<', '<=', '>', '>=',  $ }
    Follow(<表达式>’) = { ';', ')','+','-', else, '}', $ }
    Follow(<式项>) = { '*','/' , ';' , ')' ,'+','-','else', '}', $ }
    Follow(<式项>’) = { '*','/' , ';' , ')' ,'+','-','else', '}', $ }
    Follow(<因子>) = { '(', 标识符, 数字, '/', ';' ,')' ,'+','-',else,'}', $ }

Select 集:
    Select(program -> block) = {'}', $}
    Select(block -> {stmts}) = {'{' }
    Select(stmts -> stmtstmts) = {id , if, while, do, break, '{' }
    Select(stmts -> null) = { '}', $ }
    Select(stmt -> id=expr ) = { id }
    Select(stmt -> if(Bool)stmtstmt') = { if }
    Select(stmt -> while(Bool)stmt) = { while }
    Select(stmt -> dostmtwhile(Bool)) = { do }
    Select(stmt -> break) = { break }
    Select(stmt -> block) = { '{' }
    Select(stmt'-> null) = { ';', '}', $ }
    Select(stmt'-> elsestmt) = { else }
    Select(Bool -> exprBool') = {'(', 标识符, 数字 }
    Select(Bool'-> < expr ) = { < }
    Select(14) = {}
    Select(15) = {}
    Select(16) = {}
    Select(17) = {}
    Select(18) = {}
    Select(19) = {}
    Select(20) = {}
    Select(21) = {}
    Select(22) = {}
    Select(23) = {}
    Select(24) = {}
    Select(25) = {}
    Select(26) = {}
    Select(27) = {}
    Select(28) = {}
    Select(29) = {}

递归下降分析法:
    以第一次实验中输出的文件token.txt作为输入 (已将词法分析器放入本文件夹中，随时可以分析输入并输出token文件)
    逐个读入单词 对每个产生式的左边的文法符号对应一个函数的形式
    然后实现递归下降分析器
*/

using namespace std;


/*
    根据文件夹内词法分析器规定：
    保留字，一符一类，种类码0~21：
    "if","else","while","do","break","main","int","float","double","return","const","void","continue","char","unsigned","enum","long","switch","case","unsigned","auto","static"
    特殊符号，一符一类，种类码22~38，种类码39~44为==,!=,&&,||,>=,<=：
    "+","-",",","/","=","<",">","{","}",";","(",")","&","!","#","[","]"
    ID，种类码101
    数字，种类码98~100(十进制，八进制，十六进制)
*/

const string token_filepath = "./token.txt";

struct Token
{
    string      str;        //编码本身
    int         num_code;   //种别码
};


class systatic_parser_descent{
    public:
        systatic_parser_descent();
        void parse() { 
            program();
            if(!flag)
                cout<<"success!"<<endl;
        };
    private:
        void error();
        void program();
        void block();
        void stmts();
        void stmt();
        void stmt_();
        void Bool();
        void Bool_();
        void Expr();
        void Expr_();
        void term();
        void term_();
        void factor();

        vector<Token> tokens;
        int now_idx = 0;
        bool flag = false;      //出错标志
        string source = "";
        string _ans = "";
        int _pos = 0; 
};

systatic_parser_descent::systatic_parser_descent(){
    ifstream file(token_filepath, ios::in);
    if(!file) {
        cerr << "open file failed" <<endl;
        exit(1);
    }
    char ch;
    source = "";
    file.get(ch);
    do{
        source += ch;
        file.get(ch);
    } while(!file.eof());
    stringstream ss(source);
    int i = 0;
    int token_num;
    string token_str;
    while(ss >> token_num){
        ss >> token_str;
        cout << token_num << " "<<token_str<<endl;
        Token new_token;
        new_token.num_code = token_num;
        new_token.str = token_str;
        tokens.push_back(new_token);
    }
}

void systatic_parser_descent::error(){
    cout << "parse error!"<<endl;
}

void systatic_parser_descent::program(){
    cout << "program" ;
    _ans = "--> block \n";
    cout << _ans;
    block();
}

void systatic_parser_descent::block(){
    if(tokens[now_idx++].str == "{"){
        _pos = _ans.find("block");
        _ans.erase(_pos, 5);
        _ans.insert(_pos, "{stmts}");
        cout << _ans;
        stmts();
        if(flag){
            error();
            return;
        }
        if(tokens[now_idx++].str !="}") error();
    }
    else{
        error();
    }
}

void systatic_parser_descent::stmts(){
    if(tokens[now_idx].num_code == 101 || tokens[now_idx].str == "if" ||
        tokens[now_idx].str == "do" || tokens[now_idx].str == "while" || 
        tokens[now_idx].str  == "break" || tokens[now_idx].str == "{")
    {
            _pos = _ans.find("stmts");
            _ans.erase(_pos, 5);
            _ans.insert(_pos, "stmtstmts");
            cout << _ans; 
            stmt();
            if(flag) return;
            stmts();
            if(flag) return;
    }else{
        _pos = _ans.find("stmts");
        _ans.erase(_pos, 5);
        _ans.insert(_pos, "");
        cout << _ans;
    }
}

void systatic_parser_descent::stmt(){
   if (tokens[now_idx].num_code == 101)
	{
		//cout << "stmt --> id = expr ;" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "id=expr;");
		cout << _ans;
		now_idx++;
		if (tokens[now_idx++].str == "=")
		{
			Expr();
			if (flag) return;
			if (tokens[now_idx++].str != ";")
				flag = 1;
		}
		else
			flag = 1;
	}
	else if (tokens[now_idx].str == "while")
	{
		//cout << "stmt --> while (bool) stmt" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "while(Bool)stmt");
		cout << _ans;
		now_idx++;
		if (tokens[now_idx++].str == "(")
		{
			Bool();
			if (tokens[now_idx++].str == ")")
			{
				stmt();
				if (flag) return;
			}
			else
				flag = 1;
		}
		else
			flag = 1;
	}
	else if (tokens[now_idx].str == "do")
	{
		//cout << "stmt --> do stmt while (bool)" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "dostmtwhile(bool)"); 
		cout << _ans;
		now_idx++;
		stmt();
		if (flag) return;
		if (tokens[now_idx++].str == "while" && tokens[now_idx++].str == "(")
		{
			Bool();
			if (flag) return;
			if (tokens[now_idx++].str != ")")
				flag = 1;
		}
		else
			flag = 1;
	}
	else if (tokens[now_idx].str == "break")
	{
		//cout << "stmt --> break" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "break");
		cout << _ans;
		now_idx++;
	}
	else if (tokens[now_idx].str == "{")
	{
		//cout << "stmt --> block" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "block");
		cout << _ans;
		block();
	}
	else if (tokens[now_idx++].str == "if" && tokens[now_idx++].str == "(")
	{
		//cout << "stmt --> if (bool) stmt stmt1" << endl;
		_pos = _ans.find("stmt");
		_ans.erase(_pos, 4);
		_ans.insert(_pos, "if(bool)stmt stmt'");
		cout << _ans;
		stmt();
		if (flag) return;
		stmt_();
		if (flag) return;
	}
	else
		flag = 1;
}

void systatic_parser_descent::stmt_()
{
	if (tokens[now_idx++].str == "else")
	{
		//cout << "stmt1 --> else stmt" << endl;
		_pos = _ans.find("stmt'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "else stmt");
		cout << _ans;
		stmt();
		if (flag) return;
	}
	else
		return;
}

void systatic_parser_descent::Bool()
{
	//cout << "bool --> expr bool1" << endl;
	_pos = _ans.find("Bool");
	_ans.erase(_pos, 4);
	_ans.insert(_pos, "exprBool'");
	cout << _ans;
	Expr();
	if (flag) return;
	Bool_();
	if (flag) return;
}

void systatic_parser_descent::Bool_(){
    if (tokens[now_idx].str == "<" || tokens[now_idx].str == "<=" 
    || tokens[now_idx].str == ">"  || tokens[now_idx].str == ">=")
	{
		//cout << "bool1 --> " << token[p] << " expr" << endl;
		_pos = _ans.find("Bool'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, tokens[now_idx].str + "expr");
		cout << _ans;
		now_idx++;
		Expr();
		if (flag) return;
	}
}

void systatic_parser_descent::Expr()
{
	//cout << "expr --> term expr1" << endl;
	_pos = _ans.find("expr");
	_ans.erase(_pos, 4);
	_ans.insert(_pos, "termexpr'");
	cout << _ans;
	term();
	if (flag == 1) return;
	Expr_();
	if (flag == 1) return;
}
void systatic_parser_descent::Expr_()
{
	if (tokens[now_idx].str == "+")
	{
		//cout << "expr1 --> + term" << endl;
		_pos = _ans.find("expr'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "+term");
		cout << _ans;
		now_idx++;
		term();
		if(flag) return;
	}
	else if (tokens[now_idx].str == "-")
	{
		//cout << "expr1 --> - term" << endl;
		_pos = _ans.find("expr'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "-term");
		cout << _ans;
		now_idx++;
		term();
		if (flag) return;
	}
	else
	{
		//cout << "expr1 --> null" << endl;
		_pos = _ans.find("expr'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "");
		cout << _ans;
		return;
	}
}
void systatic_parser_descent::term()
{
	//cout << "term --> factor term1" << endl;
	_pos = _ans.find("term");
	_ans.erase(_pos, 4);
	_ans.insert(_pos, "factorterm'");
	cout << _ans;
	factor();
	if (flag) return;
	term_();
	if (flag) return;
}

void systatic_parser_descent::term_()
{
	if (tokens[now_idx].str == "*")
	{
		//cout << "term1 --> * factor" << endl;
		_pos = _ans.find("term'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "*factor");
		cout << _ans;
		now_idx++;
		factor();
		if (flag) return;
	}
	else if (tokens[now_idx].str  == "/")
	{
		//cout << "term1 --> / factor" << endl;
		_pos = _ans.find("term'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "/factor");
		cout << _ans;
		now_idx++;
		factor();
		if (flag) return;
	}
	else
	{
		//cout << "term1 --> null" << endl;
		_pos = _ans.find("term'");
		_ans.erase(_pos, 5);
		_ans.insert(_pos, "");
		cout << _ans;
		return;
	}
}
void systatic_parser_descent::factor()
{
	if (tokens[now_idx].str == "(")
	{
		//cout << "factor --> (expr)" << endl;
		_pos = _ans.find("factor");
		_ans.erase(_pos, 6);
		_ans.insert(_pos, "(expr)");
		cout << _ans;
		now_idx++;
		Expr();
		if (flag == 1) return;
		if (tokens[now_idx++].str != ")")
			flag = 1;
	}
	else if (tokens[now_idx].num_code == 101) // id
	{
		//cout << "factor --> id" << endl;
		_pos = _ans.find("factor");
		_ans.erase(_pos, 6);
		_ans.insert(_pos, "id");
		cout << _ans;
		now_idx++;
	}
	else if (tokens[now_idx].num_code >= 98 && tokens[now_idx].num_code <= 100) // num
	{
		//cout << "factor --> num" << endl;
		_pos = _ans.find("factor");
		_ans.erase(_pos, 6);
		_ans.insert(_pos, "num");
		cout << _ans;
		now_idx++;
	}
	else
		flag = true;
}

int main(){
    systatic_parser_descent s;
    s.parse();
    return 0;
}

