#include "patientRecord.h"

patientRecord::patientRecord() {
	this->entryDate = NULL;
	this->exitDate = NULL;
}
int patientRecord::initialize(string line) {
	char char_line[line.length() + 1];        // Char array
	for (int i = 0; i < (int)line.length(); ++i) { // Copy string to char array
		char_line[i] = line[i];
	}
	char_line[line.length()] = '\0';          // Make sure it ends with a \0
	char* saveptr = char_line;
	this->recordID = strtok_r(saveptr, " ", &saveptr);
	this->patientFirstName = strtok_r(saveptr, " ", &saveptr);
	this->patientLastName = strtok_r(saveptr, " ", &saveptr);
	this->diseaseID = strtok_r(saveptr, " ", &saveptr);
	this->country = strtok_r(saveptr, " ", &saveptr);
	string entry_date = strtok_r(saveptr, " ", &saveptr);
	string exit_date = strtok_r(saveptr, " ", &saveptr);

	if (is_date_string_OK(entry_date) == 0)
		return 2;
	this->entryDate = new Date(entry_date);

	if (exit_date == "-" || exit_date == "") {
		cout << "INITIALIZE PATIENT: File contains a patient still in hospital" << endl;
		this->exitDate = new Date(-1, -1, -1);
	}
	else {
		if (is_date_string_OK(exit_date) == 0)
			return 2;
		this->exitDate = new Date(exit_date);
		if (this->exitDate->earlier_than(this->entryDate)) {
			// cout << "=== EXIT DATE EARLIER\n";
			return 1;
		}
	}
	return 0;
}
string patientRecord::get_recordID() {
	return this->recordID;
}
Date* patientRecord::getEntryDate() {
	return this->entryDate;
}
Date* patientRecord::getExitDate() {
	return this->exitDate;
}
int patientRecord::still_in_hospital() {
	return this->exitDate->still_in_hospital();
}
int patientRecord::update_exit_date(Date* exit_date) {
	if (exit_date->earlier_than(this->entryDate) == 1) {
		delete exit_date;
		return 3;
	}
	// if (this->still_in_hospital() == 0) {
	// 	delete exit_date;
	// 	return 4;
	// }
	// cout << "Updating exit date for recordID " << this->recordID << endl;
	cout << "Record updated" << endl;
	delete this->exitDate;
	this->exitDate = exit_date;
	return 0;
}
int patientRecord::hospitalized_between_dates(Date* date1, Date* date2) {
	// cout << "---   "; date1->print(); cout << "  -  "; this->entryDate->print(); cout << "  -  "; date2->print(); cout << "   ---" << endl;
	if (this->entryDate->earlier_than(date1)) {
		// cout << "NO\n";
		return 0;
	}
	if (this->entryDate->later_than(date2)) {
		// cout << "NO\n";
		return 0;
	}
	// cout << "YES\n";
	return 1;
}
string patientRecord::getDiseaseID() {
	return this->diseaseID;
}
string patientRecord::getCountry() {
	return this->country;
}
void patientRecord::print() {
	// cout << this->recordID << endl << this->patientFirstName << endl << this->patientLastName << endl << this->diseaseID << endl << this->country << endl; this->entryDate->print(); cout << endl; this->exitDate->print(); cout << endl;
	cout << this->recordID << " " << this->patientFirstName << " " << this->patientLastName << " " << this->diseaseID << " " << this->country << " "; this->entryDate->print(); cout << " "; this->exitDate->print(); cout << " ";
	// cout << " == Patient ID: " << this->recordID << " ==" << endl;
	// cout << "First name: " << this->patientFirstName << ", Last name: " << this->patientLastName << endl;
	// cout << "Disease ID: " << this->diseaseID << endl;
	// cout << "Country: " << this->country << endl;
	// cout << "Entry date: "; this->entryDate->print(); cout << endl;
	// cout << "Exit date: "; this->exitDate->print(); cout << endl;
}
patientRecord::~patientRecord() {
	// cout << "patientRecord destructor" << endl;
	if (this->entryDate != NULL)
		delete entryDate;
	if (this->exitDate != NULL)
		delete exitDate;
}

