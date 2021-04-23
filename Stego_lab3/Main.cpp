#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <fstream>
#include <String>
#include "Cox.h"


using namespace std;

int main(int argc, char* argv[])
{


	Cox hide1;
	FILE* in = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab3\\Stego_lab3\\pepper.bmp", "rb");
	
	hide1.Read_message("Message.txt");
	hide1.Read_image(in);
	hide1.Insert_message(1000, 20);
	hide1.Write_image();
	Sleep(5000);

	FILE* in2 = fopen("C:\\Users\\Ёльдо\\source\\repos\\Stego_lab3\\Stego_lab3\\RES.bmp", "rb");
	hide1.Get_message(in, in2, "RESULT2.txt", 1000);





	getchar();
	return 0;
}