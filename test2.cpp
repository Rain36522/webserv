#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>

#define MAX_EVENTS 100
#define PORT 8080

// Déclaration de la fonction pour recevoir une requête HTTP
std::string receiveHTTPRequest(int client_fd);

class WebServer {
public:
    // Constructeur initialisant le socket serveur et kqueue
    WebServer() {
        server_fd = createServerSocket();
        kq = kqueue();
        if (kq == -1) {
            perror("Error creating kqueue");
            exit(EXIT_FAILURE);
        }

        // Configuration de l'événement pour surveiller le socket serveur en lecture
        struct kevent change;
        EV_SET(&change, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &change, 1, nullptr, 0, nullptr) == -1) {
            perror("Error adding server socket to kqueue");
            exit(EXIT_FAILURE);
        }

        std::cout << "Server is listening on port " << PORT << "...\n";
    }

    // Destructeur fermant le socket serveur
    ~WebServer() {
        close(server_fd);
    }

    // Méthode principale pour exécuter le serveur
    void run() {
		std::string htmlPage = loadHTMLFromFile("main2.html");
        while (true) {
            // Attente d'événements avec kqueue
            int nev = kevent(kq, nullptr, 0, events, MAX_EVENTS, nullptr);
            if (nev == -1) {
                perror("Error in kevent");
                exit(EXIT_FAILURE);
            }

            // Traitement des événements
            for (int i = 0; i < nev; ++i) {
                if (events[i].ident == server_fd) {
                    // Nouvelle connexion client
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

                    if (client_fd == -1) {
                        perror("Error accepting connection");
                        exit(EXIT_FAILURE);
                    }

                    std::cout << "New client connected\n";

                    // Réception et affichage de la requête HTTP du client
                    std::string httpRequest = receiveHTTPRequest(client_fd);
                    std::cout << "Received HTTP request:\n" << httpRequest << std::endl;

                    // Envoi de la page HTML depuis un fichier au client
                    sendHTMLResponse(client_fd, htmlPage);

                    // Fermeture de la connexion client
                    close(client_fd);
                }
            }
        }
    }

private:
    int server_fd;
    int kq;
    struct kevent events[MAX_EVENTS];

    // Création du socket serveur
    int createServerSocket() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("Error creating socket");
            exit(EXIT_FAILURE);
        }

        // Configuration de l'adresse du socket
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);

        // Liaison du socket à l'adresse
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("Error binding socket");
            exit(EXIT_FAILURE);
        }

        // Mise en écoute du socket
        if (listen(sockfd, SOMAXCONN) == -1) {
            perror("Error listening on socket");
            exit(EXIT_FAILURE);
        }

        return sockfd;
    }

    // Chargement du contenu HTML depuis un fichier
    std::string loadHTMLFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening HTML file: " << filePath << std::endl;
            return "";
        }

        std::stringstream content;
        content << file.rdbuf();
        return content.str();
    }

    // Envoi de la réponse HTTP (en-têtes et contenu HTML) au client
    void sendHTMLResponse(int client_fd, const std::string& htmlPage) {
        std::string responseHeaders = "HTTP/1.1 200 OK\r\n";
        responseHeaders += "Content-Type: text/html\r\n";
        responseHeaders += "Content-Length: " + std::to_string(htmlPage.size()) + "\r\n";
        responseHeaders += "\r\n";

        ssize_t sent = send(client_fd, responseHeaders.c_str(), responseHeaders.size(), 0);
        if (sent == -1) {
            perror("Error sending response headers");
            // Gérer l'erreur appropriée, fermer la connexion, etc.
        }

        sent = send(client_fd, htmlPage.c_str(), htmlPage.size(), 0);
        if (sent == -1) {
            perror("Error sending HTML content");
            // Gérer l'erreur appropriée, fermer la connexion, etc.
        }
    }
};

// Fonction pour recevoir une requête HTTP du client
std::string receiveHTTPRequest(int client_fd) {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    std::string httpRequest;

    ssize_t bytesRead;
    do {
        bytesRead = recv(client_fd, buffer, bufferSize - 1, 0);
        if (bytesRead == -1) {
            perror("Error receiving HTTP request");
            // Gérer l'erreur appropriée, fermer la connexion, etc.
            return "";
        }

        buffer[bytesRead] = '\0';
        httpRequest += buffer;
    } while (bytesRead == bufferSize - 1);

    return httpRequest;
}

int main() {
    // Création et exécution du serveur web
    WebServer server;
    server.run();

    return 0;
}
