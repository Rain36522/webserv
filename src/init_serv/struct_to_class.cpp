/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_to_class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pudry <pudry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 09:03:20 by pudry             #+#    #+#             */
/*   Updated: 2024/01/30 09:25:50 by pudry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/data.hpp"

ServConf	*init_struct(void)
{
	ServConf 	*Conf = new ServConf;

	if (Conf->name == "")
		Conf->name = "Server_" + std::to_string(Conf->port);
	Conf->port = 8080;
	Conf->host = 1;
	Conf->body_limit = 3;
	Conf->idirectory_listing = 1;
	Conf->cgi = "./path?";
	Conf->bdirectory_listing = false;
	Conf->ErrPage = "./Html_code/Err_page.html";
	Conf->Get = true;
	Conf->Post = true;
	Conf->Delete = true;
	return (Conf);
}


void	struct_to_class(void)
{
	ServConf	*Conf;

	Conf = init_struct();
}