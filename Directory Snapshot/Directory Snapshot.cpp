#include "stdafx.h"
using namespace std;

// Convert the input size in bytes to its nearest units in the ratio of 1024.
// ( Trying to do what Windows does on right-clicking the properties of a folder )
string RoundSize( long long size )
{
	double dsize = ( double )size;
	const int ratio = 1024;
	int i = 0; // 0=bytes 1=kb 2=mb 3=gb 4=gb
	while ( dsize > ratio && i < 4 )
	{
		dsize /= ratio;
		i++;
	}
	const string units[] = {"bytes", "KB", "MB", "GB", "TB"};
	return ToString( dsize ) + " " + units[i];
}

ofstream LOG( "log.txt" );
// Creates an HTML file corresponding to the sourcePath directory and stores it in destinationPath
// Returns the total size of the sourcePath directory
long long Snapshot( const string& sourcePath, const string& destinationPath )
{
	LOG << sourcePath << endl; // Logging for debugging purposes
	long long sourcePathSize = 0; // Total size of the sourcePath directory

	vector<File> files, directories; // List of files and directories contained in sourcePath
	DirectoryIterate( sourcePath, files, directories ); // Populate "files" and "directories"

	const string& sourceName = FileName( sourcePath ).first; // Name of the sourcePath folder
	string pwd = destinationPath + "\\" + sourceName; // Present working directory
	MakeDir( pwd ); // Make a new directory

	// Begin writing the HTML file header.
	ofstream HTML( pwd + "\\" + sourceName + ".html" );
	HTML << "<!DOCTYPE html>\n";
	HTML << "<html>\n";
	HTML << "<title>" << sourceName << "</title>\n";
	HTML << "<body>\n";

	// Write information about the files
	HTML << "<h1> Files </h1>\n";
	for ( unsigned i = 0; i < files.size(); i++ )
	{
		LARGE_INTEGER filesize;
		filesize.LowPart = files[i].nFileSizeLow;
		filesize.HighPart = files[i].nFileSizeHigh;

		HTML << files[i].cFileName << "----" << RoundSize( filesize.QuadPart ) << "<br/>\n";
		sourcePathSize += filesize.QuadPart;
	}

	// Write information about the directories
	HTML << "<h1> Directories </h1>\n";
	for ( unsigned i = 0; i < directories.size(); i++ )
	{
		long long size = Snapshot( sourcePath + "\\" + directories[i].cFileName, pwd );
		sourcePathSize += size;
		HTML << "<a href=\"" << directories[i].cFileName << "\\" << directories[i].cFileName << ".html\">" << directories[i].cFileName << "</a>----" << RoundSize( size ) << "</br>\n";
	}

	// Write the footer
	HTML << "</br>\n";
	HTML << "<h3>Total directory size = " << RoundSize( sourcePathSize ) << "</h3></br>\n";
	HTML << "</body>\n";
	HTML << "</html>\n";

	return sourcePathSize;
}


int main()
{
	string sourcePath, destinationPath;
	cout << "Enter source path -:\n";
	getline( cin, sourcePath );
	cout << "Enter destination path -:\n";
	getline( cin, destinationPath );
	Snapshot( sourcePath, destinationPath );
	system("pause");
}
