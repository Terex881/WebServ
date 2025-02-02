import os
import json
import cgi
import secrets
from datetime import datetime, timedelta

# File to store user data
USER_DATA_FILE = "user_data.json"
# File to store session data
SESSION_DATA_FILE = "session_data.json"

def get_cookie_expiry():
    # Set expiry to 24 hours from now
    future = datetime.now() + timedelta(days=1)
    return future.strftime('%a, %d %b %Y %H:%M:%S GMT')

def check_and_save_user(username, password):
    # Read existing users from the file
    if os.path.exists(USER_DATA_FILE):
        try:
            with open(USER_DATA_FILE, "r") as f:
                users = json.load(f)
        except json.JSONDecodeError:
            users = {}
    else:
        users = {}

    # Check if the username already exists
    if username in users:
        return False  # User exists

    # Save new user data
    users[username] = password
    with open(USER_DATA_FILE, "w") as f:
        json.dump(users, f)
    return True


def	get_session(username):
	# Load existing sessions
	if os.path.exists(SESSION_DATA_FILE):
		try:
			with open(SESSION_DATA_FILE, "r") as f:
				sessions = json.load(f)
		except json.JSONDecodeError:
			sessions = {}
	else:
		sessions = {}

	# Search through sessions to find the username
	for session_id, session_data in sessions.items():
		# print(f"session_id : {session_id}  username : {session_data['username']} |  ")
		if session_data['username'] == username:
			# Return session_id and expires if username matches
			return session_id, session_data['expires']

	# print("Not Found but no worry i'm going to create a new session for you")
	# If no session is found for the username, create new session
	return create_session(username)



def create_session(username):
	# Generate a unique session ID
	session_id = secrets.token_hex(16)
	expires = get_cookie_expiry()

	# Load existing sessions
	if os.path.exists(SESSION_DATA_FILE):
		try:
			with open(SESSION_DATA_FILE, "r") as f:
				sessions = json.load(f)
		except json.JSONDecodeError:
			sessions = {}
	else:
		sessions = {}

	# Save the session
	sessions[session_id] = {
		"username": username,
		"expires": expires
	}
	with open(SESSION_DATA_FILE, "w") as f:
		json.dump(sessions, f)

	return session_id, expires


def delete_session(session_id):
	# Load existing sessions
	if os.path.exists(SESSION_DATA_FILE):
		try:
			with open(SESSION_DATA_FILE, "r") as f:
				sessions = json.load(f)
		except json.JSONDecodeError:
			sessions = {}
	else:
		sessions = {}

	# Delete the session
	if session_id in sessions:
		del sessions[session_id]
		with open(SESSION_DATA_FILE, "w") as f:
			json.dump(sessions, f)


def main():
	# Get the form data (username and password)
	try:
		username = os.environ['username']
	except KeyError:
		username = ""
	try:
		password = os.environ["password"]
	except KeyError:
		password = ""
	try:
		logout = os.environ["logout"]
	except KeyError:
		logout = ""
	
	print("HTTP/1.1 302 OK\r\n", end="")
	print("Transfer-Encoding: chunked\r\n", end="")
	print("Connection: keep-alive\r\n", end="")
	# Check if the username exists and save if not



	if logout:
		# Get the session ID from the cookie
		try:
			cookie_header = os.environ["Cookie"]
		except KeyError:
			cookie_header = ""
		cookies = {}
		# print(f"val :: {cookie_header}")
		if cookie_header:
			for cookie in cookie_header.split(";"):
				key, value = cookie.strip().split("=", 1)
				cookies[key] = value
		session_id = cookies.get("session_id", "")

		if session_id:
			# Delete the session
			delete_session(session_id)

			# Remove the cookie by setting an expired cookie
			print("Set-Cookie: session_id=; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Path=/; HttpOnly")
			print("Content-Type: text/html\r\n\r\n", end="")
			print("<h2>Logout successful. Cookie removed.</h2>")
			print('<p><a href="/">Home</a></p>')
			print('<p><a href="/login/login.html">Login page</a></p>')

		else:
			print("Content-Type: text/html\r\n\r\n", end="")
			print("<h2>No active session found.</h2>")
			print('<p><a href="/login/login.html">Login page</a></p>')

		return



	if username and password:
		if check_and_save_user(username, password):
			# Create a session
			session_id, expires = create_session(username)

			# Set a cookie with the session ID
			print(f"Set-Cookie: session_id={session_id}; Expires={expires}; Path=/; HttpOnly")
			print("Location: http://0.0.0.0:3344/login/profile.py\r\n", end="")
			print("Content-Type: text/html\r\n\r\n", end="")
			print("<h2>Login successful. Cookie set.</h2>")
		else:
			session_id, expires = get_session(username)
			print("Location: http://0.0.0.0:3344/login/profile.py\r\n", end="")
			print(f"Set-Cookie: session_id={session_id}; Expires={expires}; Path=/; HttpOnly")
			print("Content-Type: text/html\r\n\r\n", end="")
			print("<h2>User already exists.</h2>")
	else:
		print("Content-Type: text/html\r\n\r\n", end="")
		print("<h2>Username and password are required.</h2>")


if __name__ == "__main__":
	main()
