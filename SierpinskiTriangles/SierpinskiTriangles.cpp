// SierpinskiTriangles.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <iostream>

//kazdy watek robi 3 watki
//trojkat prostokatny
int N = 1024;
using namespace std;
FILE* fp;
/* screen ( integer) coordinate */
const int pictureSize = 1000;
const int marginSize = pictureSize / (10*2);
unsigned char ImageMatrix[pictureSize][pictureSize][3];
char filename[] = "sierpinski.ppm";
char comment[] = "# ";/* comment should start with # */
const int MaxColorComponentValue = 255;
const int maxDepth = 7;
void zeros() //tworzenie bialego obrazu
{
	for (int i = 0;i < pictureSize;i++)
	{
		for (int j = 0;j < pictureSize;j++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[j][i][c] = 0;
			}
		}
	}
}
class Square
{
	int startSize;
	void downRightTriangle(int X,int Y,int size,int depth)
	{
		vector<int> Xvect;
		vector<int> Yvect;
		if (depth == maxDepth)
		{
			return;
		}
		for (int i = X;i >= X - size;i--)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[i][Y][c] = 255;
			}
			Xvect.push_back(i);
		}
		for (int i = Y;i >= Y - size;i--)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[X-size][i][c] = 255;
			}
			Yvect.push_back(i);
		}
		for (int i = 0;i <= size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[Xvect[i]][Yvect[i]][c] = 255;
			}
		}
		depth++;
		size = size / 2;
		thread tr1(&Square::downRightTriangle, this,  X, Y,size, depth);
		tr1.join();
		thread tr2(&Square::downRightTriangle, this, X-size, Y,size, depth);
		tr2.join();
		thread tr3(&Square::downRightTriangle, this, X - size, Y - size,size, depth);
		tr3.join();
	}
	
	void downLeftTriangle(int X,int Y,int size,int depth)
	{
		vector<int> Xvect;
		vector<int> Yvect;
		if (depth == maxDepth)
		{
			return;
		}
		for (int i = X;i <= X + size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[i][Y][c] = 255;
			}
			Xvect.push_back(i);
		}
		for (int i = Y;i >= Y - size;i--)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[X+size][i][c] = 255;
			}
			Yvect.push_back(i);
		}
		for (int i = 0;i <= size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[Xvect[i]][Yvect[i]][c] = 255;
			}
		}
		depth++;
		size = size / 2;
		thread tr1(&Square::downLeftTriangle, this, X, Y, size, depth);
		tr1.join();
		thread tr2(&Square::downLeftTriangle, this, X + size, Y, size, depth);
		tr2.join();
		thread tr3(&Square::downLeftTriangle, this, X + size, Y - size, size, depth);
		tr3.join();
	}
	

	void upRightTriangle(int X, int Y, int size, int depth)
	{
		vector<int> Xvect;
		vector<int> Yvect;
		if (depth == maxDepth)
		{
			return;
		}
		for (int i = X;i >= X - size;i--)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[i][Y][c] = 255;
			}
			Xvect.push_back(i);
		}
		for (int i = Y;i <= Y + size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[X-size][i][c] = 255;
			}
			Yvect.push_back(i);
		}
		for (int i = 0;i <= size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[Xvect[i]][Yvect[i]][c] = 255;
			}
		}
		depth++;
		size = size / 2;
		thread tr1(&Square::upRightTriangle, this,  X-size, Y,size, depth);
		tr1.join();
		thread tr2(&Square::upRightTriangle, this, X, Y,size, depth);
		tr2.join();
		thread tr3(&Square::upRightTriangle, this, X - size, Y + size,size, depth);
		tr3.join();
	}

	void upLeftTriangle(int X, int Y, int size, int depth)
	{
		vector<int> Xvect;
		vector<int> Yvect;
		if (depth == maxDepth)
		{
			return;
		}
		for (int i = X;i <= X + size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[i][Y][c] = 255;
			}
			Xvect.push_back(i);
		}
		for (int i = Y;i <= Y + size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[X + size][i][c] = 255;
			}
			Yvect.push_back(i);
		}
		for (int i = 0;i <= size;i++)
		{
			for (int c = 0;c < 3;c++)
			{
				ImageMatrix[Xvect[i]][Yvect[i]][c] = 255;
			}
		}
		depth++;
		size = size / 2;
		thread tr1(&Square::upLeftTriangle, this, X, Y, size, depth);
		tr1.join();
		thread tr2(&Square::upLeftTriangle, this, X + size, Y, size, depth);
		tr2.join();
		thread tr3(&Square::upLeftTriangle, this, X + size, Y + size, size, depth);
		tr3.join();
	}
	
public:
	Square(int arg)
	{
		startSize = arg;
	}
	void drawSquare()
	{
		vector<thread> drawThreads;
		drawThreads.push_back(thread(&Square::downRightTriangle,this,marginSize + startSize, pictureSize - marginSize, startSize, 1));
		drawThreads.push_back(thread(&Square::downLeftTriangle, this, marginSize + startSize, pictureSize - marginSize, startSize, 1));

		drawThreads.push_back(thread(&Square::upRightTriangle,this,marginSize + startSize, marginSize, startSize, 1));
		drawThreads.push_back(thread(&Square::upLeftTriangle, this, marginSize + startSize, marginSize, startSize, 1));

		drawThreads.push_back(thread(&Square::downLeftTriangle,this,marginSize, marginSize + startSize, startSize, 1));
		drawThreads.push_back(thread(&Square::upLeftTriangle, this, marginSize, marginSize + startSize, startSize, 1));

		drawThreads.push_back(thread(&Square::downRightTriangle,this,pictureSize - marginSize, marginSize + startSize, startSize, 1));
		drawThreads.push_back(thread(&Square::upRightTriangle, this, pictureSize - marginSize, marginSize + startSize, startSize, 1));
		for (int i = 0;i < drawThreads.size();i++)
		{
			drawThreads[i].join();
		}
	}
};

int main()
{
	/*create new file,give it a name and open it in binary mode  */
	fp = fopen(filename, "wb"); /* b -  binary mode */
	/*write ASCII header to the file*/
	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, pictureSize, pictureSize, MaxColorComponentValue);
	/* compute and write image data bytes to the file*/
	zeros();
	Square squarePicture(pictureSize/2-marginSize);
	squarePicture.drawSquare();
	for (int i = 0;i < pictureSize;i++)
	{
		for (int j = 0;j < pictureSize;j++)
		{
			fwrite(ImageMatrix[j][i], 1, 3, fp);
		}
	}
	fclose(fp);
	return 0;
}
