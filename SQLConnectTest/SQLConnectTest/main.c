#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <sqlext.h>
#include "sqlucode.h"

int main(int argc, char *argv[])
{
	SQLRETURN	returncode = 0;
	SQLHENV		hEnv;
	SQLHDBC		hDBC;
	SQLHSTMT	hStmt;
	SQLTCHAR	dataSourceName[1024] = _T("HDP24_UNICODE");
	SQLTCHAR	userId[1024] = _T("trafodion");
	SQLTCHAR	password[1024] = _T("traf123");

#ifdef UNICODE
	_tprintf(_T("UNICODE\n"));
#endif

#ifdef _UNICODE
	_tprintf(_T("_UNICODE\n"));
#endif
	// Initialize the basic handles needed by ODBC 
	returncode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);

	returncode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	returncode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);


	returncode = SQLConnect(hDBC, dataSourceName, _tcsclen(dataSourceName),
		userId, _tcslen(userId), password, _tcslen(password));
	
	if (SQL_SUCCEEDED(returncode))
	{
		_tprintf(_T("success!\n"));
	}
	else
	{
		_tprintf(_T("failed!\n"));
	}

	_gettchar();
	return 0;
}