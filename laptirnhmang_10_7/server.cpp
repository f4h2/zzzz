#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    char nickname[20];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

void send_message_all(char* message) {
    for (int i = 0; i < client_count; i++) {
        write(clients[i].socket, message, strlen(message));
    }
}

void send_message_client(char* message, int client_socket) {
    write(client_socket, message, strlen(message));
}

void handle_join(char* message, int client_socket) {
    char nickname[20];
    sscanf(message, "JOIN %s", nickname);

    if (client_count == MAX_CLIENTS) {
        send_message_client("ERROR: Server is full.", client_socket);
        close(client_socket);
        return;
    }

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].nickname, nickname) == 0) {
            send_message_client("ERROR: Nickname already exists.", client_socket);
            close(client_socket);
            return;
        }
    }

    Client new_client;
    new_client.socket = client_socket;
    strcpy(new_client.nickname, nickname);
    clients[client_count] = new_client;
    client_count++;

    send_message_all("SERVER: User joined the chat.");
}

void handle_msg(char* message) {
    char sender[20];
    char msg[BUFFER_SIZE];
    sscanf(message, "MSG %s %[^\n]", sender, msg);

    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "MSG from %s: %s", sender, msg);

    send_message_all(response);
}

void handle_pmsg(char* message, int client_socket) {
    char sender[20];
    char receiver[20];
    char msg[BUFFER_SIZE];
    sscanf(message, "PMSG %s %s %[^\n]", sender, receiver, msg);

    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "PMSG from %s: %s", sender, msg);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].nickname, receiver) == 0) {
            send_message_client(response, clients[i].socket);
            return;
        }
    }

    send_message_client("ERROR: Receiver not found.", client_socket);
}

void handle_op(char* message, int client_socket) {
    char new_owner[20];
    sscanf(message, "OP %s", new_owner);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client_socket) {
            if (i == 0) {
                send_message_client("ERROR: You are already the owner.", client_socket);
            } else {
                strcpy(clients[i].nickname, new_owner);
                send_message_client("SERVER: Ownership transferred to you.", client_socket);
                return;
            }
        }
    }

    send_message_client("ERROR: You are not authorized.", client_socket);
}

void handle_kick(char* message, int client_socket) {
    char target[20];
    sscanf(message, "KICK %s", target);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].nickname, target) == 0) {
            send_message_client("SERVER: You have been kicked.", clients[i].socket);
            close(clients[i].socket);

            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;

            return;
        }
    }

    send_message_client("ERROR: User not found.", client_socket);
}

void handle_topic(char* message, int client_socket) {
    char topic[BUFFER_SIZE];
    sscanf(message, "TOPIC %[^\n]", topic);

    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "SERVER: Topic changed to '%s'", topic);

    send_message_all(response);
}

void handle_quit(int client_socket) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client_socket) {
            char response[40];
            snprintf(response, 40, "SERVER: User %s has left the chat.", clients[i].nickname);
            send_message_all(response);

            close(clients[i].socket);

            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;

            return;
        }
    }
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        buffer[bytes_read] = '\0';

        if (strncmp(buffer, "JOIN", 4) == 0) {
            handle_join(buffer, client_socket);
        } else if (strncmp(buffer, "MSG", 3) == 0) {
            handle_msg(buffer);
        } else if (strncmp(buffer, "PMSG", 4) == 0) {
            handle_pmsg(buffer, client_socket);
        } else if (strncmp(buffer, "OP", 2) == 0) {
            handle_op(buffer, client_socket);
        } else if (strncmp(buffer, "KICK", 4) == 0) {
            handle_kick(buffer, client_socket);
        } else if (strncmp(buffer, "TOPIC", 5) == 0) {
            handle_topic(buffer, client_socket);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            handle_quit(client_socket);
            break;
        }
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("ERROR: Failed to create socket");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("ERROR: Failed to bind socket");
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        perror("ERROR: Failed to listen on socket");
        return 1;
    }

    printf("Server started. Listening on port 12345...\n");

    while (1) {
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*) &client_address, (socklen_t*) &client_address_length);
        if (client_socket < 0) {
        perror("ERROR: Failed to accept connection");
        return 1;
    }
        printf("New client connected\n");

    handle_client(client_socket);
}

close(server_socket);

return 0;
}
