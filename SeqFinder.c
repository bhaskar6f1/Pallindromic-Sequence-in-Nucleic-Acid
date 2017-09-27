//
//  inversepos.c
//
//  Created by Bhaskar on 02/01/17.
//  Copyright © 2017 Apple. All rights reserved.
//

/*************************************************************
 @author - Bhaskar Kumar Mishra
 
 COMMAND - ./SeqFinder -srt /Stuffs/Work/LanguageModel/LanguageModel/genome.faa.srt -lcp /Stuffs/Work/LanguageModel/LanguageModel/genome.faa.srt.lcp -output /Stuffs/Work/LanguageModel/LanguageModel/output.txt
 
 Input - allowed gap length (can be 0 too)
 
 ASSUMPTION -
 1). Gap can be present after one complete string
 i.e, in this form ->  x1....GGA[gap]TCC....x1'
 and not at other position i.e, -> x1....GG[gap]ATCC....x1'
 **************************************************************/

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define SUPERLEN 9999999
#define FILENAMELEN 100
#define REVPALLSIZE 99999
#define MINPALLLEN 4

void writeToFile(FILE *, unsigned int,char *);
void printMessage();

struct pos{
    unsigned int low;
    unsigned int high;
    unsigned int lcp;
    int gapStart;
    int gapEnd;
} arr[REVPALLSIZE];

unsigned int allowedLength= MINPALLLEN;
int threshold=0;
char srtFileName[FILENAMELEN], lcpFileName[FILENAMELEN], outputFile[FILENAMELEN];

unsigned int readSuffixArray(FILE * fin,char superstring[SUPERLEN], char *ptr[SUPERLEN]){
    int counter;
    unsigned int i, ssl;
    unsigned int disp;
    if (!fin){
        printf("Unable to open file!");
    }
    int num=fread(&ssl, sizeof(unsigned int), 1, fin);
    if (!num){
        printf ("ERROR: could not read superstrlen from file\n");
    }
    unsigned int k;
	 	 for (i=0;i<ssl;i++){
             if ((k = fread ((void *)&disp, sizeof(unsigned int), 1, fin)) < 1){
                 fprintf(stderr,"Wrote only %u elements of disp\n",i);
                 perror("Could not write disp");
             }
             ptr[i]=superstring + disp;
             // printf ("%u ", disp);
             
         }
    if ((k = fread (superstring, sizeof(char), ssl, fin)) < ssl){
      		fprintf(stderr,"Wrote only %u elements of Genome-String\n",k);
        
  		}
  		superstring[ssl]='\0';
    fclose(fin);
    return ssl;
}

void readLCPArray(FILE * fin,unsigned int * lcp){
    unsigned int i, ssl;
  		if (!fread(&ssl, sizeof(unsigned int), 1, fin)){
            
        }
  		printf ("\nINFO: Read SSL from LCP file. %u\n", ssl);
  		if ((i = fread((void *)lcp, sizeof(unsigned int), ssl, fin)) < ssl){
            
        }
    //    printf ("\nLCP aray is: \n");
    //    for(i=0;i<ssl;i++){
    //        printf ("%u ", lcp[i]);
    //    }
    
}

/**
 finds the reverse pallindromic sequence
 @params
 ptr - suffix array
 lcp - longest common prefix array
 ssl - total length of the string (i.e, the original string with its reverse complement)
 threshold - max gap length allowed
 */
unsigned int findInverse(char * superstring,char ** ptr,unsigned int * lcp, unsigned int ssl, int threshold){
    unsigned int i=0,j=0,len=strlen(superstring)/2;
    unsigned int size = 0;
    for(i=1;i<ssl;i++){
        unsigned int fInd=ptr[i-1]-superstring;
        unsigned int sInd=ptr[i]-superstring;
        if(fInd <sInd){
            unsigned int temp=fInd;
            fInd=sInd;
            sInd=temp;
        }
        if((fInd<len && sInd<len) || (fInd>len && sInd>len) ){
            continue;
        }
        unsigned int checkIndexUpper=len<sInd+(2*lcp[i]+threshold -1)-1?0:len-(sInd+(2*lcp[i]+threshold -1))-1;
        unsigned int checkIndexLower=len<(sInd+(lcp[i]-1))-1?0:len-(sInd+(lcp[i]-1))-1;
        if(lcp[i]>=(allowedLength/2)){
            unsigned  int j=i;
            while(lcp[j]==lcp[i]){
                if(fInd>=len + checkIndexUpper && fInd<=len+checkIndexLower && len-sInd>=MINPALLLEN){
                    unsigned int indF=fInd+lcp[i];
                    unsigned int indS=sInd+lcp[i];
                    unsigned int gapCount=0;
                    unsigned int matchCount=0;
                    while(gapCount <= threshold && matchCount<lcp[i] && indF<ssl-1 && indS<ssl-1){
                        if(superstring[indF]!=superstring[indS]){
                            if(matchCount>0){
                                gapCount+=matchCount;
                            }
                            matchCount=0;
                            gapCount++;
                        }
                        else{
                            matchCount++;
                        }
                        indF++;
                        indS++;
                    }
                    int test=-100;
                    int flag=0;
                    unsigned int low=sInd;
                    unsigned int high=sInd + 2*lcp[i] + gapCount -1;
                    unsigned int tlcp=2*lcp[i];
                    if(matchCount!=lcp[i] && lcp[i]>=allowedLength){
                        high=sInd + lcp[i]-1;
                        tlcp=lcp[i];
                        gapCount=0;
                        test=-1;
                    }else if(matchCount!=lcp[i]){
                        break;
                    }
                    if(high>len){
                        break;
                    }
                    
                    if(superstring[low]==superstring[high]){
                        j++;
                        continue;
                    }
                    unsigned int k=0;
                    if(size>0){
                        for(k=0;k<size;k++){
                            if(arr[k].low<=low && arr[k].high>=high){
                                flag=1;
                                break;
                            }
                            if (arr[k].low >=low && arr[k].high<=high) {
                                flag=2;
                                break;
                            }
                        }
                    }
                    if(flag==1){
                        break;
                    }
                    int inc=0;
                    if(flag==0){
                        k=size;
                        inc=1;
                        flag=2;
                    }
                    if(flag==2 && (lcp[i]>=allowedLength || matchCount==lcp[i])){
                        arr[k].low=low;
                        arr[k].high=high;
                        arr[k].lcp=tlcp;
                        if(gapCount>0){
                            arr[k].gapStart=sInd + lcp[i];
                            arr[k].gapEnd= arr[k].gapStart +gapCount -1;
                        }else{
                            arr[k].gapStart=0;
                            arr[k].gapEnd=0;
                        }
                        size+=inc;
                    }
                    break;
                }
                j++;
            }
        }
    }
    return size;
}


