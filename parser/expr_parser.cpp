#include<bits/stdc++.h>
#include<cstdlib>
using namespace std;
#define N 100
#define true 1
#define false -1
char wenfa[N][N];
char VN[N],VT[N];
char firstvt[N][N],lastvt[N][N],table[N][N];
int vnvt(int n)//��ȡvn vt
{
    int flag=true;
    for(int i=0;i<n;i++)
    {
        if((wenfa[i][0]>='A'&&wenfa[i][0]<='Z')&&wenfa[i][1]=='-'&&wenfa[i][2]=='>')
        {
            int sign=true;
            for(int j=0;j<strlen(VN);j++)
            {
                if(VN[j]==wenfa[i][0])
                {
                    sign=false;
                    break;
                }
            }
            if(sign==true)
            {
                VN[strlen(VN)]=wenfa[i][0];
            }
            else if(sign==false)
            {
                continue;
            }
        }
        else
        {
            flag=false;
            break;
        }
    }
    if(flag==false)
    {
        return flag;
    }
    else
    {
        int k=0;
        int l=0;
        for(int i=0;i<n;i++)
        {
            for(int j=3;wenfa[i][j]!='\0';j++)
            {
                if((wenfa[i][j]<'A'||wenfa[i][j]>'Z')&&wenfa[i][j]!='|')
                {
                    for(l=0;l<k;l++)
                    {
                        if(wenfa[i][j]==VT[l])
                        {
                            break;
                        }
                    }
                    if(l==k)
                    {
                        VT[k]=wenfa[i][j];
                        k++;
                    }
                }
            }
        }
        return flag;
    }
}
void getfirstvt(int n)
{
    // int point=0;//����ָ��ÿһ������ʽ��ǰ��������
    for(int i=0;i<n;i++)//�������ҵ�ÿ������ʽ��ǰ���������Ƿ����ս�����Ǿͼ��뵽��Ӧ��firstvt���С�
    {
        int flag=true;
        for(int j=3;;)
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if(wenfa[i][j]==VT[k])
                {
                    int mark=true;//���ڼ��firstvt�����Ƿ��Ѿ����ڸ��ս����
                    for(int l=0;l<strlen(firstvt[i]);l++)
                    {
                        if(wenfa[i][j]==firstvt[i][l])
                        {
                            mark=false;
                            break;
                        }
                    }
                    if(mark==true)//�������ڣ�����뵽firstvt����
                    {
                        int length=strlen(firstvt[i]);
                        firstvt[i][length]=wenfa[i][j];
                    }
                }
                if(wenfa[i][j+1]==VT[k])
                {
                    int mark=true;
                    for(int l=0;l<strlen(firstvt[i]);l++)
                    {
                        if(wenfa[i][j+1]==firstvt[i][l])
                        {
                            mark=false;
                            break;
                        }
                    }
                    if(mark==true)
                    {
                        int length=strlen(firstvt[i]);
                        firstvt[i][length]=wenfa[i][j+1];
                    }
                }
            }
            while(wenfa[i][j]!='|')
            {
                if(wenfa[i][j]=='\0')
                {
                    flag=false;
                    break;
                }
                j++;
            }
            j++;
            if(flag==false)
            {
                break;
            }
        }
    }
    //����Ĵ�����ѭ���鿴��Щ���ս����firstvt�����Լ��뵽��һЩ���ս����firstvt���У�һֱѭ����ӣ�ֱ��ÿ�����ս���ĳ��Ȳ��ٱ仯Ϊֹ��
    int *origin=new int[n];//���ڼ�¼����֮ǰ�����鳤�ȣ����Ƿ��б仯��
    while(1)
    {
        int sign=true;//���ڱ�ʶ����ǰ��firstvt�Ƿ��б仯��mark��symbol��
        for(int i=0;i<n;i++)
        {
            if(origin[i]!=strlen(firstvt[i]))
            {
                sign=false;//�����б仯
                origin[i]=strlen(firstvt[i]);
            }
        }
        if(sign==true)
        {
            break;
        }
        for(int i=0;i<n;i++)
        {
            for(int j=3;;)
            {
                for(int k=0;k<n;k++)
                {
                    if(k==i)
                    {
                        continue;
                    }
                    else if(wenfa[i][j]==wenfa[k][0])
                    {
                        for(int l=0;l<strlen(firstvt[k]);l++)
                        {
                            int flag=true;//���ڱ�ʶһ���ս���Ƿ��Ѿ���firstvt���У�trueΪ�������е���˼��
                            for(int m=0;m<strlen(firstvt[i]);m++)
                            {
                                if(firstvt[k][l]==firstvt[i][m])
                                {
                                    flag=false;
                                    break;
                                }
                            }
                            if(flag==false)
                            {
                                continue;
                            }
                            else
                            {
                                int length=strlen(firstvt[i]);
                                firstvt[i][length]=firstvt[k][l];
                            }
                        }
                    }
                }
                int flag=true;
                while(wenfa[i][j]!='|')
                {
                    if(wenfa[i][j]=='\0')
                    {
                        flag=false;
                        break;
                    }
                    j++;
                }
                if(flag==false)
                {
                    break;
                }
                j++;
            }
        }
    }
}
void getlastvt(int n)//��ȡlastvt�����˺����±����Ǵ�abc...��ô��ʼ�ģ���Ҳ��֪��ΪɶҪ��ô��
{
    for(int a=0;a<n;a++)
    {
        for(int b=0;;)
        {
            int sign=true;//���ڱ�ʶ�Ƿ���һ��ַ���ĩβ
            while(wenfa[a][b]!='|')
            {
                if(wenfa[a][b]=='\0')
                {
                    sign=false;
                    break;
                }
                b++;
            }
            for(int c=0;c<strlen(VT);c++)
            {
                if(wenfa[a][b-1]==VT[c])
                {
                    int flag=true;
                    for(int e=0;e<strlen(lastvt[a]);e++)
                    {
                        if(wenfa[a][b-1]==lastvt[a][e])
                        {
                            flag=false;
                            break;
                        }
                    }
                    if(flag==true)
                    {
                        int length=strlen(lastvt[a]);
                        lastvt[a][length]=VT[c];
                    }
                }
                if(wenfa[a][b-2]==VT[c])
                {
                    int flag=true;
                    for(int e=0;e<strlen(lastvt[a]);e++)
                    {
                        if(wenfa[a][b-2]==lastvt[a][e])
                        {
                            flag=false;
                            break;
                        }
                    }
                    if(flag==true)
                    {
                        int length=strlen(lastvt[a]);
                        lastvt[a][length]=VT[c];
                    }
                }
            }
            if(sign==false)
            {
                break;
            }
            b++;//��������һ��b++����Ϊwenfa[a][b]=='|',�����Ļ�ǰ���whileѭ���޷�����ǰ���±�ɨ�裬���������Ȱ��±���һ����
        }
    }
    //����Ĵ����getfirstvt�ĺ��δ���һģһ����ֻ�ǰ����е�firstvt�ĳ�lastvt�����ˡ�
    int *origin=new int[n];//���ڼ�¼����֮ǰ�����鳤�ȣ����Ƿ��б仯��
    while(1)
    {
        int sign=true;//���ڱ�ʶ����ǰ��lastvt�Ƿ��б仯��mark��symbol��
        for(int i=0;i<n;i++)
        {
            if(origin[i]!=strlen(lastvt[i]))
            {
                sign=false;//�����б仯
                origin[i]=strlen(lastvt[i]);
            }
        }
        if(sign==true)
        {
            break;
        }
        for(int i=0;i<n;i++)
        {
            for(int j=3;;)
            {
                for(int k=0;k<n;k++)
                {
                    if(k==i)
                    {
                        continue;
                    }
                    else if(wenfa[i][j]==wenfa[k][0])
                    {
                        for(int l=0;l<strlen(lastvt[k]);l++)
                        {
                            int flag=true;//���ڱ�ʶһ���ս���Ƿ��Ѿ���lastvt���У�trueΪ�������е���˼��
                            for(int m=0;m<strlen(lastvt[i]);m++)
                            {
                                if(lastvt[k][l]==lastvt[i][m])
                                {
                                    flag=false;
                                    break;
                                }
                            }
                            if(flag==false)
                            {
                                continue;
                            }
                            else
                            {
                                int length=strlen(lastvt[i]);
                                lastvt[i][length]=lastvt[k][l];
                            }
                        }
                    }
                }
                int flag=true;
                while(wenfa[i][j]!='|')
                {
                    if(wenfa[i][j]=='\0')
                    {
                        flag=false;
                        break;
                    }
                    j++;
                }
                if(flag==false)
                {
                    break;
                }
                j++;
            }
        }
    }
}
void gettable(int n)
{
    char data[N];//���ڱ���ÿ������ʽ
    for(int i=0;i<n;i++)//<
    {
        int dt=0;
        // int flag=true;
        for(int j=3;;)
        {
            if(wenfa[i][j]=='|'||wenfa[i][j]=='\0')
            {
                if(strlen(data)!=1)
                {
                    for(int k=0;k<strlen(data);k++)
                    {
                        if(data[k]<'A'||data[k]>'Z')
                        {
                            if(data[k+1]!='\0'&&data[k+1]>='A'&&data[k+1]<='Z')
                            {
                                int x;
                                int yk;
                                for(int l=0;l<strlen(VT);l++)
                                {
                                    if(data[k]==VT[l])
                                    {
                                        x=l;
                                        break;
                                    }
                                }
                                for(int l=0;l<strlen(VN);l++)
                                {
                                    if(data[k+1]==VN[l])
                                    {
                                        yk=l;
                                        break;
                                    }
                                }
                                for(int l=0;l<strlen(firstvt[yk]);l++)
                                {
                                    for(int m=0;m<strlen(VT);m++)
                                    {
                                        if(firstvt[yk][l]==VT[m])
                                        {
                                            if(table[x][m]=='\0')
                                            {
                                                table[x][m]='<';
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(wenfa[i][j]=='\0')
                {
                    break;
                }
                j++;
                memset(data,'\0',sizeof(data));
                dt=0;
            }
            else
            {
                data[dt]=wenfa[i][j];
                j++;
                dt++;
            }
        }
    }
    for(int i=0;i<n;i++)//>,�������<�����ʵ
    {
        int dt=0;
        // int flag=true;
        for(int j=3;;)
        {
            if(wenfa[i][j]=='|'||wenfa[i][j]=='\0')
            {
                if(strlen(data)!=1)
                {
                    for(int k=0;k<strlen(data);k++)
                    {
                        if(data[k]<'A'||data[k]>'Z')
                        {
                            if(k>=1&&data[k-1]>='A'&&data[k-1]<='Z')
                            {
                                int y;
                                int xk;
                                for(int l=0;l<strlen(VT);l++)
                                {
                                    if(data[k]==VT[l])
                                    {
                                        y=l;
                                        break;
                                    }
                                }
                                for(int l=0;l<strlen(VN);l++)
                                {
                                    if(data[k-1]==VN[l])
                                    {
                                        xk=l;
                                        break;
                                    }
                                }
                                for(int l=0;l<strlen(lastvt[xk]);l++)
                                {
                                    for(int m=0;m<strlen(VT);m++)
                                    {
                                        if(lastvt[xk][l]==VT[m])
                                        {
                                            if(table[m][y]=='\0')
                                            {
                                                table[m][y]='>';
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(wenfa[i][j]=='\0')
                {
                    break;
                }
                j++;
                memset(data,'\0',sizeof(data));
                dt=0;
            }
            else
            {
                data[dt]=wenfa[i][j];
                j++;
                dt++;
            }
        }
    }
    for(int i=0;i<n;i++)//=
    {
        int dt=0;
        // int flag=true;
        for(int j=3;;)
        {
            if(wenfa[i][j]=='|'||wenfa[i][j]=='\0')
            {
                if(strlen(data)!=1)
                {
                    for(int k=0;k<strlen(data);k++)
                    {
                        if(data[k]<'A'||data[k]>'Z')
                        {
                            if(data[k+1]!='\0'&&(data[k+1]<'A'||data[k+1]>'Z'))
                            {
                                int x,y;
                                for(int l=0;l<strlen(VT);l++)
                                {
                                    if(data[k]==VT[l])
                                    {
                                        x=l;
                                    }
                                    if(data[k+1]==VT[l])
                                    {
                                        y=l;
                                    }
                                }
                                if(table[x][y]=='\0')
                                {
                                    table[x][y]='=';
                                }
                            }
                            if(data[k+2]!='\0'&&(data[k+2]<'A'||data[k+2]>'Z'))
                            {
                                int x,y;
                                for(int l=0;l<strlen(VT);l++)
                                {
                                    if(data[k]==VT[l])
                                    {
                                        x=l;
                                    }
                                    if(data[k+2]==VT[l])
                                    {
                                        y=l;
                                    }
                                }
                                if(table[x][y]=='\0')
                                {
                                    table[x][y]='=';
                                }
                            }
                        }
                    }
                }
                if(wenfa[i][j]=='\0')
                {
                    break;
                }
                j++;
                memset(data,'\0',sizeof(data));
                dt=0;
            }
            else
            {
                data[dt]=wenfa[i][j];
                j++;
                dt++;
            }
        }
    }
    int jinghao=strlen(VT);
    table[jinghao][jinghao]='=';
    for(int i=0;i<strlen(VT);i++)
    {
        for(int j=0;j<strlen(firstvt[0]);j++)
        {
            if(VT[i]==firstvt[0][j])
            {
                table[jinghao][i]='<';
                break;
            }
        }
    }
    for(int i=0;i<strlen(VT);i++)
    {
        for(int j=0;j<strlen(lastvt[0]);j++)
        {
            if(VT[i]==lastvt[0][j])
            {
                table[i][jinghao]='>';
                break;
            }
        }
    }
}
void fenxi(char *sentence)
{
    char in[N];
    in[0]='#';
    char out[N];
    for(int i=0;i<strlen(sentence);i++)
    {
        out[i]=sentence[i];
    }
    out[strlen(sentence)]='#';
    int step=0;
    int lin=1;
    int lout=strlen(sentence)+1;
    cout<<step<<'\t';
    for(int i=0;i<lin;i++)
    {
        cout<<in[i];
    }
    cout<<'\t';
    for(int i=0;i<lout;i++)
    {
        cout<<out[i];
    }
    cout<<'\t'<<"Ԥ��"<<endl;
    int flag=true;
    while(true)
    {
        int i=lin-1;
        while(in[i]=='N')
        {
            i--;
        }
        int j=0;
        int x,y;
        for(int k=0;k<strlen(VT);k++)
        {
            if(in[i]==VT[k])
            {
                x=k;
            }
            if(out[0]==VT[k])
            {
                y=k;
            }
        }
        if((in[i]>='a'&&in[i]<='z')||(in[i]>='0'&&in[i]<='9'))
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if('i'==VT[k])
                {
                    x=k;
                }
            }
        }
        if((out[0]>='a'&&out[0]<='z')||(out[0]>='0'&&out[0]<='9'))
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if('i'==VT[k])
                {
                    y=k;
                }
            }
        }
        if(in[i]=='#')
        {
            x=strlen(VT);
        }
        if(out[0]=='#')
        {
            y=strlen(VT);
        }
        if(x==y&&y==strlen(VT))
        {
            break;
        }
        if(table[x][y]=='<')
        {
            in[lin]=out[0];
            lin++;
            lout--;
            for(int l=0;l<lout;l++)
            {
                out[l]=out[l+1];
            }
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<'\t';
            for(int l=0;l<lout;l++)
            {
                cout<<out[l];
            }
            cout<<'\t'<<"�ƽ�"<<endl;
        }
        else if(table[x][y]=='=')
        {
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<out[0]<<'\t';
            for(int l=1;l<lout;l++)
            {
                cout<<out[l];
            }
            cout<<'\t'<<"�ƽ�"<<endl;
            in[i]='N';
            int lin1=lin;
            for(int k=i+1;k<lin1;k++)
            {
                in[k]='\0';
                lin--;
            }
            lout--;
            for(int k=0;k<lout;k++)
            {
                out[k]=out[k+1];
            }
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<'\t';
            if(out[0]=='#')
            {
                cout<<'#';
            }
            else
            {
                for(int l=1;l<lout;l++)
                {
                    cout<<out[l];
                }
            }
            cout<<'\t'<<"��Լ"<<endl;
        }
        else if(table[x][y]=='>')
        {
            while(true)
            {
                if(in[i-1]=='N')
                {
                    i--;
                }
                else
                {
                    break;
                }
            }
            in[i]='N';
            int lin1=lin;
            for(int k=i+1;k<lin1;k++)
            {
                in[k]='\0';
                lin--;
            }
            step++;
            cout<<step<<'\t';
            for(int k=0;k<lin;k++)
            {
                cout<<in[k];
            }
            cout<<'\t';
            if(out[0]=='#')
            {
                cout<<'#';
            }
            else
            {
                for(int k=0;k<lout;k++)
                {
                    cout<<out[k];
                }
            }
            cout<<'\t'<<"��Լ"<<endl;
        }
        else
        {
            flag=false;
            break;
        }
    }
    if(flag==true)
    {
        cout<<"��ϲ�㣬��ľ���û�����⡣"<<endl;
    }
    else if(flag==false)
    {
        cout<<"������ȷ�������û���������ս���Ķ�Ӧ��ϵ��ƥ��ʧ�ܡ�"<<endl;
    }
}
int main()
{
    cout<<"��ʹ��Ӣ�����뷨�����ķ�������#������������˳�����ķ��Ҳ��޼���ܡ�"<<endl;
    int n;
    while(1)
    {
        int i=0;
        do
        {
            cin>>wenfa[i];
            i++;
        } while (wenfa[i-1][0]!='#');
        n=i-1;//�ķ�����
        // for(i=0;i<n;i++)
        // {
        //     cout<<wenfa[i]<<endl;
        // }
        if(vnvt(n)==false)
        {
            cout<<"����������ķ��󲿿���������������������롣"<<endl;
        }
        else
        {
            break;
        }
    }
    getfirstvt(n);
    getlastvt(n);
    gettable(n);
    cout<<"---------------------------------------------------"<<endl<<"���ս���У�VN={";
    for(int i=0;i<strlen(VN);i++)
    {
        cout<<VN[i];
        if(i!=strlen(VN)-1)
        {
            cout<<',';
        }
    }
    cout<<'}'<<endl<<"�ս���У�VT={";
    for(int i=0;i<strlen(VT);i++)
    {
        cout<<VT[i];
        if(i!=strlen(VT)-1)
        {
            cout<<',';
        }
    }
    cout<<'}'<<endl<<"---------------------------------------------------"<<endl;
    for(int i=0;i<strlen(VN);i++)
    {
        cout<<"FIRSTVT("<<VN[i]<<")={";
        for(int j=0;j<strlen(firstvt[i]);j++)
        {
            cout<<firstvt[i][j];
            if(j!=strlen(firstvt[i])-1)
            {
                cout<<',';
            }
        }
        cout<<'}'<<endl;
    }
    cout<<endl;
    for(int i=0;i<strlen(VN);i++)
    {
        cout<<"LASTVT("<<VN[i]<<")={";
        for(int j=0;j<strlen(lastvt[i]);j++)
        {
            cout<<lastvt[i][j];
            if(j!=strlen(lastvt[i])-1)
            {
                cout<<',';
            }
        }
        cout<<'}'<<endl;
    }
    cout<<"---------------------------------------------------"<<endl<<"������ȷ��������£�"<<endl<<'\t';
    for(int i=0;i<strlen(VT);i++)
    {
        cout<<VT[i]<<'\t';
    }
    cout<<'#'<<endl;
    for(int i=0;i<strlen(VT)+1;i++)
    {
        if(i==strlen(VT))
        {
            cout<<'#'<<'\t';
        }
        else
        {
            cout<<VT[i]<<'\t';
        }
        
        for(int j=0;j<strlen(VT)+1;j++)
        {
            cout<<table[i][j]<<'\t';
        }
        cout<<endl;
    }
    char sentence[N];
    while(true)
    {
        cout<<"��������Ҫʶ��ľ��ӣ�������#�����û����Ҫʶ��ľ����ˣ�"<<endl;
        cin>>sentence;
        if(sentence[0]=='#')
        {
            break;
        }
        cout<<"����"<<'\t'<<"ջ"<<'\t'<<"���봮"<<'\t'<<"����"<<endl;
        fenxi(sentence);
    }
    system("pause");
    return 0;
}

