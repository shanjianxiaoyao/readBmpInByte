/*  
* 文件名称：myRdWtIm.h  
* 摘    要：读写BMP图像。“格式数据”指直接从BMP读取的或能够直接写入BMP的数据， 
* 其宽度满足BMP格式要求，即满足4的倍数。“有效数据”指实际图像数据，其宽度不一定 
*  满足BMP格式要求。 
*/  

#include "myRdWtIm.h"  

RdWtIm::RdWtIm()  
{  

}  

RdWtIm::~RdWtIm()  
{  

}  

/* 
* 函数名：rd8BitBmp() 
* 功能：读取8位灰度BMP图像，返回格式数据 
* 参数： 
*   imName -文件路径及名字 
*   imWidth -图像宽度 
*   imHeight -图像高度 
* 返回值：BYTE* 
*/  

char* RdWtIm::rd8BitBmpFmt(char *imName, int *imWidth, int *imHeight)  
{  
	//对参数的有效性进行检查  
	assert(imName != NULL);  

	//二进制读方式打开图像文件  
	FILE *fp = fopen(imName, "rb");  
	if(fp == NULL)  
	{  
		return NULL;  
	}  

	//跳过位图文件头  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);  

	//读取位图信息头  
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);   

	//判断是否为8位灰度图像  
	if (head.biBitCount != 8)  
	{  
		return NULL;  
	}  

	//获取图像宽、高信息  
	*imWidth = head.biWidth ;  
	*imHeight = head.biHeight ;   

	//求图像格式宽度（必须是4的倍数）  
	int lineByte = ((*imWidth) * (head.biBitCount ) / 8 + 3) / 4 * 4;  

	//读取颜色表  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	fread(pColorTable, sizeof(RGBQUAD), 256, fp);     
	delete pColorTable;  

	//读格式数据  
	//BYTE *pBmpBuf = new BYTE[lineByte * (*imHeight)];  
	//fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  

	char *pBmpBuf = new char[lineByte * (*imHeight)];  
	fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  

	fclose(fp);  

	return pBmpBuf;  
}  

/* 
* 函数名：rd8BitBmp() 
* 功能：读取8位灰度BMP图像，并返回有效图像数据 
* 参数： 
*   imName -文件路径及名字 
*   imWidth -图像宽度 
*   imHeight -图像高度 
* 返回值：BYTE* 
*/  

BYTE* RdWtIm::rd8BitBmpNtFmt(char *imName, int *imWidth, int *imHeight)  
{  
	//对参数的有效性进行检查  
	assert(imName != NULL);  

	//二进制读方式打开图像文件  
	FILE *fp = fopen(imName, "rb");  
	if(fp == NULL)  
	{  
		return NULL;  
	}  

	//跳过位图文件头  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);  

	//读取位图信息头  
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);   

	//判断是否为8位灰度图像  
	if (head.biBitCount != 8)  
	{  
		return NULL;  
	}  

	//获取图像宽、高信息  
	*imWidth = head.biWidth ;  
	*imHeight = head.biHeight ;   

	//求图像格式宽度（必须是4的倍数）  
	int lineByte = ((*imWidth) * (head.biBitCount ) / 8 + 3) / 4 * 4;  

	//读取颜色表  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	fread(pColorTable, sizeof(RGBQUAD), 256, fp);     
	delete pColorTable;  

	//读格式数据  
	BYTE *pBmpBuf = new BYTE[lineByte * (*imHeight)];  
	fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  
	fclose(fp);  

	//提取有效数据  
	if (*imWidth < lineByte)  
	{  
		BYTE *pBmpBufData = new BYTE[(*imWidth) * (*imHeight)]();  
		for (int i=0; i<*imHeight; i++)  
		{  
			for (int j=0; j<*imWidth; j++)  
			{  
				*(pBmpBufData + i * (*imWidth) + j) = *(pBmpBuf + i * lineByte + j);      
			}  
		}  
		delete pBmpBuf;  
		return pBmpBufData; //有效数据  
	}  
	else  
	{  
		return pBmpBuf;   
	}  
}  

/* 
* 函数名：wt8BitBmpFmt() 
* 功能：将格式图像数据写入8位BMP图像 
* 参数： 
*   imBuffer - 格式图像数据 
*   imWidth -图像宽度 
*   imHeight -图像高度 
*   imName -文件路径及名字 
* 返回值：bool 
*/  

