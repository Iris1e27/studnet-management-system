/*
编写admin分支：（可以：浏览，修改，添加，删除对象，对象为学生或课程；不可以：修改学生的选课情况）
先选择操作对象：学生or课程，再选择操作类型：浏览or修改or添加or删除（共8个事件需要编写）
*/ 
/*
编写student分支：（可以：搜索个人信息or过去的成绩，选课；不可以：其他都不行）
先选择操作类型：搜索or选课，若为搜索则再选择操作对象：个人信息or过去的分数（共3个事件）
*/

#include "json.hpp"
#include "stdc++.h"
#include "Student.h"
#include "global_path.h"
using namespace std;

using json = nlohmann::json;

#define ADMIN_PASSWORD "admin"

Student student_null = Student();
Course course_null = Course(); 

vector<Student> people(10000);
vector<Course> courses(1000);

string person; // use for display info.

void branch(string);
void print_line();
string read_a_string();
int string_to_int(string str);
bool check_student_id_int_no_use(int id_int); // make sure there is a student.
bool check_course_id_int_no_use(int id_int);
void clean_screen();

void people_initialize()
{
	// read from file.
	std::ifstream i(STUDENT_PATH);
	json students_json;
	i >> students_json;
	for (json::iterator it = students_json.begin(); it != students_json.end(); ++it)
	{
		string id = it.key();
		int id_int = string_to_int(id);

		string name = students_json[id]["name"];
		int age = students_json[id]["age"];
		char gender = students_json[id]["gender"].get<char>();
		char level = students_json[id]["level"].get<char>();
		string special_info = students_json[id]["special_info"];
		string password = students_json[id]["password"];

		vector <bool> sc = students_json[id]["selected_courses"];
		vector <char> pr = students_json[id]["past_results"];

		Student new_student = std::move(Student(name, age, gender, level, special_info, password));
		people[id_int] = std::move(new_student);
		people[id_int].set_id(id); // to prevent if auto id not equal to true id.
		people[id_int].set_selected_courses(sc);
		people[id_int].set_past_results(pr);

		Student::student_list_id_use(id_int);
	}

}

void courses_initialize()
{
	// read from file.
	std::ifstream i(COURSE_PATH);
	json courses_json;
	i >> courses_json;
	for (json::iterator it = courses_json.begin(); it != courses_json.end(); ++it)
	{
		string id = it.key();
		int id_int = string_to_int(id);

		string name = courses_json[id]["name"];
		char level = courses_json[id]["level"].get<char>();

		Course new_course = std::move(Course(name, id, level));
		courses[id_int] = std::move(new_course);

		Course::course_id_list_id_use(id_int);
		Course::course_level_map_use(id_int, level);
		Course::course_name_list.insert(name);
	}
}

void people_store()
{
	// write to file.
	json students_json;
	for (int i = 1; i <= 9999; i++)
	{
		if (! check_student_id_int_no_use(i))
		{
			string id = people[i].get_id();
			students_json[id]["name"] = people[i].get_name();
			students_json[id]["age"] = people[i].get_age();
			students_json[id]["id"] = people[i].get_id();
			students_json[id]["id_int"] = people[i].get_id_int();
			students_json[id]["gender"] = people[i].get_gender();
			students_json[id]["level"] = people[i].get_level();
			students_json[id]["special_info"] = people[i].get_special_info();
			students_json[id]["password"] = people[i].get_password();
			vector <bool> sc(people[i].get_selected_courses(), people[i].get_selected_courses() + 1000);
			students_json[id]["selected_courses"] = sc;
			vector <char> pr(people[i].get_past_results(), people[i].get_past_results() + 1000);
			students_json[id]["past_results"] = pr;
		}
	}

	std::ofstream o(STUDENT_PATH);
	o << std::setw(4) << students_json << std::endl;

}
void courses_store()
{
	// write to file.
	json courses_json;
	for (int i = 1; i <= 999; i++)
	{
		if (!check_course_id_int_no_use(i))
		{
			string id = courses[i].get_id();
			courses_json[id]["name"] = people[i].get_name();
			courses_json[id]["id"] = people[i].get_id();
			courses_json[id]["id_int"] = people[i].get_id_int();
			courses_json[id]["level"] = people[i].get_level();
		}
	}

	std::ofstream o(COURSE_PATH);
	o << std::setw(4) << courses_json << std::endl;
	
}

