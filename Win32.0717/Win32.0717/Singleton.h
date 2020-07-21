#pragma once
class Singleton
{
	Singleton()
	{
		frame = 24;
		for (size_t i = 0; i < 360; i++)
		{
			radian[i] = i * 3.1415978 / 180;
			cosArray[i] = cos(radian[i]);
			sinArray[i] = sin(radian[i]);
		}
	}
public:
	const std::wstring title = L"강진 지키기";
	std::wstring name = L"";
	CSceneManager * sceneManager;
	HFONT titleFont;
	HFONT idMenuFont;
	HFONT idFont;
	HFONT idSmallFont;
	HBRUSH lightBlueBrush;
	HBRUSH BlueBrush;
	HBRUSH lightPinkBrush;
	HBRUSH darkGreenBrush;
	HBRUSH darkGreyBrush;
	HBRUSH darkRedBrush;
	ULONG score;
	HWND hWnd;
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

