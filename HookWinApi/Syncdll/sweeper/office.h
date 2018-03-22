#ifndef __office_h__
#define __office_h__

class SweepOffice
{
public:
	static SweepOffice* Instance()
	{
		static SweepOffice instance;
		return &instance;
	}

	void Sweep()
	{
		Sleep(200);

		auto h = FindWindowA("#32770", "Microsoft Office Excel");
		if (h != nullptr)
		{
			PostMessage(h, WM_CLOSE, 0, 0);
		}

		Sleep(200);

		h = FindWindowA("#32770", "Microsoft Office Excel");
		if (h != nullptr)
		{
			PostMessage(h, WM_CLOSE, 0, 0);
		}

		Sleep(200);

		h = FindWindowA("#32770", "Microsoft Office Excel");
		if (h != nullptr)
		{
			PostMessage(h, WM_CLOSE, 0, 0);
		}
	}

private:
	SweepOffice(){}
};


#endif
