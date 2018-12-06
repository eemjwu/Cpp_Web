#pragma once
#include<TSql.h>
//#include<TMongo.h>
#include<TLog.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<TDateTime.h>
#include<map>

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
	
	///@brief   ���µ�¼ip
	///@return  ������
	int UpdateLoginIp(const std::string& User, const std::string& Ip);

	///@brief   �����û���Ϣ
	///@return  ������
	int returnInfo(std::map<std::string, std::string> res, const std::string& User);

	/// @brief  ִ��SQL���,ȡ�ò�ѯ���
	/// @param  sSqlStr     ִ�е�sql���
	/// @return �Ƿ�ɹ� 
	int execSQL(const std::string sSqlStr);

	/// @brief  ִ��SQL���,ȡ�ò�ѯ���
	/// @param  sSqlStr     ִ�е�sql���
	/// @param  sqlData     ����sql���Ľ��
	/// @return �Ƿ�ɹ� 
	int execSQL(const std::string sSqlStr, WebTool::TSqlData &sqlData);



	///@brief   �鿴�ļ��б�
	///@param   user_name			[in] - �û���  �����û� ID ��߲�ѯ�ٶ�  �û�IDΪ��¼�ɹ�ʱ���ص�
	///@param   & vector<string>	[out] - ���ص��ļ��б� ÿ��[]�ṹ { �ļ����� �ļ����ԣ� ���ϴ����ڣ� �ļ���С��}
	///@return  ������


	///@brief   �˳���¼ 
	///@param   user_name			[in] - �û���  �����û� ID ��߲�ѯ�ٶ�  �û�IDΪ��¼�ɹ�ʱ���ص�
	///@return  ������


	///@brief   �г��������й������ļ� 
	///@param   vector<std::string> &			[out] - ��ŷ��ص�json 
	///@return  ������


	///@brief   �ϴ��ļ� 
	///@param   user_name 
	///@param   json			�������ļ������ļ���˽���ԣ���ǩ 
	///@return  ������


	///@brief   �����ļ� 
	///@param   file_name			
	///@param   cla									���			
	///@param   donw_url							��nginx���ݵ�uri							
	///@return  ������


	///@brief   ɾ���ļ� 
	///@param   file_name			
	///@param   cla									���			
	///@param   donw_url							��nginx���ݵ�uri							
	///@return  ������
	

private:
	///@brief   �ϴ��ɹ� �����ļ���Ӧ��ϵ 
	///@param   user_name 
	///@param   file_name			
	///@param   file_size			
	///@return  ������
	
	WebTool::TMysql SQL;



};

#include<Singleton.h>
/// ���嵥����
typedef Singleton<CSql> CSqlSingle;
#define CSQL CSqlSingle::Instance() //ֻ����main�����г�ʼ��



