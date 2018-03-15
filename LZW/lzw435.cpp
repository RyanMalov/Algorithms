//Ryan Malov
//Algorithms Project 2 LZW

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector> 
#include <sys/stat.h>
#include <cassert>
#include <bitset>

/*
  This code is derived from LZW@RosettaCode for UA CS435 
*/ 
 
// Compress a string to a list of output symbols.
// The result will be written to the output iterator
// starting at "result"; the final iterator is returned.
template <typename Iterator>
Iterator compress(const std::string &uncompressed, Iterator result) {
  // Build the dictionary.
  int dictSize = 256;
  std::map<std::string,int> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[std::string(1, i)] = i;

	//Extended ASCII Table creation here 
  std::string w;
  for (std::string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) 
  {
    char c = *it;
    std::string wc = w + c;

    if (dictionary.count(wc))
	{
      w = wc;
	}
    else 
	{
      *result++ = dictionary[w];
      // Add wc to the dictionary. Assuming the size is 4096!!!
      if (dictionary.size()<4096)
         dictionary[wc] = dictSize++;
      w = std::string(1, c);
    }
  }
 
  // Output the code for w.
  if (!w.empty())
  {
    *result++ = dictionary[w];
  }
  return result;
}
 
// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator>
std::string decompress(Iterator begin, Iterator end) {
  // Build the dictionary.
  int dictSize = 256;
  std::map<int,std::string> dictionary;
  for (int i = 0; i < 256; i++)
  {
    dictionary[i] = std::string(1, i);
  }

  std::string w(1, *begin++);
  std::string result = w;
  std::string entry;

  for ( ; begin != end; begin++) 
  {
    int k = *begin;

    if (dictionary.count(k))
	{
      entry = dictionary[k];
	}
    else if (k == dictSize)
	{
      entry = w + w[0];
	}
    else
      throw "Bad compressed k";
 
    result += entry;
 
    // Add w+entry[0] to the dictionary.
    if (dictionary.size()<4096)
	{
      dictionary[dictSize++] = w + entry[0];
	}

    w = entry;
  }

  return result;
}

std::string int2BinaryString(int c, int cl) {
      std::string p = ""; //a binary code string with code length = cl
      int code = c;
      
	  while (c>0) 
	  {         
		   if (c%2==0)
		   {
            p="0"+p;
		   }
         else
		 {
            p="1"+p;
		 }
         c=c>>1;   
      }
      int zeros = cl-p.size();
      if (zeros<0) 
	  {
         std::cout << "\nWarning: Overflow. code " << code <<" is too big to be coded by " << cl <<" bits!\n";
         p = p.substr(p.size()-cl);
      }
      else {
         for (int i=0; i<zeros; i++)
		 {	 //pad 0s to left of the binary code if needed
            p = "0" + p;
		 }
      }
      return p;
}

int binaryString2Int(std::string p) {
   int code = 0;

   if (p.size()>0) 
   {
      if (p.at(0)=='1')
	  {
         code = 1;
	  }
      p = p.substr(1);

      while (p.size()>0) 
	  { 
         code = code << 1; 
		   if (p.at(0)=='1')
		   {
            code++;
		   }
         p = p.substr(1);
      }
   }
   return code;
}
 
int main(int argc, char* argv[]) 
{
	//Throw error if not enough paramteters were  given.
	if(argc < 2)
	{
		std::cerr << "No input file given\n";
		return 1;
	}

	std::string filename = argv[2];
	std::ifstream infile(filename.c_str(), std::ios::binary);
	
	//Create a string of the file contents that a read in	
	std::string input{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
	infile.close();

	if(*argv[1] == 'c')
	{
		//Avector (or ASCII Vector) takes the input string of the selected
		//file and stores them as integer values inside of a vector.
		std::vector<int> Avector;
		compress(input, std::back_inserter(Avector));
//		copy(Avector.begin(), Avector.end(), std::ostream_iterator<int>(std::cout, ", "));
		std::cout << std::endl;
	
	
		//Convert Avector of ints to Binary
		std::string Output;
	
		for(int i = 0; i < Avector.size(); i++)
		{
			Output.append(int2BinaryString(Avector[i], 12));
		}

		//Need to change the output so that its divisible by 8
		int bitLength = Output.length() % 8;
		Output.append(bitLength, '0');

		assert(Output.length() == (Output.length() % 8 + Output.length()));
		
		std::string Segment;

		for(int i = 0; i < Output.size(); i++)
		{
			Segment = Output.substr(i, 8);
			Output.replace(i, 8, 1, (char)binaryString2Int(Segment));
		}	
		
		filename.append(".lzw");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << Output;
		outfile.close();
		std::cout << "The selected file has been compressed! To expand the compressed file please use the following command: ./lzw e <filename>.lzw\n";

	}

	else if(*argv[1] == 'e')
	{
		//Create a string
		std::string newString;
		for(auto i = 0; i < input.length(); i++)
		{
			std::bitset<8> Bit(static_cast<int>(input.at(i)));
			newString.append(Bit.to_string());
		}

		input = newString;
		std::vector<int> Svector;
		int Zeros = 4;
		for(auto i = 0; i < input.size() - Zeros; i += 12)
		{
			std::string Istring = input.substr(i, 12);
			assert(Istring.length() == 12);
//			std::cout << Istring << "\n";
			assert(Istring.length() == 12);
			Svector.push_back(binaryString2Int(Istring));
		}

		std::string d = decompress(Svector.begin(), Svector.end());
		filename.append("2");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << d;
		outfile.close();
		std::cout << "The file has been expanded! To compare the expanded file to the original file, please use the following command: diff <Original Filename> <Filename>.lzw2\n";
	}
}
