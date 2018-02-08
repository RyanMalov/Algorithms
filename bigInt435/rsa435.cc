// You need to complete this program for a part of your first project.
//Ryan Malov


// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

int main() {
	try 
	{
		//std::ofstream p_q, e_n, d_n;


		//Use flag for a prime success
		bool prime = false;
		
		//Count number of Fermat tests passed
		int FermatPassed = 0;

		std::ofstream p_q, e_n, d_n;

		//Declaration of variables
		BigUnsigned p = 1;
		BigUnsigned q = 1;
		BigUnsigned n = 1;
		BigInteger a = 7;
		BigInteger phi = 0;
		BigInteger e = 44701;
		BigInteger d = 0;
		BigInteger r = 0;
		BigInteger s = 0;
		BigInteger g = 0;
		      
		//Setup the seeding for randomization
		srand(time(NULL));

		//Generating a prime for p
		std::cout << "A prime for p is being created. Please wait....\n";
		//Actual prime is found below
		while(!prime)
		{
				//set variables for fermat test if not prime
				p = 1;
				a = 7;
				FermatPassed = 0;

				//Generate a random number that is 278 digits
				for (int i = 0; i < 278; i++)
				{
						p = p * 10 + (rand() % 10);
				}
				//Do fermat test twice
				while(FermatPassed < 2)
				{
						//in the format of: a^(p-1)%p
						if(modexp(a, p-1, p) == 1)
						{
							a = 7;
							FermatPassed++;
						}
						else break;
						prime = true;
				}
		}
		std::cout << "A prime number has been created! The prime number for p = \n";
		std::cout << p << std::endl;
		std::cout << std::endl;

		//Prepare to find a prime for q
		//Set the prime bool to false
		prime = false;
		
		std::cout << "A prime for q is being created. Please wait....\n";

		//Find a prime for q, same as above for p
		while(!prime)
		{
				//setup variables for the fermat test if not prime
				q = 1;
				a = 7;
				FermatPassed = 0;

				//Generate a random number that is 278 digits
				for (int i = 0; i < 278; i++)
				{
						q = q * 10 + (rand() % 10);
				}
				//Do the Fermat Test twice
				while(FermatPassed < 2)
				{
						//in the format of: a^(q-1)%q
						if(modexp(a, q-1, q) == 1)
						{
								a = 7;
								FermatPassed++;
						}
						else break;
						prime = true;
				}
		}
		std::cout << "A prime number has been created! The prime number for q = \n";
		std::cout << q << std::endl;
		std::cout << std::endl;

		//Finding n
		n = p * q;
		std::cout << "n has been found.\n";

		//Finding phi
		phi = (p - 1) * (q - 1);
		std::cout << "phi has been found.\n";

		//Use extended Euclidean Algorithm to find e
		while(g != 1)
		{
				extendedEuclidean(phi, e, g, r, s);
				
				if(g == 1)
				{
						break;
				}
				e += 2;
		}
		std::cout << "e has been found.\n";

		//Finding d
		d = s;
		std::cout << "d has been found.\n";

		//Open up the files to save values
		p_q.open("p_q.txt");
		e_n.open("e_n.txt");
		d_n.open("d_n.txt");

		//Write the appropriate data into each file
		p_q << p << std::endl << q;
		e_n << e << std::endl << n;
		d_n << d << std::endl << n;

		//Close the files
		p_q.close();
		e_n.close();
		d_n.close();

	}


	catch(char const* err) 
	{
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}
	

	return 0;
}
