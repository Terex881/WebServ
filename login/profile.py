import os
import json
import cgi
from datetime import datetime

# File to store session data
SESSION_DATA_FILE = "session_data.json"

def get_cookie():
	# Get the HTTP_COOKIE environment variable
	# if os.environ.get('Cookie')
	try:
		# Try to access the 'Cookie' environment variable
		cookie_header = os.environ['Cookie']
	except KeyError:
		# If 'Cookie' is missing, set cookie_header to an empty string
		cookie_header = ""
	# print(f"Cooookie : {cookie_header}")
	cookies = {}
	if cookie_header:
		# Parse the Cookie header
		for cookie in cookie_header.split(";"):
			key, value = cookie.strip().split("=", 1)
			return value
	return ""




def validate_session(session_id):
	# Load existing sessions
	if os.path.exists(SESSION_DATA_FILE):
		try:
			with open(SESSION_DATA_FILE, "r") as f:
				sessions = json.load(f)
		except json.JSONDecodeError:
			sessions = {}
	else:
		sessions = {}

	# Check if the session ID exists and is not expired
	if session_id in sessions:
		session_data = sessions[session_id]
		expires = datetime.strptime(session_data["expires"], '%a, %d %b %Y %H:%M:%S GMT')
		if datetime.now() < expires:
			return session_data["username"]  # Session is valid
	return None  # Session is invalid or expired




def main():
	print("HTTP/1.1 200 OK\r\n", end="")
	print("Transfer-Encoding: chunked\r\n", end="")
	print("Connection: keep-alive\r\n", end="")

	# Check for the session_id cookie
	cookies = get_cookie()

	session_id = cookies
	if session_id:
		username = validate_session(session_id)
		if username:
			print("Content-Type: text/html\r\n\r\n", end="")
			print(f"<h2>Welcome, {username}!</h2>")
			print("<p>You are logged in.</p>")
			print('<p><a href="login.py?logout=true">Logout</a></p>')
		else:
			print("Content-Type: text/html\r\n\r\n", end="")
			print("<h2>Invalid or expired session. Please log in again.</h2>")
			print('<p><a href="/login/login.html">Login</a></p>')
	else:
		print("Content-Type: text/html\r\n\r\n", end="")
		print("<h2>Access denied. Please log in.</h2>")
		print('<p><a href="/login/login.html">Login</a></p>')

if __name__ == "__main__":
	main()