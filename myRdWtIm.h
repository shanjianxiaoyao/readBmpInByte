/*  
*   
* 文件名称：myRdWtIm.h  
* 摘    要：读写BMP图像。“格式数据”指直接从BMP读取的或能够直接写入BMP的数据， 
* 其宽度满足BMP格式要求，即满足4的倍数。“有效数据”指实际图像数据，其宽度不一定 
*  满足BMP格式要求。 
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
	char *rd8BitBmpFmt(char *imName, int *imWidth, int *imHeight);  //读8位BMP图像，返回格式图像数据。  
	BYTE *rd8BitBmpNtFmt(char *imName, int *imWidth, int *imHeight);    //读8位BMP图像，返回有效图像数据。  

	bool wt8BitBmpFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName); //将格式图像数据写入8位BMP图像  
	bool wt8BitBmpNtFmt(BYTE *imBuffer, int imWidth, int imHeight, char *imName); //将有效图像数据写入8位BMP图像  

};  


#endif  