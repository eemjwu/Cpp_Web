#include "random_helper.h"

random_helper::random_helper()
{

}
    //˵�����˴������鶼�����ڴ�������˿ռ�ģ����������귵�ص�ָ���������free�����ͷ��ڴ�
    //��Ҳ��Ҫ����һ�����������ܷ��ڴ����Ա����������������ͷŷ�����ڴ�ռ䡣


    //��ȡָ�����ȵ��ַ���
    //��ȡָ�����ȵ��ַ�������СдӢ����ĸ+���֣�
    //length: ��Ҫ���ɵ��ַ�����
    //return: ���ɺõ�����ַ���
char* random_helper::get_string(int length)
{
    int flag, i;
    char* string;
    srand((unsigned)time(NULL));
    if ((string = (char*)malloc(length+1)) == NULL)
    {
        printf("Malloc failed!flag:14\n");
        return NULL;
    }

    for (i = 0; i < length + 1; i++)
    {
        flag = rand() % 3;
        switch (flag)
        {
        case 0:
            string[i] = 'A' + rand() % 26;
            break;
        case 1:
            string[i] = 'a' + rand() % 26;
            break;
        case 2:
            string[i] = '0' + rand() % 10;
            break;
        default:
            string[i] = 'x';
            break;
        }
    }
    string[length] = '\0';
    return string; //�����Ϊstring��ָ���ڵ�����˺������Ѿ��ͷ��ˡ�
}

    //�������Ӣ����ĸ�ַ�������СдӢ����ĸ��
    //length: ��Ҫ���ɵ��ַ�����
    //return: ���ɺõ�����ַ���
char* random_helper::get_letters(int length)
{
    int flag, i;
    char* string;
    srand((unsigned)time(NULL));
    if ((string = (char*)malloc(length+1)) == NULL)
    {
        printf("Malloc failed!flag:14\n");
        return NULL;
    }

    for (i = 0; i < length + 1; i++)
    {
        flag = rand() % 2;
        switch (flag)
        {
        case 0:
            string[i] = 'A' + rand() % 26;
            break;
        case 1:
            string[i] = 'a' + rand() % 26;
            break;
        default:
            string[i] = 'x';
            break;
        }
    }
    string[length] = '\0';
    return string;
}

    //��ȡָ�����ȵ����֣��������ַ���
    //length: ��Ҫ���ɵ��ַ�����
    //return: ���ɺõ�����ַ���
char* random_helper::get_number(int length)
{
    int flag, i;
    char* string;
    srand((unsigned)time(NULL));
    if ((string = (char*)malloc(length+1)) == NULL)
    {
        printf("Malloc failed!flag:14\n");
        return NULL;
    }

    for (i = 0; i < length + 1; i++)
    {

        string[i] = '0' + rand() % 10;

    }
    string[length] = '\0';
    return string;
}

    //��ȡָ����С��������ֵ
    //small: ��С��ֵ
    //max: �����ֵ
    //return: ���ɺõ������ֵ
    //����һ�� low<=x<high��ֵ
int random_helper::get_number_for_range(int low,int up)
{
    int i=-1;
    int k;
    srand((unsigned)time(NULL));
    i=(rand()%(up-low)) + low;
    return i;
}

    //"""
    //�ڲ���������ȡָ�����ȵ�����ַ�
    //length: ��Ҫ���ɵ��ַ�����
    //textpool: ��������ַ����ַ���
    //return: ���ɺõ�����ַ���
    //"""
char* random_helper::get_randoms(int length,char*textpool)
{
    int i,j,len;
    char *str;
    str = (char*)malloc(length+1);
    len = strlen(textpool);//���ַ��صĳ���
    srand(time(0));
    for (i=0;i<length;i++)
    {
        j = rand()%len;
        str[i] = textpool[j];
    }
    str[i]='\0';   //ĩβ�Ӹ����ַ�
    return str;
}

    //"""
    //�������uuid
    //return: ���ɵ�uuid
    //"""
    //char* get_uuid()
    //{
    //    uuid_t uu;
    //	uuid_generate( uu );
    //	return uu;
    //}


    //������ڴ�ռ�ǵ�free������������
random_helper::~random_helper()
{

}


