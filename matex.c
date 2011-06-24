#include <stdio.h>
#include <string.h>
#define len 256

char text[len];
int count=0, a=0, b=0, sum=0;

void delchar(char *string, char charactor)
{
    int i, m;
    char dummy[2], str[len];
 
    memset( str, 0, sizeof( str ) ); 
    m = strlen(string);
    
    for (i = 0; i < m; i++) {
        if (string[i] != charactor) {
            sprintf(dummy, "%c", string[i]);
            strcat(str, dummy);
        }
    }
    strcpy( string, str );
}

void replacechar(char *string, char charactor)
{
    int i, m;

    m = strlen(string);
    
    for (i = 0; i < m; i++) {
        if (string[i] == charactor) {
            string[i] = ' ';
        }
    }
}

int linesearch( char *line, char *command )
{
    int m, n, k=0;
    m = strlen( command );
    n = strlen( line );

    char string[m], dummy[3];

    int i;

    while ( k<(n-m+1) ) {
        memset( string, 0, sizeof( string ) ); 
        for (i = k; i < k+m; i++) {
            sprintf( dummy, "%c", line[i] );
            strcat( string, dummy );  
        }
        if( strcmp( string, command )==0 ){
                sum+=1;
        }
        k+=1;
    }
    return sum;
}


void search( char (*line)[len], char *command )
{
    int m, n, k;
    m = strlen( command );

    char string[m], dummy[3];
    int i, j, r=count;
    for (j = b+1; j < r; j++) {
        k=0; sum=0; n=strlen( line[j] ); 
        while ( k<(n-m+1) ) {
            memset( string, 0, sizeof( string ) ); 
            for (i = k; i < k+m; i++) {
                sprintf( dummy, "%c", line[j][i] );
                strcat( string, dummy );  
            }
            if( strcmp( string, command )==0 ){
                if(sum==0){
                    sprintf( text, "%d %d", j, k );
                    sum+=1;
                    r=0;
                }
                else {
                    sprintf(dummy, " %d", k);
                    strcat( text, dummy );
                    r=0;
                }
            }
            k+=1;
        }
    }
}


int no( char (*line)[len], char *command )
{
    int m, n, k, c=0;
    m = strlen( command );

    char string[m], dummy[3];
    int i, j, r=count;
    for (j = 0; j < r; j++) {
        k=0; sum=0; n=strlen( line[j] ); 
        while ( k<(n-m+1) ) {
            memset( string, 0, sizeof( string ) ); 
            for (i = k; i < k+m; i++) {
                sprintf( dummy, "%c", line[j][i] );
                strcat( string, dummy );  
            }
            if( strcmp( string, command )==0 ){
                c+=1;
            }
            k+=1;
        }
    }
    return c;
}

void simplify(char *str)
{
    FILE *out = fopen( "question", "w" );
    fprintf(out, "set no color\n");
    fprintf(out, str);
    fprintf(out, "simplify\n");
    fprintf(out, "exit\n" );
    fclose(out);

    char input[20][len], string[len];
    int i=0, count=0;
    FILE *file = popen("mathomatic question", "r");
    while (fgets(string, len, file)!=NULL) {
        strcpy(input[i], string);
        i+=1;
        count+=1;
    }
    pclose(file);
    input[12][0]=' '; input[12][1]='&'; input[12][2]='=';
    delchar( input[12], ' ' );
    delchar( str, '\n' );
    delchar( input[12], '\n' );
    sprintf(string, "%s \\\\ \n", input[12] );
    strcat(str, string); 
}


void find(char (*file)[len], char *string)
{
    FILE *out = fopen( "question", "w" );
    int i, j, n, l;
    a=0;  b=0; 
    char command[2][len], operation[len];
    memset( operation, 0, sizeof( operation ) ); 
    sprintf(command[0], "\\begin{%s}", string);
    sprintf(command[1], "\\end{%s}", string);
    n = no(file, command[0]);
    for (j = 0; j < n; j++) {
        search( file, command[0] );
        sscanf( text, "%d", &a );
        search( file, command[1]);
        sscanf( text, "%d", &b );
        l=a;
        if(strcmp( command[0], "\\begin{array}" )==0 ){
            search( file, command[0] );
            sscanf( text, "%d", &a );
            sum = 0;
            for (i = b; i < a; i++) {
                sum+=linesearch(file[i], "+");
            }
                if(sum!=0)
                    strcat(operation, "+");
                if(sum==0)
                    strcat(operation, " ");
        }

        for (i = l+1; i < b; i++) {
            delchar( file[i], ' ' );
            delchar( file[i], '\\' );
            replacechar( file[i], '&' );
            printf("%s", file[i]);
            simplify(file[i]); 
        }
        if(strcmp( command[0], "\\begin{array}" )==0 )
            printf("%c\n", operation[j]);
        else
            printf("\n");
    }
}

int main(int argc, char *argv[])
{
    FILE *read = fopen( argv[1], "r" );
    char string[len];
    while (fgets(string, len, read)!=NULL) {
        count+=1;
    }
    rewind(read);

    char file[count][len];
    int i = 0;
    while (fgets( string, len, read )!=NULL) {
        strcpy( file[i], string );
        i+=1;
    }

    find( file , "align*" );

    FILE *write = fopen( argv[1], "w" );

    for (i = 0; i < count; i++) {
        fprintf( write, "%s", file[i]);
    }
    fclose(read);


    return 0;
}
