#include<iostream>
#define endl '\n'
//database related imports
#include<bsoncxx/json.hpp>
#include<bsoncxx/oid.hpp>
#include<bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

const std::string db_name = "directory";
const std::string collection_name = "contacts";

//database initilization
mongocxx::instance inst{};
mongocxx::client connection(mongocxx::uri{});

//default database and collection name to access
auto coll = connection[db_name][collection_name];  //connection established

//function to insert contacts into database
void insert() {
	std::cout << "Inserted" << endl;
}

//function to update the contacts into database
void update() {
	std::cout << "updated" << endl;
}

//function to delete a coontact from the database
void remove() {
	std::cout << "removed" << endl;
}

void search() {
	std::cout << "found" << endl;
}

void showAllContacts() {
	std::cout << "printed" << endl;
}

void menu() {
	int options;
	std::cout << "==================================================";
	std::cout << endl << "Please choose from the below options" << endl;
	std::cout << "=======================================================";
	std::cout << endl << "1.New contact \t\t2.Update contact" << endl;
	std::cout << "3.Delete Contact \t4.Search contact" << endl;
	std::cout << "5.Show all contacts" << endl;
	std::cin >> options;

	//calling functions as per options
	switch (options) {
	case 1:
		insert();
		break;
	case 2:
		update();
		break;
	case 3:
		remove();
		break;
	case 4:
		search();
		break;
	case 5:
		showAllContacts();
		break;
	default:
		std::cout << "Oops , Invalid option try again" << endl;
		break;
	}
}

void welcome() {
	std::cout << "==================================" << endl;
	std::cout << "Welcome to C++ directory-cli" << endl;
	menu();
}

void bye() {
	std::cout << endl << "Thanks for using our services " << endl;
}

//driver code
int main() {
	bool more = true;
	while (more) {
		std::string opt;
		welcome();
		std::cout << endl << "More to do (y/n) : ";
		std::cin >> opt;
		if (opt != "y") {
			bye();
			more = false;
		}
	}
	return 0;
}