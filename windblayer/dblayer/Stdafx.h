// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifdef DEBUG
#pragma comment(lib, "..\\Debug\\schema.lib")
#else
#pragma comment(lib, "..\\Debug\\schema.lib")
#endif

#ifdef USE_MYSQL
//# pragma comment(lib, "C:\\Program Files\\MySQL\\MySQL Connector C 6.0.2\\lib\\opt\\libmysql.lib")
#endif
