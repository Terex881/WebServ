import os

thislist = ["My", "First", "Python", "Execution", "With CGI"]


print("<p>")
print(" ".join(thislist))
print("</p>")
print("<p>hello</p>")

print(f"<p>{os.environ['REQUEST_METHOD']}</p>")
while True:
	pass