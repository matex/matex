

//      Author:     Daniel Barry
//      email:      dangbarry@gmail.com
//  
//      This is a revised version for just algebra
//      I'm going to make a seperate tool for matrices


#include <stdio.h>
#include <string.h>
#define len 256

char Return[len];
int count=0, a, b, p;


//  this function searches the file for a string and assigns it to the globally
//  declared Return string.
void search( char (*file)[len], char *command )
{
    int m, n, k;
    m = strlen( command );

    char string[m], dummy[3];
    int i, j, r=count;

    for (j = b+1; j < r; j++) {
        k=0; n=strlen( file[j] ); 
        while ( k<(n-m+1) ) {

            //  set the string equal to the null, for the strcat function  
            memset( string, 0, sizeof( string ) ); 
            for (i = k; i < k+m; i++) {
                sprintf( dummy, "%c", file[j][i] );
                strcat( string, dummy );  
            }

            if( strcmp( string, command )==0 ){
                sprintf( Return, "%d %d", j, k+strlen(command) );
                r=0;
            }

            k+=1;
        }
    }
}


// deletes any occurrances of a charactor
void delchar(char *line, char charactor)
{
    int i, m;
    char dummy[2], str[len];
 
    memset( str, 0, sizeof( str ) ); 
    m = strlen(line);
    
    for (i = 0; i < m; i++) {
        if (line[i] != charactor) {
            sprintf(dummy, "%c", line[i]);
            strcat(str, dummy);
        }
    }
    strcpy(line, str);
}


// replaces any given charactor with a replacement charactor
void replacechar(char *string, char charactor, char replacement)
{
    int i;
    
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == charactor) {
            string[i] = replacement;
        }
    }
}


// returns the number of instances of a command in the file
int no( char (*line)[len], char *command )
{
    int m, n, k, c=0;
    m = strlen( command );

    char string[m], dummy[3];
    int i, j, r=count;
    for (j = 0; j < r; j++) {
        k=0;  n=strlen( line[j] ); 
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


// finds the kind of identation the user uses
int countindent(char *string)
{
    int i=0;
    while( string[i]==' ' )
        i+=1;
    return i;
}


void indent(char *string, int c)
{
    int i;
    char str[len], dummy[2];
    memset( str, 0, sizeof( str ) ); 
    for (i = 0; i < c; i++) {
        sprintf( dummy, "%c", ' ' );
        strcat( str, dummy );
    }
    strcat( str, string );
    strcpy( string, str );
}

//  This function just sets the commands
void setcommand(char (*command)[len], char *string)
{
    sprintf( command[0], "\\begin{%s}", string );
    sprintf( command[1], "\\end{%s}", string );
}


//  runs the search of the commands and assigns initials the globally declared a and b
void runsearch( char (*file)[len], char (*command)[len])
{
    search( file, command[0] );
    sscanf( Return, "%d", &a );
    search( file, command[1]);
    sscanf( Return, "%d", &b ); 
}


//  runs the mathomatic program to get the answer!
void simplify(char *str, int y)
{
    FILE *out = fopen( "question", "w" );
    fprintf(out, "set no color\n");
    fprintf(out, str);
    fprintf(out, "simplify\n");
    fprintf(out, "exit\n" );
    fclose(out);

    char input[20][len], string[len];
    int i=0, c=0;
    FILE *file = popen("mathomatic question", "r");
    while (fgets(string, len, file)!=NULL) {
        strcpy(input[i], string);
        i+=1;
        c+=1;
    }
    pclose(file);
    input[12][0]=' '; input[12][1]='&'; input[12][2]='=';
    delchar( input[12], ' ' );
    delchar( str, '\n' );
    delchar( input[12], '\n' );
    sprintf(string, "%s \\\\ \n", input[12] );
    strcat(str, string);
    indent( str, y );
}


//  operate on the string
void surgery(char *string)
{
    int c;
    c = countindent( string );
    delchar( string, ' ' );
    delchar( string, '\\' );
    replacechar( string, '&', ' ' );
    printf("%s", string);
    simplify( string , c);  
}


//  the find command is what is called apom in the main
void find(char (*file)[len], char *string)
{
    int i, j, n;
    a=0; b=0;
    
    //  command here is the marketing language commands \begin and \end
    char command[2][len];
    
    //  sets the commands as in the function
    setcommand( command, string );
    n = no( file, command[0] );

    for (i = 0; i < n; i++) {
        //  as above in runsearch function
        runsearch( file, command );
        for (j = a+1; j < b; j++) {
            surgery( file[j] );
        }
    }
}

// The main
int main(int argc, char *argv[])
{
    FILE *latex = fopen( argv[1], "r" );
    char string[len];
    
    // count the number of lines in the latex file
    // globally declared above
    while (fgets(string, len, latex)!=NULL) {
        count+=1;
    }
    rewind(latex);

    // declar the file
    char file[count][len];
    int i=0;

    // read in the file and copy it to the declared file
    while (fgets(string, len, latex )!=NULL ) {
        strcpy( file[i], string );
        i+=1;
    }

    find(file, "align*");

    FILE *write = fopen( argv[1], "w" );

    for (i = 0; i < count; i++) {
        fprintf( write, "%s", file[i]);
    }
    fclose(latex);
    fclose(write);
    return 0;
}
