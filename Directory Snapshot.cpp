#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include "boost/filesystem.hpp"

using namespace std;
using namespace boost::filesystem;

static ofstream Log;
static stringstream LogErrorStream;  // Buffer soft errors to output them separately after the informational messages in the log file.

// Convert any type to its string representation
template<typename T> static string ToString( const T& obj )
{
	stringstream ss;
	ss << obj;
	return ss.str();
}

// Escape HTML special characters
static string EscapeHTMLSpecialChars( const path& PathName, const bool& href = false )
{
	string in = PathName.string();
	string ret;
	for ( decltype( in.size() ) i = 0; i < in.size(); i++ )
	{
		if ( in[i] == '&' )
		{
			ret += "&amp;";
		}
		else if ( in[i] == '<' )
		{
			ret += "&lt;";
		}
		else if ( in[i] == '>' )
		{
			ret += "&gt;";
		}
		else if ( in[i] == '"' )
		{
			ret += "&quot;";
		}
		else if ( in[i] == '\'' )
		{
			ret += "&#39;";
		}
		else if ( href && in[i] == '#' ) // If the input is an href attribute
		{
			ret += "%23";
		}
		else if ( href && in[i] == '?' )
		{
			ret += "%3F";
		}
		else
		{
			ret += in[i];
		}
	}
	return ret;
}

// Convert the input size ( in bytes ) to its nearest units in the ratio of 1024.
// ( Trying to do how Windows reports size of a file on right clicking and checking its properties )
static string RoundSize( const long long& size )
{
	double ret = ( double )size;
	static const vector<string> units {"bytes", "KiB", "MiB", "GiB", "TiB"};
	const unsigned ratio = 1024;
	unsigned i;
	for ( i = 0; ret > ratio && i < units.size() - 1; i++ )
	{
		ret /= ratio;
	}
	return ToString( ret ) + " " + units[i];
}

// Iterate through a directory and store everything found ( regular files, directories or any other special files ) in the input container
static void DirectoryIterate( const path& dirPath, vector<path>& dirContents )
{
	if ( is_directory( dirPath ) )
	{
		copy( directory_iterator( dirPath ), directory_iterator(), back_inserter( dirContents ) );
	}
}

// Returns the difference in height in the filesystem tree, between the directory "parent" and the file/folder "descendant"
static int HeightDiff( const path parent, path descendant )
{
	int diff = 0;
	while ( descendant != parent )
	{
		descendant = descendant.parent_path();
		if ( descendant.empty() )
		{
			diff = -1;  // "descendant" is in fact not a descendant of "parent"
			break;
		}
		diff++;
	}
	return diff;
}

// Returns true if the file/folder "descendant" is a descendant of the directory "parent"
static bool IsDescendant( const path parent, path descendant )
{
	return HeightDiff( parent, descendant ) >= 1;
}

