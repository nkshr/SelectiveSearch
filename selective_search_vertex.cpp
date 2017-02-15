#include <vector>
#include <map>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/segmentation.hpp>
using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

#include "selective_search.h"

SelectiveSearch::Vertex::Vertex(const int _index, SelectiveSearch * ss): is_alive(true), size(0), index(_index), base0(-1), base1(-1), top_left(INT_MAX, INT_MAX), bottom_right(0, 0), ss(ss){
}

void SelectiveSearch::Vertex::init(){
  calcColHist();
  calcTexHist();
  size = gs_pts.size();
  calcRegion();
  gs_pts_set.push_back(&gs_pts);
}

void SelectiveSearch::Vertex::calcColHist(){
  DMsg dmsg("caclColHist");
  col_hist.resize(3);
  for(int i = 0; i < col_hist.size(); ++i)
    col_hist[i].resize(25, 0.f);

  float istep = 25.f / 256.f;
  for(int i = 0; i < gs_pts.size(); ++i){
    for(int j = 0; j < 3; ++j){
      const uchar * p = ss->planes[j].ptr<uchar>(0);
      int bin = floor(p[gs_pts[i]] * istep);
      if(!(bin < 25)){
	cout << "bin : " <<  bin << endl;
	exit(1);
      }
      col_hist[j][bin] += 1.0;
    }
  }

  //normalize color histgram
  float isize = 1.f/(float)(gs_pts.size()*3.f);
  for(int i = 0; i < col_hist.size(); ++i){
    for(int j = 0; j < col_hist[i].size(); ++j){
      col_hist[i][j] *= isize;
      CV_Assert(col_hist[i][j] <= 1.0);
    }
  }
}

void SelectiveSearch::Vertex::calcTexHist()
{
  DMsg dmsg("calcTexHist");
  tex_hist.resize(3);
  for(int i = 0; i < tex_hist.size(); ++i){
    tex_hist[i].resize(8);
    for(int j = 0; j < tex_hist[i].size(); ++j){
      tex_hist[i][j].resize(10, 0.f);
    }
  }
 
  float istep_col = 10.f / 256.f;
  float istep_ori = (8.f ) / (CV_PI+FLT_EPSILON);
  for(int i = 0; i < gs_pts.size(); ++i){
    int pt = gs_pts[i];
    
    for(int j = 0; j < 3; ++j){
      const uchar * pimg = ss->planes[j].ptr<uchar>(0);
      const float * pori_img = ss->ori_imgs[j].ptr<float>(0);
      float tmp = pori_img[pt] * istep_ori;
      int ori_bin;
      if(tmp < 0)
	ori_bin = 4.f + floor(tmp);
      else
	ori_bin = floor(tmp);
      if(!(ori_bin < 8)){
	cout << "ori_bin : " << ori_bin << endl;
	exit(1);
      }
      
      int col_bin = floor(pimg[pt] * istep_col);
      if(!(col_bin < 10)){
	cout << "col_bin : " << col_bin << endl;
	exit(1);
      }

      tex_hist[j][ori_bin][col_bin] += 1.0;
    }
  }
 
  float isize = 1.f/((float)gs_pts.size()*3);
  for(int i = 0; i < tex_hist.size(); ++i){
    for(int j = 0; j < tex_hist[i].size(); ++j){
      for(int k = 0; k < tex_hist[i][j].size(); ++k){
	tex_hist[i][j][k] *= isize;
	CV_Assert(tex_hist[i][j][k] <= 1.f);
      }
    }
  }
  CV_Assert(tex_hist.size() < 100000);
}

void SelectiveSearch::Vertex::calcRegion(){
  DMsg dmsg("calcRegion");
  region = Rect(top_left, Point(bottom_right.x + 1, bottom_right.y + 1));
}
