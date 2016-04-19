#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "comun.h"


//Estructuras de datos para tema y lista de temas
typedef struct tema {
	char *nombre;
	int nSuscr;
	struct sockaddr_in *suscriptores;
} tema;

typedef struct listaTemas {
	int nTemas;
  	tema **temas;
} listaTemas;


////////////////////////////////////////////////////////////////////
//Funciones empleadas en el desarrollo, no se emplean en el programa final

void imprimirTema(const tema* tema) {
	printf("tema.nombre =  %s\ttema.nSuscr = %d \n\n", tema->nombre, tema->nSuscr);

}



void imprimirTemas(const listaTemas *temas) {
	int i = 0;
	printf("Número total de temas: %d \n", temas->nTemas);
	for (i = 0; i < temas->nTemas; i++) {
		imprimirTema(temas->temas[i]);
	}
}

//////////////////////////////////////////////////////

//Crear un tema a partir de un string con su nombre 
tema* crearTema(char *nombre){

	tema *toReturn;
	//Reservar espacio para la estructura de datos
	if((toReturn = (tema*) malloc(sizeof(tema))) == NULL) {
		fprintf(stderr, "Error en malloc al crear tema.\n");
		return NULL;
	}
	//Reservar espacio para el nombre del tema
	if((toReturn->nombre = (char*) malloc(strlen(nombre) * sizeof(char))) == NULL){
		fprintf(stderr, "Error en malloc al crear tema.\n");
		free(toReturn);
		return NULL;
	}
	//Reservar espacio para los suscriptores del tema
	if((toReturn->suscriptores = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in*))) == NULL){
		fprintf(stderr, "Error en malloc al crear tema.\n");
		free(toReturn);
		free(toReturn->nombre);
		return NULL;
	}
	//Poner nombre al tema creado
	strcpy(toReturn->nombre,nombre);
	//Tema recién creado: 0 suscriptores
	toReturn->nSuscr = 0;

	return toReturn;
}

//Función que añade a una lista de temas los temas de un fichero. 
int anyadirTemas(listaTemas *lista, FILE *fichero){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	void *tmp;

	while ((read = 	getline(&line, &len, fichero)) != -1) {		
		//Reservar memoria para incluir un nuevo tema en la lista.
		if((tmp = realloc(lista->temas, (lista->nTemas +1) * sizeof(tema*))) == NULL) {
			fprintf(stderr, "Imposible reservar memoria para incluir nuevo tema en lista.\n");
			fclose(fichero);
			free(line);
			return 1;
		}
		lista->temas = tmp;
		
		//Crear tema a partir del nombre (línea del fichero) y meterlo en la lista
		line[strlen(line)-1]=0; //line contiene el \n: quitárselo para el nombre del tema
		if((lista->temas[lista->nTemas] = crearTema(line)) == NULL) {
			fprintf(stderr, "Imposible crear tema.\n");
			fclose(fichero);
			free(line);
			return 1;
		}
		lista->nTemas++;
		
	} 
	free(line);
	fclose(fichero);
	return 0;
}

//Función para limpiar la memoria ocupada por una lista de temas
void limpiarMemoria(listaTemas *lista){
	int z;
	for(z=0; z < lista->nTemas; z++){
		free(lista->temas[z]->nombre);
		free(lista->temas[z]->suscriptores);
		free(lista->temas[z]);
	}

	free(lista->temas);
	free(lista);
}

int main(int argc, char *argv[]) {
	if (argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	//Crear una lista para guardar todos los temas
	listaTemas *lista;
	if((lista = (listaTemas*) malloc(sizeof(listaTemas))) == NULL){
		fprintf(stderr, "Imposible crear la lista de temas.\n");
		return 1;
	}

	//Abrir el fichero que se pasa como argumento
	FILE *fichTemas;
	if((fichTemas = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "El fichero de temas no se puede abrir \n");
		free(lista);
		return 1;
	}
	//Añadir los temas del fichero a la lista
	if (anyadirTemas(lista, fichTemas) != 0) {
		fprintf(stderr, "No se han podido añadir los temas a la lista.\n");
		free(lista);
		return 1;
	}

	
	//	imprimirTemas(lista);   //Para pruebas, quitar esta línea al enviar

	//Crear socket TCP
	struct sockaddr_in saTCP;
	bzero((char *)&saTCP, sizeof(struct sockaddr_in));
	saTCP.sin_family=AF_INET;
	saTCP.sin_addr.s_addr=INADDR_ANY;
	saTCP.sin_port=htons(atoi(argv[1]));

	int sockTCP; //descriptror fichero socket TCP

	if((sockTCP=socket(PF_INET,SOCK_STREAM,0)) == -1){
		fprintf(stdout,"Error al crear el socket TCP del intermediario.\n");
		limpiarMemoria(lista);
		return 2;
	}
	//Reutilizar puerto
	if (setsockopt(sockTCP, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) == -1){
		fprintf(stderr,"Error en intermediario al reutilizar puerto.\n");
		limpiarMemoria(lista);
		return 2;
	}
	//Aceptar conexiones por el socket
	if(listen(sockTCP, 1) == -1){
		fprintf(stderr,"Error en el listen del intermediario.\n");
		limpiarMemoria(lista);
		return 2;
	}

	
	//Arrancar el servidor intermediario:
	while(1){
		
	}


	//Limpiar memoria
	limpiarMemoria(lista);
	return 0;
}
