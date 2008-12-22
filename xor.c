#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[])
{
if (argc!=2)
	{printf("XOR - applies XOR between a file and standard input (repeating stdin if necessary) and sends the result to standard output.\nUsage: xor file\n");
	exit(0);
	}

char *input;
int nbytes;
int infile;	
int tmp;
int n;

infile=open(argv[1], O_RDONLY);

input=malloc(512);
nbytes=0;

while ((tmp=getchar())!=EOF)
	{
	*(input+(nbytes++))= (char) tmp;
	if ((nbytes%512)==511) realloc(input, nbytes+512);
	}

n=0;
while (read(infile, &tmp, 1)==1)
	{
	putchar(tmp^(*(input+(n++))));
	if (n>=nbytes) n=0;
	}
}
