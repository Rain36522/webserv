/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:01:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/02 17:39:13 by dvandenb         ###   ########.fr       */
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

// Send news request
std::string	getHtmlPage(std::string path);
int	getHtml(std::string path, std::string &html);
void 		sendHTMLResponse(int client_fd, const std::string& htmlPage);

// Read new request;
std::string receiveHTTPRequest(int client_fd, int RequestLength);
HttpRequest	requestToStruct(int fd);
// Download gpt, useless?
void processFileUpload(const std::string& httpRequest);
int	put_in_file(std::string filename, std::string content, int length);

// Download image
HttpRequest	requestToFile(HttpRequest request);
