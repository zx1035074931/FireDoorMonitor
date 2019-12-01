#include <iostream>
#include "FireDoor.h"
#include "PrintUtils.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include "cxcore.h"

using namespace cv;


using namespace std;

int main(int argc, char* argv[]){

		string imageAdd("F:\\��ѧ����\\ʵ����Ŀ\\RZ002-����ͼ����������Ű�ȫ����ʶ��\\3�������ز�\\ʵ��ͼƬSamples\\G01\\2.JPG"); // ͼƬ�ڵ����еľ��Ե�ַ

		string Msg = "   " ;
		logging(Msg, 1);
		string ks1 = "��ʼִ��ͼƬ";
		string ks2 = "�����ſ��ϼ��";
		string startMsg = ks1 + imageAdd.c_str()+ ks2;
		logging(startMsg, 0);
		cout<<endl;
		cout<<endl;
		cout<<endl;
		//��ʼ��class
		FireDoor fireDoor = FireDoor(imageAdd, argv);
		
		//�ж�
		cout << "ͼƬλ�ڵ�" << fireDoor.getlocation() << "���" << endl;
		if (fireDoor.getlocation() == 1)
			warning("�����Ŵ��ڴ�״̬������", true);
		else if (fireDoor.getlocation() == 3)
			warning("�����Ŵ��ڹر�״̬������", true);
		else if (fireDoor.getlocation() == 2)
		{
			if (fireDoor.judgeByHSV())
				warning("�����Ŵ��ڴ�״̬������", true);
			else
				warning("�����Ŵ��ڹر�״̬������", true);
		}
		
		cvWaitKey(0);
	return 0;
}