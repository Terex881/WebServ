#include "./File_Parsing.hpp"

File_Parsing::File_Parsing()
{
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
	obj = recursive_push(&file, "", &open_b, &close_b);
	if (open_b != close_b)
	{
		file.close();
		exit(1);
	}
	Checking_Hierarchy(obj, &servers, locations);
	get_host_name();
}

File_Parsing::~File_Parsing()
{
	file.close();
}

void	trime_line(string &line)
{
	int i = 0;
	int j = 0;
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
	num = strtod(line[1].c_str(), NULL);
	if (num <= 0)
		return 0;

	return 1;
}

int	check_val(vector<string> line)
{
	size_t index;

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
	index = line[line.size() - 1].find(';');
	if (line[line.size() - 1].length() != (index + 1))
		return (0);
	if ((line[0] == "cgi" || line[0] == "directory_listing"))
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
				(*file).close(), exit(1);
			// checking keys format {}
			if ((words.size() == 3 && words[2] != "{"))
				(*file).close(), exit(1);
			if (words.size() == 2 && words[1].find("/") != string::npos)
				(*file).close(), exit(1);

			if (key != "http" && key != "servers" && key != "location")
				(*file).close(), exit(1);
			child = recursive_push(file, key, open_bracket, close_bracket);
			if (key == "location")
				child.values["path"] = words[1];
			if (parent == "")
				child.values["parent"] = ".";
			if (parent == "http")
				child.values["parent"] = "http";
			if (parent == "servers")
				child.values["parent"] = "servers";
			child.values["current"] = key;
			current.children[key].push_back(child);
		}
		else
		{
			// regular key : value
			if (key == "listen" && !check_port(words))
				(*file).close(), exit(1);
			else if (!check_val(words))
				(*file).close(), exit(1);
			if (parent == "location" && key != "root" && key != "methods"
				&& key != "cgi" && key != "directory_listing" && key != "return")
				(*file).close(), exit(1);
			if (parent == "servers" && key != "listen" && key != "error_page")
				(*file).close(), exit(1);

			if (key == "methods" || key == "error_page" || key == "return")
			{
				line = line.substr(key.length() + 1, (line.length() - key.length()));
				current.values[key] = line;
			}
			else if (key == "listen")
			{
				i++;
				servers_count = i;
				current.values[key +"_"+ std::to_string(i)] = words[1];
			}
			else
				current.values[key] = words[1];
		}
	}
	return current;
}



void File_Parsing::Checking_Hierarchy(DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name)
{
	map<string, string>::iterator it;
	string current;
	string parent;
	string path;
	static vector<string> path_cart;
	vector<string> port_cart;
	static int i;



	// http : must contain 2 key:value (default) and only 1 children
	if (name == "http" && (block.values.size() != 2 || block.children.size() != 1))
		file.close(), exit(1);
	// http : must contain at least 2 key:value (default) and only 1 children (location)
	if (name == "servers" && (block.values.size() < 3))
		file.close(), exit(1);
	if (name == "location" && block.values.size() <= 3)
		file.close(), exit(1);
	
	for (it = block.values.begin(); it != block.values.end(); it++)
	{
		it->second = it->second.substr(0, it->second.find(';'));

		if (it->first == "current")
			current = it->second;
		else if (it->first == "parent")
			parent = it->second;
		if (current != "" && parent != "")
		{
			if (current == "http" && parent != ".")
				file.close(), exit(1);
			if ((current == "servers" && parent != "http"))
				file.close(), exit(1);
			if (current == "location" && parent != "servers")
				file.close(), exit(1);
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
				file.close(), exit(1);	
		}
		if (current == "servers" && it->first != "error_page")
		{
			//  check if "Port" redundant
			if (find(port_cart.begin(), port_cart.end(), it->second) == port_cart.end())
			{
				port_cart.push_back(it->second);
			}
			else
				file.close(), exit(1);
		}
	}
	if (name == "servers")
		*server = block;
	else if (name == "location")
		locations[i++] = block;

	for (map<string, vector<DynamicStruct> >::iterator it = block.children.begin(); it != block.children.end(); ++it)
	{
		const string &block_name = it->first;
		vector<DynamicStruct> &blocks = it->second;

		for (vector<DynamicStruct>::iterator child_it = blocks.begin(); child_it != blocks.end(); ++child_it)
		{
			Checking_Hierarchy(*child_it, server, locations, block_name);
		}
	}
}


void	File_Parsing::Struct_Call(DynamicStruct inner)
{
	map<string, string>::const_iterator it;

	for (it = inner.values.begin(); it != inner.values.end(); it++)
	{
		cout << "key = " << it->first << " Value = " << it->second << endl; 
	}
}


void	File_Parsing::get_host_name(void)
{
	string	port_key;
	size_t	index;
	string	host;
	string	port;
	dt	data;
	int i = 0;

	while (i < servers_count)
	{
		port = "";
		host = "";
		port_key = "listen_"+ std::to_string(i + 1);
		index = servers.values[port_key].find(':');

		if (index != string::npos)
		{
			host = servers.values[port_key].substr(0, index);
			port = servers.values[port_key];
			port = port.substr(index + 1);
		}
		else
		{
			port = servers.values[port_key];
			host = "0.0.0.0";
		}
		data.key = host;
		data.val = port;
		host_port.push_back(data);
		i++;
	}

}

location_data	File_Parsing::get_location_val(DynamicStruct location)
{
	location_data l_data;

	l_data.root = location.values["root"];
	l_data.cgi = location.values["cgi"];
	l_data.directory_listing = location.values["directory_listing"];
	l_data.methods = location.values["methods"];
	l_data.path = location.values["path"];
	l_data.root = location.values["root"];
	l_data.rturn = location.values["return"];
	
	return l_data;
}
