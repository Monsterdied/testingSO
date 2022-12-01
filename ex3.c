#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/wait.h>
char fifoName[64] = {'\0'}; //fifoName passa a ser global e visível na função de tratamento
int n;	// = numFifos // passa a ser global para eliminar os FIFOs

/* Função para tratar sinal CTRL+C e encerrar processos */
void tratamento_sinal(int signo) {
	int i;
	/*fprintf(stderr, "\nSinal SIGINT (%d) recebido!", signo);*/
	for (i = 0; i < n ; i++) {
		sprintf(fifoName, "pipe%dto%d", 1+i, 1+(i+1)%n);
        unlink(fifoName)==0;
	} 
	kill(0, signo);	// terminar o resto do grupo de processos
	exit(1);
}
int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 1;
    }
    n = argv[1][0] - '0';

    double p = strtod(argv[2], NULL);
    double t = strtod(argv[3], NULL);

    /*printf("Creating Dynamic array\n");*/
    char** fifoNames = malloc(n * sizeof(char *));
    //Create FIFOs
    for (int i = 0; i < n; i++) {
        /*printf("Creating FIFO name\n");*/
        char *fifoName = (char*)malloc(8 * sizeof(char));
        sprintf(fifoName, "pipe%ito%i", i + 1, (i + 1) % n + 1);
        printf("%i: %s\n", i+1, fifoName);
        fifoNames[i] = fifoName;
        char *tmp = fifoNames[i];
        /*printf("fifoNames[%i]: %s\n", i, tmp);*/

        /*printf("Creating FIFOs\n");*/
        if (mkfifo(fifoName, 0666) == -1) {
            if (errno != EEXIST) {
                printf("Could not create fifo file\n");
                return 2;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        kill(getpid() + 1 + i, SIGKILL);
    }

    int counter = 0;
        { 
        struct sigaction novo, anterior;
        sigset_t smask;	//mascara de sinais a bloquear durante func()
        // preparar struct sigaction
        if (sigemptyset(&smask)==-1)	// n bloquear outros sinais
            perror ("sigsetfunctions");
        novo.sa_handler = tratamento_sinal;
        novo.sa_mask = smask;
        novo.sa_flags = 0;	// costuma ser suficiente

        if(sigaction(SIGINT, &novo, &anterior) == -1)
            perror ("sigaction()");
    } 
    /*printf("Recording parent pid\n");*/
    pid_t parent = getpid();
    for (int i = 0; i < n; i++) {
        if (getpid() == parent) {
            fork();
        }
    }
    srand(time(NULL) ^ (getpid()<<16));


    if (getpid() == parent) {
        /*printf("Writing token on first FIFO\n");*/
        int tokenTmp = 0;
        char *fifoName = fifoNames[0];
        /*printf("FIFO name: %s\n", fifoName);*/

        int fdFirst = open(fifoName, O_WRONLY);
        if (fdFirst == -1) {
            printf("Can't open first file\n");
            return -1;
        }

        if (write(fdFirst, &tokenTmp, sizeof(int)) == -1) {
            printf("Can't write\n");
            return -1;
        }
    }

    /*int fd = ptr[0];
    printf("Writes first token\n");
    printf("%i\n", fd);*/

    /*printf("Time\n");*/

    
    /*printf("Start loop\n");*/
srand(time(NULL)+getpid());
    while (getpid() != parent) {
        int i = getpid() - parent - 1;
        /*printf("Get FIFO names\n");*/
        char *fifoName1 = fifoNames[i];
        int fd = open(fifoName1, O_RDONLY);
        if (fd == -1) {
            printf("Can't open file\n");
            return 5;
        }

        int token;
        if (read(fd, &token, sizeof(int)) == -1) {
            printf("Can't read file\n");
            return 6;
        }

        token++;
        /*printf("%s: token %i->%i\n", fifoName1, token - 1, token);*/
        close(fd);

        double r = (rand() % 10000) / 10000.0;
        if (r < p) {
            /*printf(" %.0f seconds\n", t);*/
            printf("[p%c]: loocked token %i\n", fifoName1[4],token);
            sleep(t);
            printf("[p%c]unlocked\n",fifoName1[4]);
        }

        char *fifoName2 = fifoNames[(i + 1) % n];
        fd = open(fifoName2, O_WRONLY);
        if (fd == -1) {
            printf("Can't open file\n");
            return 7;
        }

        if (write(fd, &token, sizeof(int)) == -1) {
            printf("Can't write file\n");
            return 8;
        }
        close(fd); 
        counter++;
    }
while(wait(NULL)>0){
}
    free(fifoNames);
    return 0;
}