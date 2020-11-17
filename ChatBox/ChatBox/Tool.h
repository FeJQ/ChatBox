#pragma once
#include <string>
using namespace std;
class Tool
{
public:
	Tool();
	~Tool();

	static char * Encrypt(char* str);
	static string toString(CString cstr);
	static CString toCString(string str);

	static void xor(char* data);
	static void xor(string &data);
};

