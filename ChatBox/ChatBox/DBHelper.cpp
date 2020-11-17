#include "stdafx.h"
#include "DBHelper.h"


//���ݿ�����ṹ��



DBHelper::DBHelper()
{
	//mysql_library_init(NULL, NULL, nullptr);
	sqlCon = mysql_init(NULL);// mysql ��ʼ��
	char nvalue[1000] = { 0 };
	mysql_options(sqlCon, MYSQL_OPT_RECONNECT, nvalue);// �����Զ�����
	mysql_options(sqlCon, MYSQL_SET_CHARSET_NAME, "utf8");// set name gbk ������������
	mysql_real_connect(sqlCon, "127.0.0.1", "root", "08111001", "chatbox", 3306, nullptr, 0);

}


DBHelper::~DBHelper()
{
	mysql_close(sqlCon);// �ر�����
	//mysql_library_end();
}


//Summary:ִ��SQL����
//Parameter:sql:sql���
//Return:��Ӱ������
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


//Summary:��ѯ����
//Parameter:sql:sql���
//Return:��ѯ���
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

