#pragma once
#include "include\mysql.h"
#pragma  comment(lib, "libmysql.lib")
#include <map>
#include <vector>
using namespace std;

class DBHelper
{
public:
	DBHelper();
	~DBHelper();
	
	//ִ�����,��������Ӱ������
	 int Exec(const char* sql);

	//��ѯ���,���ؽ������
	 int Query(const char* sql, map<int, vector<string>> & map_results);
	 
private:

	MYSQL *sqlCon;
	MYSQL_RES *m_pResult;
	MYSQL_ROW  m_Row;

};

