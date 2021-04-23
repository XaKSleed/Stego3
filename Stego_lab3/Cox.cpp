#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <fstream>
#include <String>
#include "Cox.h"
#include <Vector>

#define M_PI 3.141592

using namespace std;

int Cox::Read_message(string message_file) {

	ifstream fin(message_file);

	if (!fin.is_open())
	{
		cout << "File not found" << endl;
		return 1;
	}
	else
	{
		getline(fin, message);
	}

	message_size = message.length();
	fin.close();
	return 0;
}


int Cox::Read_image(FILE* in)
{

	fread(&bfh, sizeof(bfh), 1, in);
	fread(&bih, sizeof(bih), 1, in);

	int height = bih.biHeight;
	int width = bih.biWidth;
	image_height = height;
	image_width = width;
	cout << image_height << endl;
	cout << image_width << endl;


	RGBTRIPLE rgb;

	red_matrix = new float *[bih.biHeight];
	green_matrix = new float *[bih.biHeight];
	blue_matrix = new float *[bih.biHeight];
	

	for (int i = 0; i < bih.biHeight; i++)
	{ 
		red_matrix[i] = new float[bih.biWidth];
		green_matrix[i] = new float[bih.biWidth];
		blue_matrix[i] = new float[bih.biWidth];
		

		for (int j = 0; j < bih.biWidth; j++)
		{
			fread(&rgb, sizeof(rgb), 1, in);

			red_matrix[i][j] = rgb.rgbtRed;

			green_matrix[i][j] = rgb.rgbtGreen;

			blue_matrix[i][j] = rgb.rgbtBlue;


		}

	}

	fseek(in, 0, SEEK_SET);
	return 0;
}

int Cox::Read_image_new(FILE* in)
{

	fread(&bfh, sizeof(bfh), 1, in);
	fread(&bih, sizeof(bih), 1, in);

	int height = bih.biHeight;
	int width = bih.biWidth;
	image_height = height;
	image_width = width;
	


	RGBTRIPLE rgb;

	red_matrix_new = new float *[bih.biHeight];
	green_matrix_new = new float *[bih.biHeight];
	blue_matrix_new = new float *[bih.biHeight];


	for (int i = 0; i < bih.biHeight; i++)
	{
		red_matrix_new[i] = new float[bih.biWidth];
		green_matrix_new[i] = new float[bih.biWidth];
		blue_matrix_new[i] = new float[bih.biWidth];


		for (int j = 0; j < bih.biWidth; j++)
		{
			fread(&rgb, sizeof(rgb), 1, in);

			red_matrix_new[i][j] = rgb.rgbtRed;

			green_matrix_new[i][j] = rgb.rgbtGreen;

			blue_matrix_new[i][j] = rgb.rgbtBlue;


		}

	}

	fseek(in, 0, SEEK_SET);
	return 0;
}

int Cox::Write_image()
{

	FILE* out = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab3\\Stego_lab3\\RES.bmp", "wb");
	fwrite(&bfh, sizeof(bfh), 1, out);
	fwrite(&bih, sizeof(bih), 1, out);

	RGBTRIPLE rgb;

	for (int i = 0; i < image_height; i++)
	{
		for (int j = 0; j < image_width; j++)
		{
			rgb.rgbtRed = (BYTE)red_matrix[i][j];
			rgb.rgbtGreen = (BYTE)green_matrix[i][j];
			rgb.rgbtBlue = (BYTE)blue_matrix[i][j];
			fwrite(&rgb, sizeof(rgb), 1, out);
		}
	}
	cout << "I write message" << endl;
	fclose(out);
	return 0;
}

float** Cox:: DCT(float** component) {
	
	float** result = new float*[image_height];
	for (int i = 0; i < image_height; i++) {
		result[i] = new float[image_width];
	}
	double Ck = 0, Cl = 0;
	for (int r = 0; r < image_height; r += N) {
		for (int c = 0; c < image_width; c += N) {
			for (int k = 0; k < N; k++) {
				for (int l = 0; l < N; l++) {
					if (l == 0) {
						Cl = sqrt(1.0 / N);
					}
					else {
						Cl = sqrt(2.0 / N);
					}
					if (k == 0) {
						Ck = sqrt(1.0 / N);
					}
					else {
						Ck = sqrt(2.0 / N);
					}
					double value = 0;
					for (int i = 0; i < N; i++) {
						for (int j = 0; j < N; j++) {
							value += component[r + i][c + j] * cos(((2 * i + 1) * M_PI * k) / (2 * N)) * cos(((2 * j + 1) * M_PI * l) / (2 * N));
						}
					}
					result[r + k][c + l] = round(Ck * Cl * value);
				}
			}
		}
	}
	cout << "DCT is done" << endl;
	return result;
}


float** Cox:: de_DCT(float** component) {

	float** result = new float*[image_height];
	for (int i = 0; i < image_height; i++) {
		result[i] = new float[image_width];
	}
	double Ck = 0, Cl = 0;
	for (int r = 0; r < image_height; r += N) {
		for (int c = 0; c < image_width; c += N) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					double value = 0;
					for (int k = 0; k < N; k++) {
						for (int l = 0; l < N; l++) {
							if (k == 0) {
								Ck = sqrt(1.0 / N);
							}
							else {
								Ck = sqrt(2.0 / N);
							}
							if (l == 0) {
								Cl = sqrt(1.0 / N);
							}
							else {
								Cl = sqrt(2.0 / N);
							}
							value += Ck * Cl * component[r + k][c + l] * cos(((2 * i + 1) * M_PI * k) / (2 * N)) * cos(((2 * j + 1) * M_PI * l) / (2 * N));
						}
					}
					result[r + i][c + j] = round(value);
				}
			}
		}
	}
	cout << "deDCT is done" << endl;
	return result;
}

