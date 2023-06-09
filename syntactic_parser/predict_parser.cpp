#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;
void input_G();  //LL(1)�ķ����뺯���Լ�VN��VT֮��ķ���
void Select();
bool Judge_LL1();
void PREDICT();
void analyze(string str);
string char2str(char ch);
///*********���������ָ���*******************
string p[100];       //��ʾ����ʽ
int n_chan;             //����ʽ�ĸ���
int n_VN;
vector<string> VN;      //�ս����,��set_VN�洢��ͬ���������洢�����������
vector<string> VT;      //���ս����
set<string> set_VN;     //���ս������,��VN�洢��ͬ�������ϴ洢���ڲ���Ԫ�غ�ȥ��
set<string> set_VT;     //�ս������
string all_left[100];      //����ʽ������VN
string all_right[100];      //����ʽ�����Ҳ�VN+VN
string S;  //��ʼVN
map<string,int> Vn2Num;    //VNӳ�䵽FIRST�����±�
map<string,int> Vt2Num;    //VNӳ�䵽FIRST�����±�
set<string> SELECT[100];
vector<string> predict[100];  //Ԥ�������
string kong = "error";  //Ԥ�������հ״���д�ַ���
int step = 0;  //Ԥ��������̲�������
int main() {
	input_G();
    Select();
    PREDICT();
    cout<<"�������Ӧ�����ķ�Ҫ�������ַ���(����'-'����)(��i*i):";
    string str;
    cin>>str;
    while(str[str.length()-1]!='-'){
        analyze(str);
        cout<<endl<<"���������Ҫ�������ַ���(����'-'����)(��i*i):";
        cin>>str;
    }
	return 0;
}
///����Ϸ��ķ�&&����VT��VN
void input_G(){
    string tmp;   //����ƴ���ַ���
    string tmp1;
    n_chan = 8;
    p[0] = "E->TE'";
    p[1] = "E'->+TE'";
    p[2] = "E'->e";
    p[3] = "T->FT'";
    p[4] = "T'->*FT'";
    p[5] = "T'->e";
    p[6] = "F->(E)";
    p[7] = "F->i";
    ///��ȡ���ս��VN��ѭ��
    for(int i=0;i<n_chan;i++){
        tmp = "";
        tmp1 = "";
        //scanf("%[^\n]",p[i]);
        //getchar();
        int j=0;
        while(p[i][j]!='-'){
            tmp += p[i][j];  //��Ϊ���ս������������E'֮��������ַ���������Ҫƴ�ӣ�������ͷ�ͽ���
            j++;
        }
        j += 2;
        for(;j<(int)p[i].length();j++)
            tmp1 += p[i][j];
        cout<<p[i]<<endl;
        all_left[i] = tmp;      //�洢ÿ������ʽ����
        all_right[i] = tmp1;    //�洢ÿ������ʽ���Ҳ�
        set_VN.insert(tmp);  //��ÿ�в���ʽƴ�Ӻõķ��ս�����뵽������
    }
    n_VN = set_VN.size();   //��ȡVN����
    ///��ȡ�ս��VT��ѭ��
    for(int i=0;i<n_chan;i++){      //n_chan�ǲ���ʽ�ĸ���
        int chan_len = p[i].length(); //ÿ������ʽ�ĳ���
        for(int j=0;j<chan_len;j++)
            if(p[i][j]=='>'){
                tmp = "";
                for(int k=j+1;k<chan_len;k++){ //k��¼"->"֮����±�
                    tmp1 = p[i][k];
                    if(set_VN.count(tmp1)==0){ //count==0��ʾ���ַ����ڼ����У�����VT
                        tmp += p[i][k];           //ƴ���ַ���
                        tmp1 = p[i][k+1];
                        if((k==chan_len-1)||((k<chan_len-1)&&set_VN.count(tmp1)==1)){
                            //�ж�(�������һ���ַ�||��һ���ַ���VN��֮ǰ���ַ�����һ��VT)
                            set_VT.insert(tmp);
                            tmp = "";
                        }
                    }
                }
                break;  //���в���ʽ��ɨ�����
            }
    }
    set_VT.erase("'");  //VT������ɾ��"'"��"e"����
    //ͨ�����ϵ�������������Ԫ��ת�Ƶ�����VN��(��Ϊ���ϲ��������ȡ��������ɸ���ظ�Ԫ��)
    for(set<string>::iterator it=set_VN.begin();it!=set_VN.end();it++)
        VN.push_back(*it);
    for(int i=0;i<(int)VN.size();i++){
        Vn2Num[VN[i]] = i;  ///ÿ��VNӳ�䵽FIRST�����±�
    }
    for(set<string>::iterator it=set_VT.begin();it!=set_VT.end();it++)
        VT.push_back(*it);
    for(int i=0;i<(int)VT.size();i++){
        if(VT[i]=="e")
            Vt2Num["#"] = i;
        else
            Vt2Num[VT[i]] = i;
    }
}
///SELECT��
void Select(){
    SELECT[0].insert("(");
    SELECT[0].insert("i");
    SELECT[1].insert("+");
    SELECT[2].insert(")");
    SELECT[2].insert("#");
    SELECT[3].insert("(");
    SELECT[3].insert("i");
    SELECT[4].insert("*");
    SELECT[5].insert("+");
    SELECT[5].insert(")");
    SELECT[5].insert("#");
    SELECT[6].insert("(");
    SELECT[7].insert("i");
}
///Ԥ�������
void PREDICT(){
    bool flag;
    for(int i=0;i<(int)VT.size();i++)
        if(VT[i]=="e"){
            VT[i] = "#";
            break;
        }
    for(int i=0;i<n_VN;i++){
        for(int j=0;j<(int)VT.size();j++){
            flag = true;
            for(int k=0;k<n_chan;k++)
                if(all_left[k]==VN[i]&&SELECT[k].count(VT[j])==1){
                    predict[i].push_back(all_right[k]);
                    flag = false;
                    break;
                }
            if(flag) predict[i].push_back(kong);
        }
    }
}
///���������ַ���
void analyze(string str){
    S = VN[0];  //��VN������Ԫ�ض�Ϊ��ʼ����Ԫ��
    vector<string> analy;  //��������(�������������������ʣ�������ʾ)
    vector<string> input;    //���봮����(�������������������ʣ�������ʾ)
    string fun;      //�洢��Ԥ����������ҵ��ı��ʽ
    analy.push_back("#");
    analy.push_back(S);
    input.push_back("#");
    for(int i=(int)str.length()-1;i>=0;i--)
        input.push_back(char2str(str[i]));
    ///**************��ʼ���ָ���**************
    cout<<endl<<"����\t����ջ\t\tʣ�����봮\t�Ƶ����ò���ʽ"<<endl;
    while(true){
        cout<<++step<<"\t";
        for(int i=0;i<(int)analy.size();i++){
            cout<<analy[i];
        }
        cout<<"\t\t";
        for(int i=(int)input.size()-1;i>=0;i--){
            cout<<input[i];
        }
        if(input.back()=="#"&&analy.back()=="#"){  //����βԪ��,ģ��ջ��Ԫ��
            cout<<"\t\t����";
            break;
        }else if(input.back()==analy.back()){   //����ջ��Ԫ��ͬ��һ��VT�����ջһ��Ԫ��
            cout<<"\t\t\""<<input.back()<<"\"ƥ��";
            analy.pop_back();
            input.pop_back();
        }//��������VT������ȣ������
        else if(set_VT.count(input.back())==1&&set_VT.count(analy.back())==1&&input.back()!=analy.back()){
            cout<<"����:��ջ�������ƥ�䣡"<<endl;
            break;
        }//����ջ��Ԫ����VN
        else if(set_VN.count(analy.back())==1){
            fun = predict[Vn2Num[analy.back()]][Vt2Num[input.back()]];
            if(fun!=kong){ //����Ԥ����������ҵ����ʽ
                cout<<"\t\t"<<analy.back()<<"->"<<fun;
                analy.pop_back(); //ģ���ջ
                for(int i=(int)fun.length()-1;i>=0;i--){   //������ջ
                    if(fun[i]=='\''){
                        analy.push_back(char2str(fun[i-1])+char2str(fun[i]));
                        i--;
                    }else if(fun[i]=='e') break;
                    else
                        analy.push_back(char2str(fun[i]));
                }
            }else{
                cout<<endl<<"����:δ��Ԥ������ҵ�����ʽ��"<<endl;
                break;
            }
        }//��������VT�������ս������
        else if(set_VT.count(input.back())==0){
            cout<<endl<<"����:���ַ������ڸ��ķ���"<<endl;
            break;
        }
        cout<<endl;
    }
}
///�ַ�ת�����ַ�������
string char2str(char ch){
    char t[] = {ch,'\0'};
    return t;
}