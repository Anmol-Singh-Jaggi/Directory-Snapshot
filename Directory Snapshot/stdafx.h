// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Debug.h>
#include <cstdio>
#include <windows.h>
#include <strsafe.h>  // For DisplayErrorBox()
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>

#define MakeDir(x) system(("mkdir \"" + x + "\"").c_str())
void DisplayErrorBox( LPTSTR lpszFunction );
typedef WIN32_FIND_DATA File;
template<typename T> string ToString( const T obj )
{
	stringstream ss;
	ss << obj;
	return ss.str();
}
DWORD DirectoryIterate( const string& dirPath, vector<File>& files, vector<File>& directories );
pair<string, string> FileName( const string& path );
// TODO: reference additional headers your program requires here
