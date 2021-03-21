#ifndef __ODYS_PATIENT_RECORD__
#define __ODYS_PATIENT_RECORD__
#include <string>
#include "date.h"
#include "date_tree.h"
#include "hash_table.h"
using namespace std;
int is_date_string_OK(string);

class patientRecord {
private:
	string recordID;
	string patientFirstName;
	string patientLastName;
	string diseaseID;
	string country;
	Date* entryDate;
	Date* exitDate;
public:
	patientRecord();
	int initialize(string line);
	string get_recordID();
	Date* getEntryDate();
	Date* getExitDate();
	int still_in_hospital();
	int update_exit_date(Date* exit_date);
	int hospitalized_between_dates(Date* date1, Date* date2);
	string getDiseaseID();
	string getCountry();
	void print();
	~patientRecord();
};

typedef struct tree_node TreeNode;
struct tree_node {
	patientRecord* data;
	TreeNode* Left;
	TreeNode* Right;
};

class BSTree {
private:
	TreeNode* Root;
	int insert(patientRecord* new_data, TreeNode*& root);
	void print_recursive(TreeNode* root);
	void tree_destructor_recursive(TreeNode* root);
	int update_patient_exit_recursive(string ID, Date* exit_date, TreeNode* root);
	void rebalance(TreeNode*& root);
public:
	BSTree();
	int insert(patientRecord* new_data);
	int update_patient_exit(string ID, string exit_date);
	// int count_cases(string virusName, Date* date1, Date* date2, string country_wanted);
	void print();
	~BSTree();
};

#endif