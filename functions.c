#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "steganography.h"
/*Function to check which operation */
int check_operation_type(int argc,char *argv[],EncodeInfo *encinfo,DecodeInfo *decinfo)
{
	int status;
	//first arguement for checking encoding with or without password
	if(  !(strcmp(*(argv+1),"-e"))  || !(strcmp(*(argv+1),"-pe")))		
	{
		//with password
		if(!(strcmp(*(argv+1),"-pe")))
		{
			printf("enter your password to be encoded\n");
			scanf("%s",(char *)encinfo->magic_string + 1);
		}
		read_and_validate_encode_args(argc,argv,encinfo);
	}
	//first arguement for checking decoding with or without password
	if(  !(strcmp(*(argv+1),"-d"))  || !(strcmp(*(argv+1),"-pd")))
	{
		//with password
		if(!(strcmp(*(argv+1),"-pd")))
		{
			printf("enter your password to be decoded\n");
			scanf("%s",(char *)decinfo->magic_string + 1);
		}
		read_and_validate_decode_args(argc,argv,decinfo);
	}
}


/*function to validate encode arguments*/
int read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encinfo)
{
	if(argc<4)
	{
		printf("command should be of type\n");
		printf("1) ./a.out -e <image.bmp> <secret.txt> <output_filename.bmp>\n");
		printf("NOTE: use -pe image with password\n");
		return 1;
	}
	encinfo->src_image_fname=*(argv+2);
	encinfo->secret_fname=*(argv+3);	
	if(argc==5)
		encinfo->stego_image_fname=*(argv+4);
	else
		encinfo->stego_image_fname="Default.bmp";

	printf("INFO:Checking for extension\n");
	if(strstr(*(argv+2),".bmp")==NULL)
	{
		printf("INFO: provide image with .bmp extension\n");
		return 1;
	}
	else
	{
		printf("INFO:Extension provided is accepted\n");
		open_enc_files(encinfo);
		return 0;
	}
	
}
/*function to validate decode arguments*/
int read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decinfo)
{

	if(argc<3)
	{
		printf("command should be of type\n");
		printf("./a.out -d <image.bmp> <outputfile>\n");
		printf("NOTE:use -pe or -pd flag to steg or decode with password\n");
		return 1;
	}
	decinfo->stego_image_fname=*(argv+2);
	if(argc==4)
	{
		decinfo->output_image_fname=*(argv+3);
		decinfo->flag_output=1;
	}
	else
	{
		printf("no out put file mentioned... ouput will ben printed on screen\n");
		decinfo->flag_output=0;
	}

	printf("INFO:Checking for extension\n");
	if(strstr(*(argv+2),".bmp")==NULL)
	{
		printf("INFO: provide image with .bmp extension\n");
		return 1;
	}
	else
	{
		printf("INFO:Extension provided is accepted\n");
		open_dec_files(decinfo);
		return 0;
	}
}
/*function to open enc files*/
int open_enc_files(EncodeInfo *encinfo)
{
	printf("INFO:opening required files\n");
	if(encinfo->fptr_src_image=fopen(encinfo->src_image_fname,"r"))
	{
		printf("INFO:src/.bmp image opened successfully\n");
		if(encinfo->fptr_secret=fopen(encinfo->secret_fname,"r"))
		{
			printf("INFO:secret.txt opened successfully\n");
			if(encinfo->fptr_stego_image=fopen(encinfo->stego_image_fname,"w+"))
			{
				printf("INFO:stego image opened successfully\n");
				printf("INFO:DONE\n");
				check_capacity(encinfo);
				return 0;
			}
			else
			{
				printf("INFO:opening stego image failed\n");
				return 1;
			}
		}
		else
		{
			printf("INFO:opening secret.txt failed\n");	
			return 1;
		}
	}
	else
	{
		printf("INFO: src/.bmp image failed to open\n");
		return 1;	
	}
}	
/*function to open decode files*/
int open_dec_files(DecodeInfo *decinfo)
{
	printf("opening required files\n");
	if(decinfo->fptr_stego_image=fopen(decinfo->stego_image_fname,"r"))
	{
		printf("INFO:stego image opened successfully\n");
		if(decinfo->flag_output==1)
		{
			if(decinfo->fptr_output_file=fopen(decinfo->output_image_fname,"w+"))
			{
				printf("INFO:output file opened succesfully\n");
				printf("INFO:decoding started\n");
				start_decode(decinfo);
				printf("INFO:decoding finished \n");
				
				return 0;
			}
			else
			{
				printf("INFO:opening output file failed\n");
				return 1;
			}
		}
		else
		{
			printf("INFO:decoding started\n");
			start_decode(decinfo);
			printf("INFO:decoding finished \n");
		}
	}
	else
	{
		printf("INFO:opening stego image failed\n");
		return 1;
	}
}
/*function to check whether file can hold data*/	
int check_capacity(EncodeInfo *encinfo)
{
	printf("INFO:saving .txt extension\n");
	//save extension in structure 
	int i;	
	for(i=0;*(encinfo->secret_fname+i);i++)
	{
		if(*(encinfo->secret_fname+i) == '.')
		break;	
	}
	int k;
	for(k=0;*(encinfo->secret_fname+i);k++)
	{
		encinfo->secret_ext[k]=*(encinfo->secret_fname+i);
		i++;	
		//printf("secret_ext %s\n",encinfo->secret_ext);
	}
	encinfo->secret_ext[k]='\0';

	//check for capacity
	
	long int total_size = data_size(encinfo);
	printf("INFO:secret data size is: %ld\n",total_size);
	img_capacity(encinfo);
	long int image_capacity=encinfo->image_capacity;
	printf("INFO:image capacity is: %ld\n",image_capacity);
	printf("INFO: checking for src/beautiful.bmp capacity to handle secret.txt\n");
	if(image_capacity<total_size)
	{
		printf("INFO:Image cannot handle data\n");
		return 1;
	}
	else
	{
		printf("\n");
		printf("INFO:Started encoding process\n");
		start_encoding(encinfo);
		printf("INFO:encoding completed sucessfully\n");
		return 0;
	}
}
long int data_size(EncodeInfo *ptr)
{
	printf("INFO:checking for secret.txt size\n");
	fseek(ptr->fptr_secret,0,SEEK_SET);
	int text_size=0;char c;
	while(c=fgetc(ptr->fptr_secret)!=EOF)
	{
		text_size++;
	}
	ptr->secret_size=text_size;
	printf("INFO:secret.txt size is  %d\n",ptr->secret_size);
	printf("INFO:DONE\n");
	int ext_size=strlen(ptr->secret_ext);

	//printf("Secret file size %d\n",(4+4+4+text_size+ext_size+strlen(ptr->magic_string)*8)); 
	return (8+8+8+text_size+ext_size+strlen(ptr->magic_string))*8;	
}
int img_capacity(EncodeInfo *ptr)
{
	fseek(ptr->fptr_src_image,34,SEEK_SET);
	fread(&(ptr->image_capacity),sizeof(int),1,ptr->fptr_src_image);
		

}
			

