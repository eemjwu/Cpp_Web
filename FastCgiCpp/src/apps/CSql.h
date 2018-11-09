#pragma once
#include<TSql.h>
#include<TMongo.h>
#include<TLog.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>

#define mongo_tmp_file_path "tmp/"
//������
enum 
{
	CSQL_SUCCESS		= 0,//�ɹ�
	CSQL_E_CONNECT		= 1,//���ӳ�ʼ��ʧ��
	CSQL_E_LOGIN		= 2,//��֤ʧ��
	CSQL_E_QUERRY		= 3, //��ѯʧ��
	CSQL_E_UPDATE       = 4, //����ʧ��
	CSQL_E_PARAM        = 5  //��������
};

class CSql
{
public:
	CSql();
	~CSql();
	
	///@brief   ��֤�û��Ƿ��¼
	///@param	�û���
	///@return  0 �Ѿ���¼  ����δ��¼
	int Islogin(const std::string & username);

	///@brief   ��ʼ����
	///@return  ������
	int InitConnect(const std::string sIp,
		unsigned short uiPort,
		const std::string sUserName,
		const std::string sPassword,
		const std::string sDatabase);

	///@brief   ��֤��¼
	///@return  ������
	int UserLogin(const std::string& User, const std::string& Passwd);

	///@brief   �鿴�ļ��б�
	///@param   user_name			[in] - �û���  �����û� ID ��߲�ѯ�ٶ�  �û�IDΪ��¼�ɹ�ʱ���ص�
	///@param   & vector<string>	[out] - ���ص��ļ��б� ÿ��[]�ṹ { �ļ����� �ļ����ԣ� ���ϴ����ڣ� �ļ���С��}
	///@return  ������
	int ListFileInfo(const std::string& User_name,std::string &);

	///@brief   �˳���¼ 
	///@param   user_name			[in] - �û���  �����û� ID ��߲�ѯ�ٶ�  �û�IDΪ��¼�ɹ�ʱ���ص�
	///@return  ������
	int UserLogout(const std::string& User);

	///@brief   �г��������й������ļ� 
	///@param   vector<std::string> &			[out] - ��ŷ��ص�json 
	///@return  ������
	int ListAllFile(std::string &);

	///@brief   �ϴ��ļ� 
	///@param   user_name 
	///@param   json			�������ļ������ļ���˽���ԣ���ǩ 
	///@return  ������
	int UploadFile(const std::string &, const std::string &,const std::string &);

	///@brief   �����ļ� 
	///@param   file_name			
	///@param   cla									���			
	///@param   donw_url							��nginx���ݵ�uri							
	///@return  ������
	int DownloadFile(const std::string &file_name, const std::string & cla,std::string & down_url);

	///@brief   ɾ���ļ� 
	///@param   file_name			
	///@param   cla									���			
	///@param   donw_url							��nginx���ݵ�uri							
	///@return  ������
	int DeleFile(const std::string& User_name,const std::string &file_name, const std::string & cla);

private:
	///@brief   �ϴ��ɹ� �����ļ���Ӧ��ϵ 
	///@param   user_name 
	///@param   file_name			
	///@param   file_size			
	///@return  ������
	int UpdateFileInfo(const std::string &username, const std::string &file_name, const std::string & table, const unsigned long long &file_size);

	WebTool::TMysql SQL;
	WebTool::TMongo Mongo;
	std::string monmgoHost = "localhost:27017";

};

#include<Singleton.h>
/// ���嵥����
typedef Singleton<CSql> CSqlSingle;
#define CSQL CSqlSingle::Instance() //ֻ����main�����г�ʼ��



