// huffman.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct myStruct {
	char element;
	int frequency;
};

struct treeLeaf {
	char element;
	int frequency;
	char codeLeft;
	char codeRight;
	treeLeaf* leafLeft;
	treeLeaf* leafRight;

};

static int myComparator(const void *elem1, const void *elem2)
{
	const myStruct a = *(myStruct*)elem1;
	const myStruct b = *(myStruct*)elem2;
	if (a.frequency < b.frequency) return 1; // 1 - less (reverse for decreasing sort)
	else if (a.frequency > b.frequency) return -1;
	return 0;
}

// GLOBAL VAR
map<char, string> codes;



void GenerateCode(treeLeaf *node) // for outside call: node is root
{
	static string sequence = "";
	if (node->leafLeft)
	{
		sequence += node->codeLeft;
		GenerateCode(node->leafLeft);
	}

	if (node->leafRight)
	{
		sequence += node->codeRight;
		GenerateCode(node->leafRight);
	}

	if (!node->leafLeft && !node->leafRight)
		codes[node->element] = sequence;

	int l = (int)sequence.length();
	if (l > 1) sequence = sequence.substr(0, l - 1);
	else sequence = "";
}



int main()
{
	cout << "Enter string:";
	string strIn;
	getline(cin, strIn);
	cout << "Your string: " << strIn << "\n";

	//myStruct* arr;
	//arr = (myStruct*) malloc (sizeof(myStruct) * strIn.size());

	myStruct arr[255];

	int arrSize = 0;

	if (strIn.size() == 0) {
		cout << "No string entered.";
		return 1;
	}
	else {
		arr[0].element = strIn[0];
		arr[0].frequency = 1;
		arrSize++;
	}

	for (int i = 1; i < strIn.size(); i++) {
		bool isNew = true;
		for (int j = 0; j < arrSize; j++) {
			if (strIn[i] == arr[j].element) {
				isNew = false;
				arr[j].frequency = arr[j].frequency + 1;
				break;
			}
		}
		if (isNew) {
			arr[arrSize].element = strIn[i];
			arr[arrSize].frequency = 1;
			arrSize++;
		}
	}

	qsort(arr, arrSize, sizeof(myStruct), myComparator);

	for (int i = 0; i < arrSize; i++) {
		cout << arr[i].element << " => " << arr[i].frequency << "\n";
	}






	treeLeaf *n;
	vector<treeLeaf*> tops; // top-nodes

	for (int i = 0; i<arrSize; i++)
	{
		n = new treeLeaf;
		n->element = arr[i].element;
		n->frequency = arr[i].frequency;
		n->leafLeft = NULL;
		n->leafRight = NULL;
		tops.push_back(n);
	}

	int buffff = arrSize;

	//  Building binary tree.
	//  Combining last two nodes, replacing them by new node
	//  without invalidating sort
	//
	while (arrSize > 1)
	{
		n = new treeLeaf;
		n->frequency = tops[arrSize - 2]->frequency + tops[arrSize - 1]->frequency;
		n->leafLeft = tops[arrSize - 2];
		n->leafRight = tops[arrSize - 1];
		if (n->leafLeft->frequency < n->leafRight->frequency)
		{
			n->codeLeft = '0';
			n->codeRight = '1';
		}
		else
		{
			n->codeLeft = '1';
			n->codeRight = '0';
		}
		tops.pop_back();
		tops.pop_back();
		bool isins = false;
		std::vector<treeLeaf*>::iterator ti;
		for (ti = tops.begin(); ti != tops.end(); ++ti)
			if ((*ti)->frequency < n->frequency)
			{
				tops.insert(ti, n);
				isins = true;
				break;
			}
		if (!isins) tops.push_back(n);
		arrSize--;
	}


	//  Building codes
	//
	treeLeaf *root = tops[0];
	GenerateCode(root);


	//  Outputing ptable and codes
	//
	cout << '\n';
	for (int i = 0; i<buffff; i++){
		cout << arr[i].element << " " << arr[i].frequency << " " << codes[arr[i].element] << '\n';
	}

	cout << '\n' << "Code:" << '\n';
	for (int i = 0; i < buffff; i++) {
		cout << codes[arr[i].element];
	}
	cout << '\n';

	system("pause");
	return 0;
}