void start_encoding(EncodeInfo *ptr)
{
	unsigned char c;
	fseek(ptr->fptr_src_image,0,SEEK_SET);		//set source to 0
	fseek(ptr->fptr_secret,0,SEEK_SET);		//set secret to 0
	fseek(ptr->fptr_stego_image,0,SEEK_SET);	//set steg to 0
	int raster_offset;
	fseek(ptr->fptr_src_image,RASTER_OFFSET,SEEK_SET);//get offset by pointing to raster
	fread(&raster_offset,sizeof(int),1,ptr->fptr_src_image);

	//printf("offset is : %x %d\n",raster_offset,raster_offset);

	printf("INFO:copying image Header\n");
	fseek(ptr->fptr_src_image,0,SEEK_SET);			//set source to zero
	for(int i=0;i<raster_offset;i++) 			//copy the header file
	{
		fread(&c,1,1,ptr->fptr_src_image);
		fwrite(&c,1,1,ptr->fptr_stego_image);
	}
	printf("INFO:DONE\n");
	func_int(strlen(ptr->magic_string),ptr->fptr_src_image,ptr->fptr_stego_image);
	printf("INFO:encoding magic string \n");
	for(int i=0;i<strlen(ptr->magic_string);i++)
	{

		func(ptr->magic_string[i],ptr->fptr_src_image,ptr->fptr_stego_image);	//encode star
	}
	printf("INFO:DONE\n");
	//encode size of extension ex: .txt  = 4
	func_int(strlen(ptr->secret_ext),ptr->fptr_src_image,ptr->fptr_stego_image);
	//encode extension ex: .txt
	printf("INFO:encoding secret.txt extension \n");
	for(int i=0;i<strlen(ptr->secret_ext);i++)
	{
		func(ptr->secret_ext[i],ptr->fptr_src_image,ptr->fptr_stego_image);

	}
	printf("INFO:DONE\n");
	//encode text size ex: abcd =  4
	func_int(ptr->secret_size,ptr->fptr_src_image,ptr->fptr_stego_image);
	//encode text in secret file
	printf("INFO:encoding secret.txt information \n");
	for(int i=0;i<ptr->secret_size;i++)
	{
		fread(&c,1,1,ptr->fptr_secret);
		func(c,ptr->fptr_src_image,ptr->fptr_stego_image);
	}
	printf("INFO:DONE\n");

	//copy rest of the data from src to steg

	//printf("Copying remaining bytes after encoding\n");
	printf("INFO:copying left over data\n");
	for(;;)
	{
		if(feof(ptr->fptr_src_image))
			break;
		unsigned char c1; 
		fread(&c1,1,1,ptr->fptr_src_image);
		fwrite(&c1,1,1,ptr->fptr_stego_image);

	}
	printf("INFO:DONE\n");

}

