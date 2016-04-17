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

void imprimir_tema(const tema* tema) {
	printf("El nombre del tema es %s y hay %d procesos suscriptos \n",
			tema->nombre, tema->nSuscr);

}



void imprimir_temas(const listaTemas *temas) {
	int i = 0;
	printf("Hay %d temas disponibles \n", temas->nTemas);
	for (i = 0; i < temas->nTemas; i++) {
		imprimir_tema(temas->temas[i]);
	}
}

//////////////////////////////////////////////////////

//Crear un tema a partir de un string con su nombre 
tema* crearTema(char *nombre){
	tema *toReturn;
	//Reservar espacio para la estructura de datos
	if((toReturn = malloc(sizeof(tema))) == NULL) {
		fprintf(stderr, "Error en malloc al crear tema.\n");
		return NULL;
	}
	//Reservar espacio para el nombre del tema
	if((toReturn->nombre = malloc(strlen(nombre) * sizeof(char))) == NULL){
		fprintf(stderr, "Error en malloc al crear tema.\n");
		return NULL;
	}
	//Reservar espacio para los suscriptores del tema
	if((toReturn->suscriptores = malloc(sizeof(struct sockaddr_in*))) == NULL){
		fprintf(stderr, "Error en malloc al crear tema.\n");
		return NULL;
	}
	//Poner nombre al tema creado
	strcpy(toReturn->nombre,nombre);
	//Tema recién creado: 0 suscriptores
	toReturn->nSuscr = 0;

	return toReturn;
}

//Función que añade a una lista de temas los temas de un fichero. PROBAR
void anyadirTemas(listaTemas *lista, FILE *fichero){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	//Revisar los realloc
	while ((read = 	getline(&line, &len, fichero)) != -1) {

		printf("WHILE...\n");

		//Reservar memoria para incluir un nuevo tema en la lista
		lista->temas = realloc(lista->temas, sizeof(lista->temas) + sizeof(tema*));
		
		printf("realloc 1 hecho\n");
		

		// Cambiar a partir de aquí
		realloc(lista->temas[lista->nTemas], sizeof(lista->temas[lista->nTemas] + len*sizeof(char)));

		printf("realloc 2 hecho\n");

		strcpy(lista->temas[lista->nTemas]->nombre, line);

		printf("strcpy hecho\n");

		lista->nTemas++;
	} 
	free(line);
}

int main(int argc, char *argv[]) {
	if (argc!=3) {
		fprintf(stderr, "Uso: %s puerto fichero_temas\n", argv[0]);
		return 1;
	}

	//Añadir los temas del fichero a una lista
	listaTemas *lista = malloc(sizeof(listaTemas));
	FILE *fichTemas;
	if((fichTemas = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "El fichero de temas no se puede abrir \n");
		exit(1);
	}

	printf("Antes de anyadirTemas\n");

	anyadirTemas(lista, fichTemas);
	fclose(fichTemas);

	printf("Después de anyadirTemas\n");

	imprimir_temas(lista);
	return 0;
}
