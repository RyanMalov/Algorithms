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
      if (dictionary.size()<65536)
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
    if (dictionary.size()<65536)
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

	//Avector (or ASCII Vector) takes the input string of the selected
	//file and stores it as an integer value inside of a vector.
	std::vector<int> Avector;

	//Create a string of the file contents that a read in	
	std::string input{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
	infile.close();

	if(*argv[1] == 'c')
	{
		compress(input, std::back_inserter(Avector));
		//This outputs the data
		//Change so that it outputs when its done compressing
//		copy(Avector.begin(), Avector.end(), std::ostream_iterator<int>(std::cout, ", "));

		//Do the variable bit size calculations:
		int bits = 9;
		int BitsReq = 256;
		std::string s;
		std::string BinCode;

		for(std::vector<int>::iterator iter = Avector.begin(); iter != Avector.end(); iter++)
		{
			s = int2BinaryString(*iter, bits);
			BitsReq++;
			
			switch(BitsReq)
			{
				case 512:
					bits = 10;
					break;
				case 1024:
					bits = 11;
					break;
				case 2048:
					bits = 12;
					break;
				case 4096:
					bits = 13;
					break;
				case 8192:
					bits = 14;
					break;
				case 16384:
					bits = 15;
					break;
				case 32768:
					bits = 16;
					break;
			}
	
			BinCode += s;
		}

		//Need to make an integer string of the passed in binary string
		int remainder = BinCode.length() % 8;
		BinCode.append(8 - remainder, '0');
		std::string str;

		for(int i = 0; i < BinCode.size(); i++)
		{
			str = BinCode.substr(i, 8);
			BinCode.replace(i, 8, 1, (char)binaryString2Int(str));
		}
		
		filename.append(".lzwm");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << BinCode;
		outfile.close();
		std::cout << "The file has been compressed! To expand the file, use the following command: ./lzwm e <filename>.lzwm\n";

	}

	else if(*argv[1] == 'e')
	{
		int bits = 9;
		int size = 256;
		std::string s;
//		std::vector<int> Svector;

		//Convert the input string into a binary string
		std::string BinaryString;
		std::string string2BinString;

		for(int i = 0; i < input.length(); i++)
		{
			std::bitset<8> bits((int)(input[i]));
			string2BinString.append(bits.to_string());
		}
		BinaryString = string2BinString;
		
		//Get the bit patterns
		for(int i = 0; i < BinaryString.length(); i++)
		{
			s += BinaryString[i];

			if(s.length() >= bits)
			{
				Avector.push_back(binaryString2Int(s));
				size++;

				switch(size)
				{
					case 512:
						bits = 10;
						break;
					case 1024:
						bits = 11;
						break;
					case 2048:
						bits = 12;
						break;
					case 4096:
						bits = 13;
						break;
					case 8192:
						bits = 14;
						break;
					case 16384:
						bits = 15;
						break;
					case 32768:
						bits = 16;
						break;
				}
				s.clear();
			}
		}
//		std::cout << "After the variable Expanding\n";
		
		//Decompress
		std::string d = decompress(Avector.begin(), Avector.end());	

//		std::cout << "After decompress is called\n";	
		filename.append("2");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << d;
		outfile.close();

		std::cout << "Expansion is complete! To compare the file to the original file please use the following command: diff <Filename> <Filename>.lzwm2\n";
		
	}		
}