void start_decode(DecodeInfo *ptr)
{
	unsigned char c;
	//fseek(ptr->fptr_src_image,0,SEEK_SET);
	//fseek(ptr->fptr_secret,0,SEEK_SET);
	fseek(ptr->fptr_stego_image,0,SEEK_SET); //point to zero at start
	// get raster offset and point to it
	int raster_offset;
	fseek(ptr->fptr_stego_image,RASTER_OFFSET,SEEK_SET);//get offset
	fread(&raster_offset,sizeof(int),1,ptr->fptr_stego_image);
	//poin to raster offset
	//	printf("offset is : %x %d\n",raster_offset,raster_offset);
	fseek(ptr->fptr_stego_image,raster_offset,SEEK_SET);//set to raster offset
	// decept *
	unsigned long int magic_size=func_dec_int(ptr->fptr_stego_image);
	//decoding magic
		
	printf("INFO:Decoding magic string structure\n");
	if(magic_size==strlen(ptr->magic_string))
	{
		for(int i=0;i<magic_size;i++)
		{
			c=func_dec(ptr->fptr_stego_image);
			if(c!=ptr->magic_string[i])
			{
				printf("wrong password:( \n");
				exit(1);
			}

			//printf("%c",c);
		}
	}
	else
	{
		printf("wrong password:( \n");
		exit(1);
	}
	printf("INFO:DONE\n");
//	printf("\n");
	//decrypt ext size
	printf("INFO:Decoding output file extension\n");
	unsigned long int  ext_size=func_dec_int(ptr->fptr_stego_image);
	printf("INFO:DONE\n");
	//printf("ext_size : %x %d\n",ext_size,ext_size);
	//decode extension
	for(int i=0;i<ext_size;i++)
	{
		func_dec(ptr->fptr_stego_image);
	}
	printf("INFO:DONE\n");
	//printf("\n");

	//decode text size
	printf("INFO:Decoding text size\n");
	unsigned long int secret_size=func_dec_int(ptr->fptr_stego_image);
	//printf("secret_size : %x %d\n",secret_size,secret_size);
	printf("INFO:DONE\n");

	//decode secret text
	printf("INFO:Decoding secret text\n");
	for(int i=0;i<secret_size;i++)
	{
		unsigned char c=func_dec(ptr->fptr_stego_image);
		if(ptr->flag_output==0)
			printf("%c",c);
		else
			fwrite(&c,1,1,ptr->fptr_output_file);
	}
	printf("INFO:DONE\n");

}





void func(unsigned char data,FILE *ptr,FILE* ptr2)
{
	unsigned char byte;
	unsigned char mask =0x01;
	for(int i=0;i<=7;i++)
	{
		fread(&byte,1,1,ptr);
		//printf("original byte %hhx\n ",byte);
		if(data & (mask))
		{
			byte=byte | 0x01;
			//printf("after %hhx\n",(byte | 0x01));
			fwrite(&byte,1,1,ptr2);

		}
		else
		{
			byte=byte & 0xfe;
			//printf("after %hhx\n ",(byte & 0xfe));
			fwrite(&byte,1,1,ptr2);
		}
		mask=mask<<1;
	}
}
unsigned char func_dec(FILE* ptr2)
{
	unsigned char data;
	unsigned char byte;
	for(int i=0;i<=7;i++)
	{
		fread(&byte,1,1,ptr2);
		if(byte & 0x01)
		{
			data=(data | gen_mask(i));
		}
		else
		{
			data=(data & (0xFF - gen_mask(i)));
		}
	}
	return data;
}
unsigned char gen_mask(int n)
{
	int res=1;
	for(int i=0;i<n;i++)
	{
		res=res*2;
	}
	//	printf("%d \n ",res);
	return res;
}

void func_int(long int data,FILE *ptr,FILE* ptr2)
{
	unsigned char byte;
	unsigned long int mask =0x0000000000000001;
	for(int i=0;i<(sizeof(long)*8);i++)
	{
		fread(&byte,1,1,ptr);
		//printf("original byte %hhx\n ",byte);
		if(data & (mask))
		{
			byte=byte | 0x01;
			//printf("after %hhx\n",(byte | 0x01));
			fwrite(&byte,1,1,ptr2);

		}
		else
		{
			byte=byte & 0xfe;
			//printf("after %hhx\n ",(byte & 0xfe));
			fwrite(&byte,1,1,ptr2);
		}
		mask=mask<<1;
	}
}
long int func_dec_int(FILE* ptr2)
{
	long int data;
	unsigned char byte;
	for(int i=0;i<(sizeof(long)*8);i++)
	{
		fread(&byte,1,1,ptr2);
		if(byte & 0x01)
		{
			data=(data | gen_mask_int(i));
		}
		else
		{
			data=(data & (0xFFFFFFFFFFFFFFFF - gen_mask_int(i)));
		}
	}
	return data;
}
unsigned long int gen_mask_int(long int n)
{
	long int res=1;
	for(int i=0;i<n;i++)
	{
		res=res*2;
	}
	//	printf("%d \n ",res);
	return res;
}


