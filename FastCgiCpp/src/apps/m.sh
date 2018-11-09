g++ -o mongo TMongo.cpp -std=c++11 -lTool -lmysqlclient -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system -pthread -L/usr/local/driver/mongo/lib -L/usr/local/driver/boost/lib -L/home/liukang/workspace/FastCgiCpp/tools -I/usr/local/driver/boost/include -I/usr/include/mysql -I/usr/local/driver/mongo/include -I/home/liukang/workspace/FastCgiCpp/tools

