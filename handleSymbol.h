#ifndef HANDLESYMBOL_H_INCLUDED
#define HANDLESYMBOL_H_INCLUDED



#endif // HANDLESYMBOL_H_INCLUDED
#include <iostream>

using namespace std;

typedef struct variable
{
	string digit_value;
	string str_value;
	int type;//0 is int,1 is string
}variable;

string getVariableName(char *buf,int *i);
string getRightStrValue(char *buf,int *i);
string getRightDigitValue(char *buf,int *i);
void str_assignment(map<string,variable>& array,string var_name,string value);
void digit_assignment(map<string,variable>& array,string var_name,string value);

//the if funtion
char* getValueLeftToIf(const char *buf2,int index,map<string,variable> array);
char* getValueRightToIf(const char *buf2,int index,map<string,variable> array);
int getCompSymbolIndex(const char *buf2,int *type);

//the for function
int getCommaIndex(const char *bbuf);
void getCircleNumberLeft(const char *buf,std::string& left,int douhao_index);
void getCircleNumberRight(const char *buf,std::string& right,int douhao_index);
