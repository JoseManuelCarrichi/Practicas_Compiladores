#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

//Nombre del archivo de lectura
const char* filename = "Palabras.txt";

int main(int argc, char *argv[])
{
	//Puntero para el archivo
    FILE *ArchivoLeido;
    //apertura del archivo en modo lectura
    ArchivoLeido = fopen(filename, "r");
    //variables de desplazamiento y detección de casos
    int caso,pos;
    //variable para el analisis por caracteres
    char letra;

    if (!ArchivoLeido) {
    	//mensaje de error
        perror("Error en la apertura del archivo");
        //finalizacion del programa
        exit(EXIT_FAILURE);
    }

    //se verifica el estado del archivo para posteriormente obtener el tamaño del mismo
    struct stat sb;
    //Si ocurre un error al verificar el estado, se interrumpe el programa.
    if (stat(filename, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    //obtiene el tamaño del archivo y asigna memoria a una variable para leer el contenido del archivo
    char *DatosDelArchivo = malloc(sb.st_size);

    /*Mientras no se encuentre el final del archivo, lee los datos formateando cada que se encuentre
    un salto de línea los datos se guardan en la variable DatosDelArchivo*/
    while (fscanf(ArchivoLeido, "%[^\t\n ] ", DatosDelArchivo) != EOF) 
    {
        //printf("> %s\n", DatosDelArchivo);
        caso=0;
        pos=0;
        letra=DatosDelArchivo[pos];
        //printf("%c\n",letra);
        //Ejecuta las acciones mientras pos sea menor al tamñano de los datos leidos
        while(pos<strlen(DatosDelArchivo))
        {
        	printf("%c", DatosDelArchivo[pos]);
        	switch(caso)
        	{
        		case 0:
        			//si el caracter leido es una letra
        			if(isalpha(letra)!=0)
        			{
        				caso = 1;
        			}
        			else {caso = 2;};
        			break;

        		//caso de ID's
        		case 1:
        			//si es una letra o un digito	
        			if(isalpha(letra)!=0||isdigit(letra)!=0)
        			{
        				caso=1;
					}
					//si no, es un error
					else {caso=15;}
        			break;
        		case 2:
        			caso = 2;
        			break;	
        	}

        	pos++;
        	letra=DatosDelArchivo[pos];
		} 
		switch(caso)
		{
			case 1:
				printf(": es un identificador (ID)\n");
				break;
			default:
				printf(": es un ERROR\n");					
		}
	}
    fclose(ArchivoLeido);
    exit(EXIT_SUCCESS);
}

