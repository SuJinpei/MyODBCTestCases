#include <stdio.h>
#include "tchar.h"
#include <errno.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
	TCHAR fileName[512] = _T("hello.txt");
	TCHAR text[1024] = _T("ÄãºÃ,ÖÐ¹ú\n");
	USHORT(*ps)[10] = text;
	UCHAR(*pc)[10] = text;
	FILE *pf = NULL;
	pf = _tfopen(fileName, _T("w"));
	if (pf){
		//fwrite(text, sizeof(TCHAR), _tcslen(text), pf);
		if (_fputts(text, pf))
		{
			printf("errno:%d, errstr:%s\n", errno, strerror(errno));
		}
		fclose(pf);
	}

	pf = _tfopen(fileName, _T("r"));
	if (pf){
		memset(text, 0, sizeof(text));
		_fgetts(text, sizeof(text)/sizeof(TCHAR), pf);
		//fread(text, sizeof(TCHAR), sizeof(text) / sizeof(TCHAR), pf);
		fclose(pf);
		_tprintf(_T("%s\n"), text);
	}

	//TCHAR buf[128] = _T("   abc   ");
	//TCHAR *p = buf;
	//while (*p++ == _T(' '));
	//_tprintf(p);
	
	return 0;
}