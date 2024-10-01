#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

//Programul se va rula astfel in terminal ./nume_executabil  nume_fisier.bmp  nume_fisier_transformat.bmp

int main(int argc, char **argv)
{
	FILE *fin = NULL;
	FILE *fout = NULL;
	
	if(argc<3)
	{
		printf("Numar insuficient de argumente");
		exit(-1);
	}
	
	if((fin = fopen(argv[1], "rb"))==NULL)
	{
		printf("uh oh");
		exit(EXIT_FAILURE);
	}
	
	if((fout = fopen(argv[2], "wb+"))==NULL)
	{
		printf("UH OH");
		exit(EXIT_FAILURE);
	}
	
	//citim header-ul byte cu byte
	uint8_t header[54];
	
	for(int i=0; i<54; i++)  
		fread(&header[i], sizeof(uint8_t), 1, fin);
	
	
	fwrite(header, sizeof(uint8_t), 54, fout);
	
	
	//extrag latimea si inaltimea din header
	int latime = (header[18] & 0xFF) | ((header[19] & 0xFF) << 8) | ((header[20] & 0xFF) << 16) | ((header[21] & 0xFF) << 24);

	int inaltime = (header[22] & 0xFF) | ((header[23] & 0xFF) << 8) | ((header[24] & 0xFF) << 16) | ((header[25] & 0xFF) << 24);

	
	int size = inaltime * latime;

	uint8_t buffer[size][3];
	
	int grey;
	
	for(int i=0; i<size; i++)
	{
		grey = 0;
		
		fread(&buffer[i][2], sizeof(uint8_t), 1, fin); //blue
		fread(&buffer[i][1], sizeof(uint8_t), 1, fin); //green
		fread(&buffer[i][0], sizeof(uint8_t), 1, fin); //red
		
		//folosim formula GREY = 0.299 * R + 0.587 * G + 0.114 * B
		
		grey = (buffer[i][0]*0.299) + (buffer[i][1]*0.587) + (buffer[i][2]*0.114);
		
		fwrite(&grey, sizeof(uint8_t), 1, fout);
		fwrite(&grey, sizeof(uint8_t), 1, fout);
		fwrite(&grey, sizeof(uint8_t), 1, fout);
	}
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}