BSTree::BSTree() {
	this->Root = NULL;
}
void height_recursive(TreeNode* root, int& current_depth, int& max_depth) {
	// cout << "current_depth: " << current_depth << endl;
	current_depth++;
	if (current_depth > max_depth)
		max_depth = current_depth;
	if (root->Left != NULL)
		height_recursive(root->Left, current_depth, max_depth);
	if (root->Right != NULL)
		height_recursive(root->Right, current_depth, max_depth);
	current_depth--;
}
int height(TreeNode* root) {
	if (root == NULL)
		return 0;
	int current_depth = 0;
	int max_depth = 0;
	height_recursive(root, current_depth, max_depth);
	return max_depth;
}
int BSTree::insert(patientRecord* new_data) {
	return this->insert(new_data, this->Root);
}
int BSTree::insert(patientRecord* new_data, TreeNode*& root) { // I wanted to use the default argument "TreeNode* root = this->Root", but that is not allowed. So I had to simulate a default argument in the one-line function just above.
	if (root == NULL) {
		root = new TreeNode;
		root->Left = NULL;
		root->Right = NULL;
		root->data = new_data;
	}
	else if (root->data->get_recordID() > new_data->get_recordID()) {
		// return this->insert(new_data, root->Left);
		int retourn = this->insert(new_data, root->Left);
		if (retourn == 1)
			return 1;
		this->rebalance(root);
	}
	else if (root->data->get_recordID() < new_data->get_recordID()) {
		// return this->insert(new_data, root->Right);
		int retourn = this->insert(new_data, root->Right);
		if (retourn == 1)
			return 1;
		this->rebalance(root);
	}
	else if (root->data->get_recordID() == new_data->get_recordID()) {
		cout << "TREE INSERT FUNCTION: Duplicate recordID being inserted" << endl;
		// return this->insert(new_data, root->Left);
		return 1;
	}
	return 0;
}
void BSTree::rebalance(TreeNode*& root) {
	if (root == NULL)
		return;
	if (root->Left != NULL && root->Left->Left != NULL && height(root->Left) - height(root->Right) >= 2 && height(root->Left->Left) - height(root->Left->Right) >= 1) {
		// cout << "a - Left Left rotation" << endl;
		TreeNode* y = root->Left;
		TreeNode* x = root->Left->Left;
		TreeNode* z = root;
		TreeNode* T3 = y->Right;
		TreeNode* T4 = root->Right;
		root = y;
		root->Left = x;
		root->Right = z;
		z->Left = T3;
		z->Right = T4;
	}
	else if (root->Left != NULL && root->Left->Right != NULL && height(root->Left) - height(root->Right) >= 2 && height(root->Left->Right) - height(root->Left->Left) >= 1) {
		// cout << "b - Left Right rotation" << endl;
		TreeNode* y = root->Left;
		TreeNode* x = root->Left->Right;
		TreeNode* z = root;
		TreeNode* T2 = x->Left;
		TreeNode* T3 = x->Right;
		root = x;
		x->Right = z;
		z->Left = T3;
		x->Left = y;
		y->Right = T2;
	}
	else if (root->Right != NULL && root->Right->Right != NULL && height(root->Right) - height(root->Left) >= 2 && height(root->Right->Right) - height(root->Right->Left) >= 1) {
		// cout << "c - Right Right rotation" << endl;
		TreeNode* y = root->Right;
		TreeNode* x = root->Right->Right;
		TreeNode* z = root;
		TreeNode* T1 = root->Left;
		TreeNode* T2 = y->Left;
		root = y;
		root->Right = x;
		root->Left = z;
		z->Left = T1;
		z->Right = T2;
	}
	else if (root->Right != NULL && root->Right->Left != NULL && height(root->Right) - height(root->Left) >= 2 && height(root->Right->Left) - height(root->Right->Right) >= 1) {
		// cout << "d - Right Left rotation" << endl;
		TreeNode* y = root->Right;
		TreeNode* x = root->Right->Left;
		TreeNode* z = root;
		TreeNode* T2 = x->Left;
		TreeNode* T3 = x->Right;
		root = x;
		x->Left = z;
		x->Right = y;
		z->Right = T2;
		y->Left = T3;
	}
}
void BSTree::print() {
	this->print_recursive(this->Root);
	cout << "HEIGHT - " << height(this->Root) << endl;
}
void BSTree::print_recursive(TreeNode* root) { // I wanted to use the default argument "TreeNode* root = this->Root", but that is not allowed. So I had to simulate a default argument in the one-line function just above.
	if (root == NULL) {
		return;
	}
	if (root->Left != NULL)
		this->print_recursive(root->Left);
	root->data->print();
	cout << endl;
	if (root->Right != NULL)
		this->print_recursive(root->Right);
}
void BSTree::tree_destructor_recursive(TreeNode* root) {
	// cout << "tree_destructor\n";
	if (root == NULL)
		return;
	if (root->Left != NULL)
		tree_destructor_recursive(root->Left);
	if (root->Right != NULL)
		tree_destructor_recursive(root->Right);
	delete root->data;
	delete root;
}
int BSTree::update_patient_exit_recursive(string ID, Date* exit_date, TreeNode* root) {
	if (root == NULL) {
		delete exit_date;
		return 2;
	}
	else if (root->data->get_recordID() > ID) {
		return this->update_patient_exit_recursive(ID, exit_date, root->Left);
	}
	else if (root->data->get_recordID() < ID) {
		return this->update_patient_exit_recursive(ID, exit_date, root->Right);
	}
	else if (root->data->get_recordID() == ID) {
		return root->data->update_exit_date(exit_date);
	}
	return 0;
}
int BSTree::update_patient_exit(string ID, string exit_date_string) {
	if (is_date_string_OK(exit_date_string) == 0)
		return 1;
	Date* exit_date = new Date(exit_date_string);
	return this->update_patient_exit_recursive(ID, exit_date, this->Root);
}
BSTree::~BSTree() {
	// cout << "TREE DESTRUCTOR\n";
	this->tree_destructor_recursive((this->Root));
}