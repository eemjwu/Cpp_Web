#include "Smtp.h"
#include <iostream>
#include <fstream>
using namespace std;

#pragma  comment(lib, "ws2_32.lib")	/*����ws2_32.lib��̬���ӿ�*/

/*base64���ñ��˵ı���,�������ⲻ���ص㣬�ص�����������ҵ�һ���ȽϺõ��ʼ����Ϳͻ���*/
char* CSmtp::base64Encode(char const* origSigned, unsigned origLength)
{
	unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set
	if (orig == NULL) return NULL;

	unsigned const numOrig24BitValues = origLength / 3;
	bool havePadding = origLength > numOrig24BitValues * 3;
	bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
	unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 3]; // allow for trailing '/0'

												 // Map each full group of 3 input bytes into 4 output base-64 characters:
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (havePadding)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2)
		{
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else
		{
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	return result;
}
CSmtp::CSmtp(void)
{
	this->content = "";
	this->port = 25;
	this->user = "";
	this->pass = "";
	this->targetAddr = "";
	this->title = "";
	this->domain = "";

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	this->sockClient = 0;

}

CSmtp::~CSmtp(void)
{
	DeleteAllAttachment();
	closesocket(sockClient);
	WSACleanup();
}


CSmtp::CSmtp(
	int port,
	string srvDomain,
	string userName,
	string password,
	string targetEmail,
	string emailTitle,
	string content
)
{
	this->content = content;
	this->port = port;
	this->user = userName;
	this->pass = password;
	this->targetAddr = targetEmail;
	this->title = emailTitle;
	this->domain = srvDomain;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	this->sockClient = 0;
}

bool CSmtp::CreateConn()
{
	//Ϊ����socket������׼������ʼ������
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0); //����socket����
	SOCKADDR_IN addrSrv;
	HOSTENT* pHostent;
	pHostent = gethostbyname(domain.c_str());  //�õ��й�����������Ϣ

	addrSrv.sin_addr.S_un.S_addr = *((DWORD *)pHostent->h_addr_list[0]);	//�õ�smtp�������������ֽ����ip��ַ   
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	int err = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));   //��������������� 
	if (err != 0)
	{
		return false;
		//printf("����ʧ��\n");
	}
	this->sockClient = sockClient;
	if (false == Recv())
	{
		return false;
	}
	return true;
}

bool CSmtp::Send(string &message)
{
	int err = send(sockClient, message.c_str(), message.length(), 0);
	if (err == SOCKET_ERROR)
	{
		return false;
	}
	cout << message.c_str() << endl;
	return true;
}

bool CSmtp::Recv()
{
	memset(buff, 0, sizeof(char)* (MAXLEN + 1));
	int err = recv(sockClient, buff, MAXLEN, 0); //��������
	if (err == SOCKET_ERROR)
	{
		return false;
	}
	buff[err] = '\0';
	cout << buff << endl;
	return true;
}

int CSmtp::Login()
{
	string sendBuff;
	sendBuff = "EHLO ";
	sendBuff += user;
	sendBuff += "\r\n";

	if (false == Send(sendBuff) || false == Recv()) //�Ƚ���Ҳ����
	{
		return 1; /*1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	sendBuff = "AUTH LOGIN\r\n";
	if (false == Send(sendBuff) || false == Recv()) //�����½
	{
		return 1; /*1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	int pos = user.find('@', 0);
	sendBuff = user.substr(0, pos); //�õ��û���

	char *ecode;
	/*������˳����һ�䣬����string���length������C�����е�strlen����������,strlen��������ĳ��ȣ�ֻ��'\0'�ַ�Ϊֹ,��string::length()����ʵ���Ϸ��ص���string�����ַ�����Ĵ�С,���Լ����Բ���һ�£���Ҳ��Ϊʲô�����治ʹ��string::length()��ԭ��*/

	ecode = base64Encode(sendBuff.c_str(), strlen(sendBuff.c_str()));
	sendBuff.empty();
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //�����û����������շ������ķ���
	{
		return 1; /*������1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	ecode = base64Encode(pass.c_str(), strlen(pass.c_str()));
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //�����û����룬�����շ������ķ���
	{
		return 1; /*������1��ʾ����ʧ�������������*/
	}

	if (NULL != strstr(buff, "550"))
	{
		return 2;/*������2��ʾ�û�������*/
	}

	if (NULL != strstr(buff, "535")) /*535����֤ʧ�ܵķ���*/
	{
		return 3; /*������3��ʾ�������*/
	}
	return 0;
}

bool CSmtp::SendEmailHead()		//�����ʼ�ͷ����Ϣ
{
	string sendBuff;
	sendBuff = "MAIL FROM: <" + user + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*��ʾ����ʧ�������������*/
	}


	sendBuff.empty();
	sendBuff = "RCPT TO: <" + targetAddr + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	sendBuff = "DATA\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; //��ʾ����ʧ�������������
	}

	sendBuff.empty();
	FormatEmailHead(sendBuff);
	if (false == Send(sendBuff))
		//������ͷ��֮�󲻱ص��ý��պ���,��Ϊ��û��\r\n.\r\n��β����������Ϊ��û�з������ݣ����Բ��᷵��ʲôֵ
	{
		return false; /*��ʾ����ʧ�������������*/
	}
	return true;
}

void CSmtp::FormatEmailHead(string &email)
{/*��ʽ��Ҫ���͵�����*/
	email = "From: ";
	email += user;
	email += "\r\n";

	email += "To: ";
	email += targetAddr;
	email += "\r\n";

	email += "Subject: ";
	email += title;
	email += "\r\n";

	email += "MIME-Version: 1.0";
	email += "\r\n";

	email += "Content-Type: multipart/mixed;boundary=qwertyuiop";
	email += "\r\n";
	email += "\r\n";
}

