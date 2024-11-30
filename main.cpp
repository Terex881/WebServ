

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
		string str2="POST /upload HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"User-Agent: Mozilla/5.0\r\n"
					"Content-Length: 68137\r\n"
					"Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryABC123\r\n"
					"\r\n"
					"------WebKitFormBoundaryABC123\r\n"
					"Content-Disposition: form-data; name=\"description\"\r\n"
					"\r\n"
					"Project documentation for Q4 software release. Contains key implementation details and project milestones.\r\n"
					"------WebKitFormBoundaryABC123\r\n"
					"Content-Disposition: form-data; name=\"myFile\"; filename=\"project_report.txt\"\r\n"
					"Content-Type: text/plain\r\n"
					"\r\n"
					"Technical Overview:\r\n"
					"- Project initiated on September 15, 2024\r\n"
					"- Primary objectives include system redesign and performance optimization\r\n"
					"- Key team members: Alice Johnson, Robert Chen, Maria Rodriguez\r\n"
					"\r\n"
					"Total estimated development time: 4 months\r\n"
					"Projected completion date: January 30, 2025\r\n"
					"\r\n"
					"Budget allocation: $450,000\r\n"
					"------WebKitFormBoundaryABC123--\r\n";
		Request request(str);
	} catch (const std::exception &e) {
		cout << e.what()  << endl;
	}
	return 0;
}