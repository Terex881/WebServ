#ifndef HEADER_HPP 
#define HEADER_HPP 

#include "./Server.hpp"

class Header{

private:
	int fd;
	std::string header;
	std::string body;
public:
	Header();
	Header(int Fd, std::string Headers, std::vector<Header> &h);
	std::string Getheader() const;
	std::string Getbody() const;
	int Getfd() const;

	~Header();
};

std::ostream& operator<<(std::ostream& os, const Header& b);

#endif