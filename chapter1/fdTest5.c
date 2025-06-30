#include <stdio.h>

// Little Endian - 작은 주소에 작은 값 저장

void main()
{
	int n = 0x1234567;	// 0x7654321;
	// 바이트의 값을 확인하기 위해 char 사용
	char *pn = &n;
	printf("1st: %p, %#x\n", &(*pn), *pn);				// 작은 주소
	printf("2nd: %p, %#x\n", &(*(pn+1)), *(pn+1));
	printf("3rd: %p, %#x\n", &(*(pn+2)), *(pn+2));
	printf("4th: %p, %#x\n", &(*(pn+3)), *(pn+3));	// 큰 주소

	
}
