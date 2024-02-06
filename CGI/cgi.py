import time
import os

time = time.strftime("%A, %d %B %Y, %H:%M")
calcul = "2 + 3"
result = "5"

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
		
html3 = """
	</body>
</html>
"""
time ="<p>" + time + "</p>"
value = "<p>" + calcul + " = <b>" + result + "</b></p>"

print(html1 + time + html2 + value + html3)
print("<==============================================================================================================================================================================================================================================================================>")
try:
	print(os.environ["num1"])
except:
	print("Variable not found")