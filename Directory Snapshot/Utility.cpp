// File containing some utility functions which are improbable to change in the future
#include "stdafx.h"

// For error-handling and displaying
// Copied from MSDN
void DisplayErrorBox( LPTSTR lpszFunction )
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
	  FORMAT_MESSAGE_ALLOCATE_BUFFER |
	  FORMAT_MESSAGE_FROM_SYSTEM |
	  FORMAT_MESSAGE_IGNORE_INSERTS,
	  NULL,
	  dw,
	  MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
	  ( LPTSTR ) &lpMsgBuf,
	  0, NULL );

	// Display the error message and clean up

	lpDisplayBuf = ( LPVOID )LocalAlloc( LMEM_ZEROINIT,
	                                     ( lstrlen( ( LPCTSTR )lpMsgBuf ) + lstrlen( ( LPCTSTR )lpszFunction ) + 40 ) * sizeof( TCHAR ) );
	StringCchPrintf( ( LPTSTR )lpDisplayBuf,
	                 LocalSize( lpDisplayBuf ) / sizeof( TCHAR ),
	                 TEXT( "%s failed with error %d: %s" ),
	                 lpszFunction, dw, lpMsgBuf );
	MessageBox( NULL, ( LPCTSTR )lpDisplayBuf, TEXT( "Error" ), MB_OK );

	LocalFree( lpMsgBuf );
	LocalFree( lpDisplayBuf );
}

// Iterate through a directory and store the details about the files and directories within in the input arguments
// Copied from MSDN
DWORD DirectoryIterate( const string& dirPath, vector<File>& files, vector<File>& directories )
{
	// Find the first file in the directory.
	File file_info;
	HANDLE hFind = FindFirstFile( ( dirPath + "\\*" ).c_str(), &file_info );
	DWORD dwError = 0;
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		DisplayErrorBox( TEXT( "FindFirstFile" ) );
		return dwError;
	}

	do
	{
		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) // File is a directory
		{
			if ( string( file_info.cFileName ) != "." && string( file_info.cFileName ) != ".." )
			{
				directories.push_back( file_info );
			}
		}
		else
		{
			files.push_back( file_info );
		}
	}
	while ( FindNextFile( hFind, &file_info ) != 0 );

	dwError = GetLastError();
	if ( dwError != ERROR_NO_MORE_FILES )
	{
		DisplayErrorBox( TEXT( "FindFirstFile" ) );
	}
	FindClose( hFind );

	return dwError;
}

// Returns the name of the file/directory given its path
pair<string, string> FileName( const string& path )
{
	size_t pos = path.find_last_of( "\\/" );
	string parent = ( pos == string::npos ) ? "" : path.substr( 0, pos );
	string file = path.substr( pos + 1 );
	assert( parent + "\\" + file == path );
	return make_pair( file, parent );
}