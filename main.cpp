

#include "Request.hpp"

int main()
{
	try {
		string str= "POST /upload HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"Content-Type:	application/json\r\n"
					"Transfer-Encoding: chunked\r\n"
					"\r\n"
        			"8\r\n"
        			"Mozilla \r\n"
        			"F\r\n"
        			"Dezvejoper Net \r\n"
        			"C\r\n"
        			"work Example\r\n"
        			"0\r\n"
        			"\r\n";
		string str1= "POST /contac_form.php HTTP/1.1\r\n"
					"Host: developer.mozilla.org\r\n"
					"Content-Length: 12\r\n"
					"Content-Type: applicationx-ww;w-form-urlencoded\r\n"
					"\r\n"
					"ncame=Jj%20U\r\n";
					//---------------------------------------------------
		string str2=	"POST /profile/upload HTTP/1.1\r\n"
						"Host: myapp.com\r\n"
						"Content-Type: multipart/form-data; boundary=----FormBoundary123456\r\n"
						"\r\n"
						// "------FormBoundary123456\r\n"
						// "Content-Disposition: form-data; name=\"user_id\"\r\n"
						// "\r\n"
						// "1234\r\n"
						// "------FormBoundary123456\r\n"
						// "Content-Disposition: form-data; name=\"full_name\"\r\n"
						// "\r\n"
						// "John Doe\r\n0"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"profile_picture\"; filename=\"profile.jpg\"\r\n"
						"Content-Type: image/jpeg\r\n"
						"\r\n"
						"(binary image data would be here)\r\n"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"resume\"; filename=\"resume.pdf\"\r\n"
						"Content-Type: application/pdf\r\n"
						"\r\n"
						"(binary PDF data would be here)\r\n"
						"------FormBoundary123456--\r\n";
		Request request(str);
	} catch (const std::exception &e) {
		cout << e.what() << endl;
	}
	return 0;
}


