This is the readme file for Ryan Malov's RSA Encryption project (for Part 1)

You might have to wait on the program to run. The shortest time I have seen my program run is 17 seconds.
The longest I have seen my program run has been around 7-8 minutes. The average is around 4-6 minutes.

------------------------------------------------------------------------------------------------------------------

The basis of this program is to find two large prime numbers for an RSA encryption. The program
starts by looking for a p value which is randomly generated and is required to pass the Fermat Test
twice via inclass discussion.

The same process takes place when finding a q value.

n, phi, d, e are found based off of the inital p and q values.

After all values are found, they are written into three seperate text files, which are used in Part 2 of
the project. The files are:
	p_q.txt
	e_n.txt
	d_n.txt
