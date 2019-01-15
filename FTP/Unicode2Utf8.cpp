#include "stdafx.h"
#include "Unicode2Utf8.h"


Unicode2Utf8::Unicode2Utf8()
{
}


Unicode2Utf8::~Unicode2Utf8()
{
}
/*
Unicode				| UTF-8
0000 0000-0000 007F | 0xxxxxxx
0000 0080-0000 07FF | 110xxxxx 10xxxxxx
0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
int Unicode2Utf8::trans(char * cur, TCHAR uni)
{
	unsigned long long u = (unsigned short)uni;
	if (u < 0x80u) {
		*cur = (unsigned char)uni;
		return 1;
	}
	else if (u < 0x800u) {
		*(cur + 1) = (0x003fu & u) | 0x0080u;
		*cur = (0x001fu & (u >> 6)) | 0x00c0u;
		return 2;
	}
	else if (u < 0x10000u) {
		*(cur + 2) = (0x003fu & u) | 0x0080u;
		*(cur + 1) = (0x003fu & (u >> 6)) | 0x0080u;
		*cur = (0x000fu & (u >> 12)) | 0x00e0u;
		return 3;
	}
	//no.4 line ignore
	return 0;
}

int Unicode2Utf8::unicode2Utf8(char * dst, CString uni)
{
	int sum = 0, n = uni.GetLength();
	char *cur = dst;
	for (int i = 0; i < n; i++) {
		sum += trans(cur, uni.GetAt(i));
		cur = dst + sum;
	}
	return sum;
}

void Unicode2Utf8::utf82Unicode(char * src, int n, CString & uni)
{
	int index = 0;
	uni = _T("");
	while (index < n) {
		unsigned char cur = src[index];
		int num = 0;
		TCHAR data = 0;
		if ((cur & 0x80u) == 0x00u) {
			//1 bytes
			num = 1;
			data = (unsigned short)(src[index++] & 0x7f);
			uni += data;
		}
		else if ((cur & 0xe0) == 0xc0) {
			//2 bytes
			num = 2;
			data = (unsigned short)(src[index++] & 0x1f);
			data <<= 6;
			data |= (unsigned short)(src[index] & 0x3f);
			uni += data;
		}
		else if ((cur & 0xf0) == 0xe0) {
			//3 bytes
			num = 3;
			data = (unsigned short)(src[index++] & 0x0f);
			data <<= 12;
			unsigned short t = (unsigned short)(src[index++] & 0x3f);
			data |= (t << 6);
			t = (unsigned short)(src[index++] & 0x3f);
			data |= t;
			uni += data;
		}
		else {
			//ignore
			index++;
		}
	}
	return;
}
