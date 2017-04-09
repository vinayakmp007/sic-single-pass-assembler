#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define _OPTAB_ "optab.txt"

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

int search_sym(const char *name){                                            //returns 2 if symbol is not present in symbol table

	int i=0;
	for(i=0;i<=symcount;i++){
		if(strcmp(sym[i].name,name)==0)return i;
	}
	return -2;
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


	}

	else
	{
		t=sym[k].start;
		while(t->next!=NULL){
			t=t->next;
		}

		po=(sym_addr*)malloc(sizeof(sym_addr));
		po->address=address;
		strcpy((po->code_line,codeline);
		po->next=NULL;
		t->next=po;

	}

}

void add_sysm_def(char* symname,int address,const char* codeline){

}


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
void sepcoma(char *str,char* r,char*p)
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
vget2(li,loc,label,opc,opr);
if(strcmp(opc,"START")==0){

sscanf(opr,"%d",&r);

}
fclose(p);
return r;
}

int rend2(const char* filein){
char li[100],opc[100],opr[100],label[100],loc[100];
int r=0,fl=0;
FILE *p=fopen(filein,"r");
if(!p)printf("\nUnable to open file");
printf("\nLooking for end");
do{
fgets(li,100,p);
vget2(li,loc,label,opc,opr);
if(strcmp(label,"END")==0){
printf("\nEnd found at %s\n",loc);
sscanf(loc,"%x",&r);
fl=1;
}

}while(!fl);



fclose(p);
return r;
}

void one_pass(const char* input,const char* output)
{
	FILE *fin=fopen(input,"r");
	FILE *fout=fopen(output,"w");
	int start=rstart(input);

	int count=start,flag=0,temp=0,cmnt=0;
	char label[100],opc[100],opr[100],li[200],startadd[10];

	sprintf(startadd,"%06X",start);

	if(!fout||!fin){printf("\nFile not found");return;}

	do{
        fgets(li,100,fin);
        cmnt=skipcomment(li);
        if(!isnulls(li))continue;
        if(feof(fin))break;




	}while(!(feof(fin)||(strcmp(opc,"END")==0)));


}


