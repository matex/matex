#include <stdio.h>
#define len 256

void update( char *str )
{
    char dummy[len], string[len]; 
    int i;
    sprintf( string, "pdflatex %s > pdflatex.log", str );
    FILE *compile = popen( string, "w" );
    sprintf( string, "pdflatex %s", str );
    FILE *wait = popen( string, "r" );
    while( fgets(string, len, wait )!=NULL );
    {   
        printf(".");
    }   
    pclose( compile );
    pclose( wait );
}

void matex(char *str)
{
    char string[len];
    sprintf(string, "./matex %s", str);
    system(string);
}

void llpp(char *str)
{
    char string[len];
    sprintf(string, "./llpp %s", str);
    system(string);
}

int main(int argc, char *argv[])
{
    matex(argv[1]);
    update(argv[1]);
    llpp(argv[1]);
    return 0;
}
