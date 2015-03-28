#include<bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define all(x) x.begin(),x.end()
#define fastin std::ios::sync_with_stdio(false);cin.tie(NULL)
#define cout_precision(x) cout<<std::fixed<<setprecision(x)
using namespace std;

// Insert 4 space before every line to make it suitable to upload to Stack-Exchange sites.

int main( int argc, char** argv )
{
	string in_file, out_file = "out.txt", line;
	if ( argc > 1 )
	{
		in_file = string( argv[1] );
		if ( argc > 2 )
		{
			out_file = string( argv[2] );
		}
	}
	else
	{
		cout << "Enter the filepath - ";
		getline( cin, in_file );
	}
	ifstream fin( in_file );
	if ( !fin )
	{
		cerr << "Error in opening the file '" << in_file << "' - " << strerror( errno ) << endl;
		return 1;
	}
	ofstream fout( out_file );
	while ( getline( fin, line ) )
	{
		line.insert( 0, "    " );
		fout << line << "\n";
	}
}
