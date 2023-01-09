// Student.h
// 2 sub-class: undergraduate and graduate.
// to record info and achieve functions of course.

#include"stdc++.h"
#include"Course.h"
using namespace std;

class Student{
	
protected:
		
	string name;
	int age;
	string id; // to display info.
	string password;
	int id_int; // to achieve some funcs of this class. (the main function does not need it.)
	char gender; // F or M.
	char level; // U or G.
	string special_info; // to store department(under) or mentor(grad).
	bool *selected_courses; // ture is selected, false is not selected. 
	char *past_results; // P or F. 

		
public:
		
	static int* student_list; 
		
	static void student_list_initialize() // set all to 0.
	{
		memset(Student::student_list, 0, sizeof(int) * 10000);
	}

	static void student_list_id_use(int id_int_) // set id used.
	{
		student_list[id_int_] = 1;
	}

	static void student_list_id_no_use(int id_int_) // set id no used.
	{
		student_list[id_int_] = 0;
	}

	Student() // default conduct function.
	{
		name = "null";
		age = 0;
		gender = 'N';
		password = "null";
		id = "0000";
		id_int = 0;
		special_info = "";
		level = 'N';
		selected_courses = nullptr; // ture is selected, false is not selected. 
		past_results = nullptr; // P or F or N.
	}

	Student (const string & name_, int age_, char gender_, 
			char level_, const string& special_info_, const string & password_)
	{
		name = name_;
		age = age_;
		gender = gender_;
		password = password_; 
		level = level_;
		special_info = special_info_;
		
		for(int i = 1; i < 10000; i++) //make sure legal id.
		{
			if(student_list[i] == 0) // no use.
			{
				id_int = i;
				stringstream id_temp; // int to string. 
				id_temp << setw(4) << setfill('0') << id_int; // convert id into 4 digits.
				id = id_temp.str();
				student_list_id_use(id_int);
				// cout << id_int << "," << student_list[id_int] << endl; 
				break; // only need one id.
			}
		}
		selected_courses = new bool[1000]; // ture is selected, false is not selected. 
		past_results = new char[1000]; // P or F or N. 
		for(int i = 0; i < 1000; i++)
		{
			selected_courses[i] = 0;
			past_results[i] = 'N';
		}
	}
		
	Student(Student && e) // solve left-value.
	{ 
		this->id = e.id;
		this->name = e.name;
		this->age = e.age;
		this->password = e.password;
		this->id_int = e.id_int;
		this->gender = e.gender;
		this->level = e.level;
		this->special_info = e.special_info;

		this->selected_courses = e.selected_courses;
		this->past_results = e.past_results;
		e.selected_courses = nullptr;
		e.past_results = nullptr;
		e.id = e.id_int = 0;
	}

	Student(const Student & e) 
	{
		this->id = e.id;
		this->name = e.name;
		this->age = e.age;
		this->password = e.password;
		this->id_int = e.id_int;
		this->gender = e.gender;
		this->level = e.level;
		this->special_info = e.special_info;

		this->selected_courses = new bool[1000];
		this->past_results = new char[1000];
		memcpy_s(this->selected_courses, sizeof(bool) * 1000, e.selected_courses, sizeof(bool) * 1000);
		memcpy_s(this->past_results, sizeof(char) * 1000, e.past_results, sizeof(char) * 1000);
	}

	Student & operator=(Student && e) 
	{
		this->id = e.id;
		this->name = e.name;
		this->age = e.age;
		this->password = e.password;
		this->id_int = e.id_int;
		this->gender = e.gender;
		this->level = e.level;
		this->special_info = e.special_info;

		this->selected_courses = e.selected_courses;
		this->past_results = e.past_results;
		e.selected_courses = nullptr;
		e.past_results = nullptr;
		e.id = e.id_int = 0;

		return *this;
	}

	~Student()
	{
		student_list_id_no_use(id_int);
		delete selected_courses;
		delete past_results;
		selected_courses = nullptr;
		past_results = nullptr;
	}


		
	/*
	static void get_student_list() // for test. 
	{
		for(int i = 1; i < 10000; i++) //make sure legal id.
		{
			cout <<"i=" <<i<<", stulist="<< student_list[i] << endl;
		}
	}
	*/
		
