#include <iostream>
#include <string>
#include <ctime>
#include <sstream>

std::string get_http_date() {
    std::time_t now = std::time(nullptr);
    std::tm* gmt_time = std::gmtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(gmt_time, "%a, %d %b %Y %H:%M:%S GMT");
    return oss.str();
}

std::string generate_set_cookie_header(const std::string& cookie_name, const std::string& cookie_value) {
    std::ostringstream oss;
    oss << "Set-Cookie: " << cookie_name << "=" << cookie_value << "; ";
    oss << "Expires=" << get_http_date() << "; ";
    oss << "Path=/; HttpOnly\r\n";
    return oss.str();
}

void send_response_with_cookie(int client_socket) {
	//123         STATIC INT CLIENT CLASS
    std::string cookie_header = generate_set_cookie_header("user", "12345");
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += cookie_header;
    response += "\r\n";
    response += "<html><body>Cookie set!</body></html>";

    send(client_socket, response.c_str(), response.length(), 0);
}