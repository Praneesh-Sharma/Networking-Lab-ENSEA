#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define TFTP_PORT 69
#define BUFFER_SIZE 516

#define WRQ 2
#define DATA 3
#define ACK 4
#define ERROR 5

void create_wrq_packet(const char *filename, char *packet) {
    packet[0] = 0; 
    packet[1] = WRQ; 
    strcpy(&packet[2], filename); 
    memset(&packet[2 + strlen(filename)], 0, 1); 
    strcpy(&packet[3 + strlen(filename)], "octet"); 
    memset(&packet[3 + strlen(filename) + 5], 0, 1); 
}

void send_file(int sockfd, struct sockaddr_in *server_addr, const char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }

    int server_len = sizeof(*server_addr);
    int block_num = 1;

    while (1) {
        int bytes_read = fread(&buffer[4], 1, 512, file);
        if (bytes_read < 0) {
            perror("Failed to read file");
            break;
        }

        buffer[0] = 0; 
        buffer[1] = DATA;
        buffer[2] = (block_num >> 8) & 0xFF;
        buffer[3] = block_num & 0xFF;

        int packet_len = bytes_read + 4;
        sendto(sockfd, buffer, packet_len, 0, (struct sockaddr *)server_addr, server_len);

        int ack_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)server_addr, &server_len);
        if (ack_len < 0) {
            perror("Failed to receive ACK");
            break;
        }

        if (buffer[1] == ACK) {
            int received_block_num = (buffer[2] << 8) | buffer[3];
            if (received_block_num != block_num) {
                fprintf(stderr, "Block number mismatch. Expected %d, received %d", block_num, received_block_num);
                break;
            }

            if (bytes_read < 512) {
                break;
            }

            block_num++;
        } else if (buffer[1] == ERROR) {
            fprintf(stderr, "Error from server: %s", &buffer[4]);
            break;
        } else {
            fprintf(stderr, "Unexpected packet type: %d", buffer[1]);
            break;
        }
    }

    fclose(file);
    printf("File '%s' uploaded successfully.\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server> <filename>", argv[0]);
        return 1;
    }

    const char *server_name = argv[1];
    const char *filename = argv[2];

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(server_name, NULL, &hints, &res) != 0) {
        perror("Failed to resolve server address");
        return 1;
    }

    struct sockaddr_in server_addr = *(struct sockaddr_in *)res->ai_addr;
    server_addr.sin_port = htons(TFTP_PORT);
    freeaddrinfo(res);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        return 1;
    }

    char packet[BUFFER_SIZE];
    create_wrq_packet(filename, packet);
    sendto(sockfd, packet, strlen(filename) + 9, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send_file(sockfd, &server_addr, filename);

    close(sockfd);
    return 0;
}
