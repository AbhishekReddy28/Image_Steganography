#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#define RASTER_OFFSET 10					// using preprocessor directives setting offset to 10
#define MAX_EXT_SIZE 5						// using preprocessor directives setting max_ext_size to 5
#define MAX_MAGIC_SIZE 20					// using preprocessor directives setting max_magic_size(password) to 20
#define STEG "$*"
long int func_dec_int(FILE* ptr2);				//function declaration for decoding long/int values 
void func_int(long int data,FILE *ptr,FILE* ptr2);		//function declaration for func long/int values 
unsigned long int gen_mask_int(long int n);			//function declaration for generating masks

typedef struct _EncodeInfo					//structure for encoding
{
	/* Source Image info */
	char *src_image_fname;
	FILE *fptr_src_image;
	char src_ext[MAX_EXT_SIZE];
	unsigned int image_capacity;

	/* Secret File Info */
	char *secret_fname;
	FILE *fptr_secret;
	char secret_ext[MAX_EXT_SIZE];
	int secret_size;

	/* Stego Image Info */
	char *stego_image_fname;
	FILE *fptr_stego_image;
	char stego_ext[MAX_EXT_SIZE];
	/*Magic strig*/
	char magic_string[MAX_MAGIC_SIZE];

} EncodeInfo;
typedef struct _DecodeInfo					//structure for decoding
{
	/*stego image file*/
	char *stego_image_fname;
	FILE *fptr_stego_image;

	/*output file */ 
	char *output_image_fname;
	FILE *fptr_output_file;
	char output_file_ext[MAX_EXT_SIZE];
	unsigned char flag_output;

	/*Magic strig*/
	char magic_string[MAX_MAGIC_SIZE];
} DecodeInfo;

//function declartions
void start_decode(DecodeInfo *ptr);				
void start_encoding(EncodeInfo *ptr);
void func(unsigned char data,FILE *ptr,FILE *ptr2);
int img_capacity(EncodeInfo *ptr);
unsigned char gen_mask(int n);
long int data_size(EncodeInfo *ptr);
unsigned char func_dec(FILE* ptr2);
int check_capacity(EncodeInfo *encInfo);
int open_enc_files(EncodeInfo *encInfo);
int open_dec_files(DecodeInfo *decInfo);
int read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encinfo);
int read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decinfo);
int check_operation_type(int argc,char *argv[],EncodeInfo *encinfo,DecodeInfo *decinfo);

#endif
