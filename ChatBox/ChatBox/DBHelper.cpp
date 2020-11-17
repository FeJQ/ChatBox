#include "stdafx.h"
#include "DBHelper.h"


//数据库操作结构体



DBHelper::DBHelper()
{
	//mysql_library_init(NULL, NULL, nullptr);
	sqlCon = mysql_init(NULL);// mysql 初始化
	char nvalue[1000] = { 0 };
	mysql_options(sqlCon, MYSQL_OPT_RECONNECT, nvalue);// 断线自动重连
	mysql_options(sqlCon, MYSQL_SET_CHARSET_NAME, "utf8");// set name gbk 中文乱码问题
	mysql_real_connect(sqlCon, "127.0.0.1", "root", "08111001", "chatbox", 3306, nullptr, 0);

}


DBHelper::~DBHelper()
{
	mysql_close(sqlCon);// 关闭连接
	//mysql_library_end();
}


//Summary:执行SQL操作
//Parameter:sql:sql语句
//Return:受影响行数
int DBHelper::Exec(const char* sql)
{
	int num = 0;
	char error[255] = { 0 };
	mysql_query(sqlCon, "set names gbk");
	if (mysql_query(sqlCon, sql) != 0) {

		if (mysql_errno(sqlCon))
		{
			num = mysql_errno(sqlCon);
			strcpy(error, mysql_error(sqlCon));
		}
		return -1;
	}
	return (int)mysql_affected_rows(sqlCon);
}


//Summary:查询数据
//Parameter:sql:sql语句
//Return:查询结果
int  DBHelper::Query(const char * sql, map<int, vector<string>> & map_results)
{
	int num = 0;
	char error[255] = { 0 };
	mysql_query(sqlCon, "set names gbk");
	if (mysql_query(sqlCon, sql) != 0) {
		if (mysql_errno(sqlCon))
		{
			num = mysql_errno(sqlCon);
			strcpy(error, mysql_error(sqlCon));
		}
		return -1;
	}

	if (!(m_pResult = mysql_use_result(sqlCon))) {
		return -1;
	}
	int i = 0;

	int count = mysql_num_fields(m_pResult);
	while (m_Row = mysql_fetch_row(m_pResult)) {
		vector<string> vVal;
		for (int j = 0; j < count; j++) {
			if (m_Row[j] != NULL)
			{
				vVal.push_back(m_Row[j]);
			}
			else
			{
				vVal.push_back("0");
			}
		}
		//map_results.insert(map<int, vector<string>>::value_type(i++, vVal));
		map_results[i++] = vVal;
	}
	mysql_free_result(m_pResult);
	return i;
}