void login()
{
	string id;
	int id_int;
	string password;
	int identity;
	
	do
	{
		print_line();
		cout << "This is login interface. " << endl;
		cout << "Please choose your identity: " << endl;
		cout << "1. Administrator. " << endl;
		cout << "2. Student. " << endl;
		cin >> identity;

		cout << "Please type your id (0000 - 9999): " << endl;
		
		id = read_a_string();
		id_int = string_to_int(id);

		if (identity == 1)
		{
			if (id_int == 0) // admin.
			{
				cout << "Please type your password: " << endl;
				password = read_a_string();
				if (strcmp(password.c_str(), ADMIN_PASSWORD) == 0)
				{
					person = "administrator";
					clean_screen();
					branch(id); break;
				}
				else cout << "Incorrect password, please type again. " << endl;
			}
			else cout << "Illegal admin id, please type again. " << endl;
		}

		if (identity == 2)
		{
			if (!(id_int >= 1 && id_int <= 9999)) cout << "Illegal student id, please type again. " << endl;
			else if (check_student_id_int_no_use(id_int)) cout << "Student id not found, please type again. " << endl; // student.
			else
			{
				cout << "Please type your password: " << endl;
				password = read_a_string();
				if (strcmp(password.c_str(), people[id_int].get_password().c_str()) == 0)
				{
					person = "student " + people[id_int].get_name();
					clean_screen();
					branch(id); break;
				}
				else cout << "Incorrect password, please type again. " << endl;
			}
		}
		
	} while (true);
}

int main(int argc, char** argv) {
	
	// set the vector capacity.

	people.reserve(10000); // Calling reserve() does not generate any elements. 
	courses.reserve(1000); // Set the memory allocation of the container, which can hold at least x elements.
	
	Student::student_list_initialize();
	Course::course_id_list_initialize();
	Course::id_level_map_initialize();

	people_initialize();
	courses_initialize();

	login();

	people_store();
	courses_store();

	return 0;
}

void print_line()
{
	cout << "**********" << endl;
}

void print_complete()
{
	cout << "Complete successfully. " << endl;
}

void print_skip()
{
	cout << "Skip successfully. " << endl;
}

void clean_screen()
{
	system("pause");
	system("cls");
}

string read_a_string() // max length = 100.
{
	char str_char[100];
	cin >> std::ws;
	gets_s(str_char);
	return str_char;
}

int string_to_int(string str)
{
	return atoi(str.c_str());
}

int get_student_id_int() // from keyboard.
{
	string id;
	int id_int;

	print_line();
	cout << "Please type student id (0001 - 9999): " << endl;
	do
	{
		id = read_a_string();
		id_int = string_to_int(id);
		if (id_int >= 1 && id_int <= 9999) break;
		else cout << "Illegal student id, please type again. " << endl;
	} while (true);
	
	return id_int;
}

int get_course_id_int() // from keyboard.
{
	string id;
	int id_int;

	print_line();
	cout << "Please type course id (001 - 999): " << endl;
	do
	{
		id = read_a_string();
		id_int = string_to_int(id);
		if (id_int >= 1 && id_int <= 999) break;
		else cout << "Illegal course id, please type again. " << endl;
	} while (true);

	return id_int;
}

int find_operation_object()
{
	int operation_object = 0;
	print_line();
	cout << "Please choose the operation object: " << endl;
	cout << "1. Students. " << endl;
	cout << "2. Courses. " << endl;
	cin >> operation_object;
	return operation_object;
}

