#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444

void safe() {
    printf("[+] Esecuzione normale\n");
}

void hacked() {
    printf("🔥 CONTROL FLOW HIJACKED\n");
   // comportamento rilevabile da Falco
   system("/bin/sh");
}

typedef struct {
    char buffer[64];
    void (*func)();
} chunk;

void handle_client(int client_sock) {
    chunk *c = malloc(sizeof(chunk));
    if (!c) {
        perror("malloc");
        return;
    }

    c->func = safe;

    unsigned char type;
    unsigned int length;

    printf("[+] Client connected\n");

    // Leggo TYPE
    recv(client_sock, &type, 1, 0);

     // Leggo LENGTH
    recv(client_sock, &length, 4, MSG_WAITALL); //aspetta tutto il messaggio
    length = ntohl(length); //converte dal formato dellla rete al formato del processore.

    printf("[+] Packet received\n");
    printf("TYPE: %d\n", type);
    printf("LENGTH: %u\n", length);

    char *data = malloc(length);
    if (!data) {
        perror("malloc");
        free(c);
        return;
    }

    // Leggo DATA
    recv(client_sock, data, length, 0);

    // FIX
    if (length == 0 || length > sizeof(c->buffer)) {
    printf("[!] Lunghezza non valida\n");
    free(data);
    free(c);
    return;
    }
    // Copia sicura
    memcpy(c->buffer, data, length);


    printf("[+] Chiamata funzione...\n");
    c->func();

    free(data);
    free(c);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    // BANNER
    printf("\n");
    printf("  ███████╗██████╗ ███╗   ██╗███████╗ ██████╗  \n");
    printf("  ██╔════╝██╔══██╗████╗  ██║██╔════╝██╔═══██╗ \n");
    printf("  ███████╗██████╔╝██╔██╗ ██║█████╗  ██║   ██║ \n");
    printf("  ╚════██║██╔═══╝ ██║╚██╗██║██╔══╝  ██║   ██║ \n");
    printf("  ███████║██║     ██║ ╚████║███████╗╚██████╔╝ \n");
    printf("  ╚══════╝╚═╝     ╚═╝  ╚═══╝╚══════╝ ╚═════╝  \n");
    printf("\n");
    printf("        SPNEGO-like Vulnerable Service\n");
    printf("=========================================\n");
    printf(" Protocol: TYPE | LENGTH | DATA\n");
    printf(" Waiting for client connection...\n\n");

    while (1) {
        client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        handle_client(client_sock);
        close(client_sock);
    }
}
