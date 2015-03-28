#ifndef SHOWBITS_H_INCLUDED
#define SHOWBITS_H_INCLUDED
// In Beta!!..
string show_bits( int n ) //check for -78...
{
	string a;
	int m = n;
	register int i;
	n = abs( n );
	while ( n )
	{
		a += ( n % 2 == 1 ) + 48;
		n = n / 2;
	}
	int length = a.length();
	for ( i = 0; i < 32 - length; i++ )
	{
		a += '0';
	}
	reverse( a.begin(), a.end() );
	if ( m < 0 )
	{
		for ( i = 0; i < 32; i++ )
		{
			a[i] = ( a[i] != '1' ) + 48;
		}
		int sum, carry;
		sum = ( a[31] - '0' ) + 1;
		a[i] = ( sum == 2 ) ? '0' : sum + 48;
		carry = ( sum == 2 );
		for ( i = 30; i >= 0; i-- )
		{
			sum = a[i] - '0' + carry;
			a[i] = ( sum == 2 ) ? '0' : sum + 48;
			carry = ( sum == 2 );
		}
	}
	return a;  //a.length()=32 ALWAYS!!..
}
string show_bits( unsigned int n )
{
	string a;
	while ( n )
	{
		a += ( n % 2 == 1 ) + 48;
		n = n / 2;
	}
	if ( a == "" )
	{
		a = '0';
	}
	reverse( a.begin(), a.end() );
	return a;
}

#endif // SHOWBITS_H_INCLUDED
