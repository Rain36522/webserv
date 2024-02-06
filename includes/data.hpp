/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:01:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/06 08:37:24 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "structs.hpp"
#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <string>

#define PORT 80
#define DEBUG std::cout << "\033[31m" << __FILE__ << __LINE__ << "\033[0m" << std::endl;

// Send news request
std::string	getHtmlPage(std::string path);
std::string	getErrorHtml(std::string File, int errorCode);
int	getHtml(std::string path, std::string &html);
void 		sendHTMLResponse(int client_fd, const std::string& htmlPage);

// Read new request;
HttpRequest receiveHTTPRequest(int client_fd, int RequestLength, HttpRequest request);
HttpRequest	requestToStruct(int fd);
HttpRequest 	pathToData(HttpRequest request);

// Download gpt, useless?
void processFileUpload(const std::string& httpRequest);
int	put_in_file(std::string filename, std::string content, int length);

// Download image
HttpRequest	requestToFile(HttpRequest request, std::string uploadPath);
