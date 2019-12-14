#include "Header.h"

void GoToXY(int x, int y) // ham chuyen con tro dem vi tri x y tren Console
{
	static HANDLE h = NULL;

	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c;

	c.X = x;
	c.Y = y;

	SetConsoleCursorPosition(h, c);
}

void GoToPrint(int x, int y, string a) // ham di chuyen con tro den vi tri x y tren Console de xuat string a
{
	GoToXY(x, y);

	cout << a;
}

void FontDesige(int x, int y)	// Thay doi kich thuoc chu trong Console cho Laptop // copy
{
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX font = new CONSOLE_FONT_INFOEX();
	font->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(cons, 0, font);
	font->dwFontSize.X = x;
	font->dwFontSize.Y = y;
	//font->FontWeight = 22;
	//font->FontFamily = FF_DECORATIVE;
	SetCurrentConsoleFontEx(cons, 0, font);
}

void SETConsole()// Ham dung de ve Giao dien tren Console
{
	system("CLS");

	for (int i = 0; i <= 119; i += 119)
	{
		for (int j = 0; j <= 29; j++)
		{
			GoToPrint(i, j, "|");
		}
	}

	for (int i = 0; i <= 29; i += 29)
	{
		for (int j = 0; j <= 119; j++)
		{
			GoToPrint(j, i, "-");
		}
	}

	FontDesige(22, 23);

	GoToPrint(52, 1, "UNG DUNG NEN");

	GoToPrint(46, 3, "* * * * * * * * * * * * *");

	GoToPrint(95, 28, "HOC, HOC NUA, HOC MAI < 3");
}

void BuildBinaryCode(HuffmanNode*& tree) // Xay dung ma huff cho tung node tree: cay huffman khi duoc build chi con 1 nut goc
{
	if (tree->left != NULL)
	{
		tree->left->binarycode.clear();
		for (int i = 0; i < int(tree->binarycode.size()); i++)
		{
			tree->left->binarycode.push_back(tree->binarycode[i]);
		}
		tree->left->binarycode.push_back('0'); // trai thi them 0 vao binarycode
		BuildBinaryCode(tree->left);
	}
	else
	{
		return;
	}

	if (tree->right != NULL)
	{
		tree->right->binarycode.clear();
		for (int i = 0; i < int(tree->binarycode.size()); i++)
		{
			tree->right->binarycode.push_back(tree->binarycode[i]);
		}
		tree->right->binarycode.push_back('1'); // phai thi them 1 vao binarycode
		BuildBinaryCode(tree->right);
	}
	else
	{
		return;
	}
}

void BuildHuffmanTree(vector<HuffmanNode*>& tree)
{
	while (tree.size() > 1)
	{
		int min1 = 0;
		for (int i = 1; i < int(tree.size()); i++) // xay dung cay huff man tu rung node trong so va ki tu
		{
			if (tree[i]->weightnumber < tree[min1]->weightnumber)
			{
				min1 = i;
			}
		}
		int min2 = 0;
		if (min1 == tree.size() - 1)
		{
			min2 = min1 - 1;
		}
		else
		{
			min2 = min1 + 1;
		}
		for (int i = 0; i < int(tree.size()); i++)
		{
			if (i != min1 && tree[i]->weightnumber < tree[min2]->weightnumber)
			{
				min2 = i;
			}
		}
		HuffmanNode* newnode = new HuffmanNode;
		newnode->left = tree[min2];
		newnode->right = tree[min1];
		newnode->data.clear();
		for (int i = 0; i < int(tree[min1]->data.size()); i++)
		{
			newnode->data.push_back(tree[min1]->data[i]);
		}
		for (int i = 0; i < int(tree[min2]->data.size()); i++)
		{
			newnode->data.push_back(tree[min2]->data[i]);
		}
		newnode->weightnumber = tree[min1]->weightnumber + tree[min2]->weightnumber;
		if (min1 < min2)
		{
			tree.erase(tree.begin() + min1);
			tree.erase(tree.begin() + min2 - 1);
		}
		else
		{
			tree.erase(tree.begin() + min1);
			tree.erase(tree.begin() + min2);
		}
		tree.push_back(newnode);
	}

	BuildBinaryCode(tree[0]);
}

