

#include "Request.hpp"

int main()
{
	try {
		string str= "POST /upload HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"Content-Type :	 application/json\r\n"
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
<<<<<<< HEAD
		std::string str2 = "POST /?=&1111=2334 HTTP/1.1\r\n"
                   "User-Agent: PostmanRuntime/7.33.0\r\n"
                   "Accept: */*\r\n"
                   "Postman-Token: aa819387-b087-4dd5-a149-02e28b7deffa\r\n"
                   "Host: 127.0.0.1:4455\r\n"
                   "Accept-Encoding: gzip, deflate, br\r\n"
                   "Connection: keep-alive\r\n"
                   "Content-Length: 4456932\r\n"
                   "Content-Type: video/mp4\r\n\r\n"
                   "ftypisomisomiso2avc1mp41��moovlmvhd���@��trak\tkhd�+@��$edtselst�+�Nmdia mdhd<@U�GhdlrvideISO Media file produced by Google Inc.��minfvmhd$dinfdref\r\n"
                   "u�����@@�l�stsd�avc1��HH��2avcCM@\r\n"
                   "D�h� colrnclxpaspbtrt/G/Gstts\r\n";
                              
=======
		string str2=	"POST /profile/upload HTTP/1.1\r\n"
						"Host: myapp.com\r\n"
						"Content-Type: multipart/form-data; boundary=----FormBoundary123456\r\n"
						"\r\n"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"user_id\"\r\n"
						"\r\n"
						"1234\r\n"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"full_name\"\r\n"
						"\r\n"
						"John Doe\r\n"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"profile_picture\"; filename=\"profile.jpg\"\r\n"
						"Content-Type: image/jpeg\r\n"
						"\r\n"
						"(binary image data would be here)\r\n"
						"------FormBoundary123456\r\n"
						"Content-Disposition: form-data; name=\"resume\"; filename=\"resume.py\"\r\n"
						"Content-Type: application/pdf\r\n"
						"\r\n"
						"(binary PDF data would be here)\r\n"
						"------FormBoundary123456--\r\n";
>>>>>>> e9909c157fed9f6979ab6426aaa30d7fd14516ab
		Request request(str2);
	} catch (const std::exception &e) {
		cout << e.what() << endl;
	}
<<<<<<< HEAD
	return 0; 
}
=======
	return 0;
}


>>>>>>> e9909c157fed9f6979ab6426aaa30d7fd14516ab
