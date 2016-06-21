#include <iostream>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

typedef unsigned long long ULL;

ULL ToULL( const string& str )
{
	stringstream ss( str );
	ULL ret;
	ss >> ret;
	return ret;
}

template<typename T>
std::string ToStringPrecision( const T& obj )
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision( 2 );
	ss << obj;
	return ss.str();
}

// Convert the input size ( in bytes ) to its nearest units in the ratio of 1024.
// ( Trying to replicate how an OS reports size of a file/directory on right clicking and checking its properties )
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

	return ToStringPrecision( ret ) + " " + units[i];
}


ULL GetSize( const path& inputPath = ".", const ULL& milestone = 1000 )
{
	ULL sizeCounted = 0;

	if ( !exists( inputPath ) )
	{
		cout << "Error : '" << inputPath << "' does not exists!\n";
	}
	else
	{
		static const string& cursorUp = "\033[1A";
		static const string& eraseLine = "\033[2K";

		ULL filesVisited = 0, filesVisitedIterator = 0;
		for ( recursive_directory_iterator it( inputPath ); it != recursive_directory_iterator(); ++it )
		{
			if ( !is_directory( *it ) )
			{
				sizeCounted += file_size( *it );
				filesVisitedIterator++;
				if ( filesVisitedIterator == milestone )
				{
					filesVisited += filesVisitedIterator;
					filesVisitedIterator = 0;
					cout << "\r" << eraseLine << "Files visited = " << filesVisited << "\n";
					cout << "\r" << eraseLine << "Size counted = " << RoundSize( sizeCounted ) << "\n";
					cout << std::flush;
					cout << cursorUp << cursorUp;
				}
			}
		}

		filesVisited += filesVisitedIterator;
		if ( filesVisitedIterator )
		{
			cout << "\r" << eraseLine << "Files visited = " << filesVisited << "\n";
			cout << "\r" << eraseLine << "Size counted = " << RoundSize( sizeCounted ) << std::flush;
		}

		cout << endl << endl;
	}

	return sizeCounted;
}

int main( int argc, char* argv[] )
{
	path inputPath = ".";
	ULL milestone = 1000;

	if ( argc > 1 )
	{
		inputPath = argv[1];
	}

	if ( argc > 2 )
	{
		milestone = ToULL( argv[2] );
	}

	cout << "Input path entered = " << inputPath << "\n";
	cout << "Input path resolved to = " << canonical( inputPath ) << "\n";
	cout << "Milestone = " << milestone << "\n\n";

	try
	{
		const auto inputSize = GetSize( inputPath, milestone );
		cout << "Size = " << RoundSize( inputSize ) << endl;
	}

	catch ( const filesystem_error& ex )
	{
		cout << ex.what() << '\n';
	}

	return 0;
}
