/*
   Incluya en este fichero todas las definiciones que pueden
   necesitar compartir todos los módulos (editor, subscriptor y
   proceso intermediario), si es que las hubiera.
*/

#define ALTA 1
#define BAJA 2
#define EVENTO 3
#define CONFIRM_OK 4
#define CONFIRM_ERR 5

/*Estructura de datos para mensajes. nombreTema puede ser NULL en caso de que
tipo sea CONFIRM_OK o CONFIRM_ERR. valor sólo se usa cuando tipo es EVENTO */
typedef struct mensaje {
	int tipo;
	const char *nombreTema;
	const char *valor;
	struct sockaddr_in *destinatario;
} mensaje;

int enviarMensaje(const mensaje *msg);
	
