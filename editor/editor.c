#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include "editor.h"
#include "comun.h"
#include "edsu_comun.h"

int generar_evento(const char *tema, const char *valor) {

	//Obtener socket al que enviar el mensaje con el evento (intermediario)
	char *hostname = getenv("SERVIDOR");
	int port = atoi(getenv("PUERTO"));
	
	struct sockaddr_in *intermediario = NULL;
	intermediario->sin_family = AF_INET;
	intermediario->sin_port = htons(port);	
	struct hostent *hostinfo;
	if((hostinfo = gethostbyname(hostname)) == NULL){
		fprintf(stderr, "Error en editor: servidor no válido.\n");
		return -1;
	}
	intermediario->sin_addr = *(struct in_addr *) hostinfo->h_addr;
	
	//Crear el mensaje a enviar (usando la estructura de comun.h) y enviarlo
	mensaje *msg = (mensaje*) malloc(sizeof(msg));
	msg->tipo = EVENTO;
	msg->nombreTema = tema;
	msg->valor = valor;


	//Descomentar cuando la función esté escrita:
	int res;
	//res = enviarMensaje(msg,intermediario);
	free(msg);
	
	return res;
}

/* solo para la version avanzada */
int crear_tema(const char *tema) {
	return 0;
}

/* solo para la version avanzada */
int eliminar_tema(const char *tema) {
	return 0;
}

