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
#include <unistd.h>

#include "comun.h"

/*Serializar mensaje para enviarlo por la red. El formato del string resultante será
  tipo
  nombreTema
  valor
*/ 
char* serialize(mensaje *msg){
	char *serialmsg;
	//El 3 * sizeof(char) es por los 3 \n del mensaje serializado
	if((serialmsg = (char*) malloc(sizeof(htons(msg->tipo)) + sizeof(msg->nombreTema) + sizeof(msg->valor)
				       + 3 * sizeof(char))) == NULL){
		fprintf(stderr, "Error al serializar mensaje.\n");
		return NULL;
	}

	sprintf(serialmsg, "%d\n%s\n%s\n", htons(msg->tipo), msg->nombreTema, msg->valor);
	return serialmsg;
}


//Dessearializar un mensaje (que se ha recibido)
mensaje unserialize(char *serialmsg){
	mensaje msg;
	msg.tipo = ntohs(atoi(strsep(&serialmsg, "\n")));
	msg.nombreTema = strsep(&serialmsg, "\n");
	msg.valor = strsep(&serialmsg, "\n");

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

	//Serializar y enviar mensaje
	char *serialmsg = serialize((mensaje *) msg);
	if((write(s, serialmsg, sizeof(serialmsg))) == -1) {
		fprintf(stderr, "Error al enviar el siguiente mensaje:\n%s", serialmsg);
		close(s);
		free(serialmsg);  //El malloc se hace en serialize()
		return -1;
	}
	free(serialmsg);  //El malloc se hace en serialize()

	//Recibir la confirmación
	char res[TAM_CONF];
	if((read(s, res, TAM_CONF)) == -1) {
		fprintf(stderr, "Error al recibir confirmación.\n");
		close(s);
		return -1;
	}

	mensaje resp = unserialize(res);
	if (resp.tipo == CONFIRM_OK)
		printf("CONFIRM_OK recibido\n"); 
	else
		fprintf(stderr, "CONFIRM_ERR recibido \n");

	return 0;	
}

