#include "sweeper.h"
#include "office.h"

bool Sweeper::Start()
{
	sigs_[0] = CreateEvent(nullptr, false, false, nullptr);
	sigs_[1] = CreateEvent(nullptr, false, false, nullptr);

	t_ = std::move(std::thread([&] {
		while (!quit_)
		{
			int index = WaitForMultipleObjects(2, sigs_, false, 500);
			if (index == WAIT_OBJECT_0) //save-as trigger
			{
				SweepOffice::Instance()->Sweep();
			}
			else if (index == WAIT_OBJECT_0 + 1) //copy trigger
			{

			}
			Sleep(500);
		}
	}));


	return true;
}

void Sweeper::Stop()
{
	quit_ = true;
	if (t_.joinable())
	{
		t_.join();
	}

	CloseHandle(sigs_[0]);
	CloseHandle(sigs_[1]);
}

void Sweeper::SaveAsTriggered()
{
	SetEvent(sigs_[0]);
}

Sweeper::Sweeper()
{

}
