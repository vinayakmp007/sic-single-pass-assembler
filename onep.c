#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define _OPTAB_ "optab.txt"
#define _INFILE_ "input.txt"
#define _OUTFILE_ "out.txt"
int optcount=0,symcount=0;

struct optab
{
	char opcode[10];
	int val;
}op[100];

typedef struct addre{
	int address;
	char code_line[200];
	struct addre *next;

}sym_addr;

struct symtab
{
	char name[30];
	int val;
	int def;                 //0 if not defined and 1 if defined
	sym_addr *start;
}sym[100];
int searchop(char* o);
void sepcoma(char *str,char *r,char *p);
int makeob(int opcode,int ind,int oprend);
int vget(char* line,char* label,char* opc,char* opr)
{
	int i=0,j=0;
	label[0]='\0';
	opc[0]='\0';
	opr[0]='\0';

	for(i=0,j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		label[j]=line[i];
	i++;
	for(;((i<=strlen(line))&&(line[i]==' '));i++);
	label[j]='\0';
	for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		opc[j]=line[i];
	i++;
	for(;((i<=strlen(line))&&(line[i]==' '));i++);
	opc[j]='\0';
	for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		opr[j]=line[i];
	opr[j]='\0';
}

int write_textrec(FILE*,int,int,int,int);
int search_sym(const char *name){                                            //returns 2 if symbol is not present in symbol table

	int i=0;
	for(i=0;i<=symcount;i++){
		if(strcmp(sym[i].name,name)==0)return i;
	}
	return -2;
}

int is_sym_def(const char *name)
{
int k=search_sym(name);
if(k==-2)return 0;                          //check the return here???
else return sym[k].def;

}
int get_sym_value(const char* name){
	int k=search_sym(name);
	if(k==-2)return -1;
	else return sym[k].val;
}
void add_sym_undef(char* symname,int address,const char* codeline)
{
	int k=search_sym(symname);
	sym_addr *po,*t;
	if(k==-2){

		strcpy(sym[symcount].name,symname);
		sym[symcount].val=-1;
		sym[symcount].def=0;
		po=(sym_addr*)malloc(sizeof(sym_addr));
		po->address=address;
		strcpy(po->code_line,codeline);
		po->next=NULL;
		sym[symcount].start=po;
		symcount++;
		printf("Added symbol %s to list start %06X\n",symname,po->address);

	}

	else
	{
		t=sym[k].start;
		while(t->next!=NULL){
			t=t->next;
		}

		po=(sym_addr*)malloc(sizeof(sym_addr));
		po->address=address;
		strcpy(po->code_line,codeline);
		po->next=NULL;
		t->next=po;
		printf("Added addres to Symbol %s with adddres %06X\n",symname,po->address);
	}

}

void add_sym_def(char* symname,int value,FILE *fout){
	int k=search_sym(symname),operand=0,wordb=0,opcode=0;
	sym_addr *po,*t;
	char label[100],opr1[100],opr2[10],opc[100],opr[200],li[300],count=0,ind=0;
	if(k==-2){
		strcpy(sym[symcount].name,symname);
		sym[symcount].val=value;
		sym[symcount].def=1;
		sym[symcount].start=NULL;
		symcount++;
	}
	else {
		if(is_sym_def(symname)){printf("ERROR:%s has already been defined \n",symname);

		}
		else

		{
			sym[k].def=1;
			sym[k].val=value;
			po=sym[k].start;
			sym[k].start=NULL;
			if(po!=NULL)                 //just in case
			{
				while(po!=NULL){

					strcpy(li,po->code_line);
					printf("\n\n\nWriting %s li",li);
					vget(li,label,opc,opr);
					sepcoma(opr,opr1,opr2);
					if(opr2[0]=='\0')ind=0;
					else ind=1;
					operand=value;

					opcode=searchop(opc);

					wordb=makeob(opcode, ind, operand);
					printf("\n %06X %02X %06X %06X   at adress %06X\n",opcode,ind,operand,wordb,po->address);

					write_textrec(fout,po->address, 3, wordb, 1);
					t=po;
					po=po->next;
					free(t);


				}
			}

		}
	}

}




void loadop(){
	FILE *p=fopen(_OPTAB_,"r");
	int opv;
	char li[50],opc[10],opvc[10],opp[10];
	int i=0;
	if(!p)printf("\nUnable to open file");
	do{
		fgets(li,100,p);
		vget(li,opc,opvc,opp);
		strcpy(op[i].opcode,opc);
		sscanf(opvc,"%x",&op[i].val);
		//printf("\n%s + %d",op[i].opcode,op[i].val);
		//scanf("%d",&j);
		i++;
	}while(!feof(p));
	optcount=i;
	/*for(i=0;i<optcount;i++)
	printf("\n%s + %d",op[i].opcode,op[i].val);*/

}


int print_op(){
	for(int i=0;i<optcount;i++)
		printf("%s    %d \n",op[i].opcode,op[i].val);

}

int searchop(char* o){

	for(int i=0;i<optcount;i++)
		if(strcmp(op[i].opcode,o)==0){
			return op[i].val;
		}

	return -1;
}


int skipcomment(char* p){
	int k=strlen(p);
	for(int i=0;i<k;i++)
		{
		if(p[i]==46){
		p[i]='\0';
		return 1;}
		}
	return 0;
	}
void sepcoma(char *str,char *r,char *p)
{
int i=0,j=0;
r[0]='\0';
p[0]='\0';
for(;(str[i]!='\0'&&str[i]!=',');i++)
{
r[j]=str[i];
j++;
}
r[j]='\0';
if(str[i]!='\0'){
i++;

j=0;
for(;(str[i]!='\0');i++)
{
p[j]=str[i];
j++;
}
p[j]='\0';
}
}

int vget2(char* line,char* loc,char* label,char* opc,char* opr)
{int i,j=0;
label[0]='\0';
opc[0]='\0';
opr[0]='\0';
opr[1]='\0';
for(;((i<=strlen(line))&&(line[i]==' '));i++);
for(i=0,j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
loc[j]=line[i];
i++;
loc[j]='\0';
for(;((i<=strlen(line))&&(line[i]==' '));i++);
for(j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
label[j]=line[i];
i++;
for(;((i<=strlen(line))&&(line[i]==' '));i++);
label[j]='\0';
for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
opc[j]=line[i];
i++;
for(;((i<=strlen(line))&&(line[i]==' '));i++);
opc[j]='\0';
for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
opr[j]=line[i];
for(;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]==' '));i++);
opr[j]='\0';
}

