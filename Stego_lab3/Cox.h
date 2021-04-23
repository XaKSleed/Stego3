#pragma once
#include <iostream>
#include <windows.h>
#include <fstream>
#include<String>

using namespace std;
class Cox
{
private:

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	string message;
	int message_size;
	int num_insert_bits = 0;
	int image_height = 0;
	int image_width = 0;
	
	float** blue_matrix;
	float** red_matrix;
	float** green_matrix;
	
	float** blue_matrix_new;
	float** red_matrix_new;
	float** green_matrix_new;

	float** dct_red;
	float** dct_green;
	float** dct_blue;
	
	int N = 8;

	int orig_msg[2648];
	int new_msg[2648];
	int result[2648];

public:

	Cox()
	{

	}

	~Cox() {}

	int Write_image();
	int Read_image(FILE* in);
	int Read_image_new(FILE* in);
	int Read_message(string message_file);
	float** DCT(float** component);
	float** de_DCT(float** component);
	int Insert_message(int critic_value, float alpha);
	int Get_message(FILE* orig, FILE* chng, string res_file, int critic_value);
	int Bits_to_byte(bool bits[]);
	void get_PSNR(float** original, float** restored);
};