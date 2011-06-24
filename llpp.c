#include <stdio.h>
#include <string.h>

#define len 256

int window_id;

void window(char *str)
{
    char string[len];
    sprintf( string,  "xdotool search --name \"%s.pdf\"", str );
    FILE *file = popen( string, "r" );
    fscanf( file, "%d", &window_id );
    pclose( file );
}

void keys( char *str )
{
    char string[len];
    sprintf( string,  "xdotool key --window %d --clearmodifier \"%s\"", window_id, str );
    FILE *file = popen( string, "w" );
    pclose( file );
}

void name(char *string)
{
    int i, a;
    a = strlen(string)-4;
    for (i = a; i < strlen(string); i++) {
        string[i] = '\0';
    }
}

int main(int argc, char *argv[])
{
    char string[len];
    strcpy( string, argv[1] );
    name(string);
    window(string);
    keys("r");
    return 0;
}
