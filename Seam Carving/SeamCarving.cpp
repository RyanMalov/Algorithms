//Ryan Malov
//Project 3
//Seam Carving

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
	void PopulateCumulativeVertMatrix();
	void PopulateCumulativeHorzMatrix();
	void FindVertSeams();
//	auto FindMinCol();
	void FindHorzSeams();
	void RemoveVertSeams(int VertSeams);
	void RemoveHorzSeams(int HorzSeams);
	void WriteFile(std::ofstream& output);
	void rotate(int a);

private:

	//2D vectors for storing information needed
	std::vector<std::vector<int>> ImageMatrix;
	std::vector<std::vector<int>> EnergyMatrix;
	std::vector<std::vector<int>> CumulativeMatrix;

	std::map<int, int> Position;
	std::string Header;
	std::string Dimensions;
	std::string GrayMax;
	int	Width;
	int Height;
	std::vector<std::vector<int>> CumulativeRotated;
	std::vector<std::vector<int>> ImageRotated;
};

int main(int argc, char** argv)
{
	std::ifstream Image;
	std::ofstream ProcessedImage;

	try
	{
		//Check to see if the correct input has been used to run the Seam Carving
		if(argc != 4)
		{
			throw "To run correctly: Program <space> filename <space> Number_of_Vertical_Seams <space> Number_of_Horizontal_Seams";
		}
		
		else
		{
			if(atoi(argv[2]) < 0 || atoi(argv[3]) < 0)
			{
				throw "Please select a valid number";
			}
			
			std::string filename = argv[1];
			Image.open(filename.c_str(), std::ios::binary);
			filename.erase(filename.length() - 4, 4);
			filename = filename + "_processed.pgm";
			ProcessedImage.open(filename.c_str(), std::ios::binary);

			if (!Image.is_open())
			{
				throw "Cannot open image file";
			}

			if (!ProcessedImage.is_open())
			{
				throw "Cannot create output file";
			}
		}
	} 
	catch (const char* err)
	{
		std::cout << "Whoops: " << err << std::endl;
	}
	
	SeamCarving sCarve;
	sCarve.getHeader(Image);
	sCarve.PopulateImageMatrix(Image);
	sCarve.RemoveVertSeams(atoi(argv[2]));
	sCarve.RemoveHorzSeams(atoi(argv[3]));
	sCarve.WriteFile(ProcessedImage);
	Image.close();
	ProcessedImage.close();

	return 0;
}

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
	
//	std::cout << std::endl;
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

//Fill a matrix with values from Image
void SeamCarving::PopulateImageMatrix(std::ifstream& Image)
{
	//Resize the vector based on image
	ImageMatrix.resize(Height);
	
	for(int i = 0; i < Height; i++)
	{
		ImageMatrix[i].resize(Width);
	}
	
	//Fill vector with the cell/pixel values
	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			Image >> ImageMatrix[y][x];
//			std::cout << "This is the ImageMatrix" << std::endl;
//			print(ImageMatrix);
//			std::cout << std::endl;
//			std::cout << ImageMatrix[y][x] << std::endl;
		}
	}
//	std::cout << ImageMatrix[y][x];
//	std::cout << std::endl;
//	std::cout << "This is the image matrix:" << std::endl;
//	print(ImageMatrix);
//	std::cout << std::endl;
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
			
			//Top of matrix
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

			//Bottom of the matrix
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

			//If they are not none of the above
			else
			{
				EnergyMatrix[y][x] = abs(ImageMatrix[y][x] - ImageMatrix[y][x - 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y][x + 1]) + abs(ImageMatrix[y][x] - ImageMatrix[y - 1][x]) + abs(ImageMatrix[y][x] - ImageMatrix[y + 1][x]);
			}

//			std::cout << "This is the energy Matrix" << std::endl;
//			print(EnergyMatrix);
//			std::cout << EnergyMatrix[y][x] << std::endl;
		}
	}
//	std::cout << "this is the energy matrix:" << std::endl;
//	print(EnergyMatrix);
//	std::cout << std::endl;
}

//Fill in the Vertical CumulativeMatrix with the difference values from the energy matrix
void SeamCarving::PopulateCumulativeVertMatrix()
{
	CumulativeMatrix.clear();
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
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(CumulativeMatrix[y-1][x], CumulativeMatrix[y-1][x+1]);
			}

			//Right side of the cumulative matrix
			else if(x == Width - 1)
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(CumulativeMatrix[y-1][x], CumulativeMatrix[y-1][x-1]);
			}

			//Others
			else
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(std::min(CumulativeMatrix[y-1][x-1], CumulativeMatrix[y-1][x]), CumulativeMatrix[y-1][x+1]);
			}
		}
	}
