/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPage.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/13 16:52:05 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

bool validateFd(std::string file, int type)
{
	int fd_type = O_RDONLY;
	if (type == EVFILT_WRITE)
		fd_type = O_WRONLY;
	int fd = open(file.c_str(), fd_type);
	if (fd == -1)
		return (false);
	int kfd = kqueue();
	if (kfd == -1)
	{
		close(fd);
		return false;
	}
	struct kevent change;
	struct kevent events[1];

	EV_SET(&change, fd, type, EV_ADD, 0, 0, NULL);
	int ret = kevent(kfd, &change, 1, events, 1, NULL);
	close(kfd);
	close(fd);
	if (ret < 1)
		return false;
	return events[0].ident == (uintptr_t) fd;
}

std::string	getErrorHtml(std::string File, int errorCode)
{
	std::ifstream	HtmlFile;
	std::string		tmp;
	std::string		Html;

	HtmlFile.open(File);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page1\n");
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

	if (!validateFd(path, EVFILT_READ))
	{
		perror("Failed to read html page2\n");
		return "";
	}
	HtmlFile.open(path);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page3\n");
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

	if (!validateFd(path, EVFILT_READ))
	{
		perror("Failed to read html page4\n");
		return 404;
	}
	HtmlFile.open(path);
	
	if (HtmlFile.fail())
	{
		perror("Failed to read html page5\n");
		return 404;	
	}
	while (getline(HtmlFile, tmp))
		html += tmp;
	HtmlFile.close();
	return 200;	
}

int	getHtmlFd(int fd, std::string &html)
{
	char	buffer[256];
	int		i = 255;

	while (i == 255)
	{
		i = read(fd, buffer, 255);
		html += buffer;
		for (int j = 0; j <= 255; j ++)
			buffer[j] = 0;
	}
	if (i < 0)
		return 500;
	return 200;
	
}
// Envoi de la réponse HTTP (en-têtes et contenu HTML) au client
void sendHTMLResponse(int client_fd, const std::string& htmlPage, int code)
{
	std::string responseHeaders = "HTTP/1.1 " + std::to_string(code) +  " \r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(htmlPage.size()) + "\r\n";
	responseHeaders += "Server: salut\r\n"; // Ajouter le nom du serveur dans le header Server
	responseHeaders += "\r\n";
	std::cout << RED << "Reponse : \n" << responseHeaders << RESET << std::endl;
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

void	erraseHtmlVar(std::string &html)
{
	size_t	i;

	if ((i = html.find("{Delete_list}")) == std::string::npos)
		return ;
	html.erase(i, 13);
}
