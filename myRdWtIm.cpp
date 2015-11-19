/*  
* �ļ����ƣ�myRdWtIm.h  
* ժ    Ҫ����дBMPͼ�񡣡���ʽ���ݡ�ֱָ�Ӵ�BMP��ȡ�Ļ��ܹ�ֱ��д��BMP�����ݣ� 
* ��������BMP��ʽҪ�󣬼�����4�ı���������Ч���ݡ�ָʵ��ͼ�����ݣ����Ȳ�һ�� 
*  ����BMP��ʽҪ�� 
*/  

#include "myRdWtIm.h"  

RdWtIm::RdWtIm()  
{  

}  

RdWtIm::~RdWtIm()  
{  

}  

/* 
* ��������rd8BitBmp() 
* ���ܣ���ȡ8λ�Ҷ�BMPͼ�񣬷��ظ�ʽ���� 
* ������ 
*   imName -�ļ�·�������� 
*   imWidth -ͼ���� 
*   imHeight -ͼ��߶� 
* ����ֵ��BYTE* 
*/  

char* RdWtIm::rd8BitBmpFmt(char *imName, int *imWidth, int *imHeight)  
{  
	//�Բ�������Ч�Խ��м��  
	assert(imName != NULL);  

	//�����ƶ���ʽ��ͼ���ļ�  
	FILE *fp = fopen(imName, "rb");  
	if(fp == NULL)  
	{  
		return NULL;  
	}  

	//����λͼ�ļ�ͷ  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);  

	//��ȡλͼ��Ϣͷ  
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);   

	//�ж��Ƿ�Ϊ8λ�Ҷ�ͼ��  
	if (head.biBitCount != 8)  
	{  
		return NULL;  
	}  

	//��ȡͼ�������Ϣ  
	*imWidth = head.biWidth ;  
	*imHeight = head.biHeight ;   

	//��ͼ���ʽ��ȣ�������4�ı�����  
	int lineByte = ((*imWidth) * (head.biBitCount ) / 8 + 3) / 4 * 4;  

	//��ȡ��ɫ��  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	fread(pColorTable, sizeof(RGBQUAD), 256, fp);     
	delete pColorTable;  

	//����ʽ����  
	//BYTE *pBmpBuf = new BYTE[lineByte * (*imHeight)];  
	//fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  

	char *pBmpBuf = new char[lineByte * (*imHeight)];  
	fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  

	fclose(fp);  

	return pBmpBuf;  
}  

/* 
* ��������rd8BitBmp() 
* ���ܣ���ȡ8λ�Ҷ�BMPͼ�񣬲�������Чͼ������ 
* ������ 
*   imName -�ļ�·�������� 
*   imWidth -ͼ���� 
*   imHeight -ͼ��߶� 
* ����ֵ��BYTE* 
*/  

BYTE* RdWtIm::rd8BitBmpNtFmt(char *imName, int *imWidth, int *imHeight)  
{  
	//�Բ�������Ч�Խ��м��  
	assert(imName != NULL);  

	//�����ƶ���ʽ��ͼ���ļ�  
	FILE *fp = fopen(imName, "rb");  
	if(fp == NULL)  
	{  
		return NULL;  
	}  

	//����λͼ�ļ�ͷ  
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);  

	//��ȡλͼ��Ϣͷ  
	BITMAPINFOHEADER head;  
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);   

	//�ж��Ƿ�Ϊ8λ�Ҷ�ͼ��  
	if (head.biBitCount != 8)  
	{  
		return NULL;  
	}  

	//��ȡͼ�������Ϣ  
	*imWidth = head.biWidth ;  
	*imHeight = head.biHeight ;   

	//��ͼ���ʽ��ȣ�������4�ı�����  
	int lineByte = ((*imWidth) * (head.biBitCount ) / 8 + 3) / 4 * 4;  

	//��ȡ��ɫ��  
	RGBQUAD *pColorTable = new RGBQUAD[256];  
	fread(pColorTable, sizeof(RGBQUAD), 256, fp);     
	delete pColorTable;  

	//����ʽ����  
	BYTE *pBmpBuf = new BYTE[lineByte * (*imHeight)];  
	fread(pBmpBuf, 1, lineByte * (*imHeight), fp);  
	fclose(fp);  

	//��ȡ��Ч����  
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
		return pBmpBufData; //��Ч����  
	}  
	else  
	{  
		return pBmpBuf;   
	}  
}  

