import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


# # /cgi-bin/hello.py/12345
# # path_info = os.environ['PATH_INFO']
# # print("Received PATH_INFO:", path_info) # 12345



# # print('Content-Type: ' + os.environ['REQUEST_METHOD']+'\r\n', end="")
# print("Set-Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205 \r\n", end="")
# A valid HTTP response with chunked transfer encoding
print("HTTP/1.1 200 OK\r\n", end="")
print("Transfer-Encoding: chunked\r\n", end="")
print("Connection: keep-alive\r\n", end="")
print("Content-Type: text/html\r\n\r\n", end="")

print("<h1>hello</h1>")

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

# import requests

# Define the URL you want to send the GET request to
# url = 'http://0.0.0.0:3344//script.py'  # Replace with your desired URL

# Send the GET request
# response = requests.get(url)


import subprocess

# Example curl command
curl_command = "curl -X GET http://0.0.0.0:3344/script.py"

# # Execute the curl command
result = subprocess.run(curl_command, shell=True, capture_output=True, text=True)

# # Check if the request was successful
# if response.status_code == 200:
#     print("Request successful!")
#     print("Response content:", response.text)  # or response.json() if the response is in JSON format
# else:
#     print(f"Failed to retrieve data. HTTP Status Code: {response.status_code}")





# res
# Set-Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;

# req
# Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;