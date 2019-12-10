#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * convert(char * s, int numRows){
	int len = strlen(s);
	//printf("len: %d\n", len);
	char * ret;

	int numCol = len % numRows == 0 ? len / numRows: len / numRows + 1;
	printf("numCol : %d\n", numCol);

	char array[numRows][numCol];

	while( *s != '\0') {
		printf("%c ", *s);
		s++;
	}
	printf("\n");

	ret = malloc(len + 1);
	memset(ret, 0, len + 1);

	return ret;
}

int main(int argc, const char *argv[])
{
	
	char test_string[40]  = "LEETCODEISHIRING";
	int cow = 4;
	printf("input String: %s\n", test_string);
	printf("len: %d, row: %d\n",(int)strlen(test_string), cow);
	
	convert(test_string, cow);
	return 0;
}

