import time
import os

time = time.strftime("%A, %d %B %Y, %H:%M")
try:
	calcul = os.environ["calcul"]
	result = str(eval(calcul))
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
		</style>
"""
html2 = "\n<h1>Python CGI</h1>\n"
		
html3 = "</body>\n</html>"
time ="<p>" + time + "</p>"
value = "<p>" + calcul + result + "</b></p>"

print(html1 + time + html2 + value + html3)