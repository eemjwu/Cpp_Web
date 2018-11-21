#ifndef RANDOM_HELPER_H
#define RANDOM_HELPER_H
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <random_helper.h>
#include <stdlib.h>
#include <time.h>



//此包应在linux环境下包括
//#include <uuid/uuid.h>


class random_helper
{
    public:
        random_helper();


        char* get_string(int length);
        char* get_letters(int length);
        char* get_number(int length);
        int get_number_for_range(int low,int up);
        char* get_randoms(int length,char*textpool);


        virtual ~random_helper();

    protected:

    private:
};

#endif // RANDOM_HELPER_H
