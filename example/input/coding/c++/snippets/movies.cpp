#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define all(x) x.begin(),x.end()
#define fastin std::ios::sync_with_stdio(false);cin.tie(NULL)
#define cout_precision(x) cout<<std::fixed<<setprecision(x)
using namespace std;

pair<double, string> parse( string line )
{
	reverse( all( line ) );

	stringstream ss( line );
	string rating, name;
	ss >> rating;
	ss.get();
	getline( ss, name );

	reverse( all( rating ) );
	reverse( all( name ) );

	ss.clear();
	ss.str( "" );
	ss << rating;

	double rating_double;
	ss >> rating_double;

	return mp( rating_double, name );
}

int main()
{
	string in_file = "movies.txt";
	ifstream fin( in_file );
	if ( !fin.good() )
	{
		cout << "Error opening '" << in_file << "' - " << strerror( errno ) << endl;
		return 1;
	}

	string line;
	multimap < double, string> mapping;
	while ( getline( fin, line ) )
	{
		mapping.insert( parse( line ) );
	}
	ofstream fout( "movies2.txt" );
	fout << std::fixed << setprecision( 1 );
	for ( auto i = mapping.rbegin(); i != mapping.rend(); ++i )
	{
		fout << i->second << " " << i->first << "\n";
	}
}
