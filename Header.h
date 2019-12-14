#pragma once
#define NumberOfElementFile 10000

#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <time.h>
#include <bitset>
#include <direct.h>

using namespace std;

struct HuffmanNode
{
	string data;
	HuffmanNode* left = NULL;
	HuffmanNode* right = NULL;
	string binarycode;
	int weightnumber = 0;
};

void GoToXY(int x, int y);
void GoToPrint(int x, int y, string a);
void FontDesige(int x, int y);
void SETConsole();
void BuildHuffmanTree(vector<HuffmanNode*>& tree);
void BuildBinaryCode(HuffmanNode*& tree);
void GeneralProcessingCompressed(string linkout, string link, string name, string tail);
void Decompression(string linkout, string link);
vector<string> get_all_files_names_within_folder(string folder, vector<string>& tail, vector<string>& name);
void Console();