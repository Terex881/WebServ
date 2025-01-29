import sys
print("HTTP/1.1 200 OK\r\n", end="")
print("Transfer-Encoding: chunked\r\n", end="")
print("Connection: keep-alive\r\n", end="")
print("Content-Type: text/html\r\n\r\n", end="")

data = sys.stdin.read()
print(data)