bool RdWtIm::wt8BitBmpFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName)   
{  
	//对参数的有效性进行检查  
	assert((imBuffer != NULL) && (imWidth > 0) && (imHeight > 0) && (imName != NULL));  

	//每个像素所占的位数(bit)  
	int biBitCount = 8;   

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节  
	int colorTablesize = 1024;  

	//待存储图像数据每行字节数为4的倍数  
	int lineByte = (imWidth * biBitCount / 8 + 3) / 4 * 4;  

	//以二进制写的方式打开文件  
	FILE *fp = fopen(imName, "wb");  
	if(fp == NULL)  
	{  
		return false;  
	}  

	//申请位图文件头结构变量，填写文件头信息  
	BITMAPFILEHEADER fileHead;  
	fileHead.bfType = 0x4D42;   //bmp类型  

	//bfSize是图像文件4个组成部分之和  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*imHeight;  
	fileHead.bfReserved1 = 0;  
	fileHead.bfReserved2 = 0;  

	//bfOffBits是图像文件前3个部分所需空间之和  
	fileHead.bfOffBits = 54 + colorTablesize;  

	//写文件头进文件  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  

	//申请位图信息头结构变量，填写信息头信息  
	BITMAPINFOHEADER head;  
	head.biSize = 40;    //本结构的长度  
	head.biWidth = imWidth; //位图的宽度，以像素为单位  
	head.biHeight  = imHeight; //位图的宽度，以像素为单位  
	head.biPlanes  = 1; //目标设备的级别，必须是1  
	head.biBitCount = 8; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
	head.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
	head.biSizeImage  = lineByte*imHeight;  //实际的位图数据占用的字节数  
	head.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
	head.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
	head.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
	head.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  

	//写位图信息头进内存  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);  

	//申请颜色表所需要的空间，写颜色表进文件  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	for (int i=0; i<256; i++)  
	{  
		pColorTable[i].rgbRed = i;  
		pColorTable[i].rgbGreen = i;  
		pColorTable[i].rgbBlue = i;  
		pColorTable[i].rgbReserved = 0;  
	}  
	fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);  
	delete pColorTable;  

	//写格式图像数据写进文件  
	fwrite(imBuffer, imHeight*lineByte, 1, fp);  

	//关闭文件  
	fclose(fp);   

	return true;  
}  

/* 
* 函数名：wt8BitBmpFmt() 
* 功能：将有效图像数据写入8位BMP图像 
* 参数： 
*   imBuffer - 格式图像数据 
*   imWidth -图像宽度 
*   imHeight -图像高度 
*   imName -文件路径及名字 
* 返回值：bool 
*/  

bool RdWtIm::wt8BitBmpNtFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName)  
{  
	//对参数的有效性进行检查  
	assert((imBuffer != NULL) && (imWidth > 0) && (imHeight > 0) && (imName != NULL));  

	//每个像素所占的位数(bit)  
	int biBitCount = 8;   

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节  
	int colorTablesize = 1024;  

	//待存储图像数据每行字节数为4的倍数  
	int lineByte = (imWidth * biBitCount / 8 + 3) / 4 * 4;  

	//以二进制写的方式打开文件  
	FILE *fp = fopen(imName, "wb");  
	if(fp == NULL)  
	{  
		return false;  
	}  

	//申请位图文件头结构变量，填写文件头信息  
	BITMAPFILEHEADER fileHead;  
	fileHead.bfType = 0x4D42;   //bmp类型  

	//bfSize是图像文件4个组成部分之和  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*imHeight;  
	fileHead.bfReserved1 = 0;  
	fileHead.bfReserved2 = 0;  

	//bfOffBits是图像文件前3个部分所需空间之和  
	fileHead.bfOffBits = 54 + colorTablesize;  

	//写文件头进文件  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  

	//申请位图信息头结构变量，填写信息头信息  
	BITMAPINFOHEADER head;  
	head.biSize = 40;    //本结构的长度  
	head.biWidth = imWidth; //位图的宽度，以像素为单位  
	head.biHeight  = imHeight; //位图的宽度，以像素为单位  
	head.biPlanes  = 1; //目标设备的级别，必须是1  
	head.biBitCount = 8; //每个像素所占的位数(bit)，其值必须为1（黑白图像），4（16色图），8（256色），24（真彩色图）  
	head.biCompression = BI_RGB; //位图压缩类型，有效的值为BI_RGB（未经压缩）、BI_RLE4，BI_RLE8，BI_BITFIEDS（均为Windows定义常量）。  
	head.biSizeImage  = lineByte*imHeight;  //实际的位图数据占用的字节数  
	head.biXPelsPerMeter = 0; //指定目标数据的水平分辨率，单位是像素/米。  
	head.biYPelsPerMeter = 0; //指定目标数据的垂直分辨率，单位是像素/米。  
	head.biClrUsed = 0; //位图实际用到的颜色数，如果该值为零，则用到的颜色数为2的biBitCount次幂  
	head.biClrImportant  = 0; //位图显示过程中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的。  

	//写位图信息头进内存  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);  

	//申请颜色表所需要的空间，写颜色表进文件  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	for (int i=0; i<256; i++)  
	{  
		pColorTable[i].rgbRed = i;  
		pColorTable[i].rgbGreen = i;  
		pColorTable[i].rgbBlue = i;  
		pColorTable[i].rgbReserved = 0;  
	}  
	fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);  
	delete pColorTable;  

	//判断位图数据宽度是否正确，并写数据进入BMP  
	if (imWidth < lineByte)  //如果有效数据宽度小于BMP格式宽度  
	{  
		BYTE *imgBufBMP = new BYTE [lineByte*imHeight](); //申请格式空间并初始化  

		//将有效数据赋给用于写BMP的数据空间  
		for (int i=0;i<imHeight;i++)   
		{  
			for (int j=0;j<imWidth;j++)  
			{  
				*(imgBufBMP+i*lineByte+j) = *(imBuffer + i*imWidth + j);      
			}  
		}  

		//写BMP格式要求的图数据写进文件  
		fwrite(imgBufBMP, imHeight*lineByte, 1, fp);  
	}  
	else  
	{  
		//写有效图数据写进文件  
		fwrite(imBuffer, imHeight*lineByte, 1, fp);  
	}  

	//关闭文件  
	fclose(fp);   

	return true;  
}  