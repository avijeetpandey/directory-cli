#include<iostream>
#include<chrono>
#include<ctime>
#pragma warning(disable : 4996)
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


//structure to store details
struct contact {
	std::string name;
	std::string phone;
	std::string email;

	//structure to store address
	struct address {
		std::string city;
		std::string district;
		int zip;
	}address;
};

//function to get time stamp

char* getTimeStamp() {
	auto e = std::chrono::system_clock::now();
	time_t time = std::chrono::system_clock::to_time_t(e);
	char* timestamp = ctime(&time);

	return timestamp;
}

void getDetails(contact &c) {
	std::cout << "Name : ";
	std::cin >> c.name;
	std::cout << "Phone : ";
	std::cin >> c.phone;
	std::cout << "E-mail : ";
	std::cin >> c.email;
	std::cout << endl << "==============Address Details==========" << endl;
	std::cout << "City : ";
	std::cin >> c.address.city;
	std::cout << "District : ";
	std::cin >> c.address.district;
	std::cout << "Zip : ";
	std::cin >> c.address.zip;
}

//function to insert contacts into database
void insert() {
	contact c;
	getDetails(c);
	char* timestamp = getTimeStamp();
	//creating document to insert;
	auto builder =bsoncxx::builder::stream::document{};

	bsoncxx::document::value doc=builder
		<< "name" << c.name
		<< "phone" << c.phone
		<< "created" << timestamp
		<< "email" << c.email
		<< "address" << open_document
		<< "city" << c.address.city
		<< "district" << c.address.district
		<< "zip" << c.address.zip << close_document << finalize;

	coll.insert_one(doc.view());
	std::cout << "Record added succesfully" << endl;

}



//function to delete a coontact from the database
void remove() {
	std::string name;
	std::cout << "Name ? : ";
	std::cin >> name;

	coll.delete_one(document{} << "name" << name << finalize);
	std::cout << "Entry deleted succesfully" << endl;
}

void search() {
	std::string name;
	std::cout << "Enter name to be searched : ";
	std::cin >> name;
	//finding specific documents
	mongocxx::cursor cursor = coll.find(
		document{} << "name" << open_document
		<< "$eq" <<name
		<<close_document<<finalize
	);

	    std::cout<< "==============================================" << endl;
	for (auto doc : cursor) {
		std::string extracted_name = doc["name"].get_utf8().value.to_string();
		std::string extracted_phone = doc["phone"].get_utf8().value.to_string();
		std::cout << "Name : " << extracted_name << "  " << extracted_phone << endl;
		std::cout<< "=============================================="<<endl;
	}

}

void showAllContacts() {
	//display only names and contact numbers
	mongocxx::cursor cursor = coll.find({});
	for (auto doc : cursor)
	{
		std::cout << bsoncxx::to_json(doc) << endl;
	}
}

void menu() {
	int options;
	std::cout << "==================================================";
	std::cout << endl << "Please choose from the below options" << endl;
	std::cout << "=======================================================";
	std::cout << endl << "1.New contact \t\t2.Show all contacts" << endl;
	std::cout << "3.Delete Contact \t4.Search contact" << endl;
	std::cin >> options;

	//calling functions as per options
	switch (options) {
	case 1:
		insert();
		break;
	case 2:
		showAllContacts();
		break;
	case 3:
		remove();
		break;
	case 4:
		search();
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