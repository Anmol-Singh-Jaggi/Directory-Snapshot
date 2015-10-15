import java.util.Scanner;

public class hello
{
	public static void main( String[] args )
	{
		System.out.print( "Enter your sentence : " );
		Scanner sc = new Scanner( System.in );
		String name = sc.nextLine();
		sc.close();
		
		int[] array = new int[256];
		
		for ( int i = 0; i < name.length(); i++ )
		{
			char character = name.charAt( i );
			int ascii = ( int )character;
			System.out.println( character + " = " + Integer.toBinaryString( ascii ) );
			array[ascii]++;
		}
		
		for ( int i = 0; i < array.length; i++ )
		{
			if ( array[i] > 0 )
			{
				System.out.println( "Frequency of " + (char)i + " = " + array[i] );
			}
		}
	}
}