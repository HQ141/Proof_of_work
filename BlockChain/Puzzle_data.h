#include"json.hpp"
#include<iostream>
#include<chrono>
#include<time.h>
#include<thread>
#include<fstream>
#include<iomanip>
#include<direct.h>
using namespace std;
using json = nlohmann::json;
class status {
public:
	int verify = 0;
	bool minned = false;
	int ans;
	size_t hash;
	int iMinners;
};
class Data {
public:
	json object;
	Data(){
		object["Name"] = "";
		object["Id"] = "";
		object["TimeStamp"] = "";
	}
	friend istream& operator >> (istream& in, Data& da) {
		in >> da.object;
		stringstream strs;
		time_t now = time(0);
		strs << ctime(&now);
		da.object["TimeStamp"] = strs.str();
		return in;
	}
	
	void set(string name, string id) {
		object["Name"] = name;
		object["Id"] = id;
		stringstream strs;
		time_t now = time(0);
		strs << ctime(&now);
		object["TimeStamp"] = strs.str();
	}
	Data(string name, string id) {
		object["Name"] = name;
		object["Id"] = id;
		stringstream strs;
		strs << chrono::system_clock::now;
		object["TimeStamp"] = strs.str();
	}
	json getjson() { return object; }
};
int Mine(json object, status& curr, int id) {
	stringstream obj;
	obj << object;
	string a;
	size_t temp = 0;
	bool me = false;
	int i = 0;
	for (; i < 1000; i++) {

		if (curr.minned)
			break;
		a = obj.str() + to_string(i);
		if (curr.minned)
			break;
		if (std::hash<string>{}(a) == curr.hash) {
			if (curr.minned)
				break;
			curr.minned = true;
			me = true;
			curr.ans = i;
			break;
		}

	}
	if (me) {
		int a;
		while (curr.verify != curr.iMinners - 1)
			this_thread::sleep_for(chrono::milliseconds(390));
		fstream config;
		string str = to_string(id) + "/Config.txt";
		if (!_mkdir(to_string(id).c_str())) {
			config.open(str, ios::out);
			config << 0;
			a = 0;
			config.close();
		}
		else {
			string str = to_string(id) + "/Config.txt";
			config.open(str, ios::in);
			config >> a;
			config.close();
		}
		config.open(str, ios::out);
		fstream fi;
		str = to_string(id)+"/" +to_string(a)+ ".txt";
		a++;
		fi.open(str, ios::app);
		object["Minner Id"] = to_string(id);
		object["Minner Item Id"] = to_string(a);
		object["Verified By"] = to_string(curr.verify);
		fi <<setw(4)<< object;
		config << a;
		config.close();
		fi.close();
		return 1;
	}
	else {
		this_thread::sleep_for(chrono::seconds(5));
		a = obj.str() + to_string(curr.ans);
		if (std::hash<string>{}(a) == curr.hash)
			curr.verify++;
		return 0;
	}

}