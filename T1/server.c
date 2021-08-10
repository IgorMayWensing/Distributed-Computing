#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int s, namelen, client_address_size, nplayers = 0;
    struct sockaddr_in client, server, p1;
    char buf[1024];
    char addr[20];
    char port[6];

    struct Player {
        char* username;
        unsigned long points;
    };

    unsigned int qtyrank = 0;
    struct Player** rank;

    setbuf(stdout, NULL);

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        exit(1);

    server.sin_family      = AF_INET;
    server.sin_port        = htons(58566);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0)
        exit(2);

    namelen = sizeof(server);
    if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
        exit(3);

    printf("Esperando jogadores...\n");

    client_address_size = sizeof(client);
    while (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client, &client_address_size) >= 0) {
        if (strcmp(buf, "rank") == 0) {
	   char* aux = (char*) malloc(sizeof(char) * 1024);
            for (int i = 0; i < qtyrank; i++) {
	       sprintf(buf, "Username: %s | %lu pontos\n", rank[i]->username, rank[i]->points);
                strcat(aux, buf);
            }
            
            if (sendto(s, aux, (strlen(aux)+1), 0, (struct sockaddr *) &client, sizeof(client)) < 0)
                exit(2);
        } else {
            if (strcmp(buf, "play") == 0) {
                if (nplayers == 0) {
                    p1.sin_family      = AF_INET;
                    p1.sin_port        = client.sin_port;
                    p1.sin_addr.s_addr = client.sin_addr.s_addr;
                    nplayers++;
                } else {
                    strcpy(addr, inet_ntoa(client.sin_addr));
                    sprintf(port, "%d", ntohs(client.sin_port));
                    strcat(addr, " ");
                    strcat(addr, port);
                    strcat(addr, "*");  // indica primeiro a jogar
                    if (sendto(s, addr, (strlen(addr)+1), 0, (struct sockaddr *) &p1, sizeof(p1)) < 0)
                        exit(2);

                    strcpy(addr, inet_ntoa(p1.sin_addr));
                    sprintf(port, "%d", ntohs(p1.sin_port));
                    strcat(addr, " ");
                    strcat(addr, port);
                    if (sendto(s, addr, (strlen(addr)+1), 0, (struct sockaddr *) &client, sizeof(client)) < 0)
                        exit(2);
                    nplayers = 0;
                }
            } else {
                char* usr = strtok(buf, " ");
                if (strcmp(usr, "won") == 0) { // comando vem do cliente assim: "won username 666"
                    usr = strtok(NULL, " ");
                    int points = atoi(strtok(NULL, " "));
                    unsigned short found = 0;
                    for (int i = 0; i < qtyrank; i++) {
                        if (strcmp(rank[i]->username, usr) == 0) {
                            rank[i]->points += points;
                            found = 1;
                        }
                    }
                    if (!found) {
                        struct Player *p = (struct Player*) malloc(sizeof(struct Player*));
                        p->username = (char*) malloc(sizeof(char) * (strlen(usr) + 1));
                        strcpy(p->username, usr);
                        p->points = points;
                        qtyrank++;
                        rank = realloc(rank, qtyrank * sizeof(struct Player*));
                        rank[qtyrank - 1] = p;
                    }
                }
            }
        }
    }

    close(s);
    return 0;
}
