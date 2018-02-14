//Ryan Malov
//Algorithms
//RSA Project
//Part 2: Generate and verify digital signatures using SHA256.


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include "sha256.h"
#include "BigIntegerLibrary.hh"
using namespace std;

string InputFile();
int main(int argc, char* argv[])
{
	if(strcmp(argv[1], "s") == 0)
	{
	//Read in the test file
	std::string test = argv[2];
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

	//Put the string through the Sha encoder	
	string input = sha256(str);

//	std::cout << "\n" << input << "\n";

	//Read in the d_n file
	std::ifstream dnValues("d_n.txt");
	string d;
	string n;
	
	//Get the d and n values
	while(dnValues >> d >> n)
	{
			//std::cout << d << "\n" << n << "\n";
	}
	
	//Decrypt the encoded file using d_n
	BigInteger M = stringToBigInteger16(input);
	BigUnsigned D = stringToBigUnsigned(d);
	BigUnsigned N = stringToBigUnsigned(n);
	BigUnsigned signedM = modexp(M, D, N);
//	std::cout << "\n" << "\n" << "signedM value is = " << signedM << "\n";
	std::string nameOfFile = argv[2];
	nameOfFile += ".signature";
//	std::cout << signedM << "\n";
	//Put this signature in a seperate file	
	std::ofstream outfile;
	outfile.open(nameOfFile.c_str());
	outfile << signedM;
	outfile.close();

	std::cout << "File successfully signed.\n";

	}


	//Verifying the signature

	if(strcmp(argv[1], "v") == 0){

	//Read in the e_n file
	std::ifstream enValues("e_n.txt");
	string e;
	string n1;

	//Get the e and n values
	while(enValues >> e >> n1)
	{
			//std::cout << e << "\n" << n1 << "\n";
	}

	//File to verify
	std::string test1 = argv[2];
	std::string nameOfFile1 = argv[3];
	std::ifstream myfile1 (test1.c_str(), std::ios::binary);
	std::ifstream nameOfFileSign (nameOfFile1.c_str(), std::ios::binary);
	std::streampos begin, end;
	begin = myfile1.tellg();
	myfile1.seekg(0, std::ios::end);
	end = myfile1.tellg();
	//size of file in the bytes
	std::streampos size = end - begin;
	myfile1.seekg(0, std::ios::beg);

	char * memblock1 = new char[size];
	myfile1.read(memblock1, size);
	memblock1[size]='\0';
	myfile1.close();

	string str1(memblock1);

	string compareMe = sha256(str1);


	BigUnsigned M1 = stringToBigUnsigned16(compareMe);
	BigUnsigned E = stringToBigUnsigned(e);
	BigUnsigned N1 = stringToBigUnsigned(n1);

	//Read the signature file
	string txtSignature;
	while(nameOfFileSign >> txtSignature)
	{
//			std::cout << txtSignature;
	}

	BigUnsigned signedM1 = stringToBigUnsigned(txtSignature);


	BigUnsigned signedMessage = modexp(signedM1, E, N1);

//	std::cout << "\n" << M1 << "\n";
//	std::cout << "\n" << signedMessage << "\n";	

	if(signedMessage == M1)
	{
			std::cout << "Files unchanged\n";
	}
	else
	{
			std::cout << "Files changed\n";
	}

	}
	return 0;
}
