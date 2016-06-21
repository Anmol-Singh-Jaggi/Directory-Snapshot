#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <algorithm>
#include <utility>
using namespace std;

typedef unsigned long long ULL;

#define SHR(x,n) ((x) >> (n)) // Shift "x" right by "n" bits
#define SHL(x,n) ((x) << (n)) // Shift "x" left by "n" bits
#define ROTR(x,n) ( (x) >> (n) | ((x) << (64 - (n))))  // Circular shift "x" right by "n" bits
#define ROTL(x,n) ( (x) << (n) | ((x) >> (64 - (n))))  // Circular shift "x" left by "n" bits
#define Sum0 (ROTR(a,28)^ROTR(a,34)^ROTR(a,39))   // Predefined function
#define Sum1 (ROTR(e,14)^ROTR(e,18)^ROTR(e,41))   // Predefined function
#define Sigma0(x) (ROTR(x,1)^ROTR(x,8)^SHR(x,7))  // Predefined function
#define Sigma1(x) (ROTR(x,19)^ROTR(x,61)^SHR(x,6))  // Predefined function
#define Ch ((e&f)^((~e)&g))  // Predefined function
#define Maj ((a&b)^(a&c)^(b&c))  // Predefined function

// Converts a string of 0's and 1's of exactly 8 binary characters to its equivalent character representation
unsigned char binary_to_char( const string& binary_string )
{
	assert( binary_string.size() == 8 );
	size_t res = 0;
	for ( int i = 0; i < ( int )binary_string.size(); i++ )
	{
		assert( binary_string[i] == '1' || binary_string[i] == '0' ); // Ensure the input is a binary string
		res <<= 1;
		res += binary_string[i] - '0';
	}
	return ( unsigned char )res;
}

// Converts a character into its equivalent binary representation of exactly 8 bits and append it to "binary"
void char_to_binary( const unsigned char& c, vector<bool>& binary )
{
	string res;
	int c_bak = c;
	int char_length = 8;
	while ( char_length-- )
	{
		res += ( char )( ( c_bak & 1 ) + '0' );
		c_bak >>= 1;
	}
	reverse( res.begin(), res.end() );
	for ( int i = 0; i < ( int )res.size(); i++ )
	{
		binary.push_back( res[i] == '1' ? true : false );
	}
	if ( res.empty() )
	{
		binary.push_back( false );
	}
}

// Converts an unsigned long long to its equivalent binary representation of exactly 64 bits
string ULL_to_binary( const ULL& n )
{
	string res;
	ULL n_bak = n;
	int ULL_size = 64;
	while ( ULL_size-- )
	{
		res += ( char )( ( n_bak & 1 ) + '0' );
		n_bak >>= 1;
	}
	reverse( res.begin(), res.end() );
	if ( res.empty() )
	{
		res += '0';
	}
	return res;
}

// Converts a binary input of exactly 64 bits to its equivalent ULL representation
ULL binary_to_ULL( const vector<bool>& binary )
{
	assert( binary.size() == 64 );
	ULL res = 0;
	for ( size_t i = 0; i < binary.size(); i++ )
	{
		res <<= 1;
		res += binary[i];
	}
	return res;
}

// Converts a binary string having size as a multiple of 4 to its equivalent HEX form
string binary_to_HEX( const string& s )
{
	assert( s.size() % 4 == 0 );
	static map<string, char> bin_to_hex_mapping;
	bin_to_hex_mapping["0000"] = '0';
	bin_to_hex_mapping["0001"] = '1';
	bin_to_hex_mapping["0010"] = '2';
	bin_to_hex_mapping["0011"] = '3';
	bin_to_hex_mapping["0100"] = '4';
	bin_to_hex_mapping["0101"] = '5';
	bin_to_hex_mapping["0110"] = '6';
	bin_to_hex_mapping["0111"] = '7';
	bin_to_hex_mapping["1000"] = '8';
	bin_to_hex_mapping["1001"] = '9';
	bin_to_hex_mapping["1010"] = 'A';
	bin_to_hex_mapping["1011"] = 'B';
	bin_to_hex_mapping["1100"] = 'C';
	bin_to_hex_mapping["1101"] = 'D';
	bin_to_hex_mapping["1110"] = 'E';
	bin_to_hex_mapping["1111"] = 'F';
	string res;
	for ( size_t i = 0; i < s.size(); i += 4 )
	{
		assert( bin_to_hex_mapping.find( s.substr( i, 4 ) ) != bin_to_hex_mapping.end() );
		// Will not be triggered unless the input has a character other than '1' or '0'
		res += bin_to_hex_mapping[s.substr( i, 4 )];
	}
	return res;
}

// Returns a particular 64-bit word from a specified 1024-bit block of "message"
ULL get_word_from_block( const string& message, const size_t& block_index, const size_t& word_index )
{
	string block = message.substr( block_index * 128, 128 );
	string word = block.substr( word_index * 8, 8 );
	vector<bool> binary_of_word;
	for ( size_t i = 0; i < word.size(); i++ )
	{
		char_to_binary( word[i], binary_of_word );
	}
	return binary_to_ULL( binary_of_word );
}

// Pads the message such that its length is a mulitple of 1024
string pad_message( const string& message )
{
	string res = message;
	size_t remaining = 1024 - ( ( message.size() * 8 ) % 1024 );
	res += binary_to_char( "10000000" ); // This has to be added always.
	remaining -= 8;
	string message_size_binary = ULL_to_binary( message.size() * 8 ); // Binary representation of the number of characters in the input
	remaining -= message_size_binary.size();
	if ( ( message.size() * 8 ) % 1024 >= 896 ) // Not enough space for padding on this block; Add another block.
	{
		remaining += 1024;
	}
	res.append( remaining / 8, '\0' ); // Append the appropriate amount of 0b"00000000" to the message
	for ( size_t i = 0; i < message_size_binary.size(); i += 8 )
	{
		res += binary_to_char( message_size_binary.substr( i, 8 ) );
	}
	assert( res.size() % 128 == 0 );
	return res;
}

// Returns a 512-bit SHA2 message digest of the input string
string SHA2_512( string message )
{
	message = pad_message( message );
	ULL message_block_count = ( message.size() * 8 ) / 1024;

	ULL H[8] =
	{
		0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
		0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
	};

	const ULL K[80] =
	{
		0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
		0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
		0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
		0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
		0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
		0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
		0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
		0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
		0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
		0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
		0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
		0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
		0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
		0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
		0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
		0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
	};

	for ( size_t i = 1; i <= message_block_count; i++ )
	{
		vector<ULL> W( 80, 0 );
		for ( int t = 0; t < 16; t++ )
		{
			W[t] = get_word_from_block( message, i - 1, t );
		}
		for ( int t = 16; t < 80; t++ )
		{
			W[t] = Sigma1( W[t - 2] ) + W[t - 7] + Sigma0( W[t - 15] ) + W[t - 16];
		}
		ULL a = H[0];
		ULL b = H[1];
		ULL c = H[2];
		ULL d = H[3];
		ULL e = H[4];
		ULL f = H[5];
		ULL g = H[6];
		ULL h = H[7];
		for ( int t = 0; t < 80; t++ )
		{
			ULL T1 = h + Ch + Sum1 + W[t] + K[t];
			ULL T2 = Sum0 + Maj;
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;
	}
	string digest;
	for ( int i = 0; i < 8; i++ )
	{
		digest += ULL_to_binary( H[i] );
	}
	return digest;
}

int main()
{
	cout << "Enter message ";
	string message;
	getline( cin, message );
	string digest = SHA2_512( message );
	cout << binary_to_HEX( digest ) << endl;
}
