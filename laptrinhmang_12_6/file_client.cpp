#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_FILE_NAME_LENGTH 256
#define MAX_FILE_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Khởi tạo socket client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error: Failed to create socket.\n");
        return 1;
    }

    // Thiết lập địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
        printf("Error: Invalid address.\n");
        return 1;
    }

    // Kết nối tới server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Failed to connect to server.\n");
        return 1;
    }

    // Nhận danh sách các file từ server
    char file_list[MAX_FILE_SIZE];
    memset(file_list, 0, sizeof(file_list));
    recv(client_socket, file_list, sizeof(file_list)-1, 0);

    if (strncmp(file_list, "ERROR", 5) == 0) {
        printf("Error: %s\n", file_list);
        close(client_socket);
        return 1;
    }

    printf("File list:\n%s", file_list);

    // Nhập tên file cần tải về từ server
    char file_name[MAX_FILE_NAME_LENGTH];
    printf("Enter file name to download: ");
    fgets(file_name, sizeof(file_name), stdin);
    file_name[strcspn(file_name, "\r\n")] = '\0';

    // Gửi tên file tới server
    send(client_socket, file_name, strlen(file_name), 0);

    // Nhận thông báo xác nhận từ server
    char confirm_message[MAX_FILE_SIZE];
    memset(confirm_message, 0, sizeof(confirm_message));
    recv(client_socket, confirm_message, sizeof(confirm_message)-1, 0);

    if (strncmp(confirm_message, "ERROR", 5) == 0) {
        printf("Error: %s\n", confirm_message);
        close(client_socket);
        return 1;
    }

    // Lấy kích thước file từ thông báo xác nhận
    long file_size;
    sscanf(confirm_message, "OK %ld", &file_size);
    printf("File size: %ld bytes\n", file_size);

    // Nhận và lưu nội dung file
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        printf("Error: Failed to open file.\n");
        close(client_socket);
        return 1;
    }

    char buffer[MAX_FILE_SIZE];
    size_t total_bytes_received = 0;
    size_t bytes_received;
    while (total_bytes_received < file_size) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Error: Failed to receive file.\n");
            fclose(file);
            close(client_socket);
            return 1;
        }
        fwrite(buffer, 1, bytes_received, file);
        total_bytes_received += bytes_received;
    }

    printf("File downloaded successfully.\n");

    fclose(file);
    close(client_socket);

    return 0;
}
