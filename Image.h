#pragma once

#include "Color.h"
#include "Vector2D.h"
#include "Types.h"

const int BYTES_PER_PIXEL = 3;


class EImageOutOfBoundsException : public std::exception
{

};

struct Image
{
	Image(IntVector2D InDimension)
		: Dimension(InDimension)
		, Size(InDimension.X * InDimension.Y)
		, Data(new Color[Size])
	{}

	~Image()
	{
		delete[] Data;
	}

	IntVector2D Dimension;
	int Size;
	Color* Data;

	Color GetColor(int X, int Y)
	{
		uint64 Index = GetIndex(X, Y);
		return Data[Index];
	}

	void SetColor(int X, int Y, Color InColor)
	{
		
		uint64 Index = GetIndex(X, Y);
		Data[Index] = InColor;
	}

	uint64 GetIndex(int X, int Y)
	{
		int result = Dimension.X * X + Y;
		if (result > Size)
			throw EImageOutOfBoundsException();
		return result;
	}

	void SaveToFile(std::string Filename)
	{
		int Width = Dimension.X;
		int Height = Dimension.Y;

		FILE* f;
		unsigned char* img = NULL;
		int filesize = 54 + 3 * Width * Height;  //w is your image width, h is image height, both int

		img = (unsigned char*)malloc(3 * Width * Height);
		// memset(img, 0, 3 * Width * Height);

		for (int i = 0; i < Width; i++)
		{
			for (int j = 0; j < Height; j++)
			{
				int x = i;
				int y = (Height - 1) - j;
				Color color = GetColor(i, j);
				int r = (int)(color.R * 255);
				int g = (int)(color.B * 255);
				int b = (int)(color.G * 255);
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;
				img[(x + y * Width) * 3 + 2] = (unsigned char)(r);
				img[(x + y * Width) * 3 + 1] = (unsigned char)(g);
				img[(x + y * Width) * 3 + 0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = {
			'B','M', 0,0,
			0,	0, 0,0, 0,
			0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(Width);
		bmpinfoheader[5] = (unsigned char)(Width >> 8);
		bmpinfoheader[6] = (unsigned char)(Width >> 16);
		bmpinfoheader[7] = (unsigned char)(Width >> 24);
		bmpinfoheader[8] = (unsigned char)(Height);
		bmpinfoheader[9] = (unsigned char)(Height >> 8);
		bmpinfoheader[10] = (unsigned char)(Height >> 16);
		bmpinfoheader[11] = (unsigned char)(Height >> 24);


		bmpinfoheader[12] = (unsigned char)(1);
		bmpinfoheader[14] = (unsigned char)(BYTES_PER_PIXEL * 8);

		f = fopen(Filename.c_str(), "wb");
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i < Height; i++)
		{
			fwrite(img + (Width * (Height - i - 1) * 3), 3, Width, f);
			fwrite(bmppad, 1, (4 - (Width * 3) % 4) % 4, f);
		}

		free(img);
		fclose(f);

	}
};
