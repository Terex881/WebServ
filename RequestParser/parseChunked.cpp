#include "./Request.hpp"
#include <stdexcept>


void Request::parseChunkedBody(string &body) {
    if (!outFile.is_open()) {
        openFile("ok.txt");
    }

    static size_t restLength = 0;
    static bool processingChunk = false;
    
    while (!body.empty()) {
        // If we're not processing a chunk, look for the next chunk size
        if (!processingChunk) {
            // Find the CRLF after the chunk size
            size_t crlfPos = body.find("\r\n");
            if (crlfPos == string::npos) {
                return; // Wait for more data
            }

            // Extract and parse the chunk size
            string hexLength = body.substr(0, crlfPos);
            size_t hexEnd = hexLength.find_first_not_of("0123456789abcdefABCDEF");
            if (hexEnd != string::npos) {
                hexLength = hexLength.substr(0, hexEnd);
            }
            
            if (hexLength.empty()) {
                cerr << "Error: Invalid chunk size" << endl;
                return;
            }

            restLength = strtol(hexLength.c_str(), NULL, 16);
            
            // Remove the chunk size line
            body.erase(0, crlfPos + 2);
            
            // If chunk size is 0, we've reached the end
            if (restLength == 0) {
                // Remove final CRLF
                size_t finalCrlfPos = body.find("\r\n");
                if (finalCrlfPos != string::npos) {
                    body.erase(0, finalCrlfPos + 2);
                }
                outFile.close();
                REQUEST_IS_FINISH = 2;
                return;
            }
            
            processingChunk = true;
        }

        // Process the chunk data
        size_t bytesToRead = min(restLength, body.length());
        if (bytesToRead > 0) {
            string chunk = body.substr(0, bytesToRead);
            outFile.write(chunk.c_str(), chunk.length());
            outFile.flush();
            
            body.erase(0, bytesToRead);
            restLength -= bytesToRead;
            
            // If we've processed the entire chunk
            if (restLength == 0) {
                // Remove the CRLF after the chunk
                if (body.length() >= 2 && body.substr(0, 2) == "\r\n") {
                    body.erase(0, 2);
                    processingChunk = false;
                } else if (body.empty()) {
                    // Wait for the CRLF
                    return;
                }
            }
        }
    }
}
void Request::parseChunkedBoundryBody(string &body)
{
	string res;
	size_t hexPos;
	unsigned long length;
	

	while (57)
	{
		hexPos = body.find_first_not_of("0123456789abcdefABCDEF");

		if (hexPos == string::npos) break;
		
		length = strtol(body.substr(0, hexPos).c_str(), NULL, 16);
		cout << RED << length << endl;

		body.erase(0, hexPos + 2);
		if (length == 0)
			break;
		res.append(body.substr(0, length));
		body.erase(0, length + 2);
	}
	parseBoundryBody(res);
}