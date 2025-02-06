#include "Request.hpp"

Request::Request()
{
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	RequestData.codeStatus = 200;
	RequestData.isDirListening = false;
	RequestData.isRedirect = false;
	RequestData.isCgi = false;
	RequestData.isUpload = false;
	HeaderData.isAlive = true;
	BodyData.bodyType = NONE;
	RequestData.errorFlag = true;
}

Request::~Request()	{}


void Request::clean(int code, string message)
{
	RequestData.errorFlag = false;
	RequestData.codeStatus = code;
	RequestData.codeStatusMap.insert(make_pair(code, message));
}


void Request::clearData()
{
	
	BodyData.bodySize = 0;
	RequestData.requestStat = 0;
	RequestData.codeStatus = 200;
	RequestData.isDirListening = false;
	RequestData.isRedirect = false;
	RequestData.isCgi = false;
	RequestData.isUpload = false;
	HeaderData.isAlive = true;
	BodyData.bodyType = NONE;
	RequestData.errorFlag = true;
	
	BodyData.boundry.clear();
	BodyData.endBoundry.clear();
	BodyData.newStr.clear();
	BodyData.buffer.clear();
	BodyData.fileName.clear();
	BodyData.pathFormData.clear();

	HeaderData.port.clear();
	HeaderData.extension.clear();
	HeaderData.requestMethod.clear();
	HeaderData.queryStringVec.clear();
	HeaderData.bigMap.clear();
	HeaderData.url.clear();
	HeaderData.urlFinal.clear();

	RequestData.header.clear();
	RequestData.requestStat = 0;
	RequestData.first.clear();
	RequestData.serverName.clear();
	RequestData.cgiError.clear();
	RequestData.timeOut.clear();
	RequestData.pathInfo.clear();
	RequestData.fileLocation.clear();
	RequestData.executable_file.clear();
	RequestData.redirection.clear();
	RequestData.default_page.clear();
	RequestData.codeStatusMap.clear();
	RequestData.hostName.clear();

}

t_Request&	Request::getRequestData()	{ return RequestData; }
t_Header&	Request::getHeaderData()	{ return HeaderData; }
t_Body&		Request::getBodyData()		{ return BodyData; }


void Request::request(string &request)
{
	if (!RequestData.requestStat)
	{		
		size_t pos = request.find(DCRLF);
		if (pos != NP)
		{
			RequestData.header.append(request.c_str(), 0, pos);
			parseHeader(RequestData.header);
			request.erase(0, pos + 4);
		}
		else
			RequestData.header.append(request);
	}
	if (RequestData.requestStat == 1)
	{
		if (HeaderData.requestMethod == "POST")
		{
			parseBodyTypes(request);
		}
		else if (HeaderData.requestMethod == "GET")
		{
			RequestData.requestStat = 2;
		}
		else if (HeaderData.requestMethod == "DELETE")
		{
			RequestData.requestStat = 2;
		}
			
	}
}

File_Parsing& Request::getConfigFileObj()
{
	return configFileObj;
}

Request::Request(const Request &src)
{
	*this = src;
}

Request& Request::operator=(const Request &copy)
{
	if (this != &copy)
	{
		BodyData = copy.BodyData;
		RequestData = copy.RequestData;
		HeaderData = copy.HeaderData;
	}
	return *this;
}