/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 14:46:39 by dvandenb          #+#    #+#             */
/*   Updated: 2024/02/20 14:42:25 by dvandenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <filesystem>

int main()
{
	const char* val = std::getenv("inputNum");
	const char* folder = std::getenv("uploadFolder");
	const char* path_t = std::getenv("PWD");

	if (!val || std::atoi(val) < 1 || std::atoi(val) > 3 || !folder || !path_t)
		return (EXIT_FAILURE);
	std::string temp1(folder);
	std::string temp2(path_t);
	std::string temp3 =  temp2 + "/" + temp1;
	const char* full = temp3.c_str();
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
		DIR *directory = opendir(full);
		struct dirent *entry;
		if (directory != NULL) {
        	while ((entry = readdir(directory)) != NULL) {
            	if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            	    char file_path[256];
            	    sprintf(file_path, "%s/%s", full, entry->d_name);
            	    if (remove(file_path) != 0)
            	        std::cout << "<p>Error deleting file: " << file_path << "</p>";
            	    else 
            	        std::cout << "<p>Deleted file: " << file_path << "</p>";
            	}
        	}
        closedir(directory);
		} else {
			std::cout << "<p>Error opening directory " << std::string(full) << ".</p>";
			std::cout << "</html>";
			return (EXIT_SUCCESS);
		}
	}
	else
		std::cout << "<p>No files deleted!</p>";
		std::cout << "</html>";
	return (EXIT_SUCCESS);
}