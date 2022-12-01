#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include<string.h>
int main( int argc, char** argv ){
    srandom(0);
    if( argc==4 ){
        // Declaramos um ponteiro(link para o endereço da memória) para o arquivo de nome: 'pf'
        FILE *pf;
        int n = atoi(argv[2]);
        int m = atoi(argv[3]);

        printf("\nO CONTEÚDO DO ARQUIVO É:\n");
        //Abre o arquivo novamente para leitura
        int count = 0;

        pf=fopen(argv[1], "r");
        //reads how many chars there are in the file
        for(char c=getc(pf);c!=EOF;c=getc(pf)){
            count++;
        }

        fclose(pf);
        //generate random numbers betwen the limits of the file simbolising the localization of the first char in the text
        int * numbers = (int*)malloc(n * sizeof(int));;
        for(int i = 0 ; i<n;i++){
            int testing = random()%(count-m);
            numbers[i] = testing;
        }



        int itr=0;
        char *result = (char *)malloc(m* sizeof(char));
        result[m] ='\0';
        //loop while every word is printed
        while(itr!=n){
            pf=fopen(argv[1], "r");
        int conting=0 , index = 0;

        for(char c=getc(pf);c!=EOF;c=getc(pf)){
            //printf("%d\n",conting);
            if(numbers[itr]<=conting && numbers[itr]+m>conting){
//substitui \n por " "
                if(c=='\n'){
                    result[index] =' ';
                }else{
                    result[index] =c;
                }
                index++;
            }
            if(numbers[itr]+m==conting-1 ){
                    index = 0;
                    itr++;
                    printf("<%s>\n",result);
                    break;
                }

            conting++;
        }

        fclose(pf);
        }
        free(result);
        free(numbers);

        // Le em conteudo o valor da variável armazenada anteriormente pf
    }else{
        printf("code is Ex.: ./ex1 arquivo.txt m n\n");
    }
    return(0);
}