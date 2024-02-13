/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:46:39 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/13 16:28:59 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <dirent.h>


int main(int ac, char*av[], char*env[])
{
	const char* val = std::getenv("inputNum");
	const char* folder = std::getenv("uploadFolder");
	if (!val || std::atoi(val) < 1 || std::atoi(val) > 3 || !folder)
		return (1);
	std::string path(folder);
	std::srand(std::time(nullptr));
	std::cout <<"<!DOCTYPE html>"
					"<html lang=\"en\">"
						"<head>"
							"<meta charset=\"utf-8\" />"
						"</head>"
						"<body>"
							"<title>Python CGI</title>"
							"<style>"
								"body{"
									"background-color: #252525;"
									"color: rgb(148, 145, 145);"
									"font-size: 15px;"
									"text-align: center;"
								"}"
							"</style>"
							"<h1>C++ CGI</h1>"
						"</body>";
	if (std::atoi(val) == std::rand() % 3 + 1)
	{
		DIR *directory = opendir(folder);
		struct dirent *entry;
		if (directory != NULL) {
        	while ((entry = readdir(directory)) != NULL) {
            	if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            	    char file_path[256];
            	    sprintf(file_path, "%s/%s", folder, entry->d_name);
            	    if (remove(file_path) != 0)
            	        std::cout << "<p>Error deleting file: " << file_path << "</p>";
            	    else 
            	        std::cout << "<p>Deleted file: " << file_path << "</p>";
            	}
        	}
        closedir(directory);
		} else {
			std::cout << "<p>Error opening directory.</p>";
			std::cout << "</html>";
			return (EXIT_FAILURE);
		}
	}
	else
		std::cout << "<p>No files deleted!</p>";
		std::cout << "</html>";
	return (EXIT_SUCCESS);
}