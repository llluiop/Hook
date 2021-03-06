#include <stdio.h>
#include <Windows.h>
#include <string>
#include <iostream>

class base
{
public:
	virtual void A() = 0;
	virtual void B() = 0;
};

class derive1 : public base
{
public:
	virtual void A() = 0;
	virtual void B() = 0;
};

class derive2 : public base
{
public:
	virtual void A() = 0;
	virtual void B() = 0;
};

class foo : public derive1, public derive2
{
public:
	virtual void A()
	{

	}

	virtual void B()
	{

	}
};

int main()
{
	foo* b = new foo();
	derive1* d = static_cast<derive1*>(b);
	derive2* d2 = static_cast<derive2*>(b);

#ifdef _WIN64
	std::string path("C:\\Users\\liuyu\\Source\\Repos\\Hook\\HookWinApi\\x64\\Debug\\dll_installer.exe 4492 TEST");
#else
	std::string path("C:\\Users\\liuyu\\Source\\Repos\\Hook\\HookWinApi\\Debug\\dll_installer.exe 7048 TEST");
#endif

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	//创建一个新进程  
	if (CreateProcess(
		NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串  
		(LPSTR)path.c_str(), // 命令行字符串  
		NULL, //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。  
		NULL, //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>  
		false,//    指示新进程是否从调用进程处继承了句柄。   
		0,  //  指定附加的、用来控制优先类和进程的创建的标  
			//  CREATE_NEW_CONSOLE  新控制台打开子进程  
			//  CREATE_SUSPENDED    子进程创建后挂起，直到调用ResumeThread函数  
		NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境  
		NULL, //    指定子进程的工作路径  
		&si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体  
		&pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体  
	))
	{
		//cout << "create process success" << endl;
		return true;

	}
	else {
		auto err = GetLastError();
	}


	return getchar();
}