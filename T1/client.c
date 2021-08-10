#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define SIDE 3

void showBoard(char board[][SIDE]) {
    printf("\n\n\n\n\n\n\n");
    printf("\t\t\t  Tic-Tac-Toe\n\n");
    printf("Digite de 1 a 9 para escolher uma posição:\n\n\n");
    printf("\t\t\t  %c | %c  | %c  \t\t\t  1 | 2  | 3  \n", board[0][0],
                             board[0][1], board[0][2]);
    printf("\t\t\t--------------\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c \t\t <--      4 | 5  | 6  \n", board[1][0],
                             board[1][1], board[1][2]);
    printf("\t\t\t--------------\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \t\t\t  7 | 8  | 9  \n\n", board[2][0],
                             board[2][1], board[2][2]);
   
    return;
}

bool notValid(char input[5]) {
    if ((strcmp(input, "1") == 0) || (strcmp(input, "2") == 0)|| (strcmp(input, "3") == 0)|| (strcmp(input, "4") == 0)|| (strcmp(input, "5") == 0)|| (strcmp(input, "6") == 0)|| (strcmp(input, "7") == 0)|| (strcmp(input, "8") == 0)|| (strcmp(input, "9") == 0) ){
        return (false);
    } else {
        printf("Input inválido. Tente Novamente:");
        return (true);
    }
}

bool rowCrossed(char board[SIDE][SIDE]) {
    for (int i=0; i<SIDE; i++)
    {
        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2] && 
            board[i][0] != ' ')
            return (true);
    }
    return(false);
}
  
bool columnCrossed(char board[SIDE][SIDE]) {
    for (int i=0; i<SIDE; i++)
    {
        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i] && 
            board[0][i] != ' ')
            return (true);
    }
    return(false);
}

bool diagonalCrossed(char board[SIDE][SIDE]) {
    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2] && 
        board[0][0] != ' ')
        return(true);
          
    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0] &&
         board[0][2] != ' ')
        return(true);
  
    return(false);
}

bool gameOver(char board[SIDE][SIDE]) {
    return(rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board) );
    return true;
}

bool alreadyExist(char input[5], int moves[9]) {
    for (int i=0; i<9; i++) {
        if ((input[0] -'0') == moves[i]){
            printf("Ja tem uma peca ai!! Tente novamente:\n");
            return (true);
        }           
    }
    return false;
}

void resetBoard(int moves[], char board[SIDE][SIDE], int *movesIndex, unsigned short *turn, unsigned short *player){
    for (int i= 0 ; i < 9; i++) {
        moves[i] = -1;
    }

    for (int i=0; i<SIDE; i++) {
        for (int j=0; j<SIDE; j++)
            board[i][j] = ' ';
    }

    *movesIndex = 0;
    *turn = 0;
    *player = 2;
}

