This is the README for the Seam Carving Project by Ryan Malov

TO COMPILE THIS PROJECT SO THAT IT RUNS USE THE FOLLOWING INSTRUCTION:

g++ -std=c++14 SeamCarving.cpp -o seam

The above code will make it so that the code compiles and can be run with the following command:

./seam <filename> <number of vertical seams> <number of horizontal seams>

When the command above is executed, the new "carved" image will  be saved as <filename>_processed.pgm

------------------------------------------------------------------------------------------------------

The main goal of the code created is to take a gray image, put its pixels into a separate location in a 2D vector
and remove "boring" pixels to resize the image. This is done by taking the pixel matrix and doing calculations learned
in class on the pixel matrix and storing the new found values in the Energy Matrix. After the values are found and stored
in the energy matrix, there is another calculation used to find the Cumulative Energy Matrix. This new Cumulative matrix
is the one that dictates which line of pixels will be removed from the original image. There is the possibility to remove
lines of pixel both vertically and horizontally withing this project.

-------------------------------------------------------------------------------------------------------

THINGS TO NOTE ABOUT MY PROJECT:

This is a re-submission. Also, horizontal has some functionality. It seems to work on some of the file but
it will core dump on a few. The vertical should work for all cases.
