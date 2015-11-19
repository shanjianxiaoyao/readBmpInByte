/*  
*   
* �ļ����ƣ�myRdWtIm.h  
* ժ    Ҫ����дBMPͼ�񡣡���ʽ���ݡ�ֱָ�Ӵ�BMP��ȡ�Ļ��ܹ�ֱ��д��BMP�����ݣ� 
* ��������BMP��ʽҪ�󣬼�����4�ı���������Ч���ݡ�ָʵ��ͼ�����ݣ����Ȳ�һ�� 
*  ����BMP��ʽҪ�� 
*   
*/  

#ifndef RWBMP_H  
#define RWBMP_H  

#include <windows.h>  
#include <stdio.h>  
#include <assert.h>  

class RdWtIm  
{  
public:  
	RdWtIm(void);  
	~RdWtIm(void);  

public:  
	char *rd8BitBmpFmt(char *imName, int *imWidth, int *imHeight);  //��8λBMPͼ�񣬷��ظ�ʽͼ�����ݡ�  
	BYTE *rd8BitBmpNtFmt(char *imName, int *imWidth, int *imHeight);    //��8λBMPͼ�񣬷�����Чͼ�����ݡ�  

	bool wt8BitBmpFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName); //����ʽͼ������д��8λBMPͼ��  
	bool wt8BitBmpNtFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName); //����Чͼ������д��8λBMPͼ��  

};  


#endif  