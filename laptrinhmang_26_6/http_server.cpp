#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <limits.h>
#include <dirent.h>

int port;

char* getParentPath(char *path)
{
    char *ptr = strrchr(path, '/');
    if (ptr == path)
    {
        return NULL;
    }
    else
    {
        int length = ptr - path;
        char *parent = (char*)malloc(length + 1);
        memcpy(parent, path, length);
        parent[length] = 0;
        return parent;
    }
}

void signal_handler(int signo)
{
    wait(NULL);
}

void *client_thread(void *param)
{
    int clientSocket = *(int *)param;
    char buff[2048];

    int ret = recv(clientSocket, buff, sizeof(buff) - 1, 0);
    if (ret <= 0)
    {
        close(clientSocket);
        return NULL;
    }
    buff[ret] = 0;
    printf("\n%s\n\n", buff);

    if (strncmp(buff, "GET /get", 8) == 0)
    {
        // Request with query
        char *pos1 = strchr(buff, '?') + 1;
        char *pos2 = strchr(pos1, ' ');
        int queryLength = pos2 - pos1;
        char *query = (char*)malloc(queryLength + 1);
        memcpy(query, pos1, queryLength);
        query[queryLength] = 0;

        if (strncmp(query, "file=", 5) == 0)
        {
            // Is file
            char *file = query + 5;
            char *extension = strrchr(file, '.');
            if (extension == NULL)
            {
                char *message = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><h1>File type not support</h1></html>";
                send(clientSocket, message, strlen(message), 0);
                close(clientSocket);
                return NULL;
            }
            if ((strcmp(extension, ".txt") == 0) || (strcmp(extension, ".c") == 0) || (strcmp(extension, ".cpp") == 0))
            {
                char *header = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\n";
                send(clientSocket, header, strlen(header), 0);
                FILE *f = fopen(file, "rb");
                if (f == NULL)
                {
                    return NULL;
                }
                while (!feof(f))
                {
                    int ret = fread(buff, 1, sizeof(buff), f);
                    if (ret <= 0) break;
                    send(clientSocket, buff, ret, 0);
                }
                fclose(f);
            }
            else if ((strcmp(extension, ".jpg") == 0) || (strcmp(extension, ".png") == 0))
            {
                char *header = (char*)"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nConnection: close\r\n\r\n";
                send(clientSocket, header, strlen(header), 0);
                FILE *f = fopen(file, "rb");
                if (f == NULL)
                {
                    return NULL;
                }
                while (!feof(f))
                {
                    int ret = fread(buff, 1, sizeof(buff), f);
                    if (ret <= 0) break;
                    send(clientSocket, buff, ret, 0);
                }
                fclose(f);
            }
            else
            {
                char *message = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><h1>File type not support</h1></html>";
                send(clientSocket, message, strlen(message), 0);
            }
        }
        else if (strncmp(query, "dir=", 4) == 0)
        {
            // Is directory
            char *dir = query + 4;
            DIR *d = opendir(dir);
            if (d == NULL)
            {
                char *message = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n<html><h1>Directory not found</h1></html>";
                send(clientSocket, message, strlen(message), 0);
                close(clientSocket);
                return NULL;
            }
            char *header = (char*)"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";
            send(clientSocket, header, strlen(header), 0);
            struct dirent *dirEntry;
            while ((dirEntry = readdir(d)) != NULL)
            {
                if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
                {
                    char entryPath[PATH_MAX];
                    snprintf(entryPath, sizeof(entryPath), "%s/%s", dir, dirEntry->d_name);
                    char *parent = getParentPath(entryPath);
                    char *encodedPath = (char*)malloc(strlen(entryPath) * 3 + 1);
                    char *ptr = entryPath;
                    char *eptr = encodedPath;
                    while (*ptr != '\0')
                    {
                        if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= '0' && *ptr <= '9') || *ptr == '/' || *ptr == '.' || *ptr == '_' || *ptr == '-')
                        {
                            *eptr = *ptr;
                            eptr++;
                        }
                        else
                        {
                            sprintf(eptr, "%%%02X", (unsigned char)*ptr);
                            eptr += 3;
                        }
                        ptr++;
                    }
                    *eptr = '\0';

                    char line[2048];
                    snprintf(line, sizeof(line), "<a href=\"/get?%s\">%s</a><br>", encodedPath, dirEntry->d_name);
                    send(clientSocket, line, strlen(line), 0);

                    free(parent);
                    free(encodedPath);
                }
            }
            closedir(d);
        }
    }

    close(clientSocket);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    port = atoi(argv[1]);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindResult = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindResult < 0)
    {
        perror("bind");
        return 1;
    }

    int listenResult = listen(serverSocket, 5);
    if (listenResult < 0)
    {
        perror("listen");
        return 1;
    }

    printf("Server is listening on port %d...\n", port);

    while (1)
    {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0)
        {
            perror("accept");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, &clientSocket) != 0)
        {
            perror("pthread_create");
            continue;
        }
    }

    close(serverSocket);

    return 0;
}