//	std::cout << "This is the PopulateCumulativeVertMatrix:" << std::endl;
//	print(CumulativeMatrix);
//	std::cout << std::endl;
}

//Find the vertical seams from the cumulative matrix
void SeamCarving::FindVertSeams()
{ 
	std::vector<int>::iterator iter;

	//Clear the map that holds the path
	Position.clear();
	
	iter = std::min_element(CumulativeMatrix[Height - 1].begin(), CumulativeMatrix[Height - 1].end());
	Position[Height-1] = std::distance(std::begin(CumulativeMatrix[Height - 1]), iter);

	for(int row = Height - 1; row > 0; row--)
	{
		//Left side calculations
		if(Position[row] == 0)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin(), CumulativeMatrix[row - 1].begin() + 2);
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//Right side calculations
		else if(Position[row] == Width - 1)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin() + Position[row] - 1, CumulativeMatrix[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//If the width is 3 or less (or a special case)
		else if (Width <= 3)
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin(), CumulativeMatrix[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}

		//Everything else
		else
		{
			iter = std::min_element(CumulativeMatrix[row - 1].begin() + Position[row] - 1, CumulativeMatrix[row - 1].begin() + Position[row] + 2);
			Position[row - 1] = std::distance(std::begin(CumulativeMatrix[row - 1]), iter);
		}
	}
//	std::cout << "This is the FindVertSeams" << std::endl;
//	print(FindVertSeams);
//	std::cout << std::endl;
}

//Remove the Vertical Seams from the Image
void SeamCarving::RemoveVertSeams(int VertSeams)
{
	while(VertSeams > 0)
	{
		PopulateEnergyMatrix();
		PopulateCumulativeVertMatrix();
		FindVertSeams();
		
		for(auto i : Position)
		{
			ImageMatrix[i.first].erase(ImageMatrix[i.first].begin() + i.second);
//			CumulativeMatrix[i.first].erase(CumulativeMatrix[i.first].begin() + i.second);
		}

		Width--;
		VertSeams--;
	}
//	std::cout << "This is the Remove Vert Seams:" << std::endl;
//	print(RemoveVertSeams);
//	std::cout << std::endl;
}

//Populate the Horizontal CumulativeMatrix
void SeamCarving::PopulateCumulativeHorzMatrix()
{
	//Resize the matrix
	CumulativeMatrix.clear();
	CumulativeMatrix.resize(Height);

	for(int i = 0; i < Height; i++)
	{
		CumulativeMatrix[i].resize(Width);
	}

	//Copy the top row of the cumulative matrix
	for(int y = 0; y < Height; y++)
	{
		CumulativeMatrix[y][0] = EnergyMatrix[y][0];
	}

	for(int x = 1; x < Width; x++)
	{
		for(int y = 0; y < Height; y++)
		{
			//The Top
			if(y == 0)
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(CumulativeMatrix[y][x - 1], CumulativeMatrix[y + 1][x - 1]);
			}

			//The Bottom
			else if(y == Height - 1)
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(CumulativeMatrix[y][x - 1], CumulativeMatrix[y - 1][x - 1]);
			}

			//The rest
			else
			{
				CumulativeMatrix[y][x] = EnergyMatrix[y][x] + std::min(std::min(CumulativeMatrix[y + 1][x - 1], CumulativeMatrix[y][x - 1]), CumulativeMatrix[y - 1][x - 1]);
			}
		}
	}
//	std::cout << "This is the Populate Cumulative Horz matrix:" << std::endl;
//	print(CumulativeMatrix);
//	std::cout << std::endl;
}

//Rotate the matricies so that I can do Horizontal paths
void SeamCarving::rotate(int a)
{
	if(a == 0)
	{
		//Resize the matrix
		CumulativeRotated.resize(Width);
		ImageRotated.resize(Width);

		for(int i = 0; i < Width; i++)
		{
			CumulativeRotated[i].resize(Height);
			ImageRotated[i].resize(Height);
		}

		//Do the rotation
		for(int i = 0; i < Width; i++)
		{
			for(int j = 0; j < Height; j++)
			{
				CumulativeRotated[i][j] = CumulativeMatrix[Height - 1 - j][i];
				ImageRotated[i][j] = ImageMatrix[Height - 1 - j][i];
			}
		}
	}

	else if(a == 1)
	{
		ImageMatrix.clear();
		ImageMatrix.resize(Height);

		for(int i = 0; i < Height; i++)
		{
			ImageMatrix[i].resize(Width);
		}

		//Rotate
		for(int i = 0; i < Height; i++)
		{
			for(int j = 0; j < Width; j++)
			{
				ImageMatrix[i][j] = ImageRotated[j][Height - 1 - i];
			}
		}
	}
	else
	{
		std::cout << "Something went horribly wrong" << std::endl;
	}
//	std::cout << "this is the rotated matrix for horizontal:" << std::endl;
//	print(ImageMatrix);
//	std::cout << std::endl;
}

