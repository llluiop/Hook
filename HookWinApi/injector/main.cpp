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

	//����һ���½���  
	if (CreateProcess(
		NULL,   //  ָ��һ��NULL��β�ġ�����ָ����ִ��ģ��Ŀ��ֽ��ַ���  
		(LPSTR)path.c_str(), // �������ַ���  
		NULL, //    ָ��һ��SECURITY_ATTRIBUTES�ṹ�壬����ṹ������Ƿ񷵻صľ�����Ա��ӽ��̼̳С�  
		NULL, //    ���lpProcessAttributes����Ϊ�գ�NULL������ô������ܱ��̳С�<ͬ��>  
		false,//    ָʾ�½����Ƿ�ӵ��ý��̴��̳��˾����   
		0,  //  ָ�����ӵġ���������������ͽ��̵Ĵ����ı�  
			//  CREATE_NEW_CONSOLE  �¿���̨���ӽ���  
			//  CREATE_SUSPENDED    �ӽ��̴��������ֱ������ResumeThread����  
		NULL, //    ָ��һ���½��̵Ļ����顣����˲���Ϊ�գ��½���ʹ�õ��ý��̵Ļ���  
		NULL, //    ָ���ӽ��̵Ĺ���·��  
		&si, // �����½��̵������������ʾ��STARTUPINFO�ṹ��  
		&pi  // �����½��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��  
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