int find_operation_type()
{
	int operation_type = 0;
	print_line();
	cout << "Please choose the operation type: " << endl;
	cout << "1. Browse. " << endl;
	cout << "2. Modify. " << endl;
	cout << "3. Add. " << endl;
	cout << "4. Delete. " << endl;
	cin >> operation_type;
	return operation_type;
}

int find_specific_object_brower_student()
{
	int specific_object = 0;
	print_line();
	cout << "Please choose specific object: " << endl;
	cout << "1. Basic infomation. " << endl;
	cout << "2. Past score. " << endl;
	cin >> specific_object;
	return specific_object;
}

int find_specific_object_modify_student()
{
	int specific_object = 0;
	print_line();
	cout << "Please choose the specific object: " << endl;
	cout << "1. Name. " << endl;
	cout << "2. ID. " << endl;
	cout << "3. Age. " << endl;
	cout << "4. Gender. " << endl;
	cout << "5. Password. " << endl;
	cout << "6. Score. " << endl;
	cin >> specific_object;
	return specific_object;
}

int find_specific_object_modify_course()
{
	int specific_object = 0;
	print_line();
	cout << "Please choose the specific object: " << endl;
	cout << "1. Name. " << endl;
	cout << "2. ID. " << endl;
	cout << "3. Level. " << endl;
	cin >> specific_object;
	return specific_object;
}

bool check_student_id_int_no_use(int id_int) // to cheak id no used.
{
	if (Student::student_list[id_int] == 0) return true;
	else return false;
}

bool check_course_name_no_use(string name) // to cheak name no used.
{
	if (Course::course_name_list.count(name) == 0) return true;
	else return false;
}

bool check_course_id_int_no_use(int id_int) // to cheak id no used.
{
	if (Course::course_id_list[id_int] == 0) return true;
	else return false;
}

void print_student_no_exist()
{
	cout << "Sorry, this student doesn't exist. " << endl;
}

void print_course_no_exist()
{
	cout << "Sorry, this course doesn't exist. " << endl;
}

void brower_student_info(int id)
{
	people[id].get_info();
	print_complete();
}

void brower_student_score(int id)
{
	people[id].get_score();
	print_complete();
}

void modify_student_name(int id) 
{
	print_line();
	cout << "Please type new name: " << endl;
	string name = read_a_string();
	people[id].set_name(name);
	print_complete();
}

void modify_student_id(int id)
{
	string id_new;
	int id_new_int;

	print_line();
	cout << "Please type new id (0001 - 9999): " << endl;
	do
	{
		id_new = read_a_string();
		id_new_int = string_to_int(id_new);
		if (id_new_int >= 1 && id_new_int <= 9999)
		{
			if (check_student_id_int_no_use(id_new_int))
			{
				people[id_new_int] = std::move(people[id]);
				people[id] = std::move(student_null);
				people[id_new_int].set_id(id_new);
				Student::student_list_id_use(id_new_int);
				Student::student_list_id_no_use(id);
				print_complete(); break;
			}
			else cout << "Illegal id which has been used, please type again. " << endl;
		}
		else cout << "Illegal student id, please type again. " << endl;
	} while (true);
}

void modify_student_age(int id)
{
	int age;
	print_line();
	cout << "Please type new age (1 - 99): " << endl;
	do
	{
		cin >> age;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "Illegal age, please type number again. " << endl;
		}
		else if (age > 0 && age <= 99)
		{
			people[id].set_age(age);
			print_complete(); break;
		}
		else cout << "Illegal age, please type again. " << endl;
	} while (true);
}

void modify_student_gender(int id)
{
	char gender;
	print_line();
	cout << "Please type new gender (M or F): " << endl;
	do
	{
		cin >> gender;
		if (gender == 'M' || gender == 'F')
		{
			people[id].set_gender(gender);
			print_complete(); break;
		}
		else cout << "Illegal gender, please type again. " << endl;
	} while (true);
}

