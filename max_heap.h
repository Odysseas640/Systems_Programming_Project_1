#ifndef __ODYS_MAX_HEAP__
#define __ODYS_MAX_HEAP__
#include <iostream>
#include <string>
#include "date.h"
#include "date_tree.h"
#include "hash_table.h"
#include "patientRecord.h"
using namespace std;

typedef struct max_heap_tree_node maxHeapTreeNode;
struct max_heap_tree_node {
	string disease;
	int cases;
	maxHeapTreeNode* Left;
	maxHeapTreeNode* Right;
	// maxHeapTreeNode* parent;
};

class maxHeapTree {
private:
	HashTable* hash_table;
	maxHeapTreeNode* HeapTreeRoot;
	maxHeapTreeNode* temp_tree;
	maxHeapTreeNode* last_inserted;
	string country;
	Date *date1, *date2;
	int diseases, heap_nodes_created, command_case;
	int temp_tree_insert(maxHeapTreeNode*& root, string new_disease);
	void populate_temp_tree_from_date_tree(DateTreeNode*& date_tree_node);
	void print_tree_recursive(maxHeapTreeNode* root, int& level);
	void delete_temp_tree(maxHeapTreeNode* root);
	void create_empty_heap_tree(maxHeapTreeNode*& root, int& level, int& depth);
	int extract_root(maxHeapTreeNode*& root, string& disease_replace, int& cases_replace);
	void heapify(maxHeapTreeNode*& root);
	void heapify_recursive(maxHeapTreeNode*& root, int& swaps_done);
	// void remove_from_heap(maxHeapTreeNode*& root, string& disease_replace, int& cases_replace);
public:
	maxHeapTree(HashTable* country_hash_table, string country, int command_case);
	int build_max_heap(string date_string_1, string date_string_2);
	void print_top_k(int k);
	~maxHeapTree();
};

#endif