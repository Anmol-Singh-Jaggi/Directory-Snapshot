// prettyprint.hpp

// Header guard
#ifndef PRETTYPRINT_INCLUDED
#define PRETTYPRINT_INCLUDED

#include<iostream>
#include<sstream>
#include<iterator>
#include<vector>
#include<map>
#include<unordered_map>
#include<string>
#include<utility>
#include<set>
#include<stack>
#include<queue>
#include<list>

template<typename T>
std::string ToString( const T& obj )
{
	std::stringstream ss;
	ss << obj;
	return ss.str();
}

// Macro to wrap the actual function calls around some extra useful information like the line number
// on which they were called, and to insert some formatting too.
#define print(i) std::cout << "Line " << __LINE__ << " : " << #i" = " << (i) << std::endl

// Print a pair
template<typename T1, typename T2>
std::ostream& operator <<( std::ostream& out, const std::pair<T1, T2>& object )
{
	out << "( " << object.first << ", " << object.second << " )";
	return out;
}

// Print an array
template < typename T1, size_t arrSize, typename = std::enable_if_t < !std::is_same<T1, char>::value >>
std::ostream & operator <<( std::ostream& out, const T1( & arr )[arrSize] )
{
	const char* separator = "";

	out << "[";
	for ( const auto& element : arr )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "]";

	return out;
}

// Print a vector
template<typename T1>
std::ostream& operator <<( std::ostream& out, const std::vector<T1>& object )
{
	const char* separator = "";

	out << "[";
	for ( const auto& element : object )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "]";

	return out;
}

// Print a set
template<typename T1>
std::ostream& operator <<( std::ostream& out, const std::set<T1>& object )
{
	const char* separator = "";

	out << "{";
	for ( const auto& element : object )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "}";

	return out;
}

// Print a map
template<typename T1, typename T2>
std::ostream& operator <<( std::ostream& out, const std::map<T1, T2>& object )
{
	const char* separator = "";

	out << "|";
	for ( const auto& element : object )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "|";

	return out;
}

// Print an unordered map
template<typename T1, typename T2>
std::ostream& operator <<( std::ostream& out, const std::unordered_map<T1, T2>& object )
{
	const char* separator = "";

	out << "|";
	for ( const auto& element : object )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "|";

	return out;
}

// Print a stack
template<typename T1>
std::ostream& operator <<( std::ostream& out, const std::stack<T1>& object )
{
	std::stack<T1> object_copy( object );
	const char* separator = "";

	out << "[";
	while ( !object_copy.empty() )
	{
		out << separator;
		out << object_copy.top();
		object_copy.pop();
		separator = ", ";
	}
	out << "]";

	return out;
}

// Print a queue
template<typename T1>
std::ostream& operator <<( std::ostream& out, const std::queue<T1>& object )
{
	std::queue<T1> object_copy( object );
	const char* separator = "";

	out << "[";
	while ( !object_copy.empty() )
	{
		out << separator;
		out << object_copy.front();
		object_copy.pop();
		separator = ", ";
	}
	out << "]";

	return out;
}

// Print a list
template<typename T1>
std::ostream& operator <<( std::ostream& out, const std::list<T1>& object )
{
	const char* separator = "";

	out << "[";
	for ( const auto& element : object )
	{
		out << separator;
		out << element;
		separator = ", ";
	}
	out << "]";

	return out;
}

#endif // PRETTYPRINT_INCLUDED
