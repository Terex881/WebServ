#include "./File_Parsing.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		File_Parsing parse(argv[1]);
		location_data l_data;
		int i = 0;

		cout << "-------- Servers Data --------" << endl;
		// servers Data
		parse.Struct_Call(parse.servers);

		cout << "\n-------- Locations Data --------" << endl;
		while (!parse.locations[i].values.empty())
		{
			parse.Struct_Call(parse.locations[i]);
			cout << endl;
			i++;
		}




		cout << "***************** Getting Servers Data ************************" << endl;
			// ports
			for (vector<dt>::iterator it = parse.host_port.begin(); it != parse.host_port.end(); it++)
			{
				cout << "host " << it->key << " port " << it->val << endl;
			}
			// error_page
			cout << "error_page " << parse.servers.values["error_page"] << endl;
		cout << "***************** Getting Servers Data ************************" << endl;





		i = 0;
		cout << "***************** Getting Location Data ************************" << endl;

			while (!parse.locations[i].values.empty())
			{
				l_data = parse.get_location_val(parse.locations[i]);
				cout << "root " << l_data.root << endl;
				i++;
			}

		cout << "***************** Getting Location Data ************************" << endl;
		cout << "\n**************** Success **************** " << endl;

		parse.get_host_name();

	}
}
