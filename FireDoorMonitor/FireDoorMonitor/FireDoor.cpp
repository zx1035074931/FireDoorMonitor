#include "FireDoor.h"

FireDoor::FireDoor(String Add, char** argv){
	String Address = Add;
	/*Mat changesizegrayImg;
	Mat origrayImg = imread(Address.c_str(), 0);
	resize(origrayImg, changesizegrayImg, Size(1920, 1080));
	grayImg = changesizegrayImg;*/

	Mat changesizeRGBImg;
	Mat oriRGBImg = imread(Address.c_str());
	resize(oriRGBImg, HSVImg, Size(1920, 1080));
	
	Mat origrayImg;
	Mat changesizegrayImg;
	cvtColor(oriRGBImg, origrayImg, COLOR_BGR2GRAY);
	resize(origrayImg, changesizegrayImg, Size(1920, 1080));
	grayImg = changesizegrayImg; 

	status = false;
	for(size_t i = 1, n = 0; i != 9; ++i, ++n){
		point[n] = atoi(*(argv + i));
	}

	bv = calcVAR(correctgrayImg());
	pro = proportion();
	cout << "亮度方差为：" << bv << endl;   
	cout << "比例为：" << pro << endl;

}

FireDoor::~FireDoor() {}

//
//截取灰度图片
Mat FireDoor::correctgrayImg()
{
	Mat correctgrayImage = correct(grayImg);
	Mat fingrayImg(correctgrayImage, Rect(126, 95, 550, 405));
	imshow("gray", fingrayImg);
	return fingrayImg;
}

//截取HSV图片
Mat FireDoor::correctHSVImg()
{
	Mat correctHSVImage = correct(HSVImg);
	Mat imgCut(correctHSVImage, Rect(126, 95, 550, 405));
	Mat finHSVImg(imgCut, Rect(0, 395, 550, 10));
	cvtColor(finHSVImg, HSVImg, COLOR_BGR2HSV);
	return HSVImg;
}

//计算HSV方差均值
double FireDoor::calcHSV()
{
	double Hvar;
	double Svar;
	double Vvar;
	double ave;
	vector<Mat> channels;
	split(correctHSVImg(), channels);   //分离通道
	Mat H = channels.at(0);
	Mat S = channels.at(1);
	Mat V = channels.at(2);
	Hvar = calcVAR(H);
	Svar = calcVAR(S);
	Vvar = calcVAR(V);
	ave = (Hvar + Svar + Vvar) / 3;
	cout << "H色调通道方差为：" << Hvar << endl;
	cout << "S饱和度通道方差为：" << Svar << endl;
	cout << "V亮度通道方差为：" << Vvar << endl;
	return ave;
}

//纠正图片
Mat FireDoor::correct(Mat target) 
{
	Mat tempImg = target;
	resize(tempImg, tempImg, Size(800, 600));
	float rowProportion = 800.0f / 1920.0f;
	float colProportion = 600.0f / 1080.0f;
	//标记800×600分辨率下的消防门的四个角点
	vector<Point2f> cornersCoor;
	cornersCoor.push_back(Point2f(point[0] * rowProportion, point[1] * colProportion));
	cornersCoor.push_back(Point2f(point[2] * rowProportion, point[3] * colProportion));
	cornersCoor.push_back(Point2f(point[4] * rowProportion, point[5] * colProportion));
	cornersCoor.push_back(Point2f(point[6] * rowProportion, point[7] * colProportion));
	//四个角点对应要校正的位置
	vector<Point2f> cornersCoor_trans;
	cornersCoor_trans.push_back(Point2f(100, 100));
	cornersCoor_trans.push_back(Point2f(700, 100));
	cornersCoor_trans.push_back(Point2f(100, 500));
	cornersCoor_trans.push_back(Point2f(700, 500));
	//图像矫正的映射关系
	Mat transform = getPerspectiveTransform(cornersCoor, cornersCoor_trans);
	Mat resultImg;
	warpPerspective(tempImg, resultImg, transform, tempImg.size());
	return resultImg;
}

//获取图片所在版块
int FireDoor::getlocation()
{
	int location = 0;
	if (bv >= 20 || pro < 0.6)
		location = 1;
	if ((bv > 10 && bv < 20 && pro >= 0.6) || (bv <= 10 && pro >= 0.6&&pro < 0.9))
		location = 2;
	if (bv <= 10 && pro >= 0.9)
		location = 3;
	return location;
}

bool FireDoor::judgeByHSV()
{
	logging("开始执行HSV检测", false);
	double HSVave = calcHSV();
	logging("HSV变化值为 " + to_string((long double)HSVave), false);
	status = ((HSVave > 8) ? true : status);
	return status;
}
//
Mat FireDoor::getTopImg() 
{
	Mat topImg(grayImg, Rect(126, 480, 550, 40));
	imshow("topImg", topImg);
	return topImg;
}
Mat FireDoor::getBottomImg() 
{
	Mat bottomImg(grayImg, Rect(126, 490, 550, 20));
	imshow("bottomImg", bottomImg);
	return bottomImg;
}

//比例
double FireDoor::proportion() 
{
	//logging("开始执行点数检测", false);
	//重新设置图片大小
	Mat gray1;
	Mat gray2;
	Mat gray3;
	resize(grayImg, gray1, Size(800, 600));
	//中值滤波
	medianBlur(gray1, gray2, 7);
	//进行一次canny边缘检测
	Canny(gray2, gray3, 30, 50, 3);
	//图像矫正
	grayImg = correct(gray3);
	//计算区域内前景点的比值
	double value = count(getTopImg(), getBottomImg());
	return value;
}

double FireDoor::count(Mat &topimg, Mat &buttomimg) {
	//迭代器统计各点数
	double pointnum0 = 0, pointnum1 = 0;
	for (MatIterator_<uchar> grayit = topimg.begin<uchar>(), grayend = topimg.end<uchar>(); grayit != grayend; ++grayit) {
		if (*grayit > 0)
			++pointnum0;
	}
	for (MatIterator_<uchar> grayit = buttomimg.begin<uchar>(), grayend = buttomimg.end<uchar>(); grayit != grayend; ++grayit) {
		if (*grayit > 0)
			++pointnum1;
	}
	return pointnum1 / pointnum0;
}

//计算方差
double FireDoor::calcVAR(Mat target) 
{
	//协方差矩阵，均值矩阵
	Mat covar, mean1;
	calcCovarMatrix(target, covar, mean1, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	//均值，标准差
	Mat mean2, stdDev;
	meanStdDev(mean1, mean2, stdDev);
	//返回方差
	return stdDev.at<double>(0, 0);
}