// Create a set of HTML files containing information about source directory's contents and store it in the destination directory, in a directory structure similar to the source directory
// Returns the total size of the source directory
static long long Snapshot( const path& sourcePath, const path& destinationPath )
{
	Log << sourcePath << endl;

	// This should be executed only once during the whole program execution ( during the first invocation of Snapshot() )
	static bool isDescendant = IsDescendant( sourcePath, destinationPath );
	if ( isDescendant )
	{
		LogErrorStream << "The destination path cannot be a descendant of the source path!! Please provide an alternate destination path !!" << endl;
		return -1;
	}

	boost::system::error_code ec;

	long long sourcePathSize = 0; // Total size of the source directory

	vector<path> dirContents, files, directories;
	try
	{
		DirectoryIterate( sourcePath, dirContents );
	}
	catch ( const filesystem_error& ex )
	{
		LogErrorStream << ex.what() << endl;
		return 0; // cannot iterate through the directory, so no point in going further
	}

	sort( dirContents.begin(), dirContents.end() ); // sort, since directory iteration is not ordered on some file systems
	for ( const auto& item : dirContents )
	{
		ec.clear();
		if ( is_directory( item, ec ) )
		{
			directories.push_back( item );
		}
		else if ( !ec )
		{
			files.push_back( item );
		}
		else
		{
			LogErrorStream << "Failed to determine if " << absolute( item ) << " is a directory or not : " << ec.message() << endl;
		}
	}

	const path pwd = destinationPath / sourcePath.filename(); // Present working directory
	ec.clear();
	create_directory( pwd, ec );
	if ( ec )
	{
		LogErrorStream << "Failed to create " << absolute( pwd ) << " : " << ec.message() << endl;
		return 0;
	}

	// Create the output file.
	const path outFilePath = ( pwd / sourcePath.filename() ).string() + ".html";
	ofstream outFile( outFilePath.string() );
	if ( !outFile )
	{
		LogErrorStream << "Failed to create " << absolute( outFilePath ) << " : " << strerror( errno ) << endl;
		return 0;
	}

	// Write the HTML file header.
	outFile << ""
					"<!DOCTYPE html>\n"
					"<html>\n"
					"<head>\n"
					"<meta charset=\"UTF-8\">\n"
					"<title>" << EscapeHTMLSpecialChars( sourcePath.filename() ) << "</title>\n"
					"<style type=\"text/css\">\n"
					" tr:nth-child(even) { background-color: #FFFFFF; }\n"
					" tr:nth-child(odd) { background-color: #F1F1F1; }\n"
					"</style>"
					"</head>\n"
					"<body>\n";

	// Write information about the files contained in the source directory
	outFile << ""
					"<h1> Files </h1>\n"
					"<table>\n";
	for ( const auto& file : files )
	{
		outFile << ""
						" <tr>\n"
						"  <td>" << EscapeHTMLSpecialChars( file.filename() ) << "</td>\n"
						"  <td>";

		ec.clear();
		auto size = file_size( file, ec );
		if ( ec )
		{
			LogErrorStream << "Failed to read size of " << absolute( file ) << " : " << ec.message() << endl;
		}
		else
		{
			outFile << RoundSize( size );
			sourcePathSize += size;
		}
		outFile << ""
						"</td>\n"
						" </tr>\n";
	}
	outFile << "</table>\n";

	// Write information about the directories contained in the source directory
	outFile << ""
					"<h1> Directories </h1>\n"
					"<table>\n";
	for ( const auto& directory : directories )
	{
		long long size = Snapshot( sourcePath / directory.filename(), pwd );
		sourcePathSize += size;
		outFile << ""
						" <tr>\n"
						"  <td><a href=\"" << EscapeHTMLSpecialChars( ( directory.filename() / directory.filename() ).generic_string(), true ) << ".html\">" << EscapeHTMLSpecialChars( directory.filename() ) << "</a></td>\n" <<
						"  <td>" << RoundSize( size ) << "</td>\n"
						" </tr>\n";
	}
	outFile << "</table>\n";

	// Write the footer
	outFile << ""
					"<br>\n"
					"<h3>Total directory size = " << RoundSize( sourcePathSize ) << "</h3><br>\n"
					"</body>\n"
					"</html>\n";

	return sourcePathSize;
}

int main( int argc, char** argv )
{
	const path defaultLogFilePath = "DirectorySnapshotLog.txt";

	if ( argc < 3 )
	{
		cout << "Usage : " << argv[0] << " <source_directory_path> <destination_directory_path> [log_file_path=" << defaultLogFilePath << "]\n";
		return -1;
	}

	const path LogFilePath = ( ( argc >= 4 ) ? path( argv[3] ) : defaultLogFilePath );
	Log.open( LogFilePath.string() );
	if ( !Log )
	{
		cerr << "Error creating " << LogFilePath << " : " << strerror( errno ) << endl;
		return -1;
	}

	try
	{
		Snapshot( canonical( argv[1] ), canonical( argv[2] ) );
	}
	catch ( const filesystem_error& ex )
	{
		LogErrorStream << ex.what() << endl;
	}

	if ( Log )
	{
		if ( LogErrorStream.str().empty() )
		{
			cout << "The program ran without any errors.\n";
		}
		else
		{
			Log << "\nERRORS -:\n\n" << LogErrorStream.str() << endl;
			cout << "There were some errors during the execution of this program !\n\nCheck " << absolute( LogFilePath ) << " for details.\n";
			return -1;
		}
	}
}
