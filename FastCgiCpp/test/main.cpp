#include"CSql.h"
#include<iostream>;

using namespace std;
int main(void)
{
	CSql S;
	if (S.UserLogin("wmj", "123456") == 0) cout << "³É¹¦" << endl;
	else cout << "Ê§°Ü" << endl;

	return 0;
}


