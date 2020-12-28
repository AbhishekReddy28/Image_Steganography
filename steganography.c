//preprocessor directives
#include<stdio.h>						
#include<stdlib.h>
#include<string.h>
#include "steganography.h"	

int main(int argc,char *argv[])
{
	EncodeInfo encinfo;				//encinfo is a variable of type structure student				
	DecodeInfo decinfo;				//decinfo is a variable of type structure student
	strcpy(encinfo.magic_string,STEG); 		//copying indicators
	strcpy(decinfo.magic_string,STEG);		//copyimn indicators
	check_operation_type(argc,argv,&encinfo,&decinfo); 	//function call for check operation
	
}
