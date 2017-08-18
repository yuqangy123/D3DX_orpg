/************************************************************************/
/*  
	2012-10-4
*/
/************************************************************************/
#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <list>
#include <queue>
//#include <pair>
#include <assert.h>

#if defined(WIN32)
#include <winsock2.h>//之前包含winsock2.h 避免与winsock.h的定义冲突
#include <TCHAR.h>
/*#include <Windows.h>*/
#else
#endif

typedef char            	int8;
typedef unsigned char   	u_int8;
typedef int             	int32;
typedef short           	int16;
typedef __int64  			int64;
typedef unsigned short		u_int16;
typedef unsigned int    		u_int32;
typedef unsigned __int64	u_int64;
typedef unsigned long   	u_long;
typedef unsigned char   	u_char;

#define	 castr_t				const char*
#define cwstr_t				const wchar_t*
#define astring_t			std::string
#define wstring_t			std::wstring
#define vector_t			std::vector
#define list_t				std::list
#define map_t				std::map
#define pair_t				std::pair
#define set_t				std::set
#define bitset_t            std::bitset
#define queue_t             std::queue


//通用字符类型
#if defined(UNICODE) || defined(_UNICODE)
#define char_t wchar_t
#define const_char_t cwstr_t
#define string_t	 wstring_t

#ifndef _T
#define _T(x)		L ## x
#endif

#else
#define char_t char
#define const_char_t castr_t
#define string_t			astring_t

#ifndef _T
#define _T(x)		x
#endif

#endif