#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "myRdWtIm.h"
using namespace std;
using namespace cv;

int main()
{
	//Mat img = imread("95b5d6eb00d1cd3e2cad228877e56668_263_²©Ê¿±¦±¦.jpg");

	//Mat imgGray;
	//cvtColor(img, imgGray, CV_BGR2GRAY);
	//imwrite("95b5d6eb00d1cd3e2cad228877e56668_263_²©Ê¿±¦±¦.bmp", imgGray);

	int width = 0;  
	int height = 0;  
	int lineByte = 0;  

	RdWtIm *rdWtIm = new RdWtIm();

	char* imData = rdWtIm->rd8BitBmpFmt("95b5d6eb00d1cd3e2cad228877e56668_263_²©Ê¿±¦±¦.bmp", &width, &height);  
	lineByte = (width + 3)/4*4; //¸ñÊ½¿í¶È  
	printf("width = %d\n", width);  
	printf("height = %d\n", height);  
	printf("lineByte = %d\n", lineByte);  

	Mat iMat(height, width, CV_8UC1, imData, lineByte);  
	imwrite("q.bmp", iMat);
}

  