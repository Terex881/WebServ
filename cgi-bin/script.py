import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


# /cgi-bin/hello.py/12345
# path_info = os.environ['PATH_INFO']
# print("Received PATH_INFO:", path_info) # 12345



# print('Content-Type: ' + os.environ['REQUEST_METHOD']+'\r\n', end="")
print(" ".join(thislist))
# print('name : ' + os.environ['name'])







# res
# Set-Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;

# req
# Cookie: prov=f0d02f8c-733c-4d5a-9494-1797b9c45205;