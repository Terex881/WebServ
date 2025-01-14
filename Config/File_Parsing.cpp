#include "./File_Parsing.hpp"

File_Parsing::File_Parsing()
{

}

string	correct_url(string path)
{
	string final_url;
	int		slash = 0;

	if (path[0] != '/')
		final_url = "/";
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '/')
		{
			if (!slash)
				final_url += '/';
			slash = 1;
		}
		else
		{
			final_url += path[i];
			slash = 0;
		}
	}
	return final_url;
}

File_Parsing::File_Parsing(string conf_path):file(conf_path)
{
	if (!file.is_open())
	{
		exit(1);
	}
	open_b = 0;
	close_b = 0;
	servers_count = 0;
	port_count = 0;
	obj = recursive_push(&file, "", &open_b, &close_b);
	if (open_b != close_b)
	{
		file.close();
		exit(2);
	}
	Checking_Hierarchy(&obj, servers, locations);
	// recursive_call(servers[0], servers, locations);

	if (!servers_count)
	{
		file.close();
		exit(3);
	}

	get_host_name();

	// DynamicStruct location;
	// DynamicStruct server;

	// getLocationByPortAndUrl("3000", correct_url("support"), location, server);

	// cout << "###########################################################################" << endl;
	// cout << "error page : " << get_error_page("404", server) << endl;
	// if (server.values.size() && location.values.size())
	// {
	// 	cout << location.values["path"] << endl;
	// 	cout << location.values["root"] << endl;
	// }
	// else
	// 	cout << "Server Or Location Doesn't Exist" << endl;
	// cout << "###########################################################################" << endl;
	file.close();
}

File_Parsing::~File_Parsing()
{
}

void	trime_line(string &line)
{
	int i = 0;
	int last = 0;

	if (line.length() > 0)
		last = line.length() - 1;
	while (line[i] && std::isspace(line[i]))
		i++;
	while (last >= 0 && line[last] && std::isspace(line[last]))
		last--;
	line = line.substr(i, (last - i + 1));
}

int contain_bracket(string line)
{
	int i = 0;
	while (line[i])
	{
		if (line[i] == '{')
			break ;
		i++;
	}
	if (line[i] == '{' && !line[i + 1])
		return (1);
	return (0);
}

std::vector<string> split(string s)
{
	stringstream ss(s);
	vector<string> words;
	string word;

	while (ss >> word)
	{
		words.push_back(word);
	}
	return words;
}

