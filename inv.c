//
//  inverse.c
//
//  Created by Bhaskar on 02/01/17.
//  Copyright © 2017 Apple. All rights reserved.
//

/*************************************************************
  @author - Bhaskar Kumar Mishra
 
 COMMAND - ./inv -input /Stuffs/Work/LanguageModel/InvertedSequence/chrM.fa -output /Stuffs/Work/LanguageModel/InvertedSequence/out1.txt
 
 Output -
 1). string concatenated with its reversecomplement
 **************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<ctype.h>
#define SUPERLEN 999999
#define BUFFER 600001
#define FILENAMELEN 100

void printMessage();
char inputFile[FILENAMELEN], strComplementFile[FILENAMELEN];

unsigned int readRawFile (FILE * fin, char *str){
    char string[SUPERLEN],buffer[BUFFER];
    str[0]='\0';
    buffer[0]='\0';
    unsigned int i=0,l=0;
    do {
        string[0]='\0';
        fgets(string,SUPERLEN,fin);
        if (string[0] == '>'){
            strcat (buffer+l, " ");
            l++;
            continue;
        }
        i=strlen(string);
        if (i){
            if (string[i-1] == '\n')
                string[--i] = '\0';
            strcat (buffer+l,string);
        }
        l+=i;
        if (l> 500000){
            strcat(str, buffer);
            buffer[0]='\0';
            l=0;
            printf("."); fflush(stdout);
        }
    } while (!feof (fin));
    fclose (fin);
    // strcat (buffer, " ");
    strcat(str, buffer);
    
    return (strlen(str));
}

void readFile(FILE * fin,char superstring[SUPERLEN]){
    unsigned int i, ssl,k;
  		if (!fread(&ssl, sizeof(unsigned int), 1, fin)){
        }
  		printf ("INFO: Read SSL from LCP file. %u\n", ssl);
  		if ((k = fread (superstring, sizeof(char), ssl, fin)) < ssl){
            fprintf(stderr,"Wrote only %u elements of Genome-String\n",k);
        }
  		superstring[ssl]='\0';
}

void writeFile(FILE * fout,char * str,char *revstr){
    fprintf (fout,"%s%s",str,revstr);
    
}
void generateInvertedSeq(char * str, unsigned int size, char * inverseTable, char * invSeq){
    int i=0;
    printf("\nSize: %u\n",size);
    for(i=size-1;i>=0;i--){
        str[i]=toupper(str[i]);
        if(str[i]>='A' && str[i]<='T'){
            invSeq[size-(i+1)]=inverseTable[str[i]-65];
        }else{
            invSeq[size-(i+1)]=str[i];
        }
    }
}
void parse_arguments(int argC, char **argV){
    
    int i,j,strNum=1, charNum;
    char *Switch[]={"input","output","##END##"};
    typedef enum   { input,output, f_end
    }Params;
    inputFile[0]='\0';
    strComplementFile[0]='\0';
    while(strNum < argC){
        if (argV[strNum][0] == '-'){
            for (i=0; i<f_end;i++)
                if(!strcmp(argV[strNum]+1,Switch[i]))
                    break;
            if (i==f_end)  {            /* The word doesnt match switch */
                fprintf(stderr,"\nInapproppriate Switch %s\tIgnored\n",argV[strNum]);
                strNum++;
                continue;
            }
            
            switch(i){
                case input:
                    strcpy(inputFile,argV[strNum+1]);
                    strNum += 2;
                    continue;
                    
                case output:
                    strcpy(strComplementFile,argV[strNum+1]);
                    strNum += 2;
                    continue;
                    
            }
        }
        strNum++; // This statement is reached only if the '-' didnt match above
    }
}
int main(int argc, char **argv)
{
    char *superstring,*invertedSeq;
    unsigned int tstart,tend,tdiff,i=0;
    superstring = (char *) malloc (sizeof(char) * SUPERLEN);
    invertedSeq = (char *) malloc (sizeof(char) * SUPERLEN);
    parse_arguments(argc, argv);
    FILE *fout;
    FILE *fin;
    if(strlen(inputFile)==0){
        printf ("\nERROR: Please pass the input string fileName correcyly in the commandline\n");
        exit(0);
    }else{
        fin=fopen(inputFile,"rb");
        if (!fin){
            printf("ERROR: Unable to open file!");
            exit(0);
        }
    }

    char inverseTable[26];
    for(i=0;i<26;i++){
        inverseTable[i]='#';
    }
    inverseTable['A'-65]='T';
    inverseTable['T'-65]='A';
    inverseTable['C'-65]='G';
    inverseTable['G'-65]='C';

    unsigned int size=readRawFile(fin,superstring);
    generateInvertedSeq(superstring,size,inverseTable,invertedSeq);
    
    if(strlen(strComplementFile)==0 ){
        printf ("\nERROR: Please pass the output fileName correcyly in the commandline\n");
        exit(0);
    }else{
        fout=fopen(strComplementFile,"w");
        if (!fout){
            printf("ERROR: Unable to open file!");
            exit(0);
        }
    }
    writeFile(fout,superstring,invertedSeq);
    printMessage();
    
    fclose(fout);
    fclose(fin);
    
    return 0;
}

void printMessage(){
    printf("Input File Name : %s\n", inputFile);
    printf("Output File Name       : %s\n", strComplementFile);

}
