#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>

//Estructuras de datos para tema y lista de temas
typedef struct tema {
	char * tema;
	int nSuscr;
	struct sockaddr_in *suscriptores;
} tema;

typedef struct listaTemas {
	int nTemas;
  	tema **temas;
} listaTemas;


//Función que añade a una lista de temas los temas de un fichero. PROBAR
void anyadirTemas(listaTemas lista, FILE *fichero){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	//Revisar el acceso a los campos de temas (. o ->) y los realloc
	while ((read = 	getline(&line, &len, fichero)) != -1) {
		realloc(lista.temas, sizeof(lista.temas) + sizeof(char*));
		realloc(lista.temas[lista.nTemas], sizeof(lista.temas[lista.nTemas] + len*sizeof(char)));
		strcpy(lista.temas[lista.nTemas], line);
		lista.nTemas++;
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
	if((fichTemas = fopen(argv[], "r")) == NULL) {
		fprintf(stderr, "El fichero de temas no se puede abrir \n");
		exit(1);
	}
	anyadirTemas(lista, fichTemas);
	fclose(fichTemas);
	return 0;
}
