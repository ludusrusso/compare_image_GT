/*
    Copyright (C) 2014 Politecnico di Torino


    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    Contributors:
        Ludovico O. Russo (ludovico.russo@polito.it)
*/



#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

using namespace cv;
using namespace std;


class ImgsData{
  public:
    ImgsData(string sdata) {
      stringstream ss(sdata);
      ss >> file >> ID >> x >> y;
      std::cout << file << " "<< ID << " "<< x << " "<< y << std::endl;
    }
    string file;
    int ID;
    double x, y;
};

int main(int argc, char *argv[])
{
  if (argc < 4) {
    cout << "usage: " << argv[0] << " input_file output_file threashold <scale>"<< endl;
    return -1;
  }

  float scale = 1;
  if (argc == 5) {
    scale = atof(argv[4]);
  }

  double tresh = atof(argv[3]);
  std::cout << "threashowl = " << tresh << std::endl;

  ofstream of(argv[2]);
  ifstream inf(argv[1]);

  std::vector<ImgsData> imgs;

  while (!inf.eof()) {
    string line;
    getline(inf, line);
    if (line.size() == 0) break;
    imgs.push_back(ImgsData(line));
  }


  for (int i = 0; i < (int)imgs.size(); ++i)
  {
    for (int j = 0; j < i; ++j)
    {
      if (sqrt(pow(imgs[i].x - imgs[j].x,2) + pow(imgs[i].y - imgs[j].y,2)) < tresh ){
        Mat img1, img2;
        img1 = imread(imgs[i].file);
        img2 = imread(imgs[j].file);

        Size newsize(scale*img1.size().width, scale*img1.size().height);

        resize(img1, img1, newsize);
        resize(img2, img2, newsize);
        Mat compare;
        vconcat(img1, img2, compare);
        imshow("Type Y/N", compare);
        char res = waitKey(-1);
        if (res == 'y' || res == 'Y') {
          of << imgs[i].ID << " " << imgs[j].ID << endl;
        }
      }
    }
  }

  return 0;
}
