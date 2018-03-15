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
//  std::cout << result<<"???:::\n";
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


/*
void binaryIODemo(std::vector<int> compressed, std::string fileName) {
   int c = 69;
   int bits = 9;
   std::string p = int2BinaryString(c, bits);
   std::cout << "c=" << c <<" : binary string="<<p<<"; back to code=" << binaryString2Int(p)<<"\n";
   
   std::string bcode= "";
   for (std::vector<int>::iterator it = compressed.begin() ; it != compressed.end(); ++it) {
      if (*it<256)
         bits = 8;
      else
         bits = 9;
      
      bits = 12;
      p = int2BinaryString(*it, bits);
      std::cout << "c=" << *it <<" : binary string="<<p<<"; back to code=" << binaryString2Int(p)<<"\n";
      bcode+=p;
   }
   
   //writing to file
   std::cout << "string 2 save : "<<bcode << "\n";
//   std::string fileName = argv[2];//"example435_test.lzw";
   fileName += ".lzw";
   std::ofstream myfile(fileName.c_str(), std::ios::binary);
//   std::ofstream myfile;
//   myfile.open(fileName.c_str(),  std::ios::binary);
   
   std::string zeros = "00000000";
   if (bcode.size()%8!=0) //make sure the length of the binary string is a multiple of 8
      bcode += zeros.substr(0, 8-bcode.size()%8);
   
   int b; 
   for (int i = 0; i < bcode.size(); i+=8) { 
      b = 1;
      for (int j = 0; j < 8; j++) {
         b = b<<1;
         if (bcode.at(i+j) == '1')
           b+=1;
      }
      char c = (char) (b & 255); //save the string byte by byte
      myfile.write(&c, 1);  
   }
   myfile.close();
   
   //reading from a file
   std::ifstream myfile2;
   myfile2.open (fileName.c_str(),  std::ios::binary);
   
   struct stat filestatus;
   stat(fileName.c_str(), &filestatus );
   long fsize = filestatus.st_size; //get the size of the file in bytes
   
   char c2[fsize];
   myfile2.read(c2, fsize);
   
   std::string s = "";
   long count = 0;
   while(count<fsize) {
      unsigned char uc =  (unsigned char) c2[count];
      std::string p = ""; //a binary string
      for (int j=0; j<8 && uc>0; j++) {         
		   if (uc%2==0)
            p="0"+p;
         else
            p="1"+p;
         uc=uc>>1;   
      }
      p = zeros.substr(0, 8-p.size()) + p; //pad 0s to left if needed
      s+= p; 
      count++;
   } 
   myfile2.close();
   std::cout << " saved string : "<<s << "\n"; 
}
*/ 
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

	//Testing a different way to read in the file to see
	//if it fixes my problem
/*	std::streampos begin,end;
	begin = infile.tellg();
	infile.seekg(0, std::ios::end);
	end = infile.tellg();
	std::streampos size = end - begin;
	infile.seekg(0, std::ios::beg);
	char* memblock = new char[size];
	infile.read(memblock, size);
	memblock[size]='\0';
//	infile.close();
*/	
	//Create a string of the file contents that a read in	
	std::string input{std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
	infile.close();

	if(*argv[1] == 'c')
	{
		//Avector (or ASCII Vector) takes the input string of the selected
		//file and stores them as integer values inside of a vector.
		std::vector<int> Avector;
		compress(input, std::back_inserter(Avector));
		copy(Avector.begin(), Avector.end(), std::ostream_iterator<int>(std::cout, ", "));
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
//			std::string Segment = Output.substr(i, 8);
			Segment = Output.substr(i, 8);
			//int newChar = binaryString2Int(Segment);
			//Output.replace(i, 8, 1, static_cast<char>(newChar));
			Output.replace(i, 8, 1, (char)binaryString2Int(Segment));
		}	
		
		filename.append(".lzw");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << Output;
		outfile.close();

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
//		assert((input.length() - Zeros) % 12 == 0);
		for(auto i = 0; i < input.size() - Zeros; i += 12)
		{
			std::string Istring = input.substr(i, 12);
			assert(Istring.length() == 12);
			std::cout << Istring << "\n";
			assert(Istring.length() == 12);
			Svector.push_back(binaryString2Int(Istring));
		}

		std::string d = decompress(Svector.begin(), Svector.end());
		filename.append("2");
		std::ofstream outfile(filename.c_str(), std::ios::binary);
		outfile << d;
		outfile.close();
	}

		
/*
//This is the example code from the original project.
		
  std::vector<int> compressed;
  compress("AAAAAAABBBBBB", std::back_inserter(compressed));
  copy(compressed.begin(), compressed.end(), std::ostream_iterator<int>(std::cout, ", "));
  std::cout << std::endl;
  std::string decompressed = decompress(compressed.begin(), compressed.end());
  std::cout << decompressed << std::endl;
  
  binaryIODemo(compressed);
  
  return 0;
*/
}
