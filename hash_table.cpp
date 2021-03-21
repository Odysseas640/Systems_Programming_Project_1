#include "hash_table.h"

HashTable::HashTable(int array_size, int bucket_size) {
	bucket_size = (bucket_size - 8) / 40; // Convert size in bytes to number of elements
	// bucket_size = bucket_size / 40;
	// cout << "bucket size: " << bucket_size << endl;
	this->array_size = array_size;
	this->bucket_size = bucket_size;
	this->hash_table = new Bucket*[this->array_size]; // Array of pointers to Bucket
	for (int i = 0; i < this->array_size; ++i) {
		this->hash_table[i] = new Bucket;
		this->hash_table[i]->date_trees = new DateTree*[this->bucket_size];
		this->hash_table[i]->disease_or_country_list = new string[this->bucket_size];
		for (int j = 0; j < this->bucket_size; ++j) {
			this->hash_table[i]->disease_or_country_list[j] = "<NULL && 0>"; // Initialize strings in this bucket to NULL (sort of)
		}
		for (int j = 0; j < this->bucket_size; ++j) {
			this->hash_table[i]->date_trees[j] = new DateTree; // Initialize Date pointers in this bucket to NULL
		}
		this->hash_table[i]->next = NULL; // At first we have one bucket/list node. If it's full we create another.
	}
}

int get_bucket_spot(Bucket* bucket, int bucket_size, string disease_or_country) { // Positive integer means: insert new element here.
	for (int i = 0; i < bucket_size; ++i) {            // -1 means: bucket full
		// cout << "++++ get_bucket_spot function: i = " << i << ", disease_or_country = " << bucket->disease_or_country_list[i] << endl;
		if (bucket->disease_or_country_list[i] == "<NULL && 0>" || bucket->disease_or_country_list[i] == disease_or_country) {
			// cout << "RETURNING : " << i << endl;
			return i;
		}
		else if (bucket->disease_or_country_list[i] != "<NULL && 0>" && bucket->disease_or_country_list[i] != disease_or_country)
			continue;
		else // DIAGNOSTIC
			cout << "MISTAKE IN BUCKET - i=" << i << endl;
	}
	return -1;
}
void HashTable::insert(string disease_or_country, Date* entry_date, patientRecord* patient_ptr) {
	int sum_of_letters = 0;
	for (int i = 0; i < (int)disease_or_country.length(); ++i) {
		sum_of_letters += (int)disease_or_country[i]; // Add the ASCII codes of all the letters
		// cout << sum_of_letters << endl;
	}
	int array_position = sum_of_letters % this->array_size;
	Bucket* current_bucket = this->hash_table[array_position]; // We're inserting to this bucket
	
	int bucket_spot;
	while ((bucket_spot = get_bucket_spot(current_bucket, this->bucket_size, disease_or_country)) < 0) { // While current bucket is full, go to the next one in the list UNLESS the disease/country we're inserting is already here, in which case we add the date there.
		// Check if next is NULL first
		// If a bucket is full and next==NULL, create a new bucket at the end of the list.
		if (current_bucket->next == NULL) {
			current_bucket->next = new Bucket;
			current_bucket->next->date_trees = new DateTree*[this->bucket_size];
			current_bucket->next->disease_or_country_list = new string[this->bucket_size];
			for (int j = 0; j < this->bucket_size; ++j) {
				current_bucket->next->disease_or_country_list[j] = "<NULL && 0>"; // Initialize strings in this bucket to NULL (sort of)
			}
			for (int j = 0; j < this->bucket_size; ++j) {
				current_bucket->next->date_trees[j] = new DateTree; // Initialize Date pointers in this bucket to NULL
			}
			current_bucket->next->next = NULL; // At first we have one bucket/list node. If it's full we create another.
		}
		current_bucket = current_bucket->next;
	}
	// cout << "bucket spot ======= " << bucket_spot << endl;
	current_bucket->disease_or_country_list[bucket_spot] = disease_or_country;
	current_bucket->date_trees[bucket_spot]->insert(entry_date, patient_ptr);
}
int is_date_string_OK(string date_string);