bool CSmtp::SendTextBody()  /*�����ʼ��ı�*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop\r\n";
	sendBuff += "Content-Type: text/plain;";
	sendBuff += "charset=\"gb2312\"\r\n\r\n";
	sendBuff += content;
	sendBuff += "\r\n\r\n";
	return Send(sendBuff);
}

int CSmtp::SendAttachment_Ex() /*���͸���*/
{
	for (list<FILEINFO *>::iterator pIter = listFile.begin(); pIter != listFile.end(); pIter++)
	{
		cout << "Attachment is sending ~~~~~" << endl;
		cout << "Please be patient!" << endl;
		string sendBuff;
		sendBuff = "--qwertyuiop\r\n";
		sendBuff += "Content-Type: application/octet-stream;\r\n";
		sendBuff += " name=\"";
		sendBuff += (*pIter)->fileName;
		sendBuff += "\"";
		sendBuff += "\r\n";

		sendBuff += "Content-Transfer-Encoding: base64\r\n";
		sendBuff += "Content-Disposition: attachment;\r\n";
		sendBuff += " filename=\"";
		sendBuff += (*pIter)->fileName;
		sendBuff += "\"";

		sendBuff += "\r\n";
		sendBuff += "\r\n";
		Send(sendBuff);
		ifstream ifs((*pIter)->filePath, ios::in | ios::binary);
		if (false == ifs.is_open())
		{
			return 4; /*������4��ʾ�ļ��򿪴���*/
		}
		char fileBuff[MAX_FILE_LEN];
		char *chSendBuff;
		memset(fileBuff, 0, sizeof(fileBuff));
		/*�ļ�ʹ��base64���ܴ���*/
		while (ifs.read(fileBuff, MAX_FILE_LEN))
		{
			//cout << ifs.gcount() << endl;
			chSendBuff = base64Encode(fileBuff, MAX_FILE_LEN);
			chSendBuff[strlen(chSendBuff)] = '\r';
			chSendBuff[strlen(chSendBuff)] = '\n';
			send(sockClient, chSendBuff, strlen(chSendBuff), 0);
			delete[]chSendBuff;
		}
		//cout << ifs.gcount() << endl;
		chSendBuff = base64Encode(fileBuff, ifs.gcount());
		chSendBuff[strlen(chSendBuff)] = '\r';
		chSendBuff[strlen(chSendBuff)] = '\n';
		int err = send(sockClient, chSendBuff, strlen(chSendBuff), 0);

		if (err != strlen(chSendBuff))
		{
			cout << "�ļ����ͳ���!" << endl;
			return 1;
		}
		delete[]chSendBuff;
	}
	return 0;
}

bool CSmtp::SendEnd() /*���ͽ�β��Ϣ*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop--";
	sendBuff += "\r\n.\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false;
	}
	cout << buff << endl;
	sendBuff.empty();
	sendBuff = "QUIT\r\n";
	return (Send(sendBuff) && Recv());
}

int CSmtp::SendEmail_Ex()
{
	if (false == CreateConn())
	{
		return 1;
	}
	//Recv();
	int err = Login(); //�ȵ�¼
	if (err != 0)
	{
		return err; //����������Ҫ����
	}
	if (false == SendEmailHead()) //����EMAILͷ����Ϣ
	{
		return 1; /*������1����������Ĵ���*/
	}
	if (false == SendTextBody())
	{
		return 1; /*������1����������Ĵ���*/
	}
	err = SendAttachment_Ex();
	if (err != 0)
	{
		return err;
	}
	if (false == SendEnd())
	{
		return 1; /*������1����������Ĵ���*/
	}
	return 0; /*0��ʾû�г���*/
}

void CSmtp::AddAttachment(string &filePath) //��Ӹ���
{
	FILEINFO *pFile = new FILEINFO;
	strcpy_s(pFile->filePath, filePath.c_str());
	const char *p = filePath.c_str();
	strcpy_s(pFile->fileName, p + filePath.find_last_of("\\") + 1);
	listFile.push_back(pFile);
}

void CSmtp::DeleteAttachment(string &filePath) //ɾ������
{
	list<FILEINFO *>::iterator pIter;
	for (pIter = listFile.begin(); pIter != listFile.end(); pIter++)
	{
		if (strcmp((*pIter)->filePath, filePath.c_str()) == 0)
		{
			FILEINFO *p = *pIter;
			listFile.remove(*pIter);
			delete p;
			break;
		}
	}
}

void CSmtp::DeleteAllAttachment() /*ɾ�����е��ļ�*/
{
	for (list<FILEINFO *>::iterator pIter = listFile.begin(); pIter != listFile.end();)
	{
		FILEINFO *p = *pIter;
		pIter = listFile.erase(pIter);
		delete p;
	}
}

void CSmtp::SetSrvDomain(string &domain)
{
	this->domain = domain;
}

void CSmtp::SetUserName(string &user)
{
	this->user = user;
}

void CSmtp::SetPass(string &pass)
{
	this->pass = pass;
}
void CSmtp::SetTargetEmail(string &targetAddr)
{
	this->targetAddr = targetAddr;
}
void CSmtp::SetEmailTitle(string &title)
{
	this->title = title;
}
void CSmtp::SetContent(string &content)
{
	this->content = content;
}
void CSmtp::SetPort(int port)
{
	this->port = port;
}