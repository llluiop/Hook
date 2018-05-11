#ifndef __tip_h__
#define __tip_h__

#include <windows.h>

class Tip
{
public:
	static Tip* Instance()
	{
		static Tip instance;
		return &instance;
	}

	enum Type
	{
		UNKNOWN,
		FROM_COM,
		FROM_API,
	};

	void Show(const Type& type)
	{
		if (type_ == UNKNOWN)
		{
			::MessageBox(GetDesktopWindow(), L"加密文件不允许此操作", L"文件防泄漏", NULL);
			type_ = type;

		}
		else
		{
			if (type_ != type)
			{
				OutputDebugStringA("type changed, not show");
			}
			else
			{
				::MessageBox(GetDesktopWindow(), L"加密文件不允许此操作", L"文件防泄漏", NULL);
				type_ = type;

			}
		}
		
	}

private:
	Tip() {}

	Type type_ = UNKNOWN;
};

#endif // __tip_h__
