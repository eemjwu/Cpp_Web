#include"CSql.h"
#include<iostream>;

using namespace std;
int main(void)
{
	CSql S;
	if (S.UserLogin("wmj", "123456") == 0) cout << "�ɹ�" << endl;
	else cout << "ʧ��" << endl;

	return 0;
}