void parse_arguments(int argC, char **argV){
    
    int i,j,strNum=1, charNum;
    char *Switch[]={"srt","lcp","output","##END##"};
    typedef enum   { srt, lcp,output, f_end
    }Params;
    srtFileName[0]='\0';
    lcpFileName[0]='\0';
    outputFile[0]='\0';
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
                case srt:
                    strcpy(srtFileName,argV[strNum+1]);
                    strNum += 2;
                    continue;
                case lcp:
                    strcpy(lcpFileName,argV[strNum+1]);
                    strNum += 2;
                    continue;
                    
                case output:
                    strcpy(outputFile,argV[strNum+1]);
                    strNum += 2;
                    continue;
                    
            }
        }
        strNum++; // This statement is reached only if the '-' didnt match above
    }
}
void writeToFile(FILE * fout,unsigned int size, char * str){
    fprintf (fout, "%u\n\n", size);
    unsigned int i=1;
    for(i=0;i<size;i++){
        fprintf (fout, "\n%u). %u %u %u %u %u ",i,arr[i].low,arr[i].high,arr[i].lcp,arr[i].gapStart,arr[i].gapEnd);
        unsigned int j=arr[i].low;
        unsigned int len=arr[i].high+1;
        for(;j<len;j++){
            fprintf (fout, "%c",str[j]);
        }
    }
    
    
}
void printMessage(){
    printf("Suffix Array File Name : %s\n", srtFileName);
    printf("LCP Array File Name    : %s\n", lcpFileName);
    printf("Output File Name       : %s\n", outputFile);
    printf("Allowed Gap Length     : %d\n", threshold);
}

int main(int argc, char **argv)
{
    char **ptr;
    char *superstring, *str ,** ngramList;
    unsigned int *lcp,*ngramCount,i=0;
    superstring = (char *) malloc (sizeof(char) * SUPERLEN);
    ptr = (char **) malloc (sizeof(char *) * SUPERLEN);
    FILE *fin, * fin3;
    
    parse_arguments(argc, argv);
    if(strlen(srtFileName)==0){
        printf ("\nERROR: Please pass the suffix Array fileName correcyly in the commandline\n");
        exit(0);
    }else{
        fin=fopen(srtFileName,"rb");
        if (!fin){
            printf("ERROR: Unable to open file!");
            exit(0);
        }
    }
    
    if(strlen(lcpFileName)==0 ){
        printf ("\nERROR: Please pass the lcp array fileName correcyly in the commandline\n");
        exit(0);
    }else{
        fin3=fopen(lcpFileName,"rb");
        if (!fin3){
            printf("ERROR: Unable to open file!");
            exit(0);
        }
    }
    
    printf("Enter the value of allowed gap:");
    fflush(stdout);
    scanf("%d",&threshold);
    
    unsigned int ssl=readSuffixArray(fin,superstring,ptr);
    lcp = (unsigned int *) malloc (sizeof(unsigned int) * ssl);
    readLCPArray(fin3,lcp);
    
    unsigned int size=findInverse(superstring,ptr,lcp,ssl,threshold);
    
    FILE *fout = NULL;
    if(strlen(outputFile)==0 ){
        printf ("\nERROR: Please pass the output fileName correcyly in the commandline\n");
        exit(0);
    }else{
        fout=fopen(outputFile,"w");
        if (!fout){
            printf("ERROR: Unable to open file!");
            exit(0);
        }
        writeToFile(fout,size,superstring);
    }
    
    printMessage();
    
    fclose(fin);
    fclose(fin3);
    fclose(fout);
    return 0;
}


