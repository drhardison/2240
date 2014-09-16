/******************************************************
Programmer: Joseph Hagan
Class: CS 2240
Professor: Dr. Trenary
Assignment: Create a basic database in c using buffers
Notes: I utilized code given in class by Dr. Trenary
******************************************************/

/***Includes/Defines****/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "apue.h"
#define buffsize 2048
/***********************/

/***Struct Definitions**/
typedef struct
{
	char id[4];
	char name[25];
	int pop;
	float life;
}record;
/***********************/

/***Method Prototypes***/
record * arrayalloc(int);
int openfile();
int fillbuffer(int *);
void processbuffer(int *, int);
void storeandsort(char *);
void searcharray();

/***********************/

/***Global Variables****/
int opened = 0;  // boolean flag 0 is false, 1 is true;
char buffer [buffsize+1];
int howmuchwasread;
int numorecords = 0;
record *storage;
/***********************/


int main(int argc, char *argv[]){

	///////////////PARAMETER ERROR CHECKER/////////////////
	//This is to check to make sure that the user 
	//inputs the correct number of parameters.

	if(argc !=2){
		err_sys("Invalid Number of Parameters. Try Again");
		}
	//////////////End of Parameter Error Checker///////////
	int num = atoi(argv[1]); 
	storage = arrayalloc(num);
	int fd = openfile();
	int endofile = 0;
	char * chunk = (char) 0;
	while(endofile !=-1){
		endofile = fillbuffer(&fd);
		processbuffer(&fd, endofile);
	}
	return 0;
}

 int fillbuffer(int * fd){
	int readcount1;
	int localfd = *fd;
	readcount1 = read(localfd,buffer,buffsize);
	buffer[buffsize] = (char) 0;
	if (readcount1 == 0){
		readcount1 = -1;	
	}
	if(readcount1<0){
	
		err_sys("Read fail");
	}
	
	*fd = localfd;
	return readcount1;

 }

 void processbuffer(int * myfd, int readcount){
	// if you change header, fix prototype.
	
	int bufferptr = 0;
	int tokenptr = 0;
	int counter = 0;
	int offset = 0;
	int records = 0;
	int localfd = *myfd;
	int chunk = 0; //boolean 0 is false , 1 is true
	char *saveptr;
	char * fullline;
	if(buffer[readcount-1] != '\n'){
		chunk = 1;
		while(bufferptr < readcount){
			if(buffer[bufferptr] == '\n'){
				records++;
			}
			bufferptr++;
		}
	}
	
	char* token= strtok_r(buffer,"\n",&saveptr);	//MAGIC LINE

	while(token != NULL && counter < records){
		//fprintf(stderr,"%s",token);
		storeandsort(token);
		token= strtok_r(NULL,"\n",&saveptr);
		counter++;
	}
	if(chunk == 1){
		offset = strlen(token) * -1;
	}
	lseek(localfd,offset,SEEK_CUR);

 }


 void storeandsort(char * line){
	// if you change header, fix prototype.
	numorecords++;
	//fprintf(stderr,"Store and Sort has now been called %d times.\n",numorecords);
	char * saveptr2;
	char * token2 = strtok_r(line,",",&saveptr2);
	int counter = 1;
	record currentrecord;
	
	while(token2 != NULL && counter<10){
		if (counter == 2){
			strncpy(currentrecord.id,token2, 4);
		}
		if (counter ==3){
			strncpy(currentrecord.name,token2, 25);
		}
		if (counter ==8){
			currentrecord.pop = atoi(token2);
		}
		if (counter ==9){
			currentrecord.life = atof(token2);
		}
		counter++;
		token2 = strtok_r(NULL,",",&saveptr2);
	}
	fprintf(stderr,"Got here %d times.\n",numorecords);
	
	*storage = currentrecord; //Magic Line?

	fprintf(stderr,"Got to here %d times.\n",numorecords);
	storage = storage + sizeof(record);
	
	
	

 }

 void searcharray(){
	// if you change header, fix prototype.


 } 

 
 

 record * arrayalloc(int number){
	//This allocates the array. I think.
	int recsize = sizeof(record); //this stores the record size
	record *recordptr = (record *)calloc(number, recsize);
	if( recordptr == NULL){
		err_sys("Calloc Failed");
	}
	else{
		return recordptr;
	}
}



int openfile(){
	
	int retval = open("AllCountries.dat", O_RDONLY);
	if (retval < 0){
		err_sys("File Open Failed");		
	}
	else{
		return retval;
	}

}
