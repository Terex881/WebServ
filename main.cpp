

#include "Request.hpp"

int main()
{
	try {
		string str= "POST /upload HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"Content-Type: application/json\r\n"
					"Transfer-Encoding: chunked\r\n"
					"\r\n"
        			"5\r\n"
        			"mozilla\r\n"
        			"B\r\n"
        			"Developer Net\r\n"
        			"C\r\n"
        			"work Example\r\n"
        			"0\r\n"
        			"\r\n";
		// string str= "POST /contact_form.php HTTP/1.1\r\n"
		// 			"Host: developer.mozilla.org\r\n"
		// 			"Content-Length: 64\r\n"
		// 			"Content-Type: application/x-www-form-urlencoded\r\n"
		// 			"\r\n"
		// 			"name=Joe%20User&request=Send%20me%20one%20of%20your%20catalogue\r\n";
		parseRequest(str);	
	} catch (const std::exception &e) {
		cout << e.what() << " ----" << endl;
	}
	return 0;
}