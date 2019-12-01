#include <iostream>
#include "FireDoor.h"
#include "PrintUtils.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include "cxcore.h"

using namespace cv;


using namespace std;

int main(int argc, char* argv[]){

		string imageAdd("F:\\大学工作\\实验项目\\RZ002-基于图像处理的消防门安全隐患识别\\3数据与素材\\实验图片Samples\\G01\\2.JPG"); // 图片在电脑中的绝对地址

		string Msg = "   " ;
		logging(Msg, 1);
		string ks1 = "开始执行图片";
		string ks2 = "消防门开合检测";
		string startMsg = ks1 + imageAdd.c_str()+ ks2;
		logging(startMsg, 0);
		cout<<endl;
		cout<<endl;
		cout<<endl;
		//初始化class
		FireDoor fireDoor = FireDoor(imageAdd, argv);
		
		//判定
		cout << "图片位于第" << fireDoor.getlocation() << "板块" << endl;
		if (fireDoor.getlocation() == 1)
			warning("消防门处于打开状态！！！", true);
		else if (fireDoor.getlocation() == 3)
			warning("消防门处于关闭状态！！！", true);
		else if (fireDoor.getlocation() == 2)
		{
			if (fireDoor.judgeByHSV())
				warning("消防门处于打开状态！！！", true);
			else
				warning("消防门处于关闭状态！！！", true);
		}
		
		cvWaitKey(0);
	return 0;
}