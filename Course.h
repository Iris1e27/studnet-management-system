// Course.h
// to record info and achieve functions of course.

#include"stdc++.h"
using namespace std;

class Course{
	
protected:
		
	string name;
	string id;
	int id_int; 
	char level; // U or G.
		
public:
		
	static set<string> course_name_list;
	static int* course_id_list; // make sure id not exist.
	static char* id_level_map; // use id to find level.
		
	static void course_id_list_initialize() // set all to 0.
	{
		memset(Course::course_id_list, 0, sizeof(int) * 1000);
	}

	static void course_id_list_id_use(int id_int_)
	{
		course_id_list[id_int_] = 1;
	}

	static void course_id_list_id_no_use(int id_int_)
	{
		course_id_list[id_int_] = 0;
	}

	static void id_level_map_initialize() // set all to 0.
	{
		memset(Course::id_level_map, 0, sizeof(char) * 1000);
	}

	static void course_level_map_use(int id_int_, char level_)
	{
		id_level_map[id_int_] = level_;
	}

	Course()
	{
		name = "null";
		course_name_list.insert(name);
		id = "000";
		id_int = 0;
		level = 'N';
		course_id_list_id_use(id_int);
		course_level_map_use(id_int, level);
	}

	Course(const string & name_, const string & id_, char level_)
	{
		if(course_name_list.count(name_) == 0) // no exists, add.
		{
			name = name_;
			course_name_list.insert(name);
		}
			
		int id_int_temp = atoi(id_.c_str()); // string to int.
		if(course_id_list[id_int_temp] == 0) // no use.
		{
			id_int = id_int_temp;
			stringstream id_temp; // int to string. 
			id_temp << setw(3) << setfill('0') << id_int; // convert id into 3 digits.
			id = id_temp.str();
			course_id_list_id_use(id_int);
		}
			
		level = level_;
		course_level_map_use(id_int, level); // id and level
	}
		
	Course(Course && e) { // solve left-value.
		this->id = e.id;
		this->name = e.name;
		this->id_int = e.id_int;
		this->level = e.level;
			
		e.id = e.id_int = 0;
	}

	Course(const Course & e) {
		this->id = e.id;
		this->name = e.name;
		this->id_int = e.id_int;
		this->level = e.level;
	}

	Course & operator=(Course && e) {
		this->id = e.id;
		this->name = e.name;
		this->id_int = e.id_int;
		this->level = e.level;

		e.id = e.id_int = 0;

		return *this;
	}

	~Course()
	{
		// all list changes should in main().
	}
				
	void get_info()
	{
		cout << "Course name: " << name << endl;
		cout << "Course ID: " << id << endl;
		cout << "Course level: " << level << endl;
	}

	string get_id()
	{
		return id;
	}

	int get_id_int()
	{
		return id_int;
	}

	string get_name()
	{
		return name;
	}
		
	char get_level()
	{
		return level;
	}

	void set_name(const string & name_) // admin uses.
	{
		name = name_;
	}
		
	void set_id(const string & id_) // admin uses.
	{
		int id_int_temp = atoi(id_.c_str());
		swap(id_level_map[id_int], id_level_map[id_int_temp]); // modify map.
		course_id_list_id_no_use(id_int); // original id_int should be 0.
		id_int = id_int_temp;
		stringstream id_temp;
		id_temp << setw(3) << setfill('0') << id_int; // convert id into 3 digits.
		id = id_temp.str();
		course_id_list_id_use(id_int);
	}
		
	void set_level(char level_) // admin uses.
	{
		level = level_;
	}
		
};

int* Course::course_id_list = new int[1000];
char* Course::id_level_map = new char[1000];
set<string> Course::course_name_list;
