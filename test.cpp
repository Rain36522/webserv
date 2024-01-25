#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>

#define MAX_EVENTS 100
#define PORT 8080

class WebServer {
public:
    WebServer() {
        server_fd = createServerSocket();
        kq = kqueue();
        if (kq == -1) {
            perror("Error creating kqueue");
            exit(EXIT_FAILURE);
        }

        struct kevent change;
        EV_SET(&change, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &change, 1, nullptr, 0, nullptr) == -1) {
            perror("Error adding server socket to kqueue");
            exit(EXIT_FAILURE);
        }

        std::cout << "Server is listening on port " << PORT << "...\n";
    }

    ~WebServer() {
        close(server_fd);
    }

    void run() {
        while (true) {
            int nev = kevent(kq, nullptr, 0, events, MAX_EVENTS, nullptr);
            if (nev == -1) {
                perror("Error in kevent");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < nev; ++i) {
                if (events[i].ident == server_fd) {
                    // New client connection
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

                    if (client_fd == -1) {
                        perror("Error accepting connection");
                        exit(EXIT_FAILURE);
                    }

                    std::cout << "New client connected : " << client_fd << " " << i << std::endl;

                    // TODO: Handle the client connection, e.g., read/write data
                    // ...

                    close(client_fd);
                }
            }
			sleep(6);
        }
    }

private:
    int server_fd;
    int kq;
    struct kevent events[MAX_EVENTS];

    int createServerSocket() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("Error creating socket");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);

        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("Error binding socket");
            exit(EXIT_FAILURE);
        }

        if (listen(sockfd, SOMAXCONN) == -1) {
            perror("Error listening on socket");
            exit(EXIT_FAILURE);
        }

        return sockfd;
    }
};

int main() {
    WebServer server;
    server.run();

    return 0;
}
