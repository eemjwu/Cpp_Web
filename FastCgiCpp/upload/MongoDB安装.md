# mongoDB在redhat下的安装
## mongoDB下载安装
1. 下载地址：[http://fastdl.mongodb.org/linux/mongodb-linux-x86_64-2.6.10.tgz](http://fastdl.mongodb.org/linux/mongodb-linux-x86_64-2.6.10.tgz)
2. root用户依次执行以下操作  
	tar -zxvf mongodb-linux-x86_64-2.6.10.tgz     
	mv mongodb-linux-x86_64-2.6.10 /usr/local/mongodb      
	cd /usr/local/mongodb     
	mkdir db  
	mkdir logs  
	cd bin  
3. 增加配置文件  
	vi mongodb.conf
    >dbpath=/usr/local/mongodb/db
	logpath=/usr/local/mongodb/logs/mongodb.log
	port=27107
	fork=true
	nohttpinterface=true
4. 开机自动启动mongodb  
	vi /etc/rc.d/rc.local
	>/usr/local/mongodb/bin/mongod --config /usr/local/mongodb/bin/mongodb.conf
	ln -s /usr/local/mongdb/bin/mongo /usr/local/bin/mongo
## C++驱动的下载源
下载地址：[http://github.com/mongodb/mongo-cxx-driver/tree/legacy](http://github.com/mongodb/mongo-cxx-driver/tree/legacy)
## c++驱动库安装
步骤：  
	1. 安装 boost 库   
	2. 安装 PCRE  库  正则表达式库  
	3. scons 编译mongodb驱动的指令   Sconstruct 这个文件是编译的关键   
	4. 编译驱动获得 头文件+库文件  
###安装 boost 库
文件地址：D:\Study\数据库\安装包\mongoDB  
文件名：boost_1_66_0  
进入文件夹：`cd boost_1_66_0 `  
依次运行：  

	./bootstrap.sh
	./b2
	./sudo ./b2 install  
###安装 PCRE  库
文件位置：D:\Study\数据库\安装包\mongoDB  
文件名：pcre-8.00.tar.gz  
上传到redhat  
依次执行：  

	tar -zxvf pcre-8.00.tar.gz  
	cd pcre-8.00.tar.gz  
	./configure  
	make  
	make install  
###	安装 scons 编译mongodb驱动的指令
文件位置：D:\Study\数据库\安装包\mongoDB  
文件名：scons-2.5.0-1.noarch.rpm  
上传至redhat  
执行：  

	rpm -ivh scons-2.5.0-1.noarch.rpm  

###安装编译驱动程序
文件位置：D:\Study\数据库\安装包\mongoDB  
文件名：mongodb-linux-x86_64-v2.2-latest.tgz  
依次执行：  

	tar -zxvf mongodb-linux-x86_64-v2.2-latest.tgz
	cd mongo-cxx-driver-v2.2
	vi src/mongo/pch.h 
	修改：#define BOOST_FILESYSTEM_VERSION 3
	sudo scons --extrapath=/usr/local install  
###得到文件部署  
进入usr/local 整合文件：   
> [eemjwu@localhost local]$ ls boost  
> include  lib  
> [eemjwu@localhost local]$ ls mongo  
> include  lib  

切换到root用户执行：  

	vi /etc/ld.so.conf
	添加:上面文件的路径
	/usr/local/boost/lib
	/usr/local/mongo/lib
	保存
	ldconfig
