#include "stdafx.h"
#include "Tool.h"
#define key 58756
Tool::Tool()
{
}


Tool::~Tool()
{
}



char * Tool::Encrypt(char * str)
{
	return str;
}

string Tool::toString(CString cs)
{
#ifdef _UNICODE
	//�����unicode����
	USES_CONVERSION;
	std::string str(W2A(cs));
	return str;
#else
	//����Ƕ��ֽڹ��� 
	std::string str(cs.GetBuffer());
	cs.ReleaseBuffer();
	return str;
#endif
}


CString Tool::toCString(string str)
{
#ifdef _UNICODE
	//�����unicode����
	USES_CONVERSION; CString s(str.c_str());
	CString ans(str.c_str());
	return ans;
#else
	//����Ƕ��ֽڹ���
	CString ans;
	ans.Format("%s", str.c_str());
	return ans;
#endif 
}

//�����
void Tool:: xor (char * data)
{
	int len = strlen(data);
		for (int i = 0; i < len; i++)
		{
			*data = *data^ key;
			data++;
		}
		data -=len;
}

void  Tool::xor(string &Str)
{
	char *s = CW2A(Tool::toCString(Str));
	xor (s);
}


