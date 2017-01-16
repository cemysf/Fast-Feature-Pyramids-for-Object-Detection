#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <cstring>

#ifdef WINDOWS
#include <Windows.h>
#include <direct.h>

#else
#include <dirent.h>

#endif


#include <core/core.hpp> 	//<opencv2/core/core.hpp>
#include <highgui/highgui.hpp> 	//<opencv2/highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>	//<opencv2/imgproc/imgproc.hpp>

#define patch_row 160
#define patch_col 96
#define crop_per_image 3 //100
#define negative_image_dir "/home/cemysf/datasets/INRIAPerson/Train/neg"		
#define save_dir "./negatives"	//"/home/cemysf/datasets/INRIAPerson/train_64x128_H96/neg"		

using namespace std;
using namespace cv;


vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
#ifdef WINDOWS
	char search_path[1000];
	sprintf_s(search_path, 1000, "%s*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
#else
	DIR *dpdf;
	struct dirent *epdf;
	dpdf = opendir(folder.c_str());
	if (dpdf != NULL){
		while (epdf = readdir(dpdf)){
			if(strncmp(epdf->d_name, ".", 1)!=0 && strncmp(epdf->d_name, "..", 2)!=0){
				string filename = "/" ;
				filename += epdf->d_name;
				names.push_back(filename);
			}
			//cout << epdf->d_name << endl;
		}
	}
#endif
	return names;
}


int main()
{
	stringstream ss;
	vector<string> file_list;
	int count = 0;
	
	srand(time(NULL));

	cout << "NEGATIVE image parsing is in processing..." << endl;

	

	ss << save_dir;
//	cout << "ss:" << ss.str() << endl;
	file_list = get_all_files_names_within_folder(ss.str());
//	cout << file_list.front() << endl;
	
	ss.str("");
	ss.clear();

	for (int i = 0; i < file_list.size(); i++)
	{
		ss << save_dir << file_list[i];
		cout << "remove " << ss.str().c_str() << endl;
		//remove(ss.str().c_str());		//delete all files in save_dir!!!
		ss.str("");
		ss.clear();
	}

	file_list.clear();
	ss.str("");
	ss.clear();
	



	ss << negative_image_dir;

	file_list = get_all_files_names_within_folder(ss.str());
	cout << file_list.front() << endl;
	ss.str("");
	ss.clear();


	for (int i = 0; i < file_list.size(); i++)
	{
		string file_name = file_list.at(i);
		
		ss << negative_image_dir << file_name;
		cout << "imread " << ss.str().c_str() << endl;
	


		Mat image;
		image = imread(ss.str());
		
		ss.str("");
		ss.clear();

		vector<int>rand_col;
		vector<int>rand_row;
		int size = 0;

		for (int negative_number = 0; negative_number < crop_per_image; negative_number++)
		{
			
			bool pass = true;
			
			int col_index = rand() % (image.cols - patch_col);
			int row_index = rand() % (image.rows - patch_row);

			for (int j = 0; j < size; j++)
			{
				if (col_index == rand_col.at(j) && row_index == rand_row.at(j))
					pass = false;
			}

			if (!pass)
				negative_number--;
			
			else
			{
				rand_col.push_back(col_index);
				rand_row.push_back(row_index);
				size++;
			}

		}

		
		for (int j = 0; j < crop_per_image; j++)
		{
			Mat crop;
			crop = image(Rect(rand_col.at(j), rand_row.at(j), patch_col, patch_row));

			ss << save_dir << "/" << count << ".jpg";
			cout << "imwrite " << ss.str() << endl;
			count++;

			imwrite(ss.str(), crop);

			ss.str("");
			ss.clear();
		}

	}
	//

	cout << "Done!" << endl;
	return 0;
}
