/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPage.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/06 16:25:50 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

std::string	getErrorHtml(std::string File, int errorCode)
{
	std::ifstream	HtmlFile;
	std::string		tmp;
	std::string		Html;

	HtmlFile.open(File);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page\n");
		return "";	
	}
	while (getline(HtmlFile, tmp))
	{
		if (tmp.find("{Error_code}") != std::string::npos)
			tmp.replace(tmp.find("{Error_code}"), 12, std::to_string(errorCode));
		Html += tmp;
	}
	HtmlFile.close();
	return (Html);
}

std::string	getHtmlPage(std::string path)
{
	std::ifstream	HtmlFile;
	std::string		tmp;
	std::string		Html;

	HtmlFile.open(path);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page\n");
		return "";	
	}
	while (getline(HtmlFile, tmp))
		Html += tmp;
	HtmlFile.close();
	return (Html);
}


int	getHtml(std::string path, std::string &html)
{
	std::ifstream	HtmlFile;
	std::string		tmp;

	HtmlFile.open(path);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page\n");
		return 404;	
	}
	while (getline(HtmlFile, tmp))
		html += tmp;
	HtmlFile.close();
	return 200;	
}

// Envoi de la réponse HTTP (en-têtes et contenu HTML) au client
void sendHTMLResponse(int client_fd, const std::string& htmlPage)
{
	std::string responseHeaders = "HTTP/1.1 200 OK\r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(htmlPage.size()) + "\r\n";
	responseHeaders += "Server: salut\r\n"; // Ajouter le nom du serveur dans le header Server
	responseHeaders += "\r\n";
	ssize_t sent = send(client_fd, responseHeaders.c_str(), responseHeaders.size(), 0);
	if (sent == -1)
	{
		perror("Error sending response headers");
		// Gérer l'erreur appropriée, fermer la connexion, etc.
	}
	sent = send(client_fd, htmlPage.c_str(), htmlPage.size(), 0);
	if (sent == -1) {
		perror("Error sending HTML content");
		// Gérer l'erreur appropriée, fermer la connexion, etc.
	}
}
