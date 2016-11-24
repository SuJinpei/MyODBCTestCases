#include <windows.h>
#include <sqlext.h>
#include <tchar.h>
#include <stdio.h>

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
				if (indicator == SQL_NULL_DATA) strcpy_s(buf, sizeof(buf), _T("NULL"));
				_tprintf(_T("  Column %u : %s\n"), i, buf);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	SQLHENV hEnv;
	SQLHDBC hDbc;
	SQLHSTMT hStmt;
	SQLRETURN retcode;

	SQLTCHAR outstr[1024];
	SQLSMALLINT outstrlen;

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &hEnv);
	retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	retcode = SQLDriverConnect(hDbc, NULL, _T("DSN=HDP24_UNICODE;UID=trafodion;PWD=traf123"), SQL_NTS, outstr, sizeof(outstr),&outstrlen, SQL_DRIVER_COMPLETE);
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	retcode = SQLTables(hStmt, NULL, 0, NULL, 0, NULL, 0, _T("TABLE"), SQL_NTS);

	fetchResult(hStmt);

	return 0;
}