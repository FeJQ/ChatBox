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
	
	//执行语句,并返回受影响行数
	 int Exec(const char* sql);

	//查询语句,返回结果行数
	 int Query(const char* sql, map<int, vector<string>> & map_results);
	 
private:

	MYSQL *sqlCon;
	MYSQL_RES *m_pResult;
	MYSQL_ROW  m_Row;

};