int makeob(int opcode,int ind,int oprend)
{
int obj=0;
obj=opcode;
obj=obj<<1;
obj=obj+ind;
obj=obj<<15;
obj=obj+oprend;
//printf("%d\n",obj);
return obj;
}
int getbyte(char *p){
int d;
sscanf(p,"%x",&d);

return d;

}

int rstart2(const char* filein){
char li[100],opc[100],opr[100],label[100],loc[100];
int r=0;
FILE *p=fopen(filein,"r");
fgets(li,100,p);
if(feof(p)){fclose(p);return r;}
 vget(li,label,opc,opr);
if(strcmp(opc,"START")==0){

sscanf(opr,"%X",&r);

}
fclose(p);
return r;
}


int isnulls(char* p){
	for(int i=0;i<strlen(p);i++)
		{
		if((p[i]!=' ')&&(p[i]!='\t')&&(p[i]!='\n')&&(p[i]!='\r'))
			return 1;
		}
	return 0;
	}

void one_pass(const char* input,const char* output)
{
	FILE *fin=fopen(input,"r");
	FILE *fout=fopen(output,"w");
	int start=rstart2(input);
	int end=0;
	int count=start,flag=0,temp=0,cmnt=0,fl=0,jj=0,wordb=0,opcode=0,operand=0,ind=0;
	char label[100],opc[100],opr[100],li[200],startadd[10],opr1[50],opr2[10],byt[8],pname[10];

	sprintf(startadd,"%06X",start);

	if(!fout||!fin){printf("\nFile ERROR: :P");return;}

	do{
        fgets(li,100,fin);
        cmnt=skipcomment(li);
        if(!isnulls(li))continue;
        if(feof(fin))break;

        vget(li,label,opc,opr);
        sepcoma(opr,opr1,opr2);
        
        if(strcmp(label,"")!=0)                                                   //label definition
			{
			//strcpy(sym[symcount].symbol,label);
			//sym[symcount++].val=count;
				add_sym_def(label,count, fout);

			}
		if((opcode=searchop(opc))>=0){                                              //opcode

			if(is_sym_def(opr1)){

				operand=get_sym_value(opr1);
				if(opr2[0]=='\0')ind=0;
				else ind=1;
				wordb=makeob(opcode, ind, operand);
				write_textrec(fout, count, 3, wordb, 0);

			}
			else if (opr1[0]!='\0'){
				add_sym_undef(opr1, count, li);
				wordb=makeob(opcode, ind, 0);
				write_textrec(fout, count, 3, wordb, 0);
				//	printf("Added");
			}
			else {
				wordb=makeob(opcode, ind, 0);
				write_textrec(fout, count, 3, wordb, 0);


			}

			count+=3;
			}
		else if(strcmp(opc,"RESW")==0){
		
			write_textrec(fout,0,0,0,2);

			sscanf(opr,"%d",&temp);
			count+=3*temp;
			}
		else if(strcmp(opc,"RESB")==0){
		
			write_textrec(fout,0,0,0,2);
			sscanf(opr,"%d",&temp);
			count+=1*temp;
			}
		else if(strcmp(opc,"BYTE")==0){
			
			
			
			
			
			if(opr[0]=='C'){
			
			fl=0;
		for(int ii=2;opr[ii]!='\''&&fl!=1;){
		wordb=0;
		for(jj=0;jj<3&&fl!=1;jj++,ii++){
		if(opr[ii]=='\'')fl=1;
		wordb=wordb<<8;
		if(fl!=1)wordb+=opr[ii];
		
		}
		count+=jj;
		write_textrec(fout, count-jj,jj, wordb, 0);

		
		
		}	
			}
			
			else if(opr[0]=='X'){
			temp=strlen(opr)-3;
			temp=temp/2+temp%2;
			
			fl=0;
		for(int ii=2;opr[ii]!='\''&&fl!=1;){
		wordb=0;
		for(jj=0;jj<3&&fl==0;jj++){
		
		byt[0]=0;
		byt[1]=0;
		byt[2]=0;
		for(int pp=0;opr[ii]!='\''&&pp<2;pp++){
		byt[pp]=opr[ii];
		ii++;                                       //byte to eord
				}  
				  
				                                        //herer
		if(opr[ii]=='\'')fl=1;
		wordb=wordb<<8;
		wordb+=getbyte(byt);
		
		}
		
		count+=jj;
		write_textrec(fout, count-jj, jj, wordb, 0);
		
		
		}
			
			
			}
			
			
			
			}

		else if(strcmp(opc,"WORD")==0){
			
			
			wordb=0;
                        sscanf(opr,"%d",&wordb);
			write_textrec(fout, count, 3, wordb, 0);
		
			
			count+=3;
			
			
			
			
			}
		else if(strcmp(opc,"START")==0){
			///ignore
			strcpy(pname,label);
				printf("\nStart found at %06X",start);
				fprintf(fout,"H^%-6s^%06X^%06X\n",label,count,0);

			}
		else if(strcmp(opc,"END")==0){					//When end has been found
			printf("\nEnd found at %06X",count);
			write_textrec(fout, 0, 0, 0, 2);
			end=count;
						fprintf(fout,"E^%06X",start);
						fseek(fout, 0, 0);
						fprintf(fout,"H^%-6s^%06X^%06X\n",pname,start,end-start);
							}


		else {
		printf("invalid opcode %s",opc);
		return ;
		}


	}while(!(feof(fin)||(strcmp(opc,"END")==0)));


}
int write_textrec(FILE *fil,int addr,int bytesize,int word,int new){         //when new==0 default        
static int tsize=0,tadd=0;                                                   //when new==1 for new single text record
static char tline[500];                                                      //when new==2 flushes the line to text record
char word_s[15],*ch;
if(bytesize==3)sprintf(word_s,"%06X",word);
else if(bytesize==2)sprintf(word_s,"%04X",word);
else if(bytesize==1)sprintf(word_s,"%02X",word);
if(new==0){
if(tsize>27){
fprintf(fil,"T^%06X^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
if(tsize==0)
{
tadd=addr;
}

strcat(tline,"^");
strcat(tline,word_s);
tsize+=bytesize;
	
}
else if(new==1)
{
	if(tsize>0)fprintf(fil,"T^%06X^%02X%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;

if(tsize==0)
{
tadd=addr;
}

strcat(tline,"^");
ch=word_s+2;                          //forward refernce is written here
tadd=tadd+1;
strcat(tline,ch);
tsize+=bytesize-1;

fprintf(fil,"T^%06X^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;


}

else if(new==2){                                               //flushes the tline to object file
if(tsize>0)fprintf(fil,"T^%06X^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;


}

}

int main()
{
loadop();
print_op();
one_pass(_INFILE_,_OUTFILE_);
return 0;
}