void HashTable::print_n_patients_within_CASE_1(string date_string1, string date_string2) { // CASE 1
	int brake = 0;
	if (is_date_string_OK(date_string1) == 0 || is_date_string_OK(date_string2) == 0) {
		// cout << "You did not type any dates, or they're not in the proper format. Printing all time stats:" << endl;
		for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
			Bucket* current_bucket = this->hash_table[i]; // In every bucket, count tree elements
			brake = 0;
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>") { // Stop if we've just passed the last bucket
						brake = 1;
						break;
					}
					// Print cases for every virus
					// cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_1() << " cases" << endl;
					cout << current_bucket->disease_or_country_list[j] << " " << current_bucket->date_trees[j]->count_patients_CASE_1() << endl;
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL && brake == 0);
		}
	}
	else {
		Date* date1 = new Date(date_string1);
		Date* date2 = new Date(date_string2);
		if (date2->earlier_than(date1)) { // If user enters the 2 dates in the wrong order
			cout << "The start date you entered is after the end date. They will be swapped." << endl;
			Date* swap = date1;
			date1 = date2;
			date2 = swap;
		}
		for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
			Bucket* current_bucket = this->hash_table[i]; // In every bucket, count tree elements
			brake = 0;
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>") { // Stop if we've just passed the last bucket
						brake = 1;
						break;
					}
					// Print cases for every virus
					// cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_2(date1, date2, "") << " cases" << endl; // Case 2 is not by mistake
					cout << current_bucket->disease_or_country_list[j] << " " << current_bucket->date_trees[j]->count_patients_CASE_2(date1, date2, "") << endl;
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL && brake == 0);
		}
		delete date1;
		delete date2;
	}
}

