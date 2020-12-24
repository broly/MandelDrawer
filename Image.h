#pragma once

#include "Color.h"
#include "Vector2D.h"
#include "Types.h"
#include <sys/stat.h>
#include <direct.h>

#include <jpeglib.h>

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
		int result = Dimension.Y * Y + X;
		if (result > Size)
			throw EImageOutOfBoundsException();
		return result;
	}

	static void CreateOutputDirectoryIfNotExists()
	{
		struct stat info;

		const char* OutputDirectory = "Output";
		
		stat(OutputDirectory, &info);

		if (!(info.st_mode & S_IFDIR))
		{
			_mkdir(OutputDirectory);
		}
	}

	void SaveToFile(std::string Filename)
	{
		CreateOutputDirectoryIfNotExists();
		
		int Width = Dimension.X;
		int Height = Dimension.Y;

		FILE* f;
		unsigned char* img = NULL;
		int filesize = 54 + 3 * Width * Height;  //w is your image width, h is image height, both int

		img = (unsigned char*)Data; 

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
		
		fclose(f);

	}

	
	
	void SaveToFileJ(std::string filename)
	{
		CreateOutputDirectoryIfNotExists();
		
		struct jpeg_compress_struct cinfo;
		
		struct jpeg_error_mgr jerr;
		/* More stuff */
		FILE* outfile;		/* target file */
		JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
		int row_stride;		/* physical row width in image buffer */

		
		cinfo.err = jpeg_std_error(&jerr);
		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);

		
		if ((outfile = fopen(filename.c_str(), "wb")) == NULL) {
			fprintf(stderr, "can't open %s\n", filename.c_str());
			exit(1);
		}
		jpeg_stdio_dest(&cinfo, outfile);

		


		cinfo.image_width = Dimension.X; 	/* image width and height, in pixels */
		cinfo.image_height = Dimension.Y;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
		
		jpeg_set_defaults(&cinfo);
		
		jpeg_set_quality(&cinfo, 90, TRUE /* limit to baseline-JPEG values */);

		
		jpeg_start_compress(&cinfo, TRUE);

		
		row_stride = Dimension.X * 3;	/* JSAMPLEs per row in image_buffer */

		Color* DataRecolor = new Color[Size];

		for (uint64 i = 0; i < Size; i++)
		{
			DataRecolor[i] = Data[i].BGR();
		}
	
		while (cinfo.next_scanline < cinfo.image_height) {

			row_pointer[0] = (unsigned char*)DataRecolor + cinfo.next_scanline * row_stride;
			jpeg_write_scanlines(&cinfo, row_pointer, 1);

		}
		delete[] DataRecolor;
		
		jpeg_finish_compress(&cinfo);
		/* After finish_compress, we can close the output file. */
		fclose(outfile);

		/* Step 7: release JPEG compression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);

		/* And we're done! */
	}
};
