#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include "handleSymbol.h"

using namespace std;

int main(int argc, char **argv)
{
  char *filename = argv[1];
	if(argc < 2)
	{
		cout<<"must add a source file"<<endl;
		exit(0);
	}

	map<string,variable> var_array;//
	vector<string> stdFunc;//基本函数名字数组
	vector<string>::iterator funcIter;
	vector<string> vec;
	string strOfBuf;
	char *buf = NULL;
	const char *input = NULL;
	char *output = (char *)malloc(1024);
	size_t len = 0;
	//size_t str_len = 0;
	FILE *fp;
	string left_value;
	string right_value;
	map<string,variable>::iterator iter;

	size_t ganN;
	int i = 0;
	string str;
	const char *buf2;
	int index;
	int type;
	char *nima_left;
	char *nima_right;
	int flag = 1;
	string left;
	string right;

	stdFunc.push_back("print");//先添加print,随后会有sum etc.
	funcIter = stdFunc.begin();
	fp = fopen(filename,"rb");
	if(fp == NULL)
	{
		cout<<"fopen is erro"<<endl;
		exit(0);
	}

//-------main loop
	while(getline(&buf,&len,fp) != -1)
	{
		memset(output,0,1024);
		strOfBuf = buf;
//=========the if
		if((0==flag) && (strncmp("end",buf,3))!=0) continue;

		flag = 1;

		if(strncmp("if ",buf,3)==0)
		{
			//cout<<buf;
			str = buf;
			str.erase(0,3);
			ganN = str.find_first_of('\n');
			if(ganN) str.erase(ganN,1);
		//cout<<str;
			buf2 = str.c_str();
			index = getCompSymbolIndex(buf2,&type);
			nima_left = getValueLeftToIf(buf2,index,var_array);
			nima_right = getValueRightToIf(buf2,index,var_array);
			if(type == 1)
			{
				if(atoi(nima_left)>atoi(nima_right))
				{
					flag = 1;
					//cout<<"wocao"<<endl;
					continue;
					//cout<<"zhen"<<endl;
				}
				else
				{
					flag = 0;
					continue;
				}
			//atoi(nima_left)>atoi(nima_right)?cout<<"zhen"<<endl:cout<<"jia"<<endl;
			}
			if(type == 2)
			{
				if(atoi(nima_left)<atoi(nima_right))
				{
					flag = 1;
					continue;
				}
				else
				{
					flag = 0;
					continue;
				}
			}
		}
//====end the if

//=====the for
        if(strncmp("for ",buf,4) == 0 /*|| strncmp("begin",buf,5) == 0 || strncmp("end",buf,3) == 0*/)
		{
		    string str2(buf);
		    str2.erase(0,9);
		    const char *buf2 = str2.c_str();
		    int douhao_index = getCommaIndex(buf2);
		    getCircleNumberLeft(buf2,left,douhao_index);
		    getCircleNumberRight(buf2,right,douhao_index);
		    //std::cout<<str2;
			continue;
		}
		//std::cout<<buf;
		if(strncmp("begin",buf,5) != 0)
		{
            str = buf;
            vec.push_back(str);
		}

		if(strncmp("end",buf,3) == 0)
		{
		    vec.pop_back();
		    for(int i=atoi(left.c_str());i<atoi(right.c_str());i++)
		    {
                for(int j=0;j<vec.size();j++)
                {
                    if(stdFunc[0] == vec[j].substr(0,5) && vec[j].substr(6,1) == "\"")
                    {
                        input = (vec[j].substr(7,vec[j].length()-8)).c_str();
                        strncpy(output,input,strlen(input)-1);//-1代表减去最后那个双引号
                        cout<<output<<endl;
                    }
		//----处理打印变量的值
                    if(stdFunc[0] == vec[j].substr(0,5) && vec[j].substr(6,1) != "\"")
                    {
                        int son_length;
                        vec[j].erase(0,6);
                        son_length = vec[j].length();
                        vec[j].erase(son_length-1,son_length);
                        iter = var_array.find(vec[j]);
                        if((iter->second).type == 0)
                            cout<<iter->second.digit_value<<endl;
                        if((iter->second).type == 1)
                        {
                            int i;
                            i = (iter->second.str_value).length();
                            (iter->second.str_value).erase(i-1,i);
                            cout<<iter->second.str_value<<endl;
                        }
                    }
		//----deal the DENGHAO "="
                    char *anotherBuf = (char *)(vec[j].c_str());
                    for(int i=0;i<strlen(anotherBuf);i++)
                    {
                        if(*(anotherBuf+i) == '=')
                            {
                                //str or int?
                                //---处理字符串赋值
                                if(*(anotherBuf+i+1) == '\"')
                                {
                                    left_value = getVariableName(anotherBuf,&i);
                                    //cout<<left_value<<endl;
                                    i++;
                                    right_value = getRightStrValue(anotherBuf,&i);
                                    str_assignment(var_array,left_value,right_value);
                                }
                                //----处理数值赋值
                                else
                                {
                                    //map<string,variable>::iterator iter;
                                    left_value = getVariableName(anotherBuf,&i);
                                    //cout<<left_value<<endl;
                                    right_value = getRightDigitValue(anotherBuf,&i);
                                    //cout<<right_value;
                                    digit_assignment(var_array,left_value,right_value);
                                }
                             }
                     }
                }
                    //std::cout<<vec[j];
            }
        }
		//std::cout<<vec[0];


//end the for

		//----the logic of func--print 处理打印字符串
		if(stdFunc[0] == strOfBuf.substr(0,5) && strOfBuf.substr(6,1) == "\"")
		{
			input = (strOfBuf.substr(7,strOfBuf.length()-8)).c_str();
			strncpy(output,input,strlen(input)-1);//-1代表减去最后那个双引号
			cout<<output<<endl;
		}
		//----处理打印变量的值
		if(stdFunc[0] == strOfBuf.substr(0,5) && strOfBuf.substr(6,1) != "\"")
		{
			int son_length;
			strOfBuf.erase(0,6);
			son_length = strOfBuf.length();
			strOfBuf.erase(son_length-1,son_length);
			iter = var_array.find(strOfBuf);
			if((iter->second).type == 0)
				cout<<iter->second.digit_value<<endl;
			if((iter->second).type == 1)
			{
				int i;
				i = (iter->second.str_value).length();
				(iter->second.str_value).erase(i-1,i);
				cout<<iter->second.str_value<<endl;
			}
		}
		//----deal the DENGHAO "="
		for(int i=0;i<strlen(buf);i++)
		{
			if(*(buf+i) == '=')
				{
					//str or int?
					//---处理字符串赋值
					if(*(buf+i+1) == '\"')
					{
						left_value = getVariableName(buf,&i);
						//cout<<left_value<<endl;
						i++;
						right_value = getRightStrValue(buf,&i);
						str_assignment(var_array,left_value,right_value);
					}
					//----处理数值赋值
					else
					{
						//map<string,variable>::iterator iter;
						left_value = getVariableName(buf,&i);
						//cout<<left_value<<endl;
						right_value = getRightDigitValue(buf,&i);
						//cout<<right_value;
						digit_assignment(var_array,left_value,right_value);
					}
				 }
		 }
	}//end while
	/*for(iter=var_array.begin();iter!=var_array.end();iter++)
		cout<<iter->first<<endl;*/

	fclose(fp);

    /*for(iter=var_array.begin();iter!=var_array.end();iter++)
        cout<<iter->second.digit_value;*/
	return 0;
}

