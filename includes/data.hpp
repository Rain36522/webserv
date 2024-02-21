/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 10:05:10 by pudry             #+#    #+#             */
/*   Updated: 2024/02/21 10:19:57 by pudry            ###   ########.ch       */
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
#include <cstdio>
#include <map>

#define PORT 80
#define DEBUG std::cout << "\033[31m" << __FILE__ << __LINE__ << "\033[0m" << std::endl;
#define DEBUGOUT std::cout

#define LOGIN "abcd"
#define PASSWORD "1234"

#define PATH_INFO "./CGI"

#define RESET   " \033[0m"
#define RESETN   << "\033[0m\n"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"
#define ORANGE  "\033[38;5;208m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"
#define	HBLUE	"\033[48;5;17m"
#define HRED	"\033[41m"
#define BOLD	"\033[1m"
#define NOBOLD	"\033[0m"

bool		validateFd(std::string file, int type);
bool		validateFd(int fd, int type);
std::string	getErrorHtml(std::string File, int errorCode);
int			getHtml(std::string path, std::string &html);
int			getHtmlFd(int fd, std::string &html);
