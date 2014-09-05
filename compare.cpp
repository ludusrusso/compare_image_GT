/*
        std::cout << compare.size() << " "<< odom.size() << std::endl;
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
      /* ss >> file >> ID >> x >> y; */
      ss >> file >> x >> y;
      /* std::cout << file << " "<< ID << " "<< x << " "<< y << std::endl; */
      std::cout << file << " "<< " "<< x << " "<< y << std::endl;
    }
    string file;
    /* int ID; */
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

  double max_x = -10000;
  double max_y = -10000;
  double min_x = 10000;
  double min_y = 10000;

  while (!inf.eof()) {
    string line;
    getline(inf, line);
    if (line.size() == 0) break;
    ImgsData im = ImgsData(line);
    imgs.push_back(im);

    if (max_x < im.x) max_x = im.x;
    if (max_y < im.y) max_y= im.y;
    if (min_x > im.x) min_x = im.x;
    if (min_y > im.y) min_y = im.y;
  }

  int tdist = 50;

  Mat img = imread(imgs[0].file);
  Size newsize(scale*img.size().width, scale*img.size().height);

  Mat odom = Mat::zeros(newsize.height*2, newsize.height*2, CV_8UC3);

  for (int i = 0; i < (int)imgs.size(); ++i)
  {
    Point p((imgs[i].x - min_x)*odom.size().height/(max_x-min_x), -(imgs[i].y - max_y)*odom.size().height/(max_y-min_y));
    circle(odom, p, 1, Scalar(255,255,255));
  }


  for (int i = 0; i < (int)imgs.size(); ++i)
  {
    for (int j = 0; j < i - tdist; ++j)
    {
      double dist = sqrt(pow(imgs[i].x - imgs[j].x,2) + pow(imgs[i].y - imgs[j].y,2));
      if (dist < tresh ){
        std::cout << imgs[i].file << " " << imgs[j].file << endl;
        std::cout << "( " <<imgs[i].x<< " , "<< imgs[i].y << " ) -- "<< "( " <<imgs[j].x<< " , "<< imgs[j].y << " ) -- " << dist << std::endl;
        Mat img1, img2;
        img1 = imread(imgs[i].file);
        img2 = imread(imgs[j].file);


        Mat podom(odom.clone());

        Point p1((imgs[i].x - min_x)*odom.size().height/(max_x-min_x), -(imgs[i].y - max_y)*odom.size().height/(max_y-min_y));
        Point p2((imgs[j].x - min_x)*odom.size().height/(max_x-min_x), -(imgs[j].y - max_y)*odom.size().height/(max_y-min_y));
        circle(podom, p1, 5, Scalar(0,0,255), -1);
        circle(podom, p2, 5, Scalar(0,255,0), -1);

        resize(img1, img1, newsize);
        resize(img2, img2, newsize);
        Mat compare;
        vconcat(img1, img2, compare);
        hconcat(compare,podom, compare);
        imshow("Type Y/N", compare);
        char res = waitKey(-1);
        if (res == 'y' || res == 'Y') {
          of << imgs[i].file << " " << imgs[j].file << endl;
        }
      }
    }
  }

  return 0;
}