void modify_student_level(int id)
{
	char level;
	print_line();
	cout << "Please type new level (U or G): " << endl;
	do
	{
		cin >> level;
		if (level == 'U' || level == 'G')
		{
			people[id].set_level(level);
			print_complete(); break;
		}
		else cout << "Illegal level, please type again. " << endl;
	} while (true);
}

void modify_student_special_info(int id)
{
	string special_info;
	print_line();
	if (people[id].get_level() == 'U')
	{
		cout << "Please type new department: " << endl;
		special_info = read_a_string();
	}

	if (people[id].get_level() == 'G')
	{
		cout << "Please type new mentor: " << endl;
		special_info = read_a_string();
	}

	people[id].set_special_info(special_info);
}

void modify_student_password(int id)
{
	print_line();
	cout << "Please type new password: " << endl;
	string password = read_a_string();
	people[id].set_password(password);
	print_complete();
}

void modify_student_score(int id)
{
	string course_id;
	int course_id_int;
	char score;

	print_line();
	cout << "Please type course id (001 - 999): " << endl;
	cout << "If you want to skip, please type 0.  " << endl;
	do
	{
		course_id = read_a_string();
		course_id_int = string_to_int(course_id);
		if (course_id_int == 0)
		{
			print_skip(); break;
		}
		if (course_id_int >= 1 && course_id_int <= 999)
		{
			if (check_course_id_int_no_use(course_id_int)) print_course_no_exist();
			else 
			{
				cout << "Please type score (P or F): " << endl;
				do
				{
					cin >> score;
					if (score == 'P' || score == 'F')
					{
						people[id].set_score(course_id, score);
						print_complete(); break;
					}
					else cout << "Illegal course score, please type P or F. " << endl;
				} while (true);
				break;
			}
			
		}
		else cout << "Illegal course id, please type again. " << endl;
	} while (true);
}

void add_student()
{
	string name;
	int age;
	char gender;
	char level;
	string special_info;
	string password;

	print_line();
	cout << "Please type student name: " << endl;
	name = read_a_string();

	cout << "Please type student age (1 - 99): " << endl;
	do
	{
		cin >> age;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			cout << "Illegal age, please type number again. " << endl;
		}
		else if (age > 0 && age <= 99) break;
		else cout << "Illegal age, please type again. " << endl;
	} while (true);
	
	cout << "Please type student gender (M or F): " << endl;
	do
	{
		cin >> gender;
		if (gender == 'M' || gender == 'F') break;
		else cout << "Illegal gender, please type again. " << endl;
	} while (true);

	cout << "Please type student level (U or G): " << endl;
	do
	{
		cin >> level;
		if (level == 'U')
		{
			cout << "Please type student department: " << endl;
			special_info = read_a_string();
			break;
		}
		else if (level == 'G')
		{
			cout << "Please type student mentor: " << endl;
			special_info = read_a_string();
			break;
		}
		else cout << "Illegal level, please type again. " << endl;
	} while (true);

	cout << "Please type student password: " << endl;
	password = read_a_string();

	Student new_student = std::move(Student(name, age, gender, level, special_info, password));
	people[new_student.get_id_int()] = std::move(new_student);
	Student::student_list_id_use(new_student.get_id_int());

	print_complete();
}

void delete_student()
{
	do
	{
		int id = get_student_id_int();
		if (!check_student_id_int_no_use(id))
		{
			people[id].~Student(); // set student_list = 0, release memory.
			people[id] = std::move(student_null); // delete object info, not delete object, so just set to null.
			print_complete(); break;
		}
		else print_student_no_exist();
	} while (true);
	
}

void brower_course_info()
{
	int id = get_course_id_int();
	
	if (!check_course_id_int_no_use(id))
	{
		courses[id].get_info();
		print_complete();
	}
	else print_course_no_exist();
}