//Find the shortest horizontal path
void SeamCarving::FindHorzSeams()
{
	std::vector<int>::iterator itor;

	//clear the map
	Position.clear();

	//Find the smallest element in the bottom row
	itor = std::min_element(CumulativeRotated[Width - 1].begin(), CumulativeRotated[Width - 1].end());
	Position[Width - 1] = std::distance(std::begin(CumulativeRotated[Width - 1]), itor);

	for(int row = Width - 1; row > 0; row--)
	{
		//The Left Side
		if(Position[row] == 0)
		{
			itor = std::min_element(CumulativeRotated[row - 1].end() - 2, CumulativeRotated[row - 1].begin() + 2);
			Position[row - 1] = std::distance(std::begin(CumulativeRotated[row - 1]), itor);
		}

		//The Right Side
		else if(Position[row] == Height - 1)
		{
			itor = std::min_element(CumulativeRotated[row - 1].end() - 2, CumulativeRotated[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeRotated[row - 1]), itor);
		}

		//For special cases to make sure everything works
		else if(Height <= 3)
		{
			itor = std::min_element(CumulativeRotated[row - 1].begin(), CumulativeRotated[row - 1].end());
			Position[row - 1] = std::distance(std::begin(CumulativeRotated[row - 1]), itor);
		}

		//Everything else
		else
		{
			itor = std::min_element(CumulativeRotated[row - 1].begin() + Position[row] - 1, CumulativeRotated[row - 1].begin() + Position[row] + 2);
			Position[row - 1] = std::distance(std::begin(CumulativeRotated[row - 1]), itor);
		}
	}
//	std::cout << "This is the FindHorzSeam:" << std::endl;
//	print(FindHOrzSeams);
//	std::cout << std::endl;
}

//Remove the Horizontal seams
void SeamCarving::RemoveHorzSeams(int HorzSeams)
{
	while(HorzSeams > 0)
	{
		PopulateEnergyMatrix();
		PopulateCumulativeHorzMatrix();
		rotate(0);
		FindHorzSeams();

		for(auto i : Position)
		{
			ImageRotated[i.first].erase(ImageRotated[i.first].begin() + i.second);
		}

		HorzSeams--;
		Height--;
		rotate(1);
	}
//	std::cout << "This is the RemoveHorzSeams:" << std::endl;
//	print(RemoveHorzSeams);
//	std::cout << std::endl;
}

			


/*

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



//Find the Horizonal Seams
void SeamCarving::FindHorzSeams()
{
	std::vector<std::vector<int>>::iterator iter;
	int row;
	std::vector<int> Comp;
	auto MinIter = FindMinCol();
	Position.clear();

	//Need to find the smallest value in the last column
	for(int y = 0; y < Height; y++)
	{
		if(CumulativeMatrix[y].begin() <= MinIter && MinIter < CumulativeMatrix[y].end())
		{
			row = y;
		}
	}

	//Think it would be handled something like this
	//which is similar to how the vertical seams were handled
	for(int column = Width - 1; column > 0; column--)
	{

		//Top calculation
		if(row == 0)
		{
			
		}

		//Bottom calculation
		else if(row == Height - 1)
		{

		}

		//If width is less than 3 (or special case)
		else if(Height <= 3)
		{

		}

		//Otherwise
		else
		{

		}
	}
}

//Remove the Horizontal Seams

//Again, figure this would work similar to the vertical seams.
//Stuck on the part above and not quite sure how to work it out


void SeamCarving::RemoveHorzSeams(int HorzSeams) {
	while(HorzSeams > 0)
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
//Output the new Image/file
void SeamCarving::WriteFile(std::ofstream& output)
{
	std::vector<std::vector<int>>::iterator row;
	std::vector<int>::iterator column;

	output << "P2\n";
	output << "#\n";
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
