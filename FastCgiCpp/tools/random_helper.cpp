#include "random_helper.h"

random_helper::random_helper()
{

}
    //说明，此处的数组都是在内存里分配了空间的，主函数用完返回的指针后必须调用free函数释放内存
    //这也是要做的一个工作，看能否在此类成员函数调用完后，自行释放分配的内存空间。


    //获取指定长度的字符串
    //获取指定长度的字符串（大小写英文字母+数字）
    //length: 将要生成的字符长度
    //return: 生成好的随机字符串
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
    return string; //这个名为string的指针在调用完此函数后已经释放了。
}

    //生成随机英文字母字符串（大小写英文字母）
    //length: 将要生成的字符长度
    //return: 生成好的随机字符串
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

    //获取指定长度的数字，类型是字符串
    //length: 将要生成的字符长度
    //return: 生成好的随机字符串
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

    //获取指定大小的整形数值
    //small: 最小数值
    //max: 最大数值
    //return: 生成好的随机数值
    //返回一个 low<=x<high的值
int random_helper::get_number_for_range(int low,int up)
{
    int i=-1;
    int k;
    srand((unsigned)time(NULL));
    i=(rand()%(up-low)) + low;
    return i;
}

    //"""
    //内部函数，获取指定长度的随机字符
    //length: 将要生成的字符长度
    //textpool: 生成随机字符的字符池
    //return: 生成好的随机字符串
    //"""
char* random_helper::get_randoms(int length,char*textpool)
{
    int i,j,len;
    char *str;
    str = (char*)malloc(length+1);
    len = strlen(textpool);//求字符池的长度
    srand(time(0));
    for (i=0;i<length;i++)
    {
        j = rand()%len;
        str[i] = textpool[j];
    }
    str[i]='\0';   //末尾加个空字符
    return str;
}

    //"""
    //随机生成uuid
    //return: 生成的uuid
    //"""
    //char* get_uuid()
    //{
    //    uuid_t uu;
    //	uuid_generate( uu );
    //	return uu;
    //}


    //分配的内存空间记得free掉！！！！！
random_helper::~random_helper()
{

}


