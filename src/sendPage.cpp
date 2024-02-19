/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendPage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:55:15 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/19 14:33:04 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/data.hpp"

std::string	getErrorHtml(std::string File, int errorCode)
{
	std::ifstream	HtmlFile;
	std::string		tmp;
	std::string		Html;

	HtmlFile.open(File);
	
	if (HtmlFile.fail())
	{
		std::cerr << ORANGE << "Failed to read html file : " << File << RESET << std::endl;
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

	std::cout << RED << "Path : " << path << RESET << std::endl;
	if (!validateFd(path, EVFILT_READ))
	{
		std::cerr << ORANGE << "Failed to read html file"<< RESET << std::endl;
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
