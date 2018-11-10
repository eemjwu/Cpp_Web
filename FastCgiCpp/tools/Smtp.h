#ifndef __SMTP_H__ //�����ظ�����
#define __SMTP_H__

#include <iostream>
#include <list>
#include <WinSock2.h>
using namespace std;

const int MAXLEN = 1024;
const int MAX_FILE_LEN = 6000;

static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

struct FILEINFO /*������¼�ļ���һЩ��Ϣ*/
{
	char fileName[128]; /*�ļ�����*/
	char filePath[256]; /*�ļ�����·��*/
};

class CSmtp
{
public:
	CSmtp(void);
	CSmtp(
		int port,
		string srvDomain,	//smtp����������
		string userName,	//�û���
		string password,	//����
		string targetEmail, //Ŀ���ʼ���ַ
		string emailTitle,  //����
		string content       //����
	);
public:
	~CSmtp(void);
public:
	int port;
public:
	string domain;
	string user;
	string pass;
	string targetAddr;
	string title;
	string content;
	/*Ϊ�˷�������ļ���ɾ���ļ�����ģ�ʹ��list������Ϊ���㣬���Ŵ�������ݽṹ���涼ѧ��*/
	list <FILEINFO *> listFile;

public:
	char buff[MAXLEN + 1];
	int buffLen;
	SOCKET sockClient;	//�ͻ��˵��׽���
public:
	bool CreateConn(); /*��������*/

	bool Send(string &message);
	bool Recv();

	void FormatEmailHead(string &email);//��ʽ��Ҫ���͵��ʼ�ͷ��
	int Login();
	bool SendEmailHead();		//�����ʼ�ͷ����Ϣ
	bool SendTextBody();	    //�����ı���Ϣ
								//bool SendAttachment();	    //���͸���
	int SendAttachment_Ex();
	bool SendEnd();
public:
	void AddAttachment(string &filePath); //��Ӹ���
	void DeleteAttachment(string &filePath); //ɾ������
	void DeleteAllAttachment(); //ɾ�����еĸ���

	void SetSrvDomain(string &domain);
	void SetUserName(string &user);
	void SetPass(string &pass);
	void SetTargetEmail(string &targetAddr);
	void SetEmailTitle(string &title);
	void SetContent(string &content);
	void SetPort(int port);
	int SendEmail_Ex();
	/*���ڴ������˵��:1.��������µĴ���2.�û�������3.�������4.�ļ�������0.�ɹ�*/
	char* base64Encode(char const* origSigned, unsigned origLength);
};

#endif // !__SMTP_H__
