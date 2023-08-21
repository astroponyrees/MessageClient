#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " hostname port" << std::endl;
        return 1;
    }

    portno = atoi(argv[2]); // Convert the port from a string to an integer.

    // Create a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "ERROR opening socket" << std::endl;
        return 1;
    }

    server = gethostbyname(argv[1]); // Get the server's DNS entry.
    if (server == nullptr) {
        std::cerr << "ERROR, no such host" << std::endl;
        return 1;
    }

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to the server.
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "ERROR connecting" << std::endl;
        return 1;
    }

    std::cout << "Connected to the server. Type your message (type 'quit' to exit):" << std::endl;

    while (true) {
        // Clear the buffer and read a message from the user.
        memset(buffer, 0, sizeof(buffer));
        std::cin.getline(buffer, sizeof(buffer));

        // Send the message to the server.
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            std::cerr << "ERROR writing to socket" << std::endl;
            break;
        }

        // If the user typed "quit", exit the loop.
        if (strcmp(buffer, "quit") == 0)
            break;
    }

    // Close the socket and exit.
    close(sockfd);
    return 0;
}
