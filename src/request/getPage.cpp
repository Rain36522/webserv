/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPage.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:51 by pudry             #+#    #+#             */
/*   Updated: 2024/02/14 10:54:58 by pudry            ###   ########.fr       */
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
		std::string err = "Failed to read html page (1) " + File;
		perror(err.c_str());
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
	if (i <= 0)
		return 500;
	return 200;
	
}

static std::string	getCodeText(int code)
{
	switch (code)
	{
	case 400:
		return ("400 Bad Request");
	case 404:
		return ("404 Not Found");
	case 413:
		return ("413 Payload to large");
	case 418:
		return ("418 I'm a tea pot");
	case 500:
		return ("400 Internal server error");
	case 302:
		return ("302 Found");
	default:
		return ("200 Ok");
	}
}


// Envoi de la réponse HTTP (en-têtes et contenu HTML) au client
void sendHTMLResponse(int client_fd, const std::string& htmlPage, int code, std::string server_name)
{
	std::string responseHeaders = "HTTP/1.1 " + getCodeText(code) +  " \r\n";
	responseHeaders += "Content-Type: text/html\r\n";
	responseHeaders += "Content-Length: " + std::to_string(htmlPage.size()) + "\r\n";
	responseHeaders += "Server: " + server_name +"\r\n";
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


void sendRedirectResponse(int client_fd, const std::string& redirectLocation) {
    std::string responseHeaders = "HTTP/1.1 302 Found\r\n";
    responseHeaders += "Location: " + redirectLocation + "\r\n";
    responseHeaders += "Server: salut\r\n";
    responseHeaders += "\r\n";

    ssize_t sent = send(client_fd, responseHeaders.c_str(), responseHeaders.size(), 0);
    if (sent == -1) {
        perror("Error sending redirect response");
    }
}

void	erraseHtmlVar(std::string &html)
{
	size_t	i;

	if ((i = html.find("{Delete_list}")) == std::string::npos)
		return ;
	html.erase(i, 13);
}
