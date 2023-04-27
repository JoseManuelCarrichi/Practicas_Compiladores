#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

//Nombre del archivo de lectura
const char* filename = "revision.txt";

//estructura para la tabla de simbolos
struct TS
{
	char Lexema [20];
	char TipTok [3];
	int Indice;
	struct TS *Sig;
};

//inicializar estructura
struct TS *ini = NULL, *fin = NULL, *aux = NULL;

//función para agregar nuevos simbolos a la tabla.
void AgregarSimTS(char Lex[20], char Tip[3], int num)
{
	if(ini == NULL)
	{
		//ini = new(struct TS);
		ini = malloc(sizeof(struct TS));
		strcpy(ini -> Lexema,Lex);
		strcpy(ini -> TipTok,Tip);
		ini ->Indice = num;
		ini -> Sig = NULL;
		fin = ini;
	}
	else
	{
		//aux = new (struct TS);
		aux = malloc(sizeof(struct TS));
		strcpy(aux -> Lexema,Lex);
		strcpy(aux -> TipTok,Tip);
		aux -> Indice = num;
		aux -> Sig = NULL;
		fin -> Sig = aux;
		fin = aux;
	}
}

//funcion que imprime la tabla de simbolos
void Imprimir()
{
	//Limpiar la consola de salida
	//system("cls");
	aux = ini;
	if (ini == NULL)
	{
		printf("\n-------NO EXISTEN ELEMENTOS EN LA TABLA DE SIMBOLOS------\n\n");		
	}
	else
	{
		printf("\n\t------TABLA DE SIMBOLOS------\n");
		printf("\nLexema\t\t|TipTok\t\t|Indice\n");
		printf("_________________________________________\n");
		while(aux != NULL)
		{
			if (strlen(aux->Lexema)<=6)
			{
				printf("%s",aux->Lexema);
				printf(" \t\t|%s",aux->TipTok);
				printf(" \t\t|%d\n",aux->Indice);
			aux = aux->Sig;
			}
			else
			{
				printf("%s",aux->Lexema);
				printf(" \t|%s",aux->TipTok);
				printf(" \t\t|%d\n",aux->Indice);
				aux = aux->Sig;
			}
			
		}
	}
	printf("\n");
}

int Analizar()
{
	//system("cls");
	printf("\n\t------ANALIZANDO ARCHIVO------\n\n");
	//Puntero para el archivo
    FILE *ArchivoLeido;
    //apertura del archivo en modo lectura
    ArchivoLeido = fopen(filename, "r");
    //variables de desplazamiento y detección de casos
    int caso = 0,pos = 0,Indice = 0;
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
        caso=0;
        pos=0;
        Indice++;
        letra=DatosDelArchivo[pos];
        //Ejecuta las acciones mientras pos sea menor al tamñano de los datos leidos
        while(pos<strlen(DatosDelArchivo))
        {
        	//printf("%c", DatosDelArchivo[pos]);
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
					//printf(": es una Palabra Reservada (PR)\n");
					AgregarSimTS(DatosDelArchivo,"PR",Indice);
				}
				else
				{
					//printf(": es un ID\n");
					AgregarSimTS(DatosDelArchivo,"ID",Indice);
				}
				break;
			case 2:
				//printf(": es un Operador Aritmetico (OA)\n");
				AgregarSimTS(DatosDelArchivo,"OA",Indice);
				break;
			case 3:
				//printf(": es un Numero Entero (NE)\n");
				AgregarSimTS(DatosDelArchivo,"NE",Indice);
				break;
			case 5:
				//printf(": es un Numero Decimal (ND)\n");
				AgregarSimTS(DatosDelArchivo,"ND",Indice);
				break;
			case 7:
				//printf(": es un Numero Exponencial (NEX)\n");
				AgregarSimTS(DatosDelArchivo,"NEX",Indice);
				break;
			case 8:
				//printf(": es un Operador Relacional (OR)\n");
				AgregarSimTS(DatosDelArchivo,"OR",Indice);
				break;
			case 9:
				//printf(": es el simbolo de ASIGNACION (AS)\n");
				AgregarSimTS(DatosDelArchivo,"AS",Indice);
				break;
			case 10:
				//printf(": es un Operador Relacional (OR)\n");
				AgregarSimTS(DatosDelArchivo,"OR",Indice);
				break;	
			case 11:
				//printf(": es un Operador Aritmetico (OA)\n");
				AgregarSimTS(DatosDelArchivo,"OA",Indice);
				break;
			case 13:
				//printf(": es un Operador Logico (OL)\n");
				AgregarSimTS(DatosDelArchivo,"OL",Indice);
				break;
			case 14:
				//printf(": es un Operador Logico (OL)\n");
				AgregarSimTS(DatosDelArchivo,"OL",Indice);
				break;
			default:
				printf("%s",DatosDelArchivo);
				printf(": es un ERROR\n");					
		}
	}
	//Imprimir();
    fclose(ArchivoLeido);
    printf("\n\t------ANALISIS COMPLETADO------\n\n");
    return 1;
    //exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
	int op = 0, bandera = 0;;

	do
	{
		printf("\t------MENU DE OPCIONES------\n\n");
		printf("1.- Analizar archivo.\n");
		printf("2.- Mostrar Tabla de Simbolos.\n");
		printf("3.- Salir.\n");
		printf("Digite el numero de la opcion deseada: ");
		scanf("%d", &op);

		switch(op)
		{
			case 1:
				bandera = Analizar();
				break;
			case 2:
				Imprimir();
				break;
			case 3:
				exit(EXIT_SUCCESS);
				break;
		}
	} while (op!=3);
}

