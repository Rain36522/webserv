# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi.py                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dvandenb <dvandenb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/13 14:46:49 by dvandenb          #+#    #+#              #
#    Updated: 2024/02/13 14:46:55 by dvandenb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import time
import os


def	getCalcul(calcul):
	calcul = calcul.replace("+", " ")
	calcul = calcul.replace("%2B", "+")
	calcul = calcul.replace("%2F", "/")
	return calcul

def	doCGI():
		tm = time.strftime("%A, %d %B %Y, %H:%M")
		try:
			calcul = os.environ["calcul"]
			calcul = getCalcul(calcul)
			result = eval(calcul)
			calcul += " = <b>"
		except:
			calcul = "<b>Invalid Input"
			result = ""

		html1 = """
		<!DOCTYPE html>
		<html lang="en">
			<head>
				<meta charset="utf-8" />
			</head>
			<body>
				<title>Python CGI</title>
				<style>

					body{
						background-color: #252525;
						color: rgb(148, 145, 145);
						font-size: 15px;
						text-align: center;
					}
					a {
						text-decoration: none;
						color: rgb(168, 165, 165);
					}
				</style>
		"""
		html2 = "\n<h1>Python CGI</h1>\n"
				
		html3 = "</body>\n</html>"
		tm ="<p>" + tm + "</p>"
		if (result == 69):
			result = "<a href=\"https://bit.ly/3UH3TNg\" target=\"_blank\">Secret web page</a>"
		elif (result == 42):
			result = "<a href=\"https://bit.ly/49bGzfa\" target=\"_blank\">Secret web page</a>"
		value = "<p>" + calcul + str(result) + "</b></p>"

		print(html1 + tm + html2 + value + html3)

doCGI()