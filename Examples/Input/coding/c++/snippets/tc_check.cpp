#include<bits/stdc++.h>
#define pb push_back
#define iter(container,it) typeof(container.begin()) it
#define all(x) x.begin(),x.end()
#define fastin std::ios::sync_with_stdio(false)
#define cout_precision(x) cout<<std::fixed<<setprecision(x)
using  namespace std;

int main()
{
	string exe;
	cout << "Enter the name of the executable - ";
	getline( cin, exe );
	string command = exe + ".exe < in.txt > out_temp.txt";
	system( command.c_str() );
	system( "FC out_temp.txt out.txt" );
	system( "pause" );
	// system( "del \"out_temp.txt\"" );
}
