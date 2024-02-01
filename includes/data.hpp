/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:01:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/01 13:46:41 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "structs.hpp"
#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#define DEBUG std::cout << "\033[31m" << __FILE__ << __LINE__ << "\033[0m" << std::endl;

HttpRequest	requestToStruct(int fd);
std::string	getHtmlPage(std::string path);
bool 		downloadImage(int sockfd, HttpRequest HttpStruct);
void 		sendHTMLResponse(int client_fd, const std::string& htmlPage);
std::string receiveHTTPRequest(int client_fd, int RequestLength);
void processFileUpload(const std::string& httpRequest);
int	put_in_file(std::string filename, std::string content, int length);