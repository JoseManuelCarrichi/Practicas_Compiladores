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
					//si no, y si es un signo
					else if(letra=='+'||letra=='-')
					{
						caso = 2;
					}
					//si no, y si es un digito
					else if(isdigit(letra)!=0)
					{
						caso = 3;
					}	
					//si no, y si es alguno de los sigos mayor o menor qué
					else if(letra=='>'||letra=='<')
					{
						caso = 8;
					}
					//si no, y si es el signo de asignacion
					else if(letra=='=')
					{
						caso=9;
					}
					//si no, y si es un signo de multiplicacion o division
					else if(letra=='*'||letra=='/')
					{
						caso = 11;
					}
					//si no, y si es un signo de admiracion
					else if(letra=='!')
					{
						caso=14;
					}
					//si no y si es un signo "and" u "or"
					else if(letra=='&'||letra=='|')
					{
						caso = 12;
					}
					//si no se encontro un caso especifico, entonces es un error
					else
					{
						caso = 15;
					}	
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
        		//caso: Validacion de un posible numero entero, decimal o exponencial
        		case 2:
        			//si es un digito
        			if(isdigit(letra)!=0)
        			{
        				caso=3;
					}
					//si no, es un error
					else {caso=15;}
        			break;
        		//caso: validar un numero entero o encontrar un punto decimal
        		case 3:
        			//si es un digito
        			if(isdigit(letra)!=0)
        			{
        				caso=3;
					}
					//si no, y si se encuentra un punto
					else if(letra=='.') {caso=4;}
					//si no, es un error
					else {caso=15;}
        			break;	
        		//caso: primera verificacion para un numero decimal
        		case 4:
        			if(isdigit(letra)!=0)
        			{caso=5;}
					else {caso=15;}
        			break;
        		//caso: numero decimal o numero exponencial
				case 5:
        			if(isdigit(letra)!=0)
        			{
        				caso=5;
					}
					else if(letra=='E'||letra=='e')
						{caso=16;}
					else {caso = 15;}
        			break;
        		//Caso: Verificar digitos luego del signo de un numero exponencial
				case 6:
        			if(isdigit(letra)!=0)
        			{
        				caso=7;
					}
					else {caso=15;}				
        			break; 
        		//Caso: Numero exponencial
				case 7:
        			if(isdigit(letra)!=0)
        			{
        				caso=7;
					}
					else {caso=15;}
        			break;
        		case 8:
        			if(letra=='=')
        				{caso=10;}
        			else {caso=15;}
        			break;
				case 9:
					if(letra=='=')
					{
						caso=10;
					}
        			else {caso=15;}
        			break;
				case 10:
					caso=15;
        			break;	
        		case 11:
        			caso=15;
					break;	
        		case 12:
        			if(letra=='&'||letra=='|')
        			{
        				caso=13;
        			}
        			else {caso=15;}
        			break;
        		case 13:
        			caso=15;
        			break;
        		case 14:
        			if(letra=='=')
        			{
        				caso=10;
        			}
        			else {caso=15;}
        			break;
        		case 15:
        			caso=15;
        			break;
        		//caso, el numero exponencial tiene signo e^(-|+)
        		case 16:
					if(letra=='+'||letra=='-')
					{
						caso=6;
					}
        			else {caso=15;}
        			break;		
        	}

        	pos++;
        	letra=DatosDelArchivo[pos];
		} 
		switch(caso)
		{
			case 1:
				if((strcmp(DatosDelArchivo,"int") == 0)||(strcmp(DatosDelArchivo,"printf") == 0)||(strcmp(DatosDelArchivo,"scanf") == 0))
				{
					printf(": es una Palabra Reservada (PR)\n");
				}
				else
				printf(": es un ID\n");
				break;
			case 2:
				printf(": es un Operador Aritmetico (OA)\n");
				break;
			case 3:
				printf(": es un Numero Entero (NE)\n");
				break;
			case 5:
				printf(": es un Numero Decimal (ND)\n");
				break;
			case 7:
				printf(": es un Numero Exponencial (NEX)\n");
				break;
			case 8:
				printf(": es un Operador Relacional (OR)\n");
				break;
			case 9:
				printf(": es el simbolo de ASIGNACION (AS)\n");
				break;
			case 10:
				printf(": es un Operador Relacional (OR)\n");
				break;	
			case 11:
				printf(": es un Operador Aritmetico (OA)\n");
				break;
			case 13:
				printf(": es un Operador Logico (OL)\n");
				break;
			case 14:
				printf(": es un Operador Logico (OL)\n");
				break;
			default:
				printf(": es un ERROR\n");					
		}
	}
    fclose(ArchivoLeido);
    exit(EXIT_SUCCESS);
}

