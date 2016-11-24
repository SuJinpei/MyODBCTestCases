#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <sqlext.h>
#include "sqlucode.h"

void fetchResult(SQLHSTMT hStmt)
{
	SQLSMALLINT columns = 0;
	SQLRETURN ret = 0;
	int row = 0;

	SQLNumResultCols(hStmt, &columns);
	while (SQL_SUCCEEDED(ret = SQLFetch(hStmt))) {
		SQLUSMALLINT i;
		_tprintf(_T("Row %d\n"), row++);

		for (i = 1; i <= columns; i++) {
			SQLLEN indicator;
			char buf[512];
			memset(buf, 0, sizeof(buf));
			ret = SQLGetData(hStmt, i, SQL_C_CHAR,
				buf, sizeof(buf), &indicator);
			if (SQL_SUCCEEDED(ret)) {
				if (indicator == SQL_NULL_DATA) strcpy(buf, _T("NULL"));
				_tprintf(_T("  Column %u : %s\n"), i, buf);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	SQLRETURN	returncode = 0;
	SQLHENV		hEnv;
	SQLHDBC		hDBC;
	SQLHSTMT	hStmt;
	SQLTCHAR	dataSourceName[1024] = _T("MySQL_ODBC");
	SQLTCHAR	userId[1024] = _T("root");
	SQLTCHAR	password[1024] = _T("123456");
	SQLTCHAR	sqlStmt[1024] = _T("select * from test");

	// Initialize the basic handles needed by ODBC 
	returncode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);

	returncode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	returncode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC);

	returncode = SQLConnect(hDBC, dataSourceName, _tcsclen(dataSourceName),
		userId, _tcslen(userId), password, _tcslen(password));

	returncode = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hStmt);

	returncode = SQLExecDirect(hStmt, sqlStmt, _tcslen(sqlStmt));

	// fetch result
	fetchResult(hStmt);

	returncode = SQLStatistics(hStmt, NULL, 0, _T("test"), 4, _T("test"), 4, SQL_INDEX_UNIQUE, SQL_ENSURE);

	fetchResult(hStmt);

	return 0;
}