/* 
* ��������wt8BitBmpFmt() 
* ���ܣ�����ʽͼ������д��8λBMPͼ�� 
* ������ 
*   imBuffer - ��ʽͼ������ 
*   imWidth -ͼ���� 
*   imHeight -ͼ��߶� 
*   imName -�ļ�·�������� 
* ����ֵ��bool 
*/  

bool RdWtIm::wt8BitBmpFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName)   
{  
	//�Բ�������Ч�Խ��м��  
	assert((imBuffer != NULL) && (imWidth > 0) && (imHeight > 0) && (imName != NULL));  

	//ÿ��������ռ��λ��(bit)  
	int biBitCount = 8;   

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽ�  
	int colorTablesize = 1024;  

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���  
	int lineByte = (imWidth * biBitCount / 8 + 3) / 4 * 4;  

	//�Զ�����д�ķ�ʽ���ļ�  
	FILE *fp = fopen(imName, "wb");  
	if(fp == NULL)  
	{  
		return false;  
	}  

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ  
	BITMAPFILEHEADER fileHead;  
	fileHead.bfType = 0x4D42;   //bmp����  

	//bfSize��ͼ���ļ�4����ɲ���֮��  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*imHeight;  
	fileHead.bfReserved1 = 0;  
	fileHead.bfReserved2 = 0;  

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��  
	fileHead.bfOffBits = 54 + colorTablesize;  

	//д�ļ�ͷ���ļ�  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ  
	BITMAPINFOHEADER head;  
	head.biSize = 40;    //���ṹ�ĳ���  
	head.biWidth = imWidth; //λͼ�Ŀ�ȣ�������Ϊ��λ  
	head.biHeight  = imHeight; //λͼ�Ŀ�ȣ�������Ϊ��λ  
	head.biPlanes  = 1; //Ŀ���豸�ļ��𣬱�����1  
	head.biBitCount = 8; //ÿ��������ռ��λ��(bit)����ֵ����Ϊ1���ڰ�ͼ�񣩣�4��16ɫͼ����8��256ɫ����24�����ɫͼ��  
	head.biCompression = BI_RGB; //λͼѹ�����ͣ���Ч��ֵΪBI_RGB��δ��ѹ������BI_RLE4��BI_RLE8��BI_BITFIEDS����ΪWindows���峣������  
	head.biSizeImage  = lineByte*imHeight;  //ʵ�ʵ�λͼ����ռ�õ��ֽ���  
	head.biXPelsPerMeter = 0; //ָ��Ŀ�����ݵ�ˮƽ�ֱ��ʣ���λ������/�ס�  
	head.biYPelsPerMeter = 0; //ָ��Ŀ�����ݵĴ�ֱ�ֱ��ʣ���λ������/�ס�  
	head.biClrUsed = 0; //λͼʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2��biBitCount����  
	head.biClrImportant  = 0; //λͼ��ʾ��������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ�ġ�  

	//дλͼ��Ϣͷ���ڴ�  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);  

	//������ɫ������Ҫ�Ŀռ䣬д��ɫ����ļ�  
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

	//д��ʽͼ������д���ļ�  
	fwrite(imBuffer, imHeight*lineByte, 1, fp);  

	//�ر��ļ�  
	fclose(fp);   

	return true;  
}  

/* 
* ��������wt8BitBmpFmt() 
* ���ܣ�����Чͼ������д��8λBMPͼ�� 
* ������ 
*   imBuffer - ��ʽͼ������ 
*   imWidth -ͼ���� 
*   imHeight -ͼ��߶� 
*   imName -�ļ�·�������� 
* ����ֵ��bool 
*/  