int Cox::Insert_message(int critic_value, float alpha)
{
	
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	dct_red = DCT(red_matrix);
	dct_green = DCT(green_matrix);
	dct_blue = DCT(blue_matrix);

	bool bit;
	int k = 7;
	int counter_msg = 0;
	int si = 0;
	int j = 1;
	bool not_first = false;
	int max = 0;
	int comp = 0;
	int v = 0;

	for (int i = 0; i < image_height; i++)
	{
		for (j; j < image_width; j++)
		{
			if (dct_red[i][j] > critic_value)
			{
				counter1++;
				if (dct_red[i][j] > max)
				{
					max = dct_red[i][j];
				}
			}
			if (dct_green[i][j] > critic_value)
			{
				counter2++;
			}
			if (dct_blue[i][j] > critic_value)
			{
				counter3++;
			}
		}
		j = 0;
		
	}

	vector<float**> vector_dct;
	vector_dct.push_back(dct_red);
	vector_dct.push_back(dct_green);
	vector_dct.push_back(dct_blue);

	cout << "TOTAL IS red " << counter1 << endl;
	cout << "TOTAL IS green " << counter2 << endl;
	cout << "TOTAL IS blue " << counter3 << endl;
	cout << max << endl;

	for (comp = 0; comp < 3; comp++)
	{
		for (int i = 0; i < image_height; i++)
		{
		
			for (int j = 0; j < image_width; j++)
			{
				if (not_first) {
					if (counter_msg < message_size)
					{
						if (vector_dct[comp][i][j] > critic_value)
						{
							unsigned char msg_char = message[counter_msg];
							bit = (bool((1 << k) & msg_char));
							orig_msg[v] = (int) bit;
							v++;
							if (bit)
							{
								si = 1;
							}
							else
							{
								si = -1;
							}
							vector_dct[0][i][j] = vector_dct[0][i][j] + (alpha * si);
							num_insert_bits++;
							k--;
							if (k < 0)
							{
								k = 7;
								counter_msg++;

							}
						}
					}
				}
				not_first = true;
			}
		}
		if (counter_msg == message_size)
		{
			break;
		}
	}

	vector<float**> orig;
	orig.push_back(red_matrix);
	orig.push_back(green_matrix);
	orig.push_back(blue_matrix);
	cout << "I insert bits "; cout << num_insert_bits << endl;

	red_matrix = de_DCT(vector_dct[0]);
	return 0;
}



int Cox::Get_message(FILE* orig, FILE* chng, string res_file, int critic_value)
{
	string res_string = "";
	bool bits[8];
	int bit_number = 0;
	int simb_value = 0;
	bool param = false;
	int b = 0;
	bool not_first = false;
	int v = 0;

	Read_image(chng);

	float** dct_red_chng = DCT(red_matrix);
	float** dct_green_chng = DCT(green_matrix);
	float** dct_blue_chng = DCT(blue_matrix);

	Read_image(orig);
	Read_image_new(chng);

	float** dct_red_orig = DCT(red_matrix);
	float** dct_green_orig = DCT(green_matrix);
	float** dct_blue_orig = DCT(blue_matrix);

	vector<float**> chng_dct;
	chng_dct.push_back(dct_red_chng);
	chng_dct.push_back(dct_green_chng);
	chng_dct.push_back(dct_blue_chng);

	vector<float**> orig_dct;
	orig_dct.push_back(dct_red_orig);
	orig_dct.push_back(dct_green_orig);
	orig_dct.push_back(dct_blue_orig);

	get_PSNR(red_matrix, red_matrix_new);

	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < image_height; i++)
		{
			for (int j = 0; j < image_width; j++)
			{
				if (not_first) {
					if (orig_dct[k][i][j] > critic_value) {
						b = chng_dct[k][i][j] - orig_dct[k][i][j];
						if (b >= 0)
						{
							bits[bit_number] = 1;
							new_msg[v] = 1;
						}

						else
						{

							bits[bit_number] = 0;
							new_msg[v] = 0;
						}
						v++;
						bit_number++;
						if (bit_number == 8)
						{

							bit_number = 0;
							char liter = Bits_to_byte(bits);
							res_string += liter;
						}

					}
					if (res_string.length() == message_size)
					{
						param = true;
						break;
					}
				}
				
				not_first = true;
				if (param)
				{
					break;
				}
			}
			if (param)
			{
				break;
			}
		}
		if (param)
		{
			break;
		}
	}

	ofstream fout;
	fout.open(res_file);
	fout << res_string;
	fout.close();
	cout << "I get message" << endl;
	int counter_wrong = 0;

	for (int i = 0; i < 2648; i++)
	{
		result[i] = orig_msg[i] + new_msg[i];
		if (result[i] == 1)
		{
			counter_wrong++;
		}
	}
	cout << "Wrong bits is " << counter_wrong << endl;
	return 0;
}


int Cox::Bits_to_byte(bool bits[])
{
	int byte_val = 0;
	int tmp = 0;
	int bit = 0;
	for (int i = 1; i <= 8; i++)
	{
		bit = (int)bits[i - 1];
		if (bit != 0)
		{
			tmp = pow(2 * bit, 8 - i);
			byte_val = byte_val + tmp;
		}

	}
	return byte_val;
}

void Cox::get_PSNR(float** original, float** restored)
{
	float sum = 0;
	float PSNR = 0;
	for (int i = 0; i < image_height; i++)
		for (int j = 0; j < image_width; j++)
			sum = sum + pow(original[i][j] - restored[i][j], 2);

	PSNR = 10 * log10(((image_height * image_width) * pow(255, 2)) / sum);
	PSNR = PSNR + 200;
	cout << "PSNR is " << PSNR << endl;
}