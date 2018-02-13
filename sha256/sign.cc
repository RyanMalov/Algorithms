//Ryan Malov
//Algorithms
//RSA Project
//Part 2: Generate and verify digital signatures using SHA256.


#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "sha256.h"
#include "BigIntegerLibrary.hh"
using namespace std;

string InputFile();
int main(int argv, char* argc[])
{
	//Read in the test file
	std::string test = "test.txt";
	std::ifstream myfile (test.c_str(), std::ios::binary);
	std::streampos begin,end;
	begin = myfile.tellg();
	myfile.seekg (0, std::ios::end);
	end = myfile.tellg();
	//Size of the file in bytes
	std::streampos size = end - begin;
	myfile.seekg (0, std::ios::beg);

	char * memblock = new char[size];
	//Read the entire file
	myfile.read (memblock, size);
	//Add a terminator
	memblock[size]='\0';
	myfile.close();

	string str(memblock);
//	std::cout << str;
//	std::cout << "\nthe content";
	
	string input = sha256(str);

//	std::cout << "\n" << input << "\n";

	//Read in the d_n file
	std::ifstream dnValues("d_n.txt");
	string d;
	string n;

	while(dnValues >> d >> n)
	{
			//std::cout << d << "\n" << n << "\n";
	}
	
	//Decrypt the encoded file using d_n
	//BigInteger test1 = stringToBigInteger16(input);
	
	BigInteger M = stringToBigInteger16(input);
	BigUnsigned D = stringToBigUnsigned(d);
	BigUnsigned N = stringToBigUnsigned(n);
	BigUnsigned signedM = modexp(M, D, N);
	//std::cout << "\n" << "\n" << "signedM value is = " << signedM << "\n";
	
	
	std::ofstream outfile;
	outfile.open("file.txt.signature");
	outfile << signedM;
	outfile.close();

	//Read in the e_n file
	std::ifstream enValues("e_n.txt");
	string e;
	string n1;

	while(enValues >> e >> n1)
	{
			//std::cout << e << "\n" << n1 << "\n";
	}

	std::string test1 = "test.txt";
	std::ifstream myfile1 (test1.c_str(), std::ios::binary);
	begin = myfile1.tellg();
	myfile1.seekg(0, std::ios::end);
	end = myfile1.tellg();
	//size of file in the bytes
	size = end - begin;
	myfile1.seekg(0, std::ios::beg);

	char * memblock1 = new char[size];
	myfile1.read(memblock1, size);
	memblock1[size]='\0';
	myfile1.close();

	string str1(memblock1);
	std::cout << str1;
	std::cout << "\n The content";

	string compareMe = sha256(str1);
	std::cout << "\n" << compareMe << "\n";

	BigUnsigned M1 = stringToBigUnsigned16(compareMe);





	BigUnsigned E = stringToBigUnsigned(e);
	BigUnsigned signedMessage = modexp(signedM, E, N);
	
	


	std::cout << "\n" << M1 << "\n";
	std::cout << "\n" << signedMessage << "\n";	

	if(signedMessage == M1)
	{
			std::cout << "Files unchanged\n";
	}
	else
	{
			std::cout << "Files changed\n";
	}














	return 0;
}
