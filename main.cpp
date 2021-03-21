#include <iostream>
// #include <cstring>
#include <fstream>
#include "date.h"
#include "patientRecord.h"
#include "hash_table.h"
#include "max_heap.h"
using namespace std;

int main(int argc, char const *argv[]) {
	// if (make_date("15-2-201600") == NULL)
	// 	cout << "NULL" << endl;
	// return 0;
	string patientRecordsFile = "";
	int diseaseHashTableNumOfEntries = -1, countryHashTableNumOfEntries = -1, bucketSize = -1;
	for (int i = 1; i < argc-1; i=i+2) {
		if (strcmp(argv[i],"-p") == 0)
			patientRecordsFile = argv[i+1];
		else if (strcmp(argv[i],"-h1") == 0)
			diseaseHashTableNumOfEntries = atoi(argv[i+1]);
		else if (strcmp(argv[i],"-h2") == 0)
			countryHashTableNumOfEntries = atoi(argv[i+1]);
		else if (strcmp(argv[i],"-b") == 0)
			bucketSize = atoi(argv[i+1]);
		else
			cout << "MISTAKE" << endl;
	}
	if (patientRecordsFile == "" || diseaseHashTableNumOfEntries < 1 || countryHashTableNumOfEntries < 1 || bucketSize < 1) {
		cout << "Could not find expected parameters! Terminating." << endl;
		return 1;
	}
	if ((bucketSize - 8) % 40 != 0 || bucketSize < 48) {
		cout << "Bucket size for k elements must be 8 + k * 40. Setting to default (48)." << endl;
		bucketSize = 48; // A string (32), a pointer to Date (8) and a pointer to the next bucket (8).
	}
	// if (bucketSize % 40 != 0 || bucketSize < 40) {
	// 	cout << "Bucket size for k elements must be k * 40. Setting to default (40)." << endl;
	// 	bucketSize = 40; // A string (32), a pointer to Date (8) and a pointer to the next bucket (8).
	// }
	// cout << "Arguments: " << patientRecordsFile << " - " << diseaseHashTableNumOfEntries << " - " << countryHashTableNumOfEntries << " - " << bucketSize << endl;
////////////////////////////////////////////////////////////////////////////////////////////////
	BSTree big_tree;
	HashTable disease_hash_table(diseaseHashTableNumOfEntries, bucketSize);
	HashTable country_hash_table(countryHashTableNumOfEntries, bucketSize);
	int same_id_error;
	Date* temp_entry_date1, *temp_entry_date2;
	// Date* temp_exit_date;
	string temp_diseaseID, temp_country;
	ifstream input_file;
	string line;
	patientRecord* temp;
	input_file.open(patientRecordsFile);
	int quitt = 0;
	int retourn;
	do {
		// cout << "-------------------------------------------------------\n";
		getline(input_file, line);
		// cout << line << endl;
		if (line == "") {
			// cout << "EOF\n";
			break;
		}
		temp = new patientRecord(); // This goes in the big tree and pointer is re-used for next line in the file // OK
		retourn = temp->initialize(line);
		if (retourn == 1) {
			cout << "MAIN - Found a record with an exit date preceding the entry date. It'll be discarded." << endl;
			delete temp;
			continue;
		}
		else if (retourn == 2) {
			cout << "MAIN - Found a record with improperly formatted dates. It'll be discarded." << endl;
			delete temp;
			continue;
		}
		same_id_error = big_tree.insert(temp); // BIG TREE IS SORTED BY patientID, NOT BY ENTRY DATE
		if (same_id_error) {
			cout << "MAIN - Found 2 patients with the same ID. Quitting program." << endl;
			quitt = 1;
			delete temp;
			break;
		}
		temp_entry_date1 = new Date(*temp->getEntryDate()); // Make a copy of this date for the diseases hash table
		temp_entry_date2 = new Date(*temp->getEntryDate()); // And another for the countries hash table

		temp_diseaseID = temp->getDiseaseID();
		disease_hash_table.insert(temp_diseaseID, temp_entry_date1, temp);
		temp_country = temp->getCountry();
		country_hash_table.insert(temp_country, temp_entry_date2, temp);
		// disease, entry date and pointer to patientRecord go in the disease hash table
		// country, entry date and pointer to patientRecord go in the disease hash table
	} while (line != ""); // This isn't actually used, this loop stops at the "break" a few lines above.
	// disease_hash_table.print();
	// cout << "PRINT BIG TREE" << endl;
	// big_tree.print();
	// cout << "PRINT BIG TREE END" << endl;
	// delete temp;

	string input;
	string input_tokens[8];
	while (quitt == 0) {
		// cout << "Type a command: ";
		getline(cin, input);
		char char_line[input.length() + 1];        // Char array
		for (int i = 0; i < (int)input.length(); ++i) { // Copy string to char array
			char_line[i] = input[i];
		}
		char_line[input.length()] = '\0';          // Make sure it ends with a \0
		// cout << "     You typed: " << char_line << endl;
		char* saveptr = char_line;
		for (int i = 0; i < 8; ++i) {
			if (strcmp(saveptr, "") != 0 && saveptr[0] != ' ') // Ignore any spaces ' ' in a row
				input_tokens[i] = strtok_r(saveptr, " ", &saveptr);
			else
				input_tokens[i] = ""; // Stop if there's no more arguments
		}
		if (input_tokens[0] == "/globalDiseaseStats") {
			// cout << "CASE 1" << endl;
			disease_hash_table.print_n_patients_within_CASE_1(input_tokens[1], input_tokens[2]);
		}
		else if (input_tokens[0] == "/diseaseFrequency") {
			// cout << "CASE 2" << endl;
			// Check whether a country has been typed
			if (input_tokens[1] == "" || input_tokens[2] == "" || input_tokens[3] == "") {
				cout << "Couldn't find required arguments. Try again if you want." << endl;
				continue;
			}
			disease_hash_table.print_n_patients_within_CASE_2(input_tokens[1], input_tokens[2], input_tokens[3], input_tokens[4]);
		}
		// else if (input_tokens[0] == "/topk-Diseases") {
		else if (input_tokens[0] == "/topk-Diseases") {
			// cout << "CASE 3" << endl;
			int brake = 0;
			if (input_tokens[1] == "" || input_tokens[2] == "") {
				cout << "Could not find required arguments. Aborting." << endl;
				continue;
			}
			for (int i = 0; i < (int)input_tokens[1].length(); ++i) {
				if (input_tokens[1][i] < '0' || input_tokens[1][i] > '9') {
					cout << "You didn't enter a number. Aborting." << endl;
					brake = 1;
					break;
				}
			}
			if (brake == 1)
				continue;
			int k = stoi(input_tokens[1]);
			maxHeapTree country_max_heap(&country_hash_table, input_tokens[2], 3);
			country_max_heap.build_max_heap(input_tokens[3], input_tokens[4]);
			country_max_heap.print_top_k(k);
		}
		else if (input_tokens[0] == "/topk-Countries") {
			// cout << "CASE 4" << endl;
			int brake = 0;
			if (input_tokens[1] == "" || input_tokens[2] == "") {
				cout << "Could not find required arguments. Aborting." << endl;
				continue;
			}
			for (int i = 0; i < (int)input_tokens[1].length(); ++i) {
				if (input_tokens[1][i] < '0' || input_tokens[1][i] > '9') {
					cout << "You didn't enter a number. Aborting." << endl;
					brake = 1;
					break;
				}
			}
			if (brake == 1)
				continue;
			int k = stoi(input_tokens[1]);
			maxHeapTree disease_max_heap(&disease_hash_table, input_tokens[2], 4);
			disease_max_heap.build_max_heap(input_tokens[3], input_tokens[4]);
			disease_max_heap.print_top_k(k);
		}
		else if (input_tokens[0] == "/insertPatientRecord") {
			// cout << "CASE 5" << endl;
			if (input_tokens[1] == "" || input_tokens[2] == "" || input_tokens[3] == "" || input_tokens[4] == "" || input_tokens[5] == "" || input_tokens[6] == "") {
				cout << "The patientRecord you're entering is incomplete, try again." << endl;
				continue;
			}
			temp = new patientRecord();
			retourn = temp->initialize(input_tokens[1] + " " + input_tokens[2] + " " + input_tokens[3] + " " + input_tokens[4] + " " + input_tokens[5] + " " + input_tokens[6] + " " + input_tokens[7]);
			if (retourn == 1) {
				cout << "The exit date you entered precedes the entry date. Try again if you want" << endl;
				delete temp;
				continue;
			}
			same_id_error = big_tree.insert(temp); // BIG TREE IS SORTED BY patientID, NOT BY ENTRY DATE
			if (same_id_error) {
				cout << "The patientRecord ID you typed already exists in the database. It won't be inserted." << endl;
				delete temp;
				continue;
			}
			temp_entry_date1 = new Date(*temp->getEntryDate()); // Make a copy of this date for the diseases hash table
			temp_entry_date2 = new Date(*temp->getEntryDate());
			temp_diseaseID = temp->getDiseaseID();
			disease_hash_table.insert(temp_diseaseID, temp_entry_date1, temp);
			temp_country = temp->getCountry();
			country_hash_table.insert(temp_country, temp_entry_date2, temp);
			cout << "Record added" << endl;
		}
		else if (input_tokens[0] == "/recordPatientExit") {
			// cout << "CASE 6" << endl;
			if (input_tokens[1] == "")
				cout << "You didn't type a recordID. Please try again." << endl;
			if (input_tokens[2] == "")
				cout << "Invalid date, please try again." << endl;
			int retourn = big_tree.update_patient_exit(input_tokens[1], input_tokens[2]);
			if (retourn == 1)
				cout << "Invalid date, please try again." << endl; // If the date typed is not a proper date in the proper format
			else if (retourn == 2)
				cout << "There's no record with this ID, please try again." << endl;
			else if (retourn == 3)
				cout << "The exit date you entered is before the entry date of this record. Please try again." << endl;
			// else if (retourn == 4)
			// 	cout << "This patient already has an exit date." << endl;
		}
		else if (input_tokens[0] == "/numCurrentPatients") {
			// cout << "CASE 7" << endl;
			disease_hash_table.print_n_patients_CASE_7(input_tokens[1]);
		}
		else if (input_tokens[0] == "/exit") {
			cout << "exiting" << endl;
			quitt = 1;
		}
		else
			cout << "Unrecognized command, try again." << endl;
	}

	// big_tree.print();

	input_file.close();
	return 0;
}