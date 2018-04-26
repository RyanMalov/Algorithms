#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

class SeamCarving {
public:
	// Constructor
	SeamCarving() = default;

	void print(std::vector<std::vector<int>> Matrix);
	void getHeader(std::ifstream& Image);
	void PopulateImageMatrix(std::ifstream& Image);
	void PopulateEnergyMatrix();
	void PopulateCumulativeMatrix();
	void FindVertSeams();
	auto FindMinCol();
//	void FindHorzSeams();
	void RemoveVertSeams(int VertSeams);
//	void RemoveHorzSeams(int HorzSeams);
	void WriteFile(std::ofstream& output);

private:

	std::vector<std::vector<int>> ImageMatrix;
	std::vector<std::vector<int>> EnergyMatrix;
	std::vector<std::vector<int>> CumulativeMatrix;
	std::map<int, int> Position;
	std::string Header;
	std::string Dimensions;
	std::string GrayMax;
	int	Width;
	int Height;
};

//Printing
void SeamCarving::print(std::vector<std::vector<int>> Matrix)
{
	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			std::cout << Matrix[y][x] << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
}

//Handle the header and gray levels
void SeamCarving::getHeader(std::ifstream& Image)
{	
	std::string x;
	std::string y;
	
	std::getline(Image, Header);
	
	while(Header[0] == '#')
	{
		std::getline(Image, Header);
	}

	std::getline(Image, Dimensions);
	
	while(Dimensions[0] == '#')
	{
		std::getline(Image, Dimensions);
	}

	std::getline(Image, GrayMax);
	
	while(GrayMax[0] == '#')
	{
		std::getline(Image, GrayMax);
	}

	const char* chars = Dimensions.c_str();

	while(*chars != ' ')
	{
		x += *chars++;
	}
	for(auto i = Dimensions.size() - x.size()-1; i < Dimensions.size(); i++)
	{
		y += *chars++;
	}

	Width = stoi(x);
	Height = stoi(y);
}

//Fill a matrix with values from image
void SeamCarving::PopulateImageMatrix(std::ifstream& Image)
{
	//Resize the vector
	ImageMatrix.resize(Height);
	
	for(int i = 0; i < Height; i++)
	{
		ImageMatrix[i].resize(Width);
	}
	
	//Fill vector with the cell/pixel values
	for(int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			Image >> ImageMatrix[y][x];
		}
	}
}

//Fill in the Matrix which tells you how "active" a cell/pixel is
void SeamCarving::PopulateEnergyMatrix()
{
	EnergyMatrix.resize(Height);
	
	for(int i = 0; i < Height; i++)
	{
		EnergyMatrix[i].resize(Width);
	}

	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			
			//Top
			if(y == 0)
			{
				//Top left corner of matrix
				if(x == 0 && y == 0)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
				}
				
				//Top right corner of matrix
				else if(x == Width - 1 && y == 0)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
				}
				
				//The other parts of the matrix
				else
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
				}
			}

			//Bottom
			else if(y == Height - 1)
			{
				//Bottom left of the matrix
				if(x == 0 && y == Height - 1)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]);
				}
				
				//Bottom right of the matrix
				else if(x == Width - 1 && y == Height - 1)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]);
				}
				
				//Otherwise
				else
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]);
				}
			}

			//The sides of the matrix
			else if (x == 0 || x == Width - 1)
			{

				//The left side of the matrix
				if (x == 0)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
				}

				//The right side of the matrix
				else if (x == Width - 1)
				{
					EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
				}
			}

			//If they are not none of the abocve
			else
			{
				EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
			}
		}
	}
}

//Fill in the CumulativeMatrix with the difference values from the energy matrix
void SeamCarving::PopulateCumulativeMatrix()
{
	CumulativeMatrix.resize(Height);
	
	for(int i = 0; i < Height; i++)
	{
		CumulativeMatrix[i].resize(Width);
	}

	//Copy the top row of energy Matrix to cumulative Matrix
	for(int x = 0; x < Width; x++)
	{
		CumulativeMatrix[0][x] = EnergyMatrix[0][x];
	}

	for(int y = 1; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			//Left side of the cumulative matrix
			if(x == 0)
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(EnergyMatrix[y-1][x], EnergyMatrix[y-1][x+1]);
			}

			//Right side of the cumulative matrix
			else if(x == Width - 1)
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(EnergyMatrix[y-1][x], EnergyMatrix[y-1][x-1]);
			}

			//Others
			else
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(std::min(EnergyMatrix[y-1][x-1], EnergyMatrix[y-1][x]), EnergyMatrix[y-1][x+1]);
			}
		}
	}
	print(CumulativeMatrix);
}