int main(int argc, char** argv) {
    int s;
    unsigned short turn;
    unsigned short player;
    struct sockaddr_in server;
    char buf[1024];
    char op[5];
    char jogada[5];
    char board[SIDE][SIDE];
    int moves[9];
    int movesIndex;

    resetBoard(moves, board, &movesIndex, &turn, &player);

    setbuf(stdout, NULL);

    if (argc != 2) {
        printf("Usage: %s <username>\n", argv[0]);
        exit(1);
    }

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        exit(1);

    server.sin_family      = AF_INET;
    server.sin_port        = htons(58566);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int server_address_size = sizeof(server);

    printf("O que você deseja? Jogar?     <play>\n                   Pontuação? <rank>\n");
    while (1) {
        printf("|$ ");
        scanf("%s", op);
        if (strcmp(op, "rank") == 0) {
            if (sendto(s, op, (strlen(op)+1), 0, (struct sockaddr *) &server, sizeof(server)) < 0)
                exit(2);

            if (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &server, &server_address_size) < 0)
                exit(4);

            printf("%s\n", buf);
        } else {
            if (strcmp(op, "play") == 0) {
                if (sendto(s, op, (strlen(op)+1), 0, (struct sockaddr *) &server, sizeof(server)) < 0)
                    exit(2);

                printf("Esperando outro jogador...\n");
                if (recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &server, &server_address_size) < 0)
                    exit(4);

                if (buf[strlen(buf)-1] == '*') {  // Se for o primeiro a jogar turn = 1
                    turn = 1;
                    player = 1;
                    buf[strlen(buf)-1] = '\0';
                }

                char* enemy_addr = strtok(buf, " ");
                char* enemy_port = strtok(NULL, " ");

                struct sockaddr_in enemy;
                enemy.sin_family      = AF_INET;
                enemy.sin_port        = htons(atoi(enemy_port));
                enemy.sin_addr.s_addr = inet_addr(enemy_addr);
                int enemy_address_size = sizeof(enemy);

                int x,y;
                
                while(1) {
                    showBoard(board);
                    printf("\n\n\n\n\n\n\n\n\n\n\n");
                    if (turn) {
                        
                        printf("Seu turno, digite uma posicao:\n");
                        
                        do {                            
                            scanf("%s", jogada);
                        } while(notValid(jogada) || alreadyExist(jogada, moves));

                        int jogadaInt =  jogada[0] - '0';

                        moves[movesIndex++] = jogadaInt; //salva a jogada
                        jogadaInt--;
                        
                        x = jogadaInt / SIDE;
                        y = jogadaInt % SIDE;

                        if(player == 1) {
                            board[x][y] = 'X';
                        } else {
                            board[x][y] = 'O';
                        }
                        
                        jogada[0] = x +'0'; //int to char
                        jogada[1] = y +'0'; //int to char
                        jogada[2] = jogadaInt + '0' + 1;
                        
                        strcpy(buf, jogada);
                        if (sendto(s, buf, (strlen(buf)+1), 0, (struct sockaddr *) &enemy, sizeof(enemy)) < 0)
                            exit(2);
                        
                        if (gameOver(board)) {
                            jogada[3]='*';
                            showBoard(board);
                            printf("\nPARABENS GANHOU EBAAAAA!!!!!!!!\n");

                            sprintf(buf, "won %s 3", argv[1]);  
                            if (sendto(s, buf, (strlen(buf)+1), 0, (struct sockaddr *) &server, sizeof(server)) < 0)  
                                exit(2);
                            //enviar resultado para server aqui...
                            resetBoard(moves, board, &movesIndex, &turn, &player);
                            break;
                        } else if(movesIndex == 9){
                            showBoard(board);
                            sprintf(buf, "won %s 1", argv[1]);  
                            if (sendto(s, buf, (strlen(buf)+1), 0, (struct sockaddr *) &server, sizeof(server)) < 0) 
                                exit(2);
                            printf("\n\n\n\n\n\n\n\n\nDeu véia kakakakakaka\n");
                            resetBoard(moves, board, &movesIndex, &turn, &player);
                            break;
                        }

                    } else {
                        printf("Turno do adversario, aguarde sua vez.\n");
                        if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &enemy, &enemy_address_size) < 0)
                            exit(4);

                        x = buf[0] -'0'; //char to int
                        y = buf[1] -'0'; //char to int
                        moves[movesIndex++] = buf[2] -'0'; //salva a jogada

                        if(player == 2) {
                            board[x][y] = 'X';
                        } else {
                            board[x][y] = 'O';
                        }
                        
                        if (gameOver(board)) {
                            showBoard(board);
                            printf("\nPERDEU TROUXA!!!!!!!!\n");
                            resetBoard(moves, board, &movesIndex, &turn, &player);
                            break;
                        } else if(movesIndex == 9){
                            showBoard(board);
                            sprintf(buf, "won %s 1", argv[1]);  
                            if (sendto(s, buf, (strlen(buf)+1), 0, (struct sockaddr *) &server, sizeof(server)) < 0)  
                                exit(2);
                            printf("\n\n\n\n\n\n\n\n\nDeu véia kakakakakaka\n");
                            resetBoard(moves, board, &movesIndex, &turn, &player);
                            break;
                        }
                    }
                    turn= !turn;
                }
                    
            } else {
                printf("Comando inválido! Saindo...\n");
                break;
            }
        }
    }

    close(s);
    return 0;
}