// void HashTable::print_n_patients_within_CASE_2(string virusName, string arg2, string arg3, string arg4) { // CASE 2
void HashTable::print_n_patients_within_CASE_2(string virusName, string date1s, string date2s, string country) { // CASE 2
	if (date1s == "" || date2s == "" || virusName == "") {
		cout << "Could not find required arguments. Try again if you want." << endl;
		return;
	}
	if (is_date_string_OK(date1s) == 0 || is_date_string_OK(date2s) == 0) {
		cout << "Couldn't find 2 dates, aborting. Try again if you want." << endl;
		return;
	}
	Date* date1 = new Date(date1s);
	Date* date2 = new Date(date2s);
	if (date2->earlier_than(date1)) { // If user enters the 2 dates in the wrong order
		cout << "The start date you entered is after the end date. They will be swapped." << endl;
		Date* swap = date1;
		date1 = date2;
		date2 = swap;
	}

	int sum_of_letters = 0;
	for (int i = 0; i < (int)virusName.length(); ++i) {
		sum_of_letters += (int)virusName[i]; // Add the ASCII codes of all the letters
		// cout << sum_of_letters << endl;
	}
	int array_position = sum_of_letters % this->array_size;
	// Bucket* current_bucket = this->hash_table[array_position]; // We're inserting to this bucket

	if (country != "") {
		// for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
			Bucket* current_bucket = this->hash_table[array_position]; // In every bucket, count tree elements
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>") {
						delete date1;
						delete date2;
						cout << "DISEASE NOT FOUND 1" << endl;
						return;
					}
					if (current_bucket->disease_or_country_list[j] == virusName) { // Only print stuff about the virus we're interested in
						// cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_2(date2, date3, arg2) << " cases in " << arg2 << " from "; date2->print(); cout << " to "; date3->print(); cout << endl;
						cout << current_bucket->disease_or_country_list[j] << " " << current_bucket->date_trees[j]->count_patients_CASE_2(date1, date2, country) << endl;
						delete date1;
						delete date2;
						// cout << "DISEASE FOUND 1" << endl;
						return;
					}
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL);
		// }
	}
	else {
		// for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
			Bucket* current_bucket = this->hash_table[array_position]; // In every bucket, count tree elements
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>") {
						delete date1;
						delete date2;
						cout << "DISEASE NOT FOUND 2" << endl;
						return;
					}
					if (current_bucket->disease_or_country_list[j] == virusName) { // Only print stuff about the virus we're interested in
						// cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_2(date1, date2, "") << " cases" << endl;
						cout << current_bucket->disease_or_country_list[j] << " " << current_bucket->date_trees[j]->count_patients_CASE_2(date1, date2, "") << endl;
						delete date1;
						delete date2;
						// cout << "DISEASE FOUND 2" << endl;
						return;
					}
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL);
		// }
	}
}
void HashTable::print_n_patients_CASE_7(string disease_wanted) { // CASE 7
	if (disease_wanted == "") {
		for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
			Bucket* current_bucket = this->hash_table[i]; // In every bucket, count tree elements
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>")
						break;
					cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_7() << " cases" << endl;
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL);
		}
		return;
	}
	else {

		int sum_of_letters = 0;
		for (int i = 0; i < (int)disease_wanted.length(); ++i) {
			sum_of_letters += (int)disease_wanted[i]; // Add the ASCII codes of all the letters
			// cout << sum_of_letters << endl;
		}
		int array_position = sum_of_letters % this->array_size;
		Bucket* current_bucket = this->hash_table[array_position]; // We're inserting to this bucket

		// for (int i = 0; i < this->array_size; ++i) { // For every list of buckets
		// 	Bucket* current_bucket = this->hash_table[i]; // In every bucket, count tree elements
			do { // For every bucket
				for (int j = 0; j < this->bucket_size; ++j) { // For every disease-element in this bucket
					// Count elements in the tree of this bucket element
					if (current_bucket->disease_or_country_list[j] == "<NULL && 0>")
						break;
					if (current_bucket->disease_or_country_list[j] == disease_wanted) {
						cout << " == " << current_bucket->disease_or_country_list[j] << ": " << current_bucket->date_trees[j]->count_patients_CASE_7() << " cases" << endl;
						return;
					}
				}
				current_bucket = current_bucket->next;
			} while (current_bucket != NULL);
		// }
	}
	cout << "The disease you typed doesn't exist in the database" << endl;
}
void print_bucket(Bucket* bucket, int bucket_size) {
	for (int i = 0; i < bucket_size; ++i) {
		if (/*bucket->dateptrs[i] == NULL &&*/ bucket->disease_or_country_list[i] == "<NULL && 0>")
			return; // Stop printing because bucket is half empty / half full, depending on your philosophical standpoint
		else if (/*bucket->dateptrs[i] != NULL &&*/ bucket->disease_or_country_list[i] != "<NULL && 0>") {
			cout << "== Disease/country : " << bucket->disease_or_country_list[i] << " ==" << endl;
			cout << "Disease tree depth: " << bucket->date_trees[i]->depth_to_bottom() << endl;
			bucket->date_trees[i]->print(); cout << endl;
		}
		else // DIAGNOSTIC
			cout << "MISTAKE IN BUCKET - i=" << i << endl;
	}
}
void HashTable::print() {
	for (int i = 0; i < this->array_size; ++i) { // For every spot in the array
		cout << "Array spot " << i << endl;
		Bucket* current_bucket = this->hash_table[i];
		int bucket_n = 0;
		do { // Print all the buckets / list nodes in said spot
			cout << "Bucket N: " << bucket_n << endl;
			print_bucket(current_bucket, this->bucket_size);
			current_bucket = current_bucket->next;
			bucket_n++;
		} while (current_bucket != NULL);
	}
}
HashTable::~HashTable() {
	for (int i = 0; i < this->array_size; ++i) {
		// delete_buckets_recursive(this->hash_table[i]);
		Bucket* current_bucket = hash_table[i];
		// Bucket* next_to_be_deleted;
		do {
			for (int i = 0; i < this->bucket_size; ++i) {
				delete current_bucket->date_trees[i];
			}
			delete[] current_bucket->date_trees;
			delete[] current_bucket->disease_or_country_list;
			Bucket* to_delete = current_bucket;
			current_bucket = current_bucket->next;
			delete to_delete;
		} while (current_bucket != NULL); // Now all buckets have been deleted
	}
	delete[] this->hash_table;
}
DateTreeNode* HashTable::fetch_date_tree_of(string country) {
	int sum_of_letters = 0;
	for (int i = 0; i < (int)country.length(); ++i) {
		sum_of_letters += (int)country[i]; // Add the ASCII codes of all the letters
		// cout << sum_of_letters << endl;
	}
	int array_position = sum_of_letters % this->array_size;
	Bucket* current_bucket = this->hash_table[array_position]; // We're inserting to this bucket
	
	int bucket_spot;
	while ((bucket_spot = get_bucket_spot(current_bucket, this->bucket_size, country)) < 0) { // While current bucket is full, go to the next one in the list UNLESS the disease/country we're inserting is already here, in which case we add the date there.
		current_bucket = current_bucket->next;
		// cout << "Next bucket\n";
		if (current_bucket == NULL) {
			cout << "Current bucket is NULL\n";
			return NULL;
		}
	}
	// cout << "bucket spot ======= " << bucket_spot << endl;
	if (current_bucket->disease_or_country_list[bucket_spot] == country)
		return current_bucket->date_trees[bucket_spot]->getRoot();
	else// if (current_bucket->disease_or_country_list[bucket_spot] == NULL)
		return NULL;
}