bool RdWtIm::wt8BitBmpNtFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName)  
{  
	//�Բ�������Ч�Խ��м��  
	assert((imBuffer != NULL) && (imWidth > 0) && (imHeight > 0) && (imName != NULL));  

	//ÿ��������ռ��λ��(bit)  
	int biBitCount = 8;   

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽ�  
	int colorTablesize = 1024;  

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���  
	int lineByte = (imWidth * biBitCount / 8 + 3) / 4 * 4;  

	//�Զ�����д�ķ�ʽ���ļ�  
	FILE *fp = fopen(imName, "wb");  
	if(fp == NULL)  
	{  
		return false;  
	}  

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ  
	BITMAPFILEHEADER fileHead;  
	fileHead.bfType = 0x4D42;   //bmp����  

	//bfSize��ͼ���ļ�4����ɲ���֮��  
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*imHeight;  
	fileHead.bfReserved1 = 0;  
	fileHead.bfReserved2 = 0;  

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��  
	fileHead.bfOffBits = 54 + colorTablesize;  

	//д�ļ�ͷ���ļ�  
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);  

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ  
	BITMAPINFOHEADER head;  
	head.biSize = 40;    //���ṹ�ĳ���  
	head.biWidth = imWidth; //λͼ�Ŀ�ȣ�������Ϊ��λ  
	head.biHeight  = imHeight; //λͼ�Ŀ�ȣ�������Ϊ��λ  
	head.biPlanes  = 1; //Ŀ���豸�ļ��𣬱�����1  
	head.biBitCount = 8; //ÿ��������ռ��λ��(bit)����ֵ����Ϊ1���ڰ�ͼ�񣩣�4��16ɫͼ����8��256ɫ����24�����ɫͼ��  
	head.biCompression = BI_RGB; //λͼѹ�����ͣ���Ч��ֵΪBI_RGB��δ��ѹ������BI_RLE4��BI_RLE8��BI_BITFIEDS����ΪWindows���峣������  
	head.biSizeImage  = lineByte*imHeight;  //ʵ�ʵ�λͼ����ռ�õ��ֽ���  
	head.biXPelsPerMeter = 0; //ָ��Ŀ�����ݵ�ˮƽ�ֱ��ʣ���λ������/�ס�  
	head.biYPelsPerMeter = 0; //ָ��Ŀ�����ݵĴ�ֱ�ֱ��ʣ���λ������/�ס�  
	head.biClrUsed = 0; //λͼʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2��biBitCount����  
	head.biClrImportant  = 0; //λͼ��ʾ��������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ�ġ�  

	//дλͼ��Ϣͷ���ڴ�  
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);  

	//������ɫ������Ҫ�Ŀռ䣬д��ɫ����ļ�  
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

	//�ж�λͼ���ݿ���Ƿ���ȷ����д���ݽ���BMP  
	if (imWidth < lineByte)  //�����Ч���ݿ��С��BMP��ʽ���  
	{  
		BYTE *imgBufBMP = new BYTE [lineByte*imHeight](); //�����ʽ�ռ䲢��ʼ��  

		//����Ч���ݸ�������дBMP�����ݿռ�  
		for (int i=0;i<imHeight;i++)   
		{  
			for (int j=0;j<imWidth;j++)  
			{  
				*(imgBufBMP+i*lineByte+j) = *(imBuffer + i*imWidth + j);      
			}  
		}  

		//дBMP��ʽҪ���ͼ����д���ļ�  
		fwrite(imgBufBMP, imHeight*lineByte, 1, fp);  
	}  
	else  
	{  
		//д��Чͼ����д���ļ�  
		fwrite(imBuffer, imHeight*lineByte, 1, fp);  
	}  

	//�ر��ļ�  
	fclose(fp);   

	return true;  
}  