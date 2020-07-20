#pragma once
class Singleton
{
	Singleton()
	{
		frame = 60;
		for (size_t i = 0; i < 360; i++)
		{
			radian[i] = i * 3.1415978 / 180;
			cosArray[i] = cos(radian[i]);
			sinArray[i] = sin(radian[i]);
		}
	}
public:
	const LPCWSTR title = L"강진 지키기";
	std::wstring name = L"";
	CSceneManager sceneManager;
	HFONT titleFont;
	HFONT idMenuFont;
	HFONT idFont;
	HFONT idSmallFont;
	ULONG score;
	size_t frame;
	double radian[360];
	double sinArray[360];
	double cosArray[360];
	static Singleton* GetInstance()
	{
		static Singleton ins;
		return &ins;
	}

	~Singleton()
	{
	}
};

