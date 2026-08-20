


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEM 1000
//MAX_MEM >= voc size
#define MAX_WLEN 50

int max_reader_score = 10;

int reader_score;

bool end_session = false;

struct memoroid {
char L1[MAX_WLEN];
char L2[MAX_WLEN];
int score;
};

struct memoroid memory[MAX_MEM];

int read_voc()
{
FILE* ptr=fopen("memvoc.txt","r");
if (ptr==NULL) {
perror("error opening file");
return 1;
}
char buf[300];
int i=0;
while (i<MAX_MEM && fgets(buf,100,ptr)) {
char* tok=strtok(buf,";");
if (tok==NULL) continue;
strncpy(memory[i].L1,tok,sizeof(memory[i].L1)-1);
memory[i].L1[sizeof(memory[i].L1)-1]='\0';
tok=strtok(NULL,";");
if (tok==NULL) continue;
strncpy(memory[i].L2,tok,sizeof(memory[i].L2)-1);
memory[i].L2[sizeof(memory[i].L2)-1]='\0';
tok=strtok(NULL,";");
if (tok==NULL) continue;
memory[i].score=atoi(tok);
i++;
}
memory[i+1].L1[0]='\0';
fclose(ptr);
return 0;
}

int m_len() {
int l=0; //memory size
while (strcmp(memory[l].L1,"\0")>0) {l++;}
return l;
}

int memory_len;

//random number gen (min,max]
int rn(int min, int max) {
return min+rand()%(max-min)+1;}

//save mem progress
int save_prog() {
FILE *f=fopen("memvoc.txt","w");
if (f==NULL) {
printf("Error opening file.\n");
exit(1);
}

int i;
for (i=0;memory[i].L1[0]!='\0';i++){
fprintf(f,"%s;%s;%d\n",memory[i].L1,memory[i].L2,memory[i].score);
}

fclose(f);
}

//choices
bool multichoice (int test_word_ind) {
bool correct=false;
int a1;
int a2;
int a3;
int maxind;

maxind=m_len();
a1=rn(-1,maxind-1);
a2=rn(-1,maxind-1);
a3=rn(-1,maxind-1);

int choices[3]={a1,a2,a3};

int ci; //correct index, plant the answer
ci=rn(0,3);
choices[ci-1]=test_word_ind;
printf("%s\n",memory[test_word_ind].L1);
printf("1)%s,2)%s,3)%s\n",memory[choices[0]].L2,memory[choices[1]].L2,memory[choices[2]].L2);

int ki=1;
while(!scanf("%d",&ki)){getchar();} //clear bad input from stdin
if (ki==4) {
end_session=true;
save_prog();
}
else if (ki==ci) {printf("correct\n");correct=true;}
else {printf("%s=%s\n",memory[test_word_ind].L1,memory[test_word_ind].L2);}


return correct;
}


bool completion_check() {
bool completion=true;
int i;
for (i=0;i<memory_len;i++){
if (memory[i].score<max_reader_score) {completion=false;}
}
if (completion) {save_prog();printf("set is completed");}

return completion;
}
//set completed when every word reaches max_reader_score



void main(){

srand(time(NULL));

reader_score = rn(-1,max_reader_score-1);

read_voc();

memory_len=m_len();

while (true) {
int i;
for (i=0;i<memory_len;++i){
if (end_session==true) {break;}
if (memory[i].score==reader_score) {
if (multichoice(i)) {memory[i].score+=1;}
printf("%s %d/%d\n",memory[i].L1,memory[i].score,max_reader_score);
}
}

reader_score++;

if (reader_score>=max_reader_score) {reader_score=0;}

if (completion_check()) {break;}

if (end_session==true) {printf("end of session\n");break;}

}
}
