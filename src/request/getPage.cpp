/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getPage.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 10:13:51 by pudry             #+#    #+#             */
/*   Updated: 2024/01/31 10:20:05 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

std::string	getHtmlPage(std::string path)
{
	std::ifstream	HtmlFile;
	std::string		tmp;
	std::string		Html;

	HtmlFile.open(path);
	if (HtmlFile.fail())
		return ("");
	while (getline(HtmlFile, tmp))
		Html += tmp;
	HtmlFile.close();
	return (Html);	
}