string getVariableName(char *buf,int *i)
{
	string str_value;
	char *return_value = (char *)malloc(32);
	memset(return_value,0,32);
	strncpy(return_value,buf,*i);

	str_value = return_value;
	return str_value;
}

string getRightStrValue(char *buf,int *i)
{
	int f;
	f = strlen(buf)-(*i)-1;
	string str(buf);
	str.erase(str.begin(),str.end()-f);
	str.erase(str.find_first_of("\""),1);
	return str;
}

string getRightDigitValue(char *buf,int *i)
{
	string str = buf;
	str.erase(0,*i+1);
    if(str.find_first_of('\n')) str.erase(str.find_first_of('\n'),1);
	return str;
}

void str_assignment(map<string,variable>& array,string var_name,string value)
{
	variable var = {" ",value,1};
	array.insert(make_pair(var_name,var));
}

void digit_assignment(map<string,variable>& array,string var_name,string value)
{
	/*const char *transfer = NULL;
	int temp_value;
	transfer = value.c_str();
	temp_value = atoi(transfer);*/
	variable var = {value," ",0};
	array.insert(make_pair(var_name,var));
}


//the if function
int getCompSymbolIndex(const char *buf2,int *type)
{
	for(int i=0;i<strlen(buf2);i++)
	{
		if(*(buf2+i)=='>')
		{
			*type = 1;
			return i;
		}
		if(*(buf2+i)=='<')
		{
			*type = 2;
			return i;
		}
		if(*(buf2+i)=='=')
		{
			*type = 3;
			return i;
		}
	}

	return -1;
}

//buf2 without "if "
char* getValueLeftToIf(const char *buf2,int index,map<string,variable> array)
{
    char *out = (char *)malloc(index);
    strncpy(out,buf2,index);

    if(isdigit(*(out+0)))
    {
        return out;
    }

    else
    {
        map<string,variable>::iterator iter;
        iter = array.find(out);
        return (char *)((iter->second).digit_value).c_str();
    }

}
char* getValueRightToIf(const char *buf2,int index,map<string,variable> array)
{
	char *temp;
	char c = *(buf2+index+1);
	temp = (char *)buf2;

	temp = strchr(temp,c);

	if(isdigit(*(temp+0)))
	{
        return temp;
    }
    else
    {
        map<string,variable>::iterator iter;
        iter = array.find(temp);
        return (char *)((iter->second).digit_value).c_str();
    }

}
//=====the for function
int getCommaIndex(const char *bbuf)
{
	int index = 1;
	while(*(bbuf+index) != ',')
	{
		index++;
	}

	return index;
}

void getCircleNumberLeft(const char *bbuf,std::string& left,int douhao_index)
{
    for(int i=1;i<douhao_index;i++)
		left.push_back(*(bbuf+i));
}

void getCircleNumberRight(const char *bbuf,std::string& right,int douhao_index)
{
    int i = douhao_index+1;
	while(*(bbuf+i) != ')')
	{
		right.push_back(*(bbuf+i));
		i++;
	}
}
