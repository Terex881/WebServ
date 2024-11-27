#include "./File_Parsing.hpp"

int main()
{

	File_Parsing parse;
	int open_b = 0;
	int close_b = 0;
	int i = 0;
	parse.obj = parse.recursive_push(&(parse.file), "", &open_b, &close_b);
	if (open_b != close_b)
	{
		parse.file.close();
		return 1;
	}
	parse.Checking_Hierarchy(parse.obj, &parse.servers, parse.locations);

	// servers Data
	cout << "-------- Servers Data --------" << endl;
	parse.Struct_Call(parse.servers);

	cout << "\n-------- Locations Data --------" << endl;
	while (!parse.locations[i].values.empty())
	{
		parse.Struct_Call(parse.locations[i]);
		cout << endl;
		i++;
	}

	cout << "\n**************** Success **************** " << endl;
	// parse.obj = parse.recursive_push();
}
