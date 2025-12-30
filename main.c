#include <stdio.h>

char *output = "out.data";

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("You need to pass in a file to me to get an output\n");
		return 0;
	}
	if(argc >= 3){
		output = argv[2];
	}
	FILE * MacPaint_File;
	FILE * Output_File;
	MacPaint_File = fopen(argv[1], "rb");
	Output_File = fopen(output, "wb");

	if(MacPaint_File == NULL){
		printf("ERROR CAN'T OPEN THE MACPAINT FILE!\n");
		return 1;
	}
	if(Output_File == NULL){
		printf("ERROR CAN'T OPEN OUTPUT FILE FOR WRITING!\n");
		return 1;
	}
	fseek(MacPaint_File, 0, SEEK_END);
	int length_of_file = ftell(MacPaint_File);
	fseek(MacPaint_File, 512, SEEK_SET); //MacPaint starting offset

	//Decode Packbits
	while(ftell(MacPaint_File) < length_of_file){
		unsigned char c = fgetc(MacPaint_File);
		if(c == 128){ //Ignored
			continue;
		}
		if(c >= 129){ //Row of same bytes
			char cont_c = fgetc(MacPaint_File);
			for(char i = 0; i < (257 - c); i++){
				fputc(cont_c ^ 0xFF, Output_File);
			}
		}else{ //Read bytes raw
			for(char i = 0; i < c+1; i++){
				fputc(fgetc(MacPaint_File) ^ 0xFF, Output_File);
			}
		
		}
	}

	return 0;
}
