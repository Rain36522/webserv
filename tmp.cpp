/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:55:28 by pudry             #+#    #+#             */
/*   Updated: 2024/01/31 15:57:22 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


bool handleClientRequest(int client_fd, std::map<int, time_t>& clientActivity, std::map<int, std::string>& clientDataToSend)
{
    char buffer[1024] = {0}; // Buffer pour stocker les données lues à partir du socket du client

    std::cout << "Client request, socket: " << client_fd << std::endl; // Affiche le socket du client dont une requête est reçue

    ssize_t valread = read(client_fd, buffer, 1024); // Lit les données à partir du socket du client dans le buffer

    static int to_read = 0; // Stocke le nombre d'octets restants à lire dans le corps d'une requête POST
    static bool is_post_body = 0; // Indique si le script est en train de lire le corps d'une requête POST
    bool hasDataToSend = false; // Indique s'il y a des données à envoyer au client après avoir

    if (valread > 0) // Si des données ont été lues avec succès depuis le socket du client
    {
        std::string buf2(buffer, valread); // Convertit les données lues en une chaîne de caractères

        static HttpRequest request; // Initialise une variable statique pour stocker la requête HTTP
        if (!is_post_body) // Si le script n'est pas déjà en train de lire le corps d'une requête POST
        {
            request = HttpRequest(buf2, client_fd); // Analyse la requête HTTP
            if (request.method == "POST") // Si la méthode de la requête est POST
            {
                is_post_body = 1; // Indique que le script est en train de lire le corps d'une requête POST
                to_read = request.headers["Content-Length"].empty() ? 0 : std::atoi(request.headers["Content-Length"].c_str()); // Stocke le nombre d'octets à lire dans le corps de la requête
                to_read -= request.rawBody.size(); // Soustrait la taille du corps de la requête déjà lue
                if (to_read <= 0) // Si tout le corps de la requête a déjà été lu
                {
                    is_post_body = 0; // Réinitialise le drapeau indiquant que le script est en train de lire le corps d'une requête POST
                    to_read = 0; // Réinitialise le nombre d'octets à lire dans le corps de la requête
                }
            }
        }
        else // Si le script est déjà en train de lire le corps d'une requête POST
        {
            request.rawBody += buf2; // Ajoute les données lues au corps de la requête
            to_read -= valread; // Soustrait la taille des données lues du nombre d'octets restants à lire dans le corps de la requête
            if (to_read <= 0) // Si tout le corps de la requête a été lu
            {
                is_post_body = 0; // Réinitialise le drapeau indiquant que le script est en train de lire le corps d'une requête POST
                to_read = 0; // Réinitialise le nombre d'octets à lire dans le corps de la requête
            }
        }

        // Ouvre un fichier de sortie pour journaliser la requête HTTP (à des fins de débogage)
        std::ofstream outfile("test.txt");
        if (1) // Si l'ouverture du fichier a réussi
        {
            // Écrit les détails de la requête HTTP dans le fichier de sortie
            outfile << std::endl << "Client request in class : " << std::endl;
            outfile << "method : " << request.method << std::endl;
            outfile << "uri : " << request.uri << std::endl;
            outfile << "httpVersion : " << request.httpVersion << std::endl;
            std::map<std::string, std::string>::iterator it;
            for (it = request.headers.begin(); it != request.headers.end(); it++)
            {
                outfile << it->first << " <-> " << it->second << std::endl;
            }
            outfile << "body : " << request.rawBody << std::endl;
        }
        outfile.close(); // Ferme le fichier de sortie

        if (!is_post_body) // Si le script n'est pas en train de lire le corps d'une requête POST
            hasDataToSend = request.HandleRequest(clientDataToSend, client_fd); // Génère les données à envoyer au client

        clientActivity[client_fd] = time(nullptr);  // Mettre à jour l'heure de la dernière activité du client
    }
    else if (valread == 0) // Si le client a fermé la connexion
    {
        std::cout << "Client disconnected, socket: " << client_fd << std::endl; // Affiche le socket du client qui s'est déconnecté
        close(client_fd); // Ferme le socket du client
        clientActivity.erase(client_fd);  // Supprime le client de la map des activités des clients
    }
    else // Si une erreur s'est produite lors de la lecture des données depuis le socket du client
    {
        perror("read"); // Affiche un message d'erreur
        close(client_fd); // Ferme le socket du client
        clientActivity.erase(client_fd);  // Supprime le client de la map des activités des clients
    }
    return hasDataToSend; // Renvoie un indicateur indiquant s'il y a des données à envoyer au client
}
