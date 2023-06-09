#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <stack>
using namespace std;
void preprocess(string &source);
void GetToken();
//保留字，一符一类，种类码0~21
string keyword[22] = {"if", "else", "while", "do", "break", "main", "int", "float", "double", "return", "const", "void", "continue", "char", "unsigned", "enum", "long", "switch", "case", "unsigned", "auto", "static"};
//特殊符号，一符一类，种类码22-38，种类码39-44为==,!=,&&,||,>=,<=
string special[17] = {"+", "-", ",", "/", "=", "<", ">", "{", "}", ";", "(", ")", "&", "!", "#", "[", "]"};
string source = "";
string token;
stack<char> s[3];
char ch;
int p = 0; //表示当前正在处理的字符；
int sym;   //sym表示种类码；
int main()
{
	string filename;
	cout << "输入文件名：";
	cin >> filename;
	ifstream file(filename, ios::in);
	ofstream outfile1("./token.txt", ios::out);
	if (!file)
	{
		cerr << "文件打开失败！" << endl;
		exit(1);
	}
	file.get(ch);
	do
	{
		source += ch;
		file.get(ch);
	} while (!file.eof());
	file.close();
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	cout << "源程序：\n"
		 << source << endl;
	preprocess(source);
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	cout << "处理后程序：\n"
		 << source << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	do
	{
		GetToken();
		if (sym != 9999)
		{
			outfile1 << sym << " " << token << " ";
		}
		if (sym == -1)
		{
			cout << "词法分析出错!" << endl;
			break;
		}
		else if (sym >= 0 && sym <= 21)
			cout << "<" << sym << "," << token << ">" << endl;
		else if (sym >= 22 && sym <= 44)
			cout << "<" << sym << "," << token << ">" << endl;
		else if (sym == 98)
			cout << "<DEC," << token << ">" << endl;
		else if (sym == 99)
			cout << "<OTC," << token << ">" << endl;
		else if (sym == 100)
			cout << "<HEX," << token << ">" << endl;
		else if (sym == 101)
			cout << "<ID," << token << ">" << endl;
		else if (sym == 102)
			cout << "<字符串常量," << token << ">" << endl;
		else if (sym == 103)
			cout << "<字符常量," << token << ">" << endl;
		//      else if (sym == 9999)
		//      {
		//          cout << "词法分析器出错！请检查是否输入非法字符" << endl; break;
		//      }
	} while (p < source.length());
	if (!s[0].empty())
		cout << "ERROR:{不匹配！" << endl;
	if (!s[1].empty())
		cout << "ERROR:[不匹配！" << endl;
	if (!s[2].empty())
		cout << "ERROR:(不匹配！" << endl;
	outfile1.close();
	system("pause");
	return 0;
}
void GetToken()
{
	sym = 9999;
	token = "";
	ch = source[p++];
	while (ch == ' ')
		ch = source[p++];
	//识别标识符
	if (ch == '_')
	{
		sym = 101;
		do
		{
			token += ch;
			ch = source[p++];
		} while (ch == '_' || isalpha(ch) || isdigit(ch));
		p--;
	}
	//识别保留字和标识符
	else if (isalpha(ch))
	{
		sym = 101;
		do
		{
			token += ch;
			ch = source[p++];
		} while (isalpha(ch));
		int i;
		for (i = 0; i < 22; i++)
		{
			if (token == keyword[i])
			{
				sym = i;
				break;
			}
		}
		p--;
	}

	//识别各进制无符号整数
	else if (isdigit(ch))
	{
		if (ch >= '1' && ch <= '9')
		{
			sym = 98;
			do
			{
				token += ch;
				ch = source[p++];
			} while (isdigit(ch));
			p--;
		}
		else
		{
			token += ch;
			ch = source[p];
		}
		if (ch == 'x')
		{
			sym = 100;
			do
			{
				token += ch;
				ch = source[p++];
			} while (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'));
			p--;
		}
		else if (ch >= '0' && ch <= '7')
		{
			sym = 99;
			do
			{
				token += ch;
				ch = source[p++];
			} while (ch >= '0' && ch <= '7');
			p--;
		}
		else
		{
			sym = 98;
		}
	}

	//识别字符串常量
	else if (ch == '"')
	{
		sym = 102;
		do
		{
			token += ch;
			ch = source[p++];
		} while (ch != '"');
		token += ch;
		ch = source[p++];
		p--;
	}
	//识别字符常量
	else if (ch == '\'')
	{
		sym = 103;
		do
		{
			token += ch;
			ch = source[p++];
		} while (ch != '\'');
		token += ch;
		ch = source[p++];
		p--;
	}

	//识别特殊符号
	else
	{
		int i = 0;
		for (i = 0; i < 17; i++)
		{
			if (ch == special[i][0])
			{
				token += ch;
				sym = i + 22;
			}
		}
		//括号匹配
		if (ch == '{')
			s[0].push(ch);
		if (ch == '[')
			s[1].push(ch);
		if (ch == '(')
			s[2].push(ch);
		if (ch == '}' && (!s[0].empty()))
			s[0].pop();
		if (ch == ']' && (!s[1].empty()))
			s[1].pop();
		if (ch == ')' && (!s[2].empty()))
			s[2].pop();
		if (ch == '=')
		{
			ch = source[p++];
			if (ch == '=')
			{
				token += ch;
				sym = 39;
			}
			else
				p--;
		}
		if (ch == '!')
		{
			ch = source[p++];
			if (ch == '=')
			{
				token += ch;
				sym = 40;
			}
			else
				p--;
		}
		if (ch == '&')
		{
			ch = source[p++];
			if (ch == '&')
			{
				token += ch;
				sym = 41;
			}
			else
				p--;
		}
		if (ch == '|')
		{
			ch = source[p++];
			if (ch == '|')
			{
				token += ch;
				sym = 42;
			}
			else
				p--;
		}
		if (ch == '>')
		{
			ch = source[p++];
			if (ch == '=')
			{
				token += ch;
				sym = 43;
			}
			else
				p--;
		}
		if (ch == '<')
		{
			ch = source[p++];
			if (ch == '=')
			{
				token += ch;
				sym = 44;
			}
			else
				p--;
		}
	}
}

void preprocess(string &source)
{
	string str = "";
	//删除注释行
	for (int i = 0; i < source.length(); i++)
	{
		if (source[i] == '/' && source[i + 1] == '/')
		{
			while (source[i] != '\n')
			{
				i++;
			}
		}
		if (source[i] == '/' && source[i + 1] == '*')
		{
			i += 2;
			while (source[i] != '*' || source[i + 1] != '/')
			{
				i++;
				if (i == source.length() - 1)
				{
					cout << "注释出错，没有找到 */，程序结束！\n";
					exit(0);
				}
			}
			i += 2;
		}
		//删除换行和制表符
		if (source[i] != '\n' && source[i] != '\t')
		{
			str += source[i];
		}
	}
	source = str;
}
