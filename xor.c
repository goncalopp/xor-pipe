#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int min( int a, int b) {if (a<b) return a; return b;}

int main(int argc, char *argv[])
{
unsigned char usagemessage[]= "Usage: xor [OPTION] FILE\nXORs stdin with file contents outputting to stdout.\nRepeats FILE if it is shorter than stdin length.\n\n  -t     do not repeat FILE (truncates to FILE length)\n  -r     repeats stdin instead of FILE\n  -f     writes to original file instead of stdout; assumes -t\n";
if (argc==1)
	{puts(usagemessage);
	exit(0);
	}

int repeatfromstdin = 0, truncate = 0, writetofile=0;

FILE *infile;	
unsigned char filebuffer[512];
unsigned char stdinbuffer[512];
int nreadfile, nreadstdin;
int i;


while ((i = getopt (argc, argv, "rtf")) != -1)
	switch (i)
		{
		case 'r':
			repeatfromstdin = 1;
			break;
		case 't':
			truncate = 1;
			break;
		case 'f':
			writetofile = 1;
			truncate = 1;
			break;
		case '?':
			return -1;
		} 


if (writetofile==1)
	infile=fopen(argv[optind], "r+");
else
	infile=fopen(argv[optind], "r");

if (fseek(infile, 0, SEEK_SET)) 
	{
	fputs("Error opening file\n", stderr);
	return -1;	
	}


if (repeatfromstdin==0 || truncate==1)
	{
	do
		{
		nreadstdin=fread(&stdinbuffer, 1, 512, stdin);
		nreadfile=fread(&filebuffer, 1, 512, infile);
		for (i=0; i< min(nreadstdin, nreadfile); i++)
			stdinbuffer[i]^=filebuffer[i];
		if (writetofile==1)
			{
			fseek(infile, -min(nreadstdin, nreadfile), SEEK_CUR);
			fwrite(&stdinbuffer, 1, min(nreadstdin, nreadfile), infile);
			}
		else		
		fwrite(&stdinbuffer, 1, min(nreadstdin, nreadfile), stdout);
		if (nreadfile < 512) 
			if (truncate==1)
				break;
			else
				fseek(infile, 0, SEEK_SET);
		}
	while (nreadstdin==512);
	}





if (repeatfromstdin==1)
	{
	char *input=malloc(512);
	unsigned int nbytes=0;
	unsigned long position=0;

	do
		{
		nreadstdin=fread(input+nbytes, 1, 512, stdin);
		nbytes+=nreadstdin;
		realloc(input, nbytes+512);
		}
	while (nreadstdin==512);

	do
		{
		nreadfile=fread(&filebuffer, 1, 512, infile);
		for (i=0; i< nreadfile; i++)
			filebuffer[i]^=input[(position++)%nbytes];

		fwrite(&filebuffer, 1, nreadfile, stdout);
		}
	while (nreadfile==512);

	}

fclose(infile);
}