void modify_course_name(int id)
{
	string name;
	print_line();
	cout << "Please type new name: " << endl;
	do
	{
		name = read_a_string();
		if (check_course_name_no_use(name))
		{
			Course::course_name_list.erase(courses[id].get_name());
			courses[id].set_name(name); // set_name has changed course_name_list.
			Course::course_name_list.insert(name);
			print_complete(); break;
		}
		else cout << "Illegal course name which has been used, please type again. " << endl;
	} while (true);
}

void modify_course_id(int id)
{
	string id_new;
	int id_new_int;

	print_line();
	cout << "Please type new id: " << endl;
	do
	{
		id_new = read_a_string();
		id_new_int = string_to_int(id_new);
		if (id_new_int >= 1 && id_new_int <= 999)
		{
			if (check_course_id_int_no_use(id_new_int))
			{
				char level = courses[id].get_level();
				courses[id_new_int] = std::move(courses[id]);
				courses[id] = std::move(course_null);
				courses[id_new_int].set_id(id_new);
				Course::course_id_list_id_use(id_new_int);
				Course::course_id_list_id_no_use(id);
				Course::course_level_map_use(id_new_int, level);
				Course::course_level_map_use(id, 'N');
				print_complete(); break;
			}
			else cout << "Illegal course id which has been used, please type again.  " << endl;
		}
		else cout << "Illegal course id, please type again. " << endl;
	} while (true);
}

void modify_course_level(int id)
{
	char level;

	print_line();
	cout << "Please type new level (U or G): " << endl;
	do
	{
		cin >> level;
		if (level == 'U' || level == 'G')
		{
			courses[id].set_level(level);
			Course::course_level_map_use(id, level);
			print_complete(); break;
		}
		else cout << "Illegal level, please type again. " << endl;
	} while (true);
}

void add_course()
{
	string name;
	string id;
	int id_int;
	char level;

	print_line();

	cout << "Please type course name: " << endl;
	do
	{
		name = read_a_string();
		if (check_course_name_no_use(name)) break;
		else cout << "Illegal course name which has been used, please type again. " << endl;
	} while (true);

	cout << "Please type course id: " << endl;
	do
	{
		id = read_a_string();
		id_int = string_to_int(id);
		if (id_int >= 1 && id_int <= 999)
		{
			if (check_course_id_int_no_use(id_int)) break;
			else cout << "Illegal course id which has been used, please type again.  " << endl;
		}
		else cout << "Illegal course id, please type again. " << endl;
	} while (true);

	cout << "Please type course level (U or G): " << endl;
	do
	{
		cin >> level;
		if (level == 'U' || level == 'G') break;
		else cout << "Illegal level, please type again. " << endl;
	} while (true);

	Course new_course = std::move(Course(name, id, level));
	courses[id_int] = std::move(new_course);
	Course::course_id_list_id_use(id_int);
	Course::course_name_list.insert(name);
	Course::course_level_map_use(id_int, level);
	print_complete();
}

void delete_course()
{
	do
	{
		int id = get_course_id_int();
		if (!check_course_id_int_no_use(id))
		{
			Course::course_id_list_id_no_use(courses[id].get_id_int());
			Course::course_name_list.erase(courses[id].get_name());
			Course::course_level_map_use(courses[id].get_id_int(), 'N');
			courses[id].~Course();
			courses[id] = std::move(course_null); // delete object info, not delete object, so just set to null.
			print_complete(); break;
		}
		else print_course_no_exist();
	} while (true);
	
}

