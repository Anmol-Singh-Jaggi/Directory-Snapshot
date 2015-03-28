#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

const path LogFileName = "DirectorySnapshotLog.txt";
ofstream Log;
// Buffer soft errors to output them separately after the informational messages in the log file.
stringstream LogErrorStream;

// Convert the input size ( in bytes ) to its nearest units in the ratio of 1024.
// ( Trying to do what Windows does on right-clicking the properties of a folder )
string RoundSize( const long long& size )
{
	double ret = ( double )size;

	vector<string> units;
	units.push_back( "bytes" );
	units.push_back( "KB" );
	units.push_back( "MB" );
	units.push_back( "GB" );
	units.push_back( "TB" );

	const unsigned ratio = 1024;
	unsigned i = 0;

	while ( ret > ratio && i < units.size() - 1 )
	{
		ret /= ratio;
		i++;
	}

	return ToString( ret ) + " " + units[i];
}

// Iterate through a directory and store everything found ( regular files, directories or any other special files ) in the input container
void DirectoryIterate( const path& dirPath, vector<path>& dirContents )
{
	if ( exists( dirPath ) && is_directory( dirPath ) )
	{
		copy( directory_iterator( dirPath ), directory_iterator(), back_inserter( dirContents ) );
	}
}

// Create a set of HTML files containing information about source directory's contents and store it in the destination directory, in a directory structure similar to the source directory
// Returns the total size of the sourcePath directory
long long Snapshot( const path& sourcePath, const path& destinationPath )
{
	Log << sourcePath << endl;

	long long sourcePathSize = 0; // Total size of the source directory

	vector<path> dirContents, files, directories;
	DirectoryIterate( sourcePath, dirContents );
	sort( dirContents.begin(), dirContents.end() ); // sort, since directory iteration is not ordered on some file systems
	for ( const auto& item : dirContents )
	{
		if ( is_directory( item ) )
		{
			directories.push_back( item );
		}
		else
		{
			files.push_back( item );
		}
	}

	const string sourcePathName = sourcePath.filename().string(); // Name of the source folder
	path pwd = destinationPath / sourcePathName; // Present working directory
	boost::system::error_code ec;
	create_directory( pwd, ec );
	if ( ec )
	{
		LogErrorStream << "Error creating " << absolute( pwd ) << " : " << ec.message() << endl;
		return 0;
	}

	// Write the HTML file header.
	const path outFilePath = ( pwd / sourcePathName ).string() + ".html";
	ofstream outFile( outFilePath.string() );
	if ( !outFile )
	{
		LogErrorStream << "Error creating " << absolute( outFilePath ) << " : " << strerror( errno ) << endl;
		return 0;
	}

	outFile << "<!DOCTYPE html>\n";
	outFile << "<meta charset=\"UTF-8\">\n";
	outFile << "<html>\n";
	outFile << "<title>" << sourcePathName << "</title>\n";
	outFile << "<body>\n";

	// Write information about the files
	outFile << "<h1> Files </h1>\n";
	for ( const auto& file : files )
	{
		auto size = file_size( file );
		outFile << file.filename() << "----" << RoundSize( size ) << "<br>\n";
		sourcePathSize += size;
	}

	// Write information about the directories
	outFile << "<h1> Directories </h1>\n";
	for ( const auto& directory : directories )
	{
		long long size = Snapshot( sourcePath / directory.filename(), pwd );
		sourcePathSize += size;
		outFile << "<a href=\"" << ( directory.filename() / directory.filename() ).generic_string() << ".html\">" << directory.filename() << "</a>----" << RoundSize( size ) << "<br>\n";
	}

	// Write the footer
	outFile << "<br>\n";
	outFile << "<h3>Total directory size = " << RoundSize( sourcePathSize ) << "</h3><br>\n";
	outFile << "</body>\n";
	outFile << "</html>\n";

	return sourcePathSize;
}


int main()
{
	string sourcePath, destinationPath;

	cout << "Enter source directory path -:\n";
	getline( cin, sourcePath );
	if ( !is_directory( sourcePath ) )
	{
		cout << absolute( sourcePath ) << " is not a directory !\n";
		return -1;
	}

	cout << "Enter destination directory path -:\n";
	getline( cin, destinationPath );
	if ( !is_directory( destinationPath ) )
	{
		cout << absolute( destinationPath ) << " is not a directory !\n";
		return -1;
	}

	cout << "\n";
	Log.open( LogFileName.string() );
	if ( !Log )
	{
		cerr << "Error creating " << absolute( LogFileName ) << " : " << strerror( errno ) << endl;
	}

	Snapshot( sourcePath, destinationPath );

	if ( Log )
	{
		if ( LogErrorStream.str().empty() )
		{
			cout << "The program ran without any errors.\n";
		}
		else
		{
			Log << "ERRORS -:\n" << LogErrorStream.str() << endl;
			cout << "There were some errors during the execution of this program !\n\nCheck " << absolute( LogFileName ) << " for details.\n";
		}
	}
}
