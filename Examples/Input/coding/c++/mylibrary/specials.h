#ifndef SPECIALS_H_INCLUDED
#define SPECIALS_H_INCLUDED
/*
    Algorithms included -:
 1). Given "n" and "r", use bitmask to generate all the possible nCr subsets of an array of size "n"..

*/

// First Algo starts..
void advance( int& i ) // Magic...Kindly do not touch..
{
	if ( i == 0 ) // Special care for i=0
	{
		i = -1;
	}
	int l = i&~( i - 1 );
	int z = ( i + l )&~i;
	i |= z;
	i &= ~( z - 1 );
	i |= ( ( z / l ) >> 1 ) - 1;
}
int bitmask = ( 1 << r ) - 1; // The smallest number having exactly "r" bits set..
// The set bits in the binary of "bitmask" denote the positions included in a subset..
// The number of set bits in the binary of "bitmask" is always = r...
while ( !( bitmask & 1 << n ) ) // This loop should run exactly nCr times..
{
	// Process bitmask..
	advance( bitmask ); // Generate the lowest number bigger than "bitmask" having exactly "r" set bits
}
// First Algo ends..


#endif // SPECIALS_H_INCLUDED
