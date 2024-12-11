#include "./Header.hpp"
#include <vector>


Header::Header()
{
	fd = 0;
	header = "";
	body = "";
}

std::string Header::Getheader() const
{
	return this->header;
}

std::string Header::Getbody() const
{
	return this->body;
}

int Header::Getfd() const
{
	return this->fd;
}

Header::Header(int Fd, std::string Headers, std::vector<Header> &h)
{
	static int n;
	fd = Fd;

	int a = 0;
	for (std::vector<Header>::const_iterator p = h.begin(); p != h.end(); ++p)
	{
		if (fd == p->Getfd())
		{
			if ((a = Headers.find("\r\n\r\n")) != std::string::npos)
			{
				if (a != 0)
				this->header = Headers.substr(0, a);
				this->body = Headers.substr(a, Headers.size());
				n = 1;
			}
			else if (n != 1)
				this->header = Headers;
			else if (n == 1)
				this->body = Headers;
			std::cout << "got here \n";
		}
	}
	if ((a = Headers.find("\r\n\r\n")) != std::string::npos)
	{
		if (a != 0)
		this->header = Headers.substr(0, a);
		this->body = Headers.substr(a, Headers.size());
		n = 1;
	}
	else if (n != 1)
		this->header = Headers;
	else if (n == 1)
		this->body = Headers;
}

Header::~Header()
{

}

std::ostream& operator<<(std::ostream& os, const Header& b)
{
	return os << "body" << b.Getbody() << "  header "<< b.Getheader();
}