//Find the vertical seams from the cumulative matrix
void SeamCarving::FindVertSeams()
{ 
	std::vector<int>::iterator iter;
	
	iter = std::min_element(CumulativeMatrix[Height - 1].begin(), CumulativeMatrix[Height - 1].end());
	Position[Height-1] = std::distance(std::begin(CumulativeMatrix[Height - 1]), iter);

	for(int row = Height - 1; row > 0; row--)
	{
		//Left side calculation
		if(Position[row] == 0)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin(), CumulativeMatrix[row - 1].begin() + 2);
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//Right side calculation
		else if(Position[row] == Width - 1)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin() + Position[row] - 1, CumulativeMatrix[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//If the width is 3 or less
		else if (Width <= 3)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin(), CumulativeMatrix[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//Everything else
		else
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin() + Position[row] - 1, CumulativeMatrix[row - 1].begin() + Position[row] + 1);
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}
	}
}

//Remove the Vertical Seams from the image
void SeamCarving::RemoveVertSeams(int VertSeams)
{
	while(VertSeams > 0)
	{
		PopulateEnergyMatrix();
		PopulateCumulativeMatrix();
		FindVertSeams();
		
		for(auto i : Position)
		{
			ImageMatrix[i.first].erase(ImageMatrix[i.first].begin() + i.second);
			CumulativeMatrix[i.first].erase(CumulativeMatrix[i.first].begin() + i.second);
		}

		Width--;
		VertSeams--;
	}

	print(ImageMatrix);
}

//Find the minimal column so that we can get ready for removal
auto SeamCarving::FindMinCol()
{
	std::vector<int> values;
	int column = Width - 1;

	for(int row = 0; row < Height; row++)
	{
		values.push_back(CumulativeMatrix[row][column]);
	}

	auto min_elem = std::min_element(values.begin(), values.end());

	for(int row = 0; row < Height; row++)
	{
		if(CumulativeMatrix[row][column] == *min_elem) {
			return CumulativeMatrix[row].begin() + column;
		}
	}
}

/*

//Find the Horizonal Seams
void SeamCarving::FindHorzSeams()
{
	std::vector<std::vector<int>>::iterator iter;
	int row;
	std::vector<int> Comp;
	auto MinIter = FindMinCol();
	Position.clear();

	//Find the row of the last column that holds the smallest value
	for(int y = 0; y < Height; y++)
	{
		if(CumulativeMatrix[y].begin() <= MinIter && MinIter < CumulativeMatrix[y].end())
		{
			row = y;
		}
	}

	for(int column = Width - 1; column > 0; column--)
	{

		//Top
		if(row == 0)
		{
			
		}

		//Bottom
		else if(row == Height - 1)
		{

		}

		//Special case
		else if (Height <= 3)
		{

		}

		//Otherwise
		else
		{

		}
	}
}

//Remove the Horizontal Seams
void SeamCarving::RemoveHorzSeams(int HorzSeams) {
	while (HorzSeams > 0)
	{
		PopulateEnergyMatrix();
		PopulateCumulativeMatrix();
		FindHorzSeams();

		for(auto i : Position)
		{
			ImageMatrix[i.first].erase(ImageMatrix[i.first].begin() + i.second);
			CumulativeMatrix[i.first].erase(CumulativeMatrix[i.first].begin() + i.second);
		}

		Height--;
		HorzSeams--;
	}

	print(ImageMatrix);
}

*/

//Output the new image/file
void SeamCarving::WriteFile(std::ofstream& output)
{
	std::vector<std::vector<int>>::iterator row;
	std::vector<int>::iterator column;

	output << "P2\n";
	output << Width << " " << Height << "\n";
	output << GrayMax;
	
	for(row = ImageMatrix.begin(); row != ImageMatrix.end(); row++)
	{
		for(column = row->begin(); column != row->end(); column++)
		{
			output << *column << " ";
		}
		output << "\n";
	}
	output << "\n";
}
