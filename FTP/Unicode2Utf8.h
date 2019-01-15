#pragma once
class Unicode2Utf8
{
public:
	Unicode2Utf8();
	~Unicode2Utf8();
	int trans(char* cur, TCHAR uni);
	int unicode2Utf8(char *dst, CString uni);
	void utf82Unicode(char *src, int n, CString &uni);
};

