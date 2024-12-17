import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]



print("Server: TTS \r\n", end="")
print("Set-Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205 \r\n", end="")
print("Content-Type: text/html       \r\n", end="")
print("Content-Type: fgfh/fhfgh       \r\n\r\n", end="")

# print('Content-Type: ' + os.environ['REQUEST_METHOD']+'\r\n', end="")
print(" ".join(thislist))
# print('name : ' + os.environ['name'])







# res
# Set-Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;

# req
# Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;