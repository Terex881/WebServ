

#include "Request.hpp"

int main()
{
	try {
		string str= "POST /upload HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"Content-Type: application/json\r\n"
					"Transfer-Encoding: chunked\r\n"
					"\r\n"
        			"8\r\n"
        			"Mozilla \r\n"
        			"F\r\n"
        			"Dezveloper Net \r\n"
        			"C\r\n"
        			"work Example\r\n"
        			"0\r\n"
        			"\r\n";
		string str1= "POST /contact_form.php HTTP/1.1\r\n"
					"Host: developer.mozilla.org\r\n"
					"Content-Length: 12\r\n"
					"Content-Type: applicationx-www-form-urlencoded\r\n"
					"\r\n"
					"ncame=Jj%20U\r\n";
		Request request(str1);
	} catch (const std::exception &e) {
		cout << e.what()  << endl;
	}
	return 0;
}