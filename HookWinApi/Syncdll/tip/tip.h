#ifndef __tip_h__
#define __tip_h__
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

	void Show(const Type& type);

private:
	Tip();

	Type type_ = UNKNOWN;

private:


};

#endif // __tip_h__