	/*
	void get_courses_info() // for test.
	{
		for(int i = 1; i < 1000; i++) //make sure legal id.
		{
			cout <<"i=" <<i<<", selected_course="<< selected_courses[i] <<", past_results=" << past_results[i] << endl;
		}
	}
	*/
		
	void get_info() // students and admin.
	{
		cout << "Student name: " << name << endl;
		cout << "Student ID: " << id << endl;
		cout << "Student age: " << age << endl;
		cout << "Student gender: " << gender << endl;
		if(level == 'U')
		{
			cout << "Student level: Undergraduate" << endl;
			cout << "Student department: " << special_info << endl;
		}
		else
		{
			cout << "Student level: Graduate" << endl;
			cout << "Student mentor: " << special_info << endl;
		}
			
	}
		
	void get_score() // students and admin.
	{
		int flag = 0;
		cout << "Scores: " << endl;
		for(int i = 1; i < 1000; i++)
		{
			if(selected_courses[i]) // have been selected. 
			{
				cout << "Course ID: " << i << ", Course score: " << past_results[i] << endl;
				flag = 1;
			}
		}
		if(flag == 0) cout << "No course scores were recorded. " << endl;
	}
		
	int get_id_int() // because id is randomly assigned, we don't know.
	{
		return id_int;
	}

	int get_age()
	{
		return age;
	}

	string get_id()
	{
		return id;
	}

	string get_special_info()
	{
		return special_info;
	}

	bool* get_selected_courses()
	{
		return selected_courses;
	}

	char* get_past_results()
	{
		return past_results;
	}

	string get_name()
	{
		return name;
	}

	char get_level()
	{
		return level;
	}

	char get_gender()
	{
		return gender;
	}

	string get_password()
	{
		return password;
	}

	void set_selected_courses(vector<bool> sc)
	{
		std::copy(sc.begin(), sc.end(), this->selected_courses);
	}

	void set_past_results(vector<char> pr)
	{
		std::copy(pr.begin(), pr.end(), this->past_results);
	}

	void set_name(const string & name_) // students and admin.
	{
		name = name_;
	}
		
	void set_id(const string & id_) // admin. default id all legal.
	{
		student_list_id_no_use(id_int);

		int id_int_temp = atoi(id_.c_str()); // string to int.
		this->id_int = id_int_temp;
		stringstream id_temp;
		id_temp << setw(4) << setfill('0') << id_int; // convert id into 4 digits.
		this->id = id_temp.str();

		student_list_id_use(id_int);

	}
		
	void set_age(int age_) // students and admin.
	{
		age = age_;
	}
		
	void set_gender(char gender_) // students and admin.
	{
		gender = gender_;
	}
		
	void set_password(const string & password_)
	{
		password = password_;
	}

	void set_level(char level_)
	{
		level = level_;
	}
		
	void set_special_info(const string & special_info_)
	{
		special_info = special_info_;
	}

	void set_score(const string & id_, char score_) // admin. default score legal.
	{
		int id_int_temp = atoi(id_.c_str());
		selected_courses[id_int_temp] = true;
		past_results[id_int_temp] = score_; 
	}
		
	void choose_course() // students. student level corresponding to course level.
	{
		cout << "You can choose from the following courses: " << endl;
		for (int i = 1; i < 1000; i++)
		{
			if (!selected_courses[i] && Course::id_level_map[i] == level)
			{
				cout << "Course ID: " << i << " ( Not learned ) " << endl;
			}
			if (selected_courses[i] && past_results[i] == 'F') // selected and not pass. 
			{
				cout << "Course ID: " << i << " ( Failed ) " << endl;
			}
		}
	}

	bool check_course_id_not_selected(int course_id)
	{
		if (selected_courses[course_id] == false) return true;
		else return false;
	}
};

int* Student::student_list = new int[10000];