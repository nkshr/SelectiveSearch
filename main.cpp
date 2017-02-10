#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include "selective_search.h"

int main(int argc, char ** argv){
  if(argc != 2){
    cout << "Invalid arguments" << endl;
    cout << "Usage ./ss <imgae>" << endl;
    return 1;
  }
  
  const char * img_name = argv[1];
  Mat img = imread(img_name);
  if(img.empty()){
    cout << img_name << "couldn't be loaded" << endl;
    return 1;
  }
  SelectiveSearch ss;
  ss.setColSimWeight(1.f);
  ss.setTexSimWeight(1.f);
  ss.setSizeSimWeight(1.f);
  ss.setMaxNumVertexs(10000);
  ss.processImage(img);

  return 0;
}