void GeneralProcessingCompressed(string linkout, string link, string name, string tail)
{
	vector<HuffmanNode*> HuffTree; HuffTree.clear();
	vector<HuffmanNode*> vecHuffcoding; vecHuffcoding.clear();
	vector<string> charhuffcode(256);

	int arr[256]; // doc file cap nhat trong so
	for (int i = 0; i < 256; i++)
	{
		arr[i] = 0;
	}
	ifstream op;
	op.open(link, ios::binary);
	char readfile[NumberOfElementFile];

	while (op.read(readfile, NumberOfElementFile))
	{
		for (int i = 0; i < NumberOfElementFile; i++)
		{
			if (int(readfile[i]) < 0)
			{
				arr[int(readfile[i]) + 256]++;
			}
			else
			{
				arr[readfile[i]]++;
			}
		}
	}

	for (int i = 0; i < op.gcount(); i++)
	{
		if (int(readfile[i]) < 0)
		{
			arr[int(readfile[i]) + 256]++;
		}
		else
		{
			arr[readfile[i]]++;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (arr[i] != 0)
		{
			HuffmanNode* tmp = new HuffmanNode;
			tmp->weightnumber = arr[i]; tmp->data.clear();
			tmp->data.push_back(i);
			HuffTree.push_back(tmp);
			vecHuffcoding.push_back(tmp); // xay dung rung cay 
		}
	}

	int numbervecHuff = int(vecHuffcoding.size());

	BuildHuffmanTree(HuffTree); // build cay
	for (int i = 0; i < numbervecHuff; i++)
	{
		if (int(vecHuffcoding[i]->data[0]) < 0)
		{
			charhuffcode[int(vecHuffcoding[i]->data[0]) + 256] = vecHuffcoding[i]->binarycode;
		}
		else
		{
			charhuffcode[int(vecHuffcoding[i]->data[0])] = vecHuffcoding[i]->binarycode;
		}
	}

	op.clear();
	op.seekg(0, ios::beg);
	ofstream outfile(linkout + name + "." + tail + "compressed", ios::binary); // link xuat ra
	outfile << " " << endl << numbervecHuff << endl;
	for (int i = 0; i < numbervecHuff; i++)
	{
		outfile << vecHuffcoding[i]->data;
		outfile << vecHuffcoding[i]->weightnumber;
		outfile << "\n";
	}

	string bit_buffer = "";
	while (op.read(readfile, NumberOfElementFile))
	{
		for (int j = 0; j < NumberOfElementFile; j++)
		{
			if (int(readfile[j]) < 0)
			{
				bit_buffer += charhuffcode[int(readfile[j]) + 256];
			}
			else
			{
				bit_buffer += charhuffcode[int(readfile[j])];
			}
		}
		for (int i = 0; i < int(bit_buffer.length() - bit_buffer.length() % 8); i += 8)
		{
			int dec_value = 0;
			int data = 1;
			for (int j = i + 7; j >= i; j--)
			{
				if (bit_buffer[j] == '1') dec_value += data;
				data = data * 2;

			}
			outfile << char(dec_value);
		}
		bit_buffer = bit_buffer.substr(bit_buffer.length() - bit_buffer.length() % 8, bit_buffer.length());
	}
	for (int j = 0; j < op.gcount(); j++)
	{
		if (int(readfile[j]) < 0)
		{
			bit_buffer += charhuffcode[int(readfile[j]) + 256];
		}
		else
		{
			bit_buffer += charhuffcode[int(readfile[j])];
		}
	}
	int end_bit = 8 - bit_buffer.length() % 8;
	for (int i = 0; i < end_bit; i++)
	{
		bit_buffer += '0';
	}
	for (int i = 0; i < int(bit_buffer.length()); i += 8)
	{
		int dec_value = 0;
		int data = 1;
		for (int j = i + 7; j >= i; j--)
		{
			if (bit_buffer[j] == '1') dec_value += data;
			data = data * 2;

		}
		outfile << char(dec_value);
	}
	outfile.clear();
	outfile.seekp(0, ios::beg);
	outfile << end_bit;
	op.close();
	outfile.close();
}

void Decompression(string linkout, string link) // giai nen
{
	ifstream op;
	op.open(link, ios::binary);

	int numberbitend = 0;
	op >> numberbitend;
	int numbervecHuff = 0;
	op >> numbervecHuff;

	vector<HuffmanNode*>HuffTree;
	vector<HuffmanNode*>vecHuffcoding;

	char* des = new char[2];
	op.read(des, 1);//doc tung ki tu de xay dung lai cay huff man
	for (int i = 0; i < numbervecHuff; i++)
	{
		char des2;
		op.read(&des2, 1);
		HuffmanNode* tmp = new HuffmanNode;
		tmp->data = des2;
		op >> tmp->weightnumber;
		op.read(des, 1);
		vecHuffcoding.push_back(tmp);
		HuffTree.push_back(tmp);
	}

	BuildHuffmanTree(HuffTree);

	string name;
	for (int i = int(link.size() - 1); i >= 0; i--)
	{
		if (link[i] == '/')
		{
			string tmp(link.begin() + i + 1, link.end() - 10); // link xuat file
			name += tmp;
			break;
		}
	}

	string bit; bit.clear();
	char readfile[NumberOfElementFile]; // doc buffer phan tu luu vao char
	HuffmanNode* decompressed = HuffTree[0];
	ofstream outfile(linkout + name + "decompressed", ios::binary);
	while (op.read(readfile, NumberOfElementFile))
	{

		for (int i = 0; i < NumberOfElementFile; i++)
		{
			bitset<8> x(readfile[i]); // lay tung 8 bit
			bit += x.to_string(); // luu mang
		}
		for (int i = 0; i < int(bit.length()); i++)
		{
			if (bit[i] == '0')
				decompressed = decompressed->left;
			else
				decompressed = decompressed->right;
			if (decompressed->left == NULL && decompressed->right == NULL)
			{
				outfile << decompressed->data;
				decompressed = HuffTree[0]; // tach chay theo cay dem khi tim duoc ki tu
			}
		}
		bit = "";
	}
	for (int i = 0; i < op.gcount(); i++)
	{
		bitset<8> x(readfile[i]);
		bit += x.to_string();
	}
	bit = bit.substr(0, bit.length() - numberbitend);
	for (int i = 0; i < int(bit.length()); i++)
	{
		if (bit[i] == '0')
			decompressed = decompressed->left;
		else
			decompressed = decompressed->right;
		if (decompressed->left == NULL && decompressed->right == NULL)
		{
			outfile << decompressed->data;
			decompressed = HuffTree[0];
		}
	}
	op.close();
	outfile.close();
}
// lay ten tung file kem link truy cap
vector<string> get_all_files_names_within_folder(string folder, vector<string>& tail, vector<string>& name) //https://www.it-swarm.net/vi/c%2B%2B/lam-cach-nao-toi-co-nhan-danh-sach-cac-tep-trong-thu-muc-bang-c-hoac-c/957770349/
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do 
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(folder+'/');
				names[names.size() - 1] += fd.cFileName;

				int flat = -1;
				for (int i = int(names[names.size() - 1].size() - 1); i >= 0; i--)
				{
					if (names[names.size() - 1][i] == '.' && flat == -1)
					{
						string tmp(names[names.size() - 1].begin() + i + 1, names[names.size() - 1].end());
						tail.push_back(tmp);
						flat = i;
					}

					if (flat != -1 && names[names.size() - 1][i] == '/')
					{
						string tmp1(names[names.size() - 1].begin() + i + 1, names[names.size() - 1].begin() + flat);
						name.push_back(tmp1);
						break;
					}
				}
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}
void Console()
{
	system("clear");
	SETConsole();

	GoToPrint(50, 11, "1: Compress file.");
	GoToPrint(50, 13, "2: Compress folder.");
	GoToPrint(45, 9, "What do you want compressed? "); int wantdo(0); cin >> wantdo;

	if (wantdo == 2)
	{
		GoToPrint(19,15, "Link: ");
		string link; cin.ignore();
		getline(cin, link);
		string foldername;
		for (int i = int(link.size() - 1); i >= 0; i--)
		{
			if (link[i] == '/')
			{
				string tmp(link.begin() + i + 1, link.end());
				string tmp1(link.begin(), link.begin() + i + 1);
				foldername += tmp;
				break;
			}
		}

		vector<string> vecfilename;
		vector<string> tail;
		vector<string> name;
		vecfilename = get_all_files_names_within_folder(link,tail,name);

		int successcomfolder = _mkdir((link + "Compressed").c_str()); // tach ten tao folder

		string linkfoldercompressed = link + "Compressed/";
		for (int i = 0; i < int(vecfilename.size()); i++)
		{
			GeneralProcessingCompressed(linkfoldercompressed,vecfilename[i],name[i],tail[i]);
		}

		GoToPrint(19, 17, "Do you want decompress?(0: No, 1: Yes): "); int decom(0); cin >> decom;
		if (decom)
		{
			int successdecomfolder = _mkdir((link + "Decompressed").c_str()); // tach ten tao folder
			string linkfolderdecompressed = link + "Decompressed/";
			vecfilename = get_all_files_names_within_folder(link + "Compressed", tail, name);
			for (int i = 0; i < int(vecfilename.size()); i++)
			{
				Decompression(linkfolderdecompressed, vecfilename[i]);
			}
		}

		GoToPrint(19, 19, "DONE!");
		GoToXY(1, 30);
		system("Pause");
		return Console();
	}
	if (wantdo == 1)
	{
		GoToPrint(19, 15, "Link: ");
		string link; cin.ignore();
		getline(cin, link);
		string name;
		string tail;
		string linkout;
		int flat = -1;
		for (int i = int(link.size() - 1); i >= 0; i--)
		{
			if (link[i] == '.' && flat == -1)
			{
				string tmp(link.begin() + i + 1, link.end());
				tail = tmp;
				flat = i;
			}

			if (flat != -1 && link[i] == '/')
			{
				string tmp1(link.begin() + i + 1, link.begin() + flat);
				name = tmp1;
				string tmp2(link.begin(), link.begin() + i + 1);
				linkout = tmp2;
				break;
			}
		}
		GeneralProcessingCompressed(linkout, link, name, tail);

		GoToPrint(19, 17, "Do you want decompress?(0: No, 1: Yes): "); int decom(0); cin >> decom;
		if (decom)
		{
			Decompression(linkout, linkout + name + "." + tail + "compressed");
		}
		GoToPrint(19, 19, "DONE!");
		GoToXY(1, 30);
		system("Pause");
		return Console();
	}
}

int main()
{
	Console();
}