
//LONGEST COMMON SUBSEQUENCE....
// Wikipedia has a very nice article about it...

#ifndef LCS_H_INCLUDED
#define LCS_H_INCLUDED

// If the motive is to only find the length of the LCS and not use retrace() function...then we can put all the global vars inside lcs()...
string a; // "babxaab"
int size1;
string b; // "abcaba"
int size2;
vector<vector<int> >dp; // The dp matrix...
set<string> solution; // To contain all the possible longest common subsequences....I've taken a set instead of a vector specifically..


void retrace( string parent, int i, int j ) // Function to fill 'solution'...
{
	if ( i == 0 || j == 0 )
	{
		if ( parent.size() ) // i.e if( parent!=blank string("") )...
		{
			solution.insert( parent );
		}
		return;
	}
	if ( b[i - 1] == a[j - 1] )
	{
		parent += a[j - 1];
		retrace( parent, i - 1, j - 1 );
	}
	else
	{
		if ( dp[i][j - 1] >= dp[i - 1][j] )
		{
			retrace( parent, i, j - 1 );
		}
		if ( dp[i][j - 1] <= dp[i - 1][j] )
		{
			retrace( parent, i - 1, j );
		}
	}
}

//  Note that all the words inside "solution" are reversed of what they should be in actual (See line #17).So before outputting them,reverse them...

int lcs()
{
	size1 = a.length();
	size2 = b.length();
	dp.resize( size2 + 1, vector<int> ( size1 + 1, 0 ) );
	register int i, j;
	for ( i = 0; i < size2; i++ )
	{
		for ( j = 0; j < size1; j++ )
		{
			if ( b[i] == a[j] )
			{
				dp[i + 1][j + 1] = dp[i][j] + 1;
			}
			else
			{
				dp[i + 1][j + 1] = max( dp[i + 1][j], dp[i][j + 1] );
			}
		}
	}
	retrace( "", size2, size1 );
	return dp[size2][size1];
}
#endif // LCS_H_INCLUDED
