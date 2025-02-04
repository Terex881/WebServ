import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


print("<p>")
print(" ".join(thislist))
print("</p>")
print("<p>hello</p>")
print(f"<h2>{os.environ['PATH_INFO']}</h2>")
# print(f"<h2>{os.environ['cdf']}</h2>")


# while True:
# 	pass

for i in range(1001):
	print(f"<p>{i}</p>", end="")
print("end", end="")

# data = input()
# print(data)


# import subprocess

# # Example curl command
# curl_command = "curl -X GET http://0.0.0.0:3344/script.py"

# # # Execute the curl command
# result = subprocess.run(curl_command, shell=True, capture_output=True, text=True)