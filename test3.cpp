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
#include "includes/data.hpp"

#define MAX_EVENTS 100
#define PORT 443

// Declaration of the function to receive an HTTP request
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

	std::string extractImageURL(const std::string& httpRequest) {
		std::istringstream iss(httpRequest); // Create a string stream to parse the HTTP request
		std::string line; // String to hold each line of the request
		std::string imageURL; // String to store the extracted image URL

		bool inBody = false; // Flag to indicate if we are in the body of the request

		// Iterate through each line of the HTTP request
		while (std::getline(iss, line)) {
			std::cout << "Content : " << line << std::endl;
			
			// Check if the line marks the beginning of the body
			if (!inBody && line.empty()) {
				inBody = true;
				continue;
			}

			// If we are in the body, we can read the file data
			if (inBody) {
				// Here, you can process the file data as needed
				std::cout << "File data: " << line << std::endl;
				// For example, you can store it in a separate variable or write it to a file
			}

			// Check if the line contains the "Content-Type" header
			if (!inBody && line.find("Content-Type") != std::string::npos) {
				// Assuming the Content-Type header contains "multipart/form-data"
				// You may need to adjust this condition based on your actual request format
				if (line.find("multipart/form-data") != std::string::npos) {
					// Assuming the URL is in the Content-Disposition header
					// You may need to adjust this condition and extraction method based on your actual request format
					size_t pos = line.find("filename=\"");
					if (pos != std::string::npos) {
						// Extracting the URL from the Content-Disposition header
						imageURL = line.substr(pos + 10); // 10 is the length of "filename=\""
						// Trimming the quotation mark at the end
						imageURL = imageURL.substr(0, imageURL.size() - 1);
						break; // Stop parsing once the URL is found
					}
				}
			}
		}

		return imageURL; // Return the extracted image URL
	}


	void downloadImage(const std::string& imageURL) {
		// Open a new HTTP connection to the image URL
		// Here, you would implement the logic to open a connection and retrieve the image data
		// I'll show you an example based on the curl library to simplify this

		// std::string command = "curl -o downloaded_image.jpg " + imageURL;
		// system(command.c_str()); // Execute the curl command to download the image
		std::cout << "img url : \n" << imageURL << std::endl;

		// You can also use other libraries like libcurl to download the image
	}

	void run() {
		std::string htmlPage = loadHTMLFromFile("./Html_code/file.html");
		while (true) {
			// Wait for events with kqueue
			int nev = kevent(kq, nullptr, 0, events, MAX_EVENTS, nullptr);
			if (nev == -1) {
				perror("Error in kevent");
				exit(EXIT_FAILURE);
			}

			// Handle events
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
					std::cout << "<=======================================================================================>\n";
					std::cout << "<                              New client connected                                     >\n";
					std::cout << "<=======================================================================================>\n";

					// Receive and display the client's HTTP request
					// std::string httpRequest = receiveHTTPRequest(client_fd);
					std::string httpRequest = requestToStruct(client_fd).body;
					std::cout << "Received HTTP request:\n" << httpRequest << std::endl;
					
					if (httpRequest.find("POST") != std::string::npos)
					{
						std::cout << std::endl << "post methode?\n";
						// Extract the image URL from the HTTP request
						std::string imageURL = extractImageURL(httpRequest);
						std::cout << "start download\n";
						// Download the image
						downloadImage(imageURL);
					}

					sendHTMLResponse(client_fd, htmlPage);

					// Send HTTP response to the client
					// Here, you can send a response indicating that the image was successfully downloaded or perform other actions based on your use case

					// Close the client connection
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
		responseHeaders += "Server: salut\r\n"; // Ajouter le nom du serveur dans le header Server
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
