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
	cout << "���ȷ���Ϊ��" << bv << endl;   
	cout << "����Ϊ��" << pro << endl;

}

FireDoor::~FireDoor() {}

//
//��ȡ�Ҷ�ͼƬ
Mat FireDoor::correctgrayImg()
{
	Mat correctgrayImage = correct(grayImg);
	Mat fingrayImg(correctgrayImage, Rect(126, 95, 550, 405));
	imshow("gray", fingrayImg);
	return fingrayImg;
}

//��ȡHSVͼƬ
Mat FireDoor::correctHSVImg()
{
	Mat correctHSVImage = correct(HSVImg);
	Mat imgCut(correctHSVImage, Rect(126, 95, 550, 405));
	Mat finHSVImg(imgCut, Rect(0, 395, 550, 10));
	cvtColor(finHSVImg, HSVImg, COLOR_BGR2HSV);
	return HSVImg;
}

//����HSV�����ֵ
double FireDoor::calcHSV()
{
	double Hvar;
	double Svar;
	double Vvar;
	double ave;
	vector<Mat> channels;
	split(correctHSVImg(), channels);   //����ͨ��
	Mat H = channels.at(0);
	Mat S = channels.at(1);
	Mat V = channels.at(2);
	Hvar = calcVAR(H);
	Svar = calcVAR(S);
	Vvar = calcVAR(V);
	ave = (Hvar + Svar + Vvar) / 3;
	cout << "Hɫ��ͨ������Ϊ��" << Hvar << endl;
	cout << "S���Ͷ�ͨ������Ϊ��" << Svar << endl;
	cout << "V����ͨ������Ϊ��" << Vvar << endl;
	return ave;
}

//����ͼƬ
Mat FireDoor::correct(Mat target) 
{
	Mat tempImg = target;
	resize(tempImg, tempImg, Size(800, 600));
	float rowProportion = 800.0f / 1920.0f;
	float colProportion = 600.0f / 1080.0f;
	//���800��600�ֱ����µ������ŵ��ĸ��ǵ�
	vector<Point2f> cornersCoor;
	cornersCoor.push_back(Point2f(point[0] * rowProportion, point[1] * colProportion));
	cornersCoor.push_back(Point2f(point[2] * rowProportion, point[3] * colProportion));
	cornersCoor.push_back(Point2f(point[4] * rowProportion, point[5] * colProportion));
	cornersCoor.push_back(Point2f(point[6] * rowProportion, point[7] * colProportion));
	//�ĸ��ǵ��ӦҪУ����λ��
	vector<Point2f> cornersCoor_trans;
	cornersCoor_trans.push_back(Point2f(100, 100));
	cornersCoor_trans.push_back(Point2f(700, 100));
	cornersCoor_trans.push_back(Point2f(100, 500));
	cornersCoor_trans.push_back(Point2f(700, 500));
	//ͼ�������ӳ���ϵ
	Mat transform = getPerspectiveTransform(cornersCoor, cornersCoor_trans);
	Mat resultImg;
	warpPerspective(tempImg, resultImg, transform, tempImg.size());
	return resultImg;
}

//��ȡͼƬ���ڰ��
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
	logging("��ʼִ��HSV���", false);
	double HSVave = calcHSV();
	logging("HSV�仯ֵΪ " + to_string((long double)HSVave), false);
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

//����
double FireDoor::proportion() 
{
	//logging("��ʼִ�е������", false);
	//��������ͼƬ��С
	Mat gray1;
	Mat gray2;
	Mat gray3;
	resize(grayImg, gray1, Size(800, 600));
	//��ֵ�˲�
	medianBlur(gray1, gray2, 7);
	//����һ��canny��Ե���
	Canny(gray2, gray3, 30, 50, 3);
	//ͼ�����
	grayImg = correct(gray3);
	//����������ǰ����ı�ֵ
	double value = count(getTopImg(), getBottomImg());
	return value;
}

double FireDoor::count(Mat &topimg, Mat &buttomimg) {
	//������ͳ�Ƹ�����
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

//���㷽��
double FireDoor::calcVAR(Mat target) 
{
	//Э������󣬾�ֵ����
	Mat covar, mean1;
	calcCovarMatrix(target, covar, mean1, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	//��ֵ����׼��
	Mat mean2, stdDev;
	meanStdDev(mean1, mean2, stdDev);
	//���ط���
	return stdDev.at<double>(0, 0);
}