int	is_num(string word)
{
	int i = 0;
	while (word[i])
	{
		if (!isnumber(word[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_port(vector<string> line)
{
	double num = 0;

	size_t index = line[1].find(';');
	if (line[1].length() != (index + 1))
		return 0;
	if (!(line[1].length() >= 2))
		return (0);
	line[1] = line[1].substr(0, index);
	index = line[1].find(':');
	if (index == string::npos)
	{
		if (!is_num(line[1]))
			return 0;
	}
	else
	{
		line[1] = line[1].substr(index + 1, line[1].length() - index);
		if (!is_num(line[1]))
			return 0;
	}
	num = strtod(line[1].c_str(), NULL);
	if (num <= 0)
		return 0;
	return 1;
}

int	check_val(vector<string> line)
{
	char	*end = NULL;
	size_t	mb;
	size_t	index;

	if (line[0] == "return" && line.size() != 3)
		return (0);
	else if (line[0] == "methods" && line.size() > 4)
		return (0);
	else if (line[0] == "error_page" && line.size() != 3)
		return (0);
	else if (line[0] != "error_page" && line[0] != "methods" && line[0] != "return")
	{
		if (line.size() != 2)
			return (0);
	}
	if (line[0] == "client_max_body_size")
	{
		if (!is_num(line[1].substr(0, line[1].length() - 1)) || !line[1].substr(0, line[1].length() - 1).length())
			return (0);
		mb = strtod(line[1].c_str(), &end);
		if (*end && *end != ';' && strlen(end) > 1)
		{
			return (0);
		}
	}
	index = line[line.size() - 1].find(';');
	if (line[line.size() - 1].length() != (index + 1))
		return (0);
	if ((line[0] == "directory_listing"))
	{
		if (line[1] != "on;" && line[1] != "off;")
			return (0);
	}
	return (1);
}

//// Storing Data In A Recursive Way
DynamicStruct	File_Parsing::recursive_push(ifstream *file, string parent, int *open_bracket, int *close_bracket)
{
	DynamicStruct current;
	vector<string> words;
	DynamicStruct child;
	DynamicStruct tmp;
	string line;
	static int i;
	static int j;
	static int	n;

	while (getline(*file, line))
	{
		trime_line(line);
		if (line.empty() || line[0] == '#')
			continue ;
		words = split(line);
		if (line.find('{') != string::npos)
			(*open_bracket)++;
		if (line.find('}') != string::npos)
			(*close_bracket)++;
		if (words[0] == "}")
			break ;
		
		// Ensure there are at least two words
		string key = words[0];
		if (contain_bracket(line))
		{
			if (words.size() < 2 || words.size() > 3)
				(*file).close(), exit(4);
			// checking keys format {}
			if (key == "location" && (words.size() != 3 || words[2] != "{"))
				(*file).close(), exit(5);
			if ((key == "http" || key == "server") && words[1] != "{")
				(*file).close(), exit(6);

			if (key != "http" && key != "server" && key != "location")
				(*file).close(), exit(7);
			if ((key == "location" && parent != "server") || (key == "server" && parent != "http"))
				(*file).close(), exit(8);
			child = recursive_push(file, key, open_bracket, close_bracket);
			if (!i && key == "server")
				(*file).close(), exit(100);
			if (key == "location")
				child.values["path"] = correct_url(words[1]);
			if (parent == "")
				child.values["parent"] = ".";
			if (parent == "http")
				child.values["parent"] = "http";
			if (parent == "server")
				child.values["parent"] = "server";
			child.values["current"] = key;

			if (key == "location")
			{
				if (child.values["path"] == "cgi-bin"  && child.values.find("methods") != child.values.end())
				{
					if (child.values["methods"].find("DELETE") != string::npos)
						(*file).close(), exit(111);
				}
				if (child.values.find("root") != child.values.end())
					child.values["root"] = correct_url(child.values["root"]);
			}
			if (key == "server")
			{
				if (child.values.find("root") == child.values.end())
					(*file).close(), exit(112);
				else
					child.values["root"] = correct_url(child.values["root"]);
				if (child.values.find("client_max_body_size") == child.values.end())
					child.values["client_max_body_size"] = "1";
				i = 0;
				servers_count++;
				current.children[key].push_back(child);
			}
			else
				current.children[key].push_back(child);
		}
		else
		{
			// regular key : value
			if (key == "listen" && !check_port(words))
				(*file).close(), exit(9);
			else if (!check_val(words))
				(*file).close(), exit(10);
			if (parent == "location" && key != "root" && key != "methods"
				&& key != "directory_listing" && key != "return")
				(*file).close(), exit(11);
			if (parent == "server" && key != "listen" && key != "error_page" &&
					key != "client_max_body_size" && key != "server_name" && key != "root" && key != "index")
				(*file).close(), exit(12);

			if (key == "methods" || key == "return")
			{
				line = line.substr(key.length() + 1, (line.length() - key.length()));
				current.values[key] = line;
			}
			else if (key == "listen")
			{
				i++;
				port_count++;
				current.values[key +"_"+ std::to_string(i)] = words[1];

				words[1] = words[1].substr(0, words[1].find(';'));
				words[1] = words[1].substr(words[1].find(':') + 1);
				current.values["ports"] += " " + words[1];
				current.values["port_count"] = std::to_string(i);
			}
			else if (key == "error_page")
			{
				j++;
				current.values[words[1]] = correct_url(words[2]);
			}
			else
				current.values[key] = words[1];
		}
		n++;
	}
	return current;
}

bool isPortInString(const std::string& portString, const std::string& searchPort)
{
	std::istringstream iss(portString);
	std::string port;
	
	// Read each port from the string
	while (iss >> port)
	{
		if (searchPort.find(port) != string::npos)
			return true;
	}
	return false;
}

string	get_port_index(DynamicStruct *server, string port)
{
	int i = 1;
	double port_1;
	double port_2;

	for (map<string, string>::iterator it = server->values.begin(); it != server->values.end(); it++)
	{
		if (it->first.find("listen") != string::npos)
		{
			port_1 = strtod(it->second.c_str(), NULL);
			port_2 = strtod(port.c_str(), NULL);
			if (port_1 == port_2)
			{
				return (std::to_string(i));
			}
			i++;
		}
	}
	return (std::to_string(i));
}

// override port if it is already in use
void	File_Parsing::override_server(int index, DynamicStruct *server)
{
	int	i = 0;
	int curr_index = index;
	string	port_index;

	while (i < curr_index)
	{
		for (map<string, string>::iterator it = server[i].values.begin(); it != server[i].values.end(); it++)
		{
			if (it->first.find("listen") != string::npos)
			{
				if (isPortInString(server[index].values["ports"], it->second))
				{
					port_count--;
					port_index = get_port_index(&server[index], it->second.substr(it->second.find(':') + 1));
					server[index].values["listen_"+port_index] = "override";
				}
			}
		}
		i++;
	}
}

void File_Parsing::Checking_Hierarchy(DynamicStruct *block, DynamicStruct *server, DynamicStruct *locations, const string &name)
{
	map<string, string>::iterator it;
	string current;
	string parent;
	string path;
	static vector<string> path_cart;
	vector<string> port_cart;
	static int i;
	static int j;

	// http : must contain 2 key:value (default) and at least 1 children
	if (name == "http" && (block->values.size() != 2 || block->children.size() == 0))
		file.close(), exit(13);
	// http : must contain at least 2 key:value (default) and only 1 children (location)
	if (name == "server" && (block->values.size() < 3 || block->children.size() == 0))
		file.close(), exit(14);
	if (name == "location" && block->values.size() <= 3)
		file.close(), exit(15);
	for (it = block->values.begin(); it != block->values.end(); it++)
	{
		it->second = it->second.substr(0, it->second.find(';'));
		if (it->second.empty())
			file.close(), exit(16);
		if (it->first == "current")
			current = it->second;
		else if (it->first == "parent")
			parent = it->second;

		if (current != "" && parent != "")
		{
			if (current == "http" && parent != ".")
				file.close(), exit(17);
			if ((current == "server" && parent != "http"))
				file.close(), exit(18);
			if (current == "location" && parent != "server")
				file.close(), exit(19);
		}
		if (it->first == "path")
			path = it->second;
		if (current == "location" && path != "")
		{
			//  check if location "/URL" redundant
			if (find(path_cart.begin(), path_cart.end(), path) == path_cart.end())
			{
				path_cart.push_back(path);
				path = "";
			}
			else
				file.close(), exit(20);
		}
		if (current.find("server") != string::npos && it->first.find("listen") != string::npos)
		{
			//  check if "Port" redundant
			string portt = it->second.substr(it->second.find(':') + 1);
			if (find(port_cart.begin(), port_cart.end(), portt) == port_cart.end())
				port_cart.push_back(portt);
			else
				file.close(), exit(21);
		}
	}
	if (name.find("server") != std::string::npos)
	{
		string port_to_erase;

		path_cart.clear();
		server[j] = *block;
		override_server(j, server);
		j++;
	}
	else if (name == "location")
	{
		if (!block->values["methods"].empty())
		{
			vector<string> methds = split(block->values["methods"]);
			for (vector<string>::iterator it = methds.begin(); it != methds.end(); it++)
			{
				if (*it != "POST" && *it != "GET" && *it != "DELETE")
					file.close(), exit(22);
			}
		}
		locations[i++] = *block;
	}

	for (map<string, vector<DynamicStruct> >::iterator it = block->children.begin(); it != block->children.end(); ++it)
	{
		const string &block_name = it->first;
		vector<DynamicStruct> &blocks = it->second;
		for (vector<DynamicStruct>::iterator child_it = blocks.begin(); child_it != blocks.end(); ++child_it)
		{
			Checking_Hierarchy(&(*child_it), server, locations, block_name);
		}
	}
}


void File_Parsing::recursive_call(DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name)
{
	map<string, string>::iterator it;

	for (it = block.values.begin(); it != block.values.end(); it++)
	{
		cout << "" << it->first << "   ----->  " << it->second<< endl;
	}

	for (map<string, vector<DynamicStruct> >::iterator it = block.children.begin(); it != block.children.end(); ++it)
	{
		const string &block_name = it->first;
		cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&& "<< block_name << endl;
		vector<DynamicStruct> &blocks = it->second;
		for (vector<DynamicStruct>::iterator child_it = blocks.begin(); child_it != blocks.end(); ++child_it)
		{
			recursive_call(*child_it, server, locations, block_name);
		}
	}
}

void	File_Parsing::get_host_name(void)
{
	// string	port_key;
	size_t	index;
	string	host;
	string	port;
	dt	data;
	int i = 0;
	int j = 0;

	while (i < servers_count)
	{
		for (map<std::string, std::string>::iterator it = servers[i].values.begin(); it != servers[i].values.end(); it++)
		{
			if (it->first.find("listen") != string::npos && it->second != "override")
			{
				port = "";
				host = "";

				index = it->second.find(':');
				if (index != string::npos)
				{
					host = it->second.substr(0, index);
					port = it->second.substr(index + 1);
				}
				else
				{
					port = it->second;
					host = "0.0.0.0";
				}
				data.key = host;
				data.val = port;
				host_port.push_back(data);
			}
		}
		i++;
	}
}

location_data	File_Parsing::get_location_val(DynamicStruct location)
{
	location_data l_data;

	l_data.root = location.values["root"];
	l_data.directory_listing = location.values["directory_listing"];
	l_data.methods = split(location.values["methods"]);
	l_data.path = location.values["path"];
	l_data.rturn = location.values["return"];

	return l_data;
}

server_data	File_Parsing::get_server_val(DynamicStruct server)
{
	server_data s_data;

	s_data.root = server.values["root"];
	s_data.client_max_body_size = server.values["client_max_body_size"];
	s_data.server_name = server.values["server_name"];
	s_data.default_page = server.values["index"];


	return s_data;
}

string	File_Parsing::get_body_size()
{
	return servers[0].values["client_max_body_size"];
}

// return error page based on statut_code
string	File_Parsing::get_error_page(string statut_code, DynamicStruct server)
{
	return server.values[statut_code];
}

// Location_path must start with /
std::string matchUrl(const std::string& requestUrl, std::string Location_path)
{
	std::string bestMat = "";
	size_t bestMatLength = 0;

	// Remove tail trailing slash
	std::string normalizedUrl = requestUrl;
	if (normalizedUrl.length() > 1 && normalizedUrl[normalizedUrl.length() - 1] == '/')
		normalizedUrl = normalizedUrl.substr(0, normalizedUrl.length() - 1);

	if (Location_path.length() > 1 && Location_path[Location_path.length() - 1] == '/')
		Location_path = Location_path.substr(0, Location_path.length() - 1);

	if (normalizedUrl.find(Location_path) != string::npos)
	{
		if (normalizedUrl.length() == Location_path.length() || 
			normalizedUrl[Location_path.length()] == '/')
			{
				if (Location_path.length() > bestMatLength)
				{
					bestMatLength = Location_path.length();
					bestMat = Location_path;
				}
			}
	}
	return bestMat;
}


// allow us to find server based on Port and location based on URL
void	File_Parsing::getLocationByPortAndUrl(string port, string url, 	DynamicStruct &location, DynamicStruct &server)
{
	int i = 0;
	while (i < servers_count)
	{
		for (map<std::string, std::string>::iterator it = servers[i].values.begin(); it != servers[i].values.end(); it++)
		{
			if (it->first.find("listen") != string::npos)
			{
				if (it->second == port)
				{
					server.values = servers[i].values;
					for (map<string, vector<DynamicStruct> >::iterator it = servers[i].children.begin(); it != servers[i].children.end(); it++)
					{
						vector<DynamicStruct> &locations = it->second;
						for (vector<DynamicStruct>::iterator child_it = locations.begin(); child_it != locations.end(); ++child_it)
						{
							if (!matchUrl(url, child_it->values["path"]).empty())
							{
								location.values = child_it->values;
								if (location.values.find("root") == location.values.end())
								{
									location.values["root"] = servers[i].values["root"];
								}
							}
						}
					}
				}
			}
		}
		i++;
	}
}
