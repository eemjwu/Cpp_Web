#include "CSql.h"



CSql::CSql()
{
}


CSql::~CSql()
{
}

int CSql::UserLogin(const std::string& User, const std::string& Passwd)
{
	
	// �����������ݿ�
	if (SQL.openDB()) return  -1;//����ʧ��
	//��ֹ sql ע��
	if (Passwd.find("or") != std::string::npos) return -1;//sql ע�� ����ʧ��
	std::string rSql = "select user_id from t_user_info where user_id = " + User + " and pass_word = " + Passwd;
	WebTool::TSqlData sqlData;
	if(SQL.execSQL(rSql, sqlData)) return -1;//ʧ�� Ӧ��д��log��
	U_ID = sqlData.getData(0, "user_id");
	return 0;//��֤�ɹ�

}