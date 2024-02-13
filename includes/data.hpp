/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:01:35 by pudry             #+#    #+#             */
/*   Updated: 2024/02/13 16:41:25 by pudry            ###   ########.fr       */
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
#include <dirent.h>
#include <cstdlib>

#define PORT 80
#define DEBUG std::cout << "\033[31m" << __FILE__ << __LINE__ << "\033[0m" << std::endl;
#define RESET   "\033[0m"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"  

bool validateFd(std::string file, int type);
// Send news request
std::string	getHtmlPage(std::string path);
std::string	getErrorHtml(std::string File, int errorCode);
int			getHtml(std::string path, std::string &html);
int			getHtmlFd(int fd, std::string &html);
void 		sendHTMLResponse(int client_fd, const std::string& htmlPage, int code);
void		erraseHtmlVar(std::string &html);

// Read new request;
HttpRequest receiveHTTPRequest(int client_fd, int RequestLength, HttpRequest request);
HttpRequest	requestToStruct(int fd);
HttpRequest 	pathToData(HttpRequest request);

// Download gpt, useless?
void processFileUpload(const std::string& httpRequest);
int	put_in_file(std::string filename, std::string content, int length);

// Download image
HttpRequest	requestToFile(HttpRequest &request, std::string uploadPath);
