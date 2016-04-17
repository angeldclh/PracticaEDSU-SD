#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>

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
		//Reservar memoria para incluir un nuevo tema en la lista. ESTE REALLOC ESTÁ MAL
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

int main(int argc, char *argv[]) {
	if (argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	//Crear una lista para guardar todos los temas
	listaTemas *lista;
	if((lista = (listaTemas*) malloc(sizeof(listaTemas))) == NULL){
		fprintf(stderr, "Imposible crear la lista de temas.\n");
		exit(-1);
	}

	//Abrir el fichero que se pasa como argumento
	FILE *fichTemas;
	if((fichTemas = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "El fichero de temas no se puede abrir \n");
		free(lista);
		exit(1);
	}
	//Añadir los temas del fichero a la lista
	if (anyadirTemas(lista, fichTemas) != 0) {
		fprintf(stderr, "No se han podido añadir los temas a la lista.\n");
		free(lista);
		exit(1);
	}

	
	imprimirTemas(lista);   //Para pruebas, quitar esta línea al enviar


	//Limpiar memoria
	int z;
	for(z=0; z < lista->nTemas; z++){
		free(lista->temas[z]->nombre);
		free(lista->temas[z]->suscriptores);
		free(lista->temas[z]);
	}

	free(lista->temas);
	free(lista);
	return 0;
}
