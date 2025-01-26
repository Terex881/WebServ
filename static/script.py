import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


# # /cgi-bin/hello.py/12345
# # path_info = os.environ['PATH_INFO']
# # print("Received PATH_INFO:", path_info) # 12345



# # print('Content-Type: ' + os.environ['REQUEST_METHOD']+'\r\n', end="")
print(" ".join(thislist))
print('first name : ' + os.environ['fname'])
print('last name : ' + os.environ['lname'])
# while True:
# 	pass
# for i in range(1001):
#     print(i)
# print("end")

# import requests

# Define the URL you want to send the GET request to
# url = 'http://0.0.0.0:3344/cgi-bin/script.py'  # Replace with your desired URL

# Send the GET request
# response = requests.get(url)


# import subprocess

# # Example curl command
# curl_command = "curl -X GET http://0.0.0.0:3344/script.py"

# # Execute the curl command
# result = subprocess.run(curl_command, shell=True, capture_output=True, text=True)

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