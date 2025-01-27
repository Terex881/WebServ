import os
import json
import cgi
import time
import requests
from email.utils import formatdate
from html import escape
import subprocess  # Add this import at the top of your script
from datetime import datetime, timedelta

# File to store user data
USER_DATA_FILE = "user_data.json"

def get_cookie_expiry():
    # Set expiry to 24 hours from now
    future = datetime.now() + timedelta(days=1)
    return future.strftime('%a, %d %b %Y %H:%M:%S GMT')

# Function to check if username exists and save if not
def check_and_save_user(username, password):
	# Read existing users from the file
	if os.path.exists(USER_DATA_FILE):
		try:
			with open(USER_DATA_FILE, "r") as f:
				users = json.load(f)  # Try to load the JSON content
		except json.JSONDecodeError:
			# If the file is empty or contains invalid JSON, initialize as an empty dict
			users = {}
	else:
		# If the file doesn't exist, initialize as an empty dict
		users = {}

	# Check if the username already exists
	if username in users:
		return False  # User exists

	# Save new user data
	users[username] = password
	with open(USER_DATA_FILE, "w") as f:
		json.dump(users, f)
	return True

# Function to get the current HTTP date for the 'Expires' header
def get_http_date():
	return formatdate(time.time(), usegmt=True)

# Function to send the GET request with a Set-Cookie header
def make_request_with_cookie(username):

	cookie_name = "session_id="+username
	expires = get_cookie_expiry()
	print(f"Set-Cookie: {cookie_name}; Expires={expires}; Path=/; HttpOnly;\r\n", end="")
	print("Content-Type: text/html\r\n\r\n", end="")


	print("Login Successfully")

# Main function to process the form submission
def main():

	print("HTTP/1.1 200 OK\r\n", end="")
	print("Transfer-Encoding: chunked\r\n", end="")
	print("Connection: keep-alive\r\n", end="")



	# Get the form data (username and password)
	form = cgi.FieldStorage()
	username = os.environ['username']
	password = os.environ["password"]

	# Check if the username exists and save if not
	if check_and_save_user(username, password):
		# print(f"<h2>User '{username}' saved successfully.</h2>")
		# After saving, send the GET request with cookie
		make_request_with_cookie(username)
	else:
		print("Content-Type: text/html\r\n\r\n", end="")
		print(f"<h2>User '{username}' already exists.</h2>")

if __name__ == "__main__":
	main()
