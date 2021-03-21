#include "max_heap.h"

int two_to_the(int power) {
	int result = 2;
	for (int i = 1; i < power; ++i) {
		result = 2 * result;
	}
	// cout << "2 to the " << power << ": " << result << endl;
	return result;
}
maxHeapTree::maxHeapTree(HashTable* hash_table, string country, int command_case) {
	this->hash_table = hash_table;
	this->HeapTreeRoot = NULL;
	this->temp_tree = NULL;
	this->country = country;
	this->diseases = 0;
	this->command_case = command_case;
} // This is implemented for countries (CASE 3), but it works exactly the same for diseases (CASE 4)
int maxHeapTree::build_max_heap(string date_string_1, string date_string_2) {
	if (is_date_string_OK(date_string_1) == 1 && is_date_string_OK(date_string_2) == 1) {
		this->date1 = new Date(date_string_1);
		this->date2 = new Date(date_string_2);
	}
	else {
		cout << "You did not enter dates, or they're wrong. Continuing without dates." << endl;
		this->date1 = NULL;
		this->date2 = NULL;
	}

	DateTreeNode* date_tree_root = hash_table->fetch_date_tree_of(this->country);
	if (date_tree_root == NULL)
		cout << "Fetch returned NULL\n";
	// For every date in the tree
	// Look at every patient_ptr
	// Send his country to the heap
	// If country is already there, cases++. If it isn't, add a node with it
	this->populate_temp_tree_from_date_tree(date_tree_root);
	int level = 0;
	// this->print_tree_recursive(this->temp_tree, level);
	// cout << "Diseases inserted to temp tree: " << this->diseases << endl;

	// Temp tree: Similar to iterative deepening search, creating nodes left to right until we have the number of nodes we want
	this->heap_nodes_created = 0; // This is used as a global variable so that we create exactly the number of nodes we want in the proper manner
	// do {
	// 	this->create_empty_heap_tree(this->HeapTreeRoot, level);
	// } while (this->heap_nodes_created < this->diseases);
	// for (int depth = 1; depth <= 3; ++depth) {
	int depth = 1;
	do {
		this->create_empty_heap_tree(this->HeapTreeRoot, level, depth);
		depth++;
	} while (this->heap_nodes_created < this->diseases);
	// cout << "print heap before heapify:\n";
	// this->print_tree_recursive(this->HeapTreeRoot, level);
	// if (this->HeapTreeRoot->Left == NULL)
	// 	cout << "==============\n";
	/* Insert to temp tree (string disease and int cases):
	if (this node is empty) insert here
	if (left kid is empty) insert to left kid
	if (left kid is NULL) we've reached the bottom, do nothing. A previous recursion will continue.
	if (left kid is populated and right kid is empty) insert to right kid
	*/
	return 0;
}
void maxHeapTree::heapify(maxHeapTreeNode*& root) {
	// cout << "Heapify" << endl;
	int swaps_done;
	do {
		// cout << "Iteration" << endl;
		swaps_done = 0;
		this->heapify_recursive(root, swaps_done);
		// cout << "end of iteration: swaps_done " << swaps_done << endl;
	} while (swaps_done > 0);
}
void maxHeapTree::heapify_recursive(maxHeapTreeNode*& root, int& swaps_done) {
	// cout << "heapify_recursive\n";
	if (root == NULL) {
		// cout << "root is NULL\n";
		return;
	}
	if (root->Left != NULL) {
		if (root->Left->cases > root->cases) {
			string swap_disease = root->disease;
			int swap_cases = root->cases;
			root->disease = root->Left->disease;
			root->cases = root->Left->cases;
			root->Left->disease = swap_disease;
			root->Left->cases = swap_cases;
			swaps_done++;
		}
		this->heapify_recursive(root->Left, swaps_done);
	}
	if (root->Right != NULL) {
		if (root->Right->cases > root->cases) {
			string swap_disease = root->disease;
			int swap_cases = root->cases;
			root->disease = root->Right->disease;
			root->cases = root->Right->cases;
			root->Right->disease = swap_disease;
			root->Right->cases = swap_cases;
			swaps_done++;
		}
		this->heapify_recursive(root->Right, swaps_done);
	}
}
void maxHeapTree::print_top_k(int k) {
	string disease;
	int cases;
	if (k > this->diseases)
		k = this->diseases;
	// cout << "Top " << k << " diseases in " << this->country << ":" << endl;
	for (int i = 0; i < k; ++i) {
		this->heapify(this->HeapTreeRoot);
		this->extract_root(this->HeapTreeRoot, disease, cases);
		// this->remove_from_heap(this->HeapTreeRoot, disease, cases);
		// cout << disease << ": " << cases << " cases---------------------------------------" << endl;
		cout << disease << " " << cases << endl;
	}
}
// void maxHeapTree::remove_from_heap(maxHeapTreeNode*& root, string& disease_return, int& cases_return) {
// 	disease_return = root->disease;
// 	cases_return = root->cases;
// 	root->disease = this->last_inserted->disease;
// 	root->cases = this->last_inserted->cases;
// 	if (root->parent == NULL) {
// 		delete root;
// 		return;
// 	}
// 	maxHeapTreeNode* temp_node = root;
// 	// maxHeapTreeNode* temp_node_parent = root->parent;
// 	while (temp_node->parent->Left == temp_node) {
// 		temp_node = temp_node->parent;
// 	}
// 	temp_node = temp_node->parent;
// 	while (temp_node->Right != NULL)
// 		temp_node = temp_node->Right;
// 	this->last_inserted = temp_node;
// }
int maxHeapTree::extract_root(maxHeapTreeNode*& root, string& disease_replace, int& cases_replace) {
	// Pass this node's values as reference, and the next recursion call will replace them with the max node below
	// If this node has no kids, delete it
	// If it has one kid, recursion
	// If it has 2 kids, recursion to the max kid
	if (root == NULL) {
		cout << "extract root - MISTAKE - root is NULL" << endl;
		return 0;
	}
	disease_replace = root->disease;
	cases_replace = root->cases;
	if (root->Left == NULL && root->Right == NULL) {
		delete root;
		root = NULL;
		return 1;
	}
	else if (root->Left == NULL && root->Right != NULL) {
		if (extract_root(root->Right, root->disease, root->cases) == 1)
			root->Right = NULL;
	}
	else if (root->Right == NULL && root->Left != NULL) {
		if (extract_root(root->Left, root->disease, root->cases) == 1)
			root->Left = NULL;
	}
	else if (root->Left->cases > root->Right->cases) {
		if (extract_root(root->Left, root->disease, root->cases) == 1)
			root->Left = NULL;
	}
	else if (root->Left->cases <= root->Right->cases) {
		if (extract_root(root->Right, root->disease, root->cases) == 1)
			root->Right = NULL;
	}
	else
		cout << "MISTAKE IN EXTRACT ROOT" << endl;
	return 0;
}
void maxHeapTree::create_empty_heap_tree(maxHeapTreeNode*& root, int& level, int& depth) {
	// cout << "nodes created: " << this->heap_nodes_created << endl;
	// getchar();
	level++;
	if (root == NULL && this->heap_nodes_created < this->diseases && depth == level) {
		// cout << "1\n";
		root = new maxHeapTreeNode;
		this->last_inserted = root; // Save last node created

		root->disease = "BLANK"; // DON'T CREATE EMPTY NODE, TAKE A LEAF FROM THE TEMP TREE -- kid->Left==NULL, kid->right==NULL => get kid. Left to right, order doesn't matter
		root->cases = 0;
		if (this->extract_root(this->temp_tree, root->disease, root->cases) == 1) {
			this->temp_tree = NULL; // possible mistake
			// delete 
		}
		// this->extract_root(this->temp_tree, root->disease, root->cases);

		root->Left = NULL;
		root->Right = NULL;
		this->heap_nodes_created++;
		level--;
		return;
	}
	if (this->heap_nodes_created < this->diseases)
		this->create_empty_heap_tree(root->Left, level, depth);
	if (this->heap_nodes_created < this->diseases)
		this->create_empty_heap_tree(root->Right, level, depth);
	level--;
}
void maxHeapTree::populate_temp_tree_from_date_tree(DateTreeNode*& date_tree_node) { //
	// cout << "populate" << endl;
	if (date_tree_node == NULL) {
		cout << "NULL" << endl;
		return;
	}
	if (date_tree_node->Left != NULL)
		populate_temp_tree_from_date_tree(date_tree_node->Left);

	struct date_tree_list_node* current_list_node = date_tree_node->date_and_patient_ptr_list;
	while (current_list_node != NULL) {
		// send patient_ptr->disease to temp_tree
		if ((this->date1 == NULL && this->date2 == NULL) || (date1 != NULL && current_list_node->patient_ptr_to_big_tree->hospitalized_between_dates(this->date1, this->date2) == 1)) {
			if (this->command_case == 3)
				this->temp_tree_insert(this->temp_tree, current_list_node->patient_ptr_to_big_tree->getDiseaseID());
			else
				this->temp_tree_insert(this->temp_tree, current_list_node->patient_ptr_to_big_tree->getCountry());
		}
		current_list_node = current_list_node->Next;
	}

	if (date_tree_node->Right != NULL)
		populate_temp_tree_from_date_tree(date_tree_node->Right);
}
int maxHeapTree::temp_tree_insert(maxHeapTreeNode*& root, string disease_to_insert) { // This is called for every case in the country
	if (root == NULL) {
		root = new maxHeapTreeNode;
		root->disease = disease_to_insert;
		root->cases = 1;
		root->Left = NULL;
		root->Right = NULL;
		this->diseases++;
	}
	else if (root->disease == disease_to_insert) // If disease exists already, cases++
		root->cases++;
	else if (root->disease > disease_to_insert)
		this->temp_tree_insert(root->Left, disease_to_insert);
	else if (root->disease < disease_to_insert)
		this->temp_tree_insert(root->Right, disease_to_insert);
	else
		cout << "MISTAKE IN TEMP TREE INSERT\n";
	return 0;
}
void maxHeapTree::print_tree_recursive(maxHeapTreeNode* root, int& level) {
	level++;
	if (root == NULL) {
		cout << "temp tree print: root == NULL\n";
		return;
	}
	if (root->Left != NULL)
		this->print_tree_recursive(root->Left, level);
	cout << "Level: " << level << " --- " << root->disease << " --- " << root->cases << endl;
	if (root->Right != NULL)
		this->print_tree_recursive(root->Right, level);
	level--;
}
void maxHeapTree::delete_temp_tree(maxHeapTreeNode* root) {
	if (root == NULL)
		return;
	if (root->Left != NULL)
		delete_temp_tree(root->Left);
	if (root->Right != NULL)
		delete_temp_tree(root->Right);
	delete root;
}
maxHeapTree::~maxHeapTree() {
	if (this->date1 != NULL) {
		delete this->date1;
		delete this->date2;
	}
	this->delete_temp_tree(this->temp_tree);
	this->delete_temp_tree(this->HeapTreeRoot);
}