import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


print("HTTP/1.1 200 OK\r\n", end="")
print("Transfer-Encoding: chunked\r\n", end="")
print("Connection: keep-alive\r\n", end="")
print("Content-Type: text/html\r\n\r\n", end="")

print("<h1>hello</h1>")
print("<p>")
print(" ".join(thislist))
print("</p>")

try:
	print(f"<p>first name : {os.environ['fname']}</p>")
	print(f"<p>last name : {os.environ['lname']}</p>")
except KeyError:
	print("Incomplete Params")




# while True:
# 	pass
# for i in range(1001):
#     print(i)
# print("end")




# import subprocess

# # Example curl command
# curl_command = "curl -X GET http://0.0.0.0:3344/script.py"

# # # Execute the curl command
# result = subprocess.run(curl_command, shell=True, capture_output=True, text=True)
