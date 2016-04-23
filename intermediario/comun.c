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

#include "comun.h"

//Serializar mensaje para enviarlo por la red
char* serialize(const mensaje *msg){
	char *serialmsg;

}

//Dessearializar un mensaje (que se ha recibido)
mensaje* unserialize(char *serialmsg){
	mensaje *msg = (mensaje*) sizeof(mensaje);


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

