import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


print("<p>")
print(" ".join(thislist))
print("</p>")
print("<p>hello</p>")
print(f"<h2>{os.environ['PATH_INFO']}</h2>")
print(f"<h2>{os.environ['REQUEST_METHOD']}</h2>")
print(f"<h2>{os.environ['SERVER_NAME']}</h2>")
print(f"<h2>{os.environ['SCRIPT_NAME']}</h2>")
# print(f"<h2>{os.environ['name']}</h2>")
print(f"<h2>{os.environ['dfh']}</h2>")
# print(f"<h2>{os.environ['cdf']}</h2>")


while True:
	pass

# for i in range(1001):
# 	print(f"<p>{i}</p>", end="")
# print("end", end="")

# data = input()
# print(data)