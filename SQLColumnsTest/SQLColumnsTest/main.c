
#include <windows.h>
#include <sqlext.h>
#include <tchar.h>

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

int main() {
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt = 0;
	SQLRETURN retcode;

#define TEST_TRAFODION
#ifdef TEST_TRAFODION
	SQLTCHAR	dataSourceName[1024] = _T("HDP24_UNICODE");
	SQLTCHAR	userId[1024] = _T("trafodion");
	SQLTCHAR	password[1024] = _T("traf123");
	SQLTCHAR	catalogName[1024] = _T("TRAFODION");
	SQLTCHAR	schemaName[1024] = _T("SU_ODBC_TEST_R22_WIN10_UNICODE_RC2");
	SQLTCHAR	tableName[1024] = _T("TEST");
#else
	SQLTCHAR	dataSourceName[1024] = _T("MySQL_ODBC");
	SQLTCHAR	userId[1024] = _T("root");
	SQLTCHAR	password[1024] = _T("123456");
	SQLTCHAR	catalogName[1024] = _T("");
	SQLTCHAR	schemaName[1024] = _T("test");
	SQLTCHAR	tableName[1024] = _T("test");
#endif

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	retcode = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
	retcode = SQLConnect(hdbc, dataSourceName, _tcsclen(dataSourceName),
		userId, _tcslen(userId), password, _tcslen(password));
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	retcode = SQLSetStmtAttr(hstmt, SQL_ATTR_METADATA_ID, (SQLPOINTER*)SQL_TRUE, 0);
	retcode = SQLStatistics(hstmt, catalogName, _tcslen(catalogName), schemaName, _tcslen(schemaName), tableName, _tcslen(tableName), SQL_INDEX_ALL, SQL_QUICK);

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

		fetchResult(hstmt);
	}
	return 0;
}