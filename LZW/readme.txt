This is the readme file for Ryan Malov's LZW Project.

The project is split into two parts.

Part 1:
This is the original LZW435.cpp file using fixed length codes for compression and expansion/decompression. It takes in a given file and will take the information inside of the file into a string and convert into binary values where is can be compressed down into a smaller file size.

This program can also take the compressed file and expand it (or decompress it) and return it to a readable file similar to the original file that was to be compressed.

TO BUILD AND RUN THIS PROGRAM:

compile on the Linux servers using the command: g++ -std=c++11 lzw435.cpp -o lzw

If built using this command set, the output comments givin within the file itself will help the user along to the next step. 

Addional information, in the cout during the execution of the program when <filename> is seen. it is expecting an input similar to the following: example.txt OR example.docx
Please include the .txt or other file type to make the command run as expected.

To check the size of a file after it has been compressed or expanded, please use the following instruction:

ll <filename>.lzw    --- This is for checking the size of the compressed file
ll <filename>.lzw2   --- This is for checking the size of the expanded file

For checking part 2 file sizes:

ll <filename>.lzwm   --- This is for checking the size of the compressed file
ll <filename>.lzwm2  --- This is for checking the size of the expanded file

Part 2:
This is the modified file called lzw435M.cpp. This file is a modified version of the original lzw435 file. In this part of the project, it was required to use a variable length to compress and expand/decompress a given file up to 16 bits. It functions in a similar way to the first part of the project, except for the variable length part.

TO BUILD AND RUN THIS PROGRAM:

compile on the Linus servers using the command: g++ -std=c++11 lzw435M.cpp -o lzwm

If built using this command set, the output comments given within the file itself will help the user along to the next step.

Please refer to the addional information section above if more help is needed to make a certain command run.


NOTE: Some files take a minute or two to compress. Please do not be impatient. They do in fact compress all the way and work correctly!
