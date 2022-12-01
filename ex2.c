#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
void txt2epb(char *string){
    char* filename = malloc(1124*sizeof(char));
    strcpy(filename,string);
    char* command="pandoc";
    char* epub = strtok(string,".");
    strcat(epub,".epub");
    char *args[] = {command,"--quiet", filename, "-o",epub, NULL};
    if(execvp(command,args)== -1){
                perror("execvp");
            }
            free(filename);
    }
char* copyString(char s[])
{
    char* s2;
    s2 = malloc(1124*sizeof(char));

    strcpy(s2, s);
    return (char*)s2;
}
int main(int argc, char *argv[]){
    pid_t pid=1,wpid;
    int MAXSIZE =1124;
    char* comando=malloc(MAXSIZE*sizeof(char));
    int num=0,status;
    while(pid>=1){
        //parent
        if((argc-1)==num){
            break;
        }
        num++;
        pid = fork();
    }
    if(pid==0){
    //child
    printf("[pid%d] converting %s\n",getpid(),argv[num]);
    char buff[1052];
    strcpy (buff, argv[num]);
    txt2epb(buff);
    exit(pid);
}else{
    while ((wpid = wait(NULL)) > 0);

}
char* tmp1= malloc(MAXSIZE*sizeof(char));
char* tmp = malloc(MAXSIZE*sizeof(char));
char **list_args= malloc((argc+3)*sizeof(char*));
list_args[0]="zip";
list_args[1]="ebooks.zip";
list_args[2]="--quiet";

for (int i = 1;i<argc;i++){
    /*printf("%s\n",argv[i]);*/
    strcpy(tmp, argv[i]);
    tmp1 = strtok(tmp, ".");
    /*printf("%s\n",tmp1);*/
    strcat(tmp1,".epub");
    /*printf("%s\n",tmp1);*/
    list_args[i+2]=copyString(tmp1);
}
execvp("zip",list_args);
free(list_args);
free(tmp);
}