#ifndef _FIREDOOR
#define _FIREDOOR
#include <cv.h>  
#include <highgui.h>  
#include <math.h>  
#include <iostream>
#include <string>
#include <windows.h>
#include <opencv2/opencv.hpp>  
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  
#include "cxcore.h"
#include "PrintUtils.h"

using namespace cv;
using namespace std;

class FireDoor
{
private:
	//

	Mat grayImg;
	Mat HSVImg;
	double calcVAR(Mat target);
	Mat correct(Mat target);
	Mat correctgrayImg();
	Mat correctHSVImg();
	double calcHSV();
	double proportion();
	double count(Mat &topimg, Mat &buttomimg);
	Mat getTopImg();
	Mat getBottomImg();
	int point[8];
	bool status;
	double bv;
	double pro;

public:
	explicit FireDoor(String Add, char** argv);
	~FireDoor();

	int getlocation();
	bool FireDoor::judgeByHSV();

};

#endif  