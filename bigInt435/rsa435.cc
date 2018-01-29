// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 


// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try 
	{
		      
      std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      BigInteger p = BigInteger(1);
	  BigInteger q = BigInteger(1);
	  BigInteger n = BigInteger(1);

	  //Setup the seeding for randomization
	  srand(time(NULL));

	  //Create the p and q for the RSA
      for (int i=0; i<278; i++) 
	  {
         p = p*10 + (rand() % 10);
		 q = q*10 + (rand() % 10);
      }


	  //solve for n
	  n = p * q;

	  //Output p, q and n
      std::cout << "p =\n";
	  std::cout << p << std::endl;
	  std::cout << "q = " << std::endl;
	  std::cout << q << std::endl;
	  std::cout << "n = " << std::endl;
	  std::cout << n << std::endl;
      
	} 
	catch(char const* err) 
	{
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