void admin_sub_branch()
{
	// choose sub-branch.

	int operation_object = find_operation_object();
	int operation_type = find_operation_type();
	
	// specific operations for 8 events.

	if (operation_object == 1 && operation_type == 1) // Browse students info.
	{
		int id = get_student_id_int();
		if (!check_student_id_int_no_use(id))
		{
			//people[id].get_student_list();

			int specific_object = find_specific_object_brower_student();

			if (specific_object == 1) brower_student_info(id);

			if (specific_object == 2) brower_student_score(id);
		}
		else print_student_no_exist();
		
	}
	
	if (operation_object == 1 && operation_type == 2) // Modify students info.
	{
		int id = get_student_id_int();

		if (!check_student_id_int_no_use(id))
		{
			int specific_object = find_specific_object_modify_student();

			if (specific_object == 1) modify_student_name(id);

			if (specific_object == 2) modify_student_id(id);

			if (specific_object == 3) modify_student_age(id);

			if (specific_object == 4) modify_student_gender(id);

			if (specific_object == 5) modify_student_password(id);

			if (specific_object == 6) modify_student_score(id);
		}
		else print_student_no_exist();
		
	}
	
	if (operation_object == 1 && operation_type == 3) add_student();
	
	if (operation_object == 1 && operation_type == 4) delete_student();
	
	if (operation_object == 2 && operation_type == 1) brower_course_info();
	
	if (operation_object == 2 && operation_type == 2) // Modify courses info.
	{
		int id = get_course_id_int();

		if (!check_course_id_int_no_use(id))
		{
			int specific_object = find_specific_object_modify_course();

			if (specific_object == 1) modify_course_name(id);

			if (specific_object == 2) modify_course_id(id);

			if (specific_object == 3) modify_course_level(id);
		}
		else print_course_no_exist();
	}
	
	if (operation_object == 2 && operation_type == 3) add_course();
	
	if (operation_object == 2 && operation_type == 4) delete_course();
}

int find_operation_type_stu()
{
	int operation_type = 0;
	print_line();
	cout << "Please choose the operation type: " << endl;
	cout << "1. Browse. " << endl;
	cout << "2. Choose courses. " << endl;
	cin >> operation_type;
	return operation_type;
}

void student_choose_course(string id)
{
	int id_int = string_to_int(id);
	people[id_int].choose_course();
	print_line();
	cout << "Please type course id which you want to choose: " << endl;
	cout << "If you want to skip, please type 0. " << endl;
	do
	{
		string course_id = read_a_string();
		int course_id_int = string_to_int(course_id);

		if (course_id_int == 0)
		{
			print_skip(); break;
		}
		else if (people[id_int].check_course_id_not_selected(course_id_int) == true)
		{
			if (!check_course_id_int_no_use(course_id_int))
			{
				people[id_int].set_score(course_id, 'N');
				print_complete(); break;
			}
			else cout << "Illegal course id, please type again. " << endl;
		}
		else cout << "This course has been selected, please type again. " << endl;
		
	} while (true);
	
}

void student_sub_branch(string id) // admin: 0000, student: 0001 - 9999.
{
	int id_int = string_to_int(id);
	// choose sub-branch.

	int operation_type = find_operation_type_stu();

	// specific operations for 3 events.

	if (operation_type == 1) // brower student info.
	{
		int specific_object = find_specific_object_brower_student();

		if (specific_object == 1) brower_student_info(id_int);

		if (specific_object == 2) brower_student_score(id_int);
	}

	if (operation_type == 2) student_choose_course(id);
}

void branch(string id)
{
	int id_int = string_to_int(id);

	print_line();
	cout << "Welcome, " << person << "! " << endl;

	do
	{
		int type_num = 0;
		if (id_int == 0) admin_sub_branch();
		else student_sub_branch(id);
		
		clean_screen();
		print_line();
		cout << "Please type whether you want to continue: " << endl;
		cout << "1. Continue. " << endl;
		cout << "2. Log in with other account. " << endl;
		cout << "3. Log out. " << endl;
		cin >> type_num;
		clean_screen();

		if (type_num == 2)
		{
			login(); break;
		}
		if (type_num == 3)
		{
			print_line();
			cout << "Your operations have been recorded. " << endl;
			cout << "Goodbye. " << endl;
			getchar();
			break;
		}
		else continue;
	} while (true);
}