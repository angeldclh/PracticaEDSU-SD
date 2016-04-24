/*
   Incluya en este fichero todas las implementaciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>

#include "comun.h"

//Serializar mensaje para enviarlo por la red. El formato del string resultante será
//tipo\nnombreTema\nvalor 

char* serialize(mensaje *msg){
	char *serialmsg;
	//sizeof(char) porque tipo es 1, 2, 3, 4 o 5: solo va a ocupar 1 char
	if((serialmsg = (char*) malloc(sizeof(char) + sizeof(msg->nombreTema) + sizeof(msg->valor))) == NULL){
		fprintf(stderr, "Error al serializar mensaje.\n");
		return NULL;
	}

	sprintf(serialmsg, "%d\n%s\n%s\n", htons(msg->tipo), msg->nombreTema, msg->valor);
	return serialmsg;
}


//Dessearializar un mensaje (que se ha recibido)
mensaje* unserialize(char *serialmsg){
	mensaje *msg = (mensaje*) sizeof(mensaje);
	msg->tipo = atoi(strsep(&serialmsg, "\n"));
	msg->nombreTema = strsep(&serialmsg, "\n");
	msg->valor = strsep(&serialmsg, "\n");

	return msg;
}

//Enviar mensaje a un sockaddr_in
int enviarMensaje(const mensaje *msg, const struct sockaddr_in *dir) {
	//Crear socket
	int s;
	if((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		fprintf(stderr, "Error al crear socket en enviarMensaje.\n");
		return -1;
	}

	//Conectar el socket creado a la dirección pasada como parámetro
	if(connect(s, (struct sockaddr *) dir, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "Error al conectar el socket en enviarMensaje.\n");
		close(s);
		return -1;
	}
	
	return 0;	
}

