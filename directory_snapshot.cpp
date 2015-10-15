#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


// File handles for log files
static ofstream logError, logInfo;

typedef unsigned long long ULL;
// Used for displaying progress status
static ULL filesInSourcePath;

// Convert string to ULL
ULL ToULL( const string& str )
{
	stringstream ss( str );
	ULL ret;
	ss >> ret;
	return ret;
}

// Convert number to string
template<typename T>
std::string NumberToString( const T& obj )
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision( 2 );
	ss << obj;
	return ss.str();
}

// Convert the input size ( in bytes ) to its nearest units in the ratio of 1024.
// Trying to replicate the way in which an OS reports size of a file on right clicking and checking its properties.
static string RoundSize( const long long& size )
{
	static const vector<string> units {"bytes", "KiB", "MiB", "GiB", "TiB"};
	const unsigned ratio = 1024;

	double ret = ( double )size;
	unsigned i;
	for ( i = 0; ret > ratio && i < units.size() - 1; i++ )
	{
		ret /= ratio;
	}

	return NumberToString( ret ) + " " + units[i];
}

// Returns the number of files in the input directory
// The progress status is printed after every `milestone` files are visited.
// Here "file" refers to everything (regular_files, directories, symlinks etc.)
ULL GetNumberOfFiles( const path& inputPath = ".", const ULL& milestone = 1000 )
{
	ULL numberOfFiles = 0;

	if ( milestone == 0 )
	{
		numberOfFiles = std::distance( recursive_directory_iterator( inputPath ), recursive_directory_iterator() );
	}
	else
	{
		ULL filesVisitedIterator = 0;
		for ( recursive_directory_iterator it( inputPath ); it != recursive_directory_iterator(); ++it )
		{
			filesVisitedIterator++;
			if ( filesVisitedIterator == milestone )
			{
				numberOfFiles += filesVisitedIterator;
				filesVisitedIterator = 0;
				cout << "\rFiles visited = " << numberOfFiles << std::flush;
			}
		}

		numberOfFiles += filesVisitedIterator;
		if ( filesVisitedIterator )
		{
			cout << "\rFiles visited = " << numberOfFiles << std::flush;
		}

		cout << endl;
	}

	return numberOfFiles;
}

// Escape HTML special characters
static string EscapeHtmlSpecialChars( const path& fileName, const bool& href = false )
{
	string fileNameString = fileName.string();

	string escapedFileName;
	for ( size_t i = 0; i < fileNameString.size(); i++ )
	{
		if ( fileNameString[i] == '&' )
		{
			escapedFileName += "&amp;";
		}
		else if ( fileNameString[i] == '<' )
		{
			escapedFileName += "&lt;";
		}
		else if ( fileNameString[i] == '>' )
		{
			escapedFileName += "&gt;";
		}
		else if ( fileNameString[i] == '"' )
		{
			escapedFileName += "&quot;";
		}
		else if ( fileNameString[i] == '\'' )
		{
			escapedFileName += "&#39;";
		}
		// If the input filename is to be put inside an href attribute
		else if ( href && fileNameString[i] == '#' )
		{
			escapedFileName += "%23";
		}
		else if ( href && fileNameString[i] == '?' )
		{
			escapedFileName += "%3F";
		}
		else
		{
			escapedFileName += fileNameString[i];
		}
	}

	return escapedFileName;
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
	logInfo << sourcePath << endl;

	// This should be executed only once during the whole program execution
	// ( only during the first invocation of this function )
	static bool isDescendant = IsDescendant( sourcePath, destinationPath );
	if ( isDescendant )
	{
		logError << "The destination path cannot be a descendant of the source path!! Please provide an alternate destination path !!" << endl;
		return -1;
	}

	boost::system::error_code ec;

	// Total size of the source directory ( in bytes )
	long long sourcePathSize = 0;

	// Containers to hold list of paths of files and directories
	vector<path> dirContents, files, directories;
	try
	{
		DirectoryIterate( sourcePath, dirContents );
	}
	catch ( const filesystem_error& ex )
	{
		logError << ex.what() << endl;
		// Cannot iterate through the directory, so no point in going further
		return 0;
	}

	// Sort, since directory iteration is not ordered on some file systems
	sort( dirContents.begin(), dirContents.end() );
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
			logError << "Failed to determine if " << absolute( item ) << " is a directory or not : " << ec.message() << endl;
		}

		/********************* Show progress status *******************/
		// Progress will be shown after every `milestoneIncrement`% of files have been processed
		static const double milestoneIncrement = 1;
		static double milestone = milestoneIncrement;
		static ULL filesVisited = 0;
		filesVisited++;
		double filesVisitedPercentage = ( filesVisited * 100.0 ) / filesInSourcePath;
		if ( filesVisitedPercentage >= milestone - 0.000001 )
		{
			milestone += milestoneIncrement;
			cout << "\r" << ( int )filesVisitedPercentage << "%" << std::flush;
		}

	}
	dirContents.clear();

	// Present working directory
	const path pwd = destinationPath / sourcePath.filename();
	ec.clear();
	create_directory( pwd, ec );
	if ( ec )
	{
		logError << "Failed to create " << absolute( pwd ) << " : " << ec.message() << endl;
		return 0;
	}

	// Create the output file.
	const path outFilePath = ( pwd / sourcePath.filename() ).string() + ".html";
	ofstream outFile( outFilePath.string() );
	if ( !outFile )
	{
		logError << "Failed to create " << absolute( outFilePath ) << " : " << strerror( errno ) << endl;
		return 0;
	}

	// Write the HTML file header.
	outFile << ""
					"<!DOCTYPE html>\n"
					"<html>\n"
					"<head>\n"
					"<meta charset=\"UTF-8\">\n"
					"<title>" << EscapeHtmlSpecialChars( sourcePath.filename() ) << "</title>\n"
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
						"  <td>" << EscapeHtmlSpecialChars( file.filename() ) << "</td>\n"
						"  <td>";

		ec.clear();
		auto size = file_size( file, ec );
		if ( ec )
		{
			logError << "Failed to read size of " << absolute( file ) << " : " << ec.message() << endl;
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
	files.clear();

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
						"  <td><a href=\"" << EscapeHtmlSpecialChars( ( directory.filename() / directory.filename() ).generic_string(), true ) << ".html\">" << EscapeHtmlSpecialChars( directory.filename() ) << "</a></td>\n" <<
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
	path logFolderPath = "./logs";

	if ( argc < 3 )
	{
		cout << "Usage : " << argv[0] << " <source_directory_path> <destination_directory_path> [log_folder_path=" << logFolderPath << "]\n";
		return -1;
	}

	if ( argc > 3 )
	{
		logFolderPath = path( argv[3] );
	}

	try
	{
		const path sourcePath = canonical( argv[1] );
		create_directories( argv[2] );
		const path destinationPath = canonical( argv[2] );

		create_directories( logFolderPath );
		const path errorLogPath = logFolderPath / "errors.txt";
		logError.open( errorLogPath.string() );
		const path infoLogPath = logFolderPath / "info.txt";
		logInfo.open( infoLogPath.string() );

		cout << "Calculating size of input ...\n";
		filesInSourcePath = GetNumberOfFiles( sourcePath );

		cout << "\nInitiating the snapshot process ...\n\n";
		Snapshot( canonical( argv[1] ), canonical( argv[2] ) );
		cout << "\n\n";
	}
	catch ( const filesystem_error& ex )
	{
		cerr << ex.what() << endl;
		logError << ex.what() << endl;
	}
}

