/* Copyright (C) 2018
* Course: CO2003
* Author: Rang Nguyen
* Ho Chi Minh City University of Technology
*/

#include "TreeSet.h"

TreeSet::TreeSet()
{
	root = NULL;
	count = 0;
}


TreeSet::~TreeSet()
{
	clear();
}


AVLNode *rotateLeft(AVLNode *root){
    AVLNode *tempPtr = root->right;
    root->right = tempPtr->left;
    tempPtr->left = root;
    root  = tempPtr;
    return root;
}

AVLNode *rotateRight(AVLNode *root){
    AVLNode *tempPtr = root->left;
    root->left = tempPtr->right;
    tempPtr->right = root;
    root = tempPtr;
    return root;
};

AVLNode *leftBalance(AVLNode *root, bool &taller){
    AVLNode *leftTree = root->left;
    if(leftTree->balance == 1){
        root->balance = 0;
        leftTree->balance = 0;
        root = rotateRight(root);
        taller = false;
    }
    else{
        AVLNode *rightTree = leftTree->right;
        if(rightTree->balance == 1){
            root->balance = -1;
            leftTree->balance = 0;
        }
        else if (rightTree->balance == 0){
            leftTree->balance = 0;
            root->balance = 0;
        }
        else{
            root->balance = 0;
            leftTree->balance = 1;
        }

        rightTree->balance = 0;
        root->left = rotateLeft(leftTree);
        root = rotateRight(root);
        taller = false;
    }

    return root;
}

AVLNode *rightBalance(AVLNode *root, bool &taller){
    AVLNode *rightTree = root->right;

    if(rightTree->balance == -1){
        root->balance = 0;
        rightTree->balance = 0;
        root = rotateLeft(root);
        taller = false;
    }

    else{

        AVLNode *leftTree = rightTree->left;
        if(leftTree->balance == -1){
            root->balance = 1;
            rightTree->balance = 0;
        }
        else if(leftTree->balance == 0){
            root->balance = 0;
            rightTree->balance = 0;
        }
        else{
            root->balance = 0;
            rightTree->balance = -1;
        }

        leftTree->balance = 0;
        root->right = rotateRight(rightTree);
        root = rotateLeft(root);
        taller = false;
    }
    return root;
}

AVLNode* TreeSet::AVLInsert(AVLNode *&root, int inputData, bool &taller){

    if(root == NULL){
        root = new AVLNode(inputData);
        taller = true;
    }

    else if(inputData == root->key){
        taller = false;
        duplicateData = true;
    }

    else if(inputData < root->key){
        root->left = AVLInsert(root->left, inputData, taller);
        if(taller){
            if(root->balance == 1)
            {
                root = leftBalance(root, taller);
            }
            else if (root->balance == 0){
                root->balance = 1;
            }
            else { root->balance = 0; taller = false;}
        }
    }

    else{

        root->right = AVLInsert(root->right, inputData, taller);

        if(taller){
            if(root->balance == 1){
                root->balance = 0;
                taller = false;
            }
            else if (root->balance == 0){
                root->balance = -1;
            }
            else{
                root = rightBalance(root, taller);
            }
        }
    }

    return root;
};

bool searchVal(AVLNode *root, int val){
    if(root == NULL){
    return false;}

    if(root->key < val){
        return searchVal(root->right, val);
    }
    else if(root->key > val){
        return searchVal(root->left, val);
    }
    else{return true;}

    return false;
}

AVLNode *deleteRightBalance(AVLNode *&root, int &shorter){
    if(root->balance == 1){
        root->balance = 0;
    }
    else if(root->balance == 0){
        root->balance = -1;
        shorter = 0;
    }

    else{
        AVLNode *rightTree = root->right;
        if(rightTree->balance == 1){
            AVLNode *leftTree = rightTree->left;
            if(leftTree->balance == 1){
                rightTree->balance = -1;
                root->balance = 0;
            }
            else if(leftTree->balance == 0){
                root->balance = 1;
                rightTree->balance = 0;
            }
            else{
                root->balance = 1;
                rightTree->balance = 0;
            }

            leftTree->balance = 0;
            root->right = rotateRight(rightTree);
            root = rotateLeft(root);
        }

        else{
            if(rightTree->balance != 0){
                root->balance = 0;
                rightTree->balance = 0;
            }
            else{
                root->balance = -1;
                rightTree->balance = 1;
                shorter = 0;
            }

            root = rotateLeft(root);
        }

    }
    return root;
}

AVLNode *deleteLeftBalance(AVLNode *&root, int &shorter){
    if (root->balance == -1){
        root->balance = 0;
    }

    else if (root->balance == 0){
        root->balance = 1;
        shorter = 0;
    }

    else{
        AVLNode *leftTree = root->left;
        if(leftTree->balance == -1){
            AVLNode *rightTree = leftTree->right;
            if(rightTree->balance == -1){
                leftTree->balance = 1;
                root->balance = 0;
            }
            else if(rightTree->balance == 0){
                root->balance = -1;
                leftTree->balance = 0;
            }
            else{
                root->balance = -1;
                leftTree->balance = 0;
            }

            rightTree->balance = 0;
            root->left = rotateLeft(leftTree);
            root = rotateRight(root);
        }
        else{
            if(leftTree->balance != 0){
                root->balance = 0;
                leftTree->balance = 0;
            }
            else{
                root->balance = 1;
                leftTree->balance = -1;
                shorter = 0;
            }
            root = rotateRight(root);
        }
    }
    return root;
}

AVLNode* TreeSet::AVLDelete(AVLNode *&root, int val, int &shorter, int &success){

    if(root == NULL){
        shorter = 0;
        success = 0;
        return NULL;
    }

    if(val < root->key){
        root->left = AVLDelete(root->left, val, shorter, success);
        if(shorter){
            root = deleteRightBalance(root,shorter);
        }

    }
    else if(val > root->key){
        root->right = AVLDelete(root->right, val, shorter, success);
        if(shorter){
            root = deleteLeftBalance(root,shorter);
        }
    }
    else{
        AVLNode *deleteAVLNode = root;
        if(root->right == NULL){
            AVLNode *newRoot = root->left;
            success = 1;
            shorter = 1;
            free(deleteAVLNode);
            return newRoot;
        }
        else if(root->left == NULL){
            AVLNode *newRoot = root->right;
            success = 1;
            shorter = 1;
            free(deleteAVLNode);
            return newRoot;
        }
        else{
            AVLNode *exchPtr = root->left;
            while(exchPtr->right != NULL){
                exchPtr = exchPtr->right;
            }
            root->key = exchPtr->key;
            root->left = AVLDelete(root->left,exchPtr->key,shorter,success);
            if (shorter){
                root = deleteRightBalance(root,shorter);
            }
        }
    }

    return root;
}

void TreeSet::addBST(AVLNode *&root, int addData){
    if (root == NULL){
        AVLNode *pTemp = new AVLNode(addData);
        root = pTemp;
    }

    else{
        if(addData < root->key){
            addBST(root->left, addData);
        }
        else{
            addBST(root->right, addData);
        }
    }
}



void TreeSet::clearRec(AVLNode* root) {
	if (root != NULL) {
		clearRec(root->left);
		clearRec(root->right);
		delete root;
	}
}

void TreeSet::clear() {
	clearRec(root);
	root = NULL;
	count = 0;
}

int TreeSet::add(int val) {
	// TODO
    bool taller = true;
    AVLInsert(root, val, taller);
    if(duplicateData){duplicateData = false; return 0;}
    count++;
    return 1;
}

bool TreeSet::contains(int val) {
	// TODO
	return searchVal(root,val);
}

void exportVal(AVLNode *root, int *arr, int *i){
    if(root == NULL){return;}
    if(root != NULL){
        arr[*i] = root->key;
        ++*i;
        exportVal(root->left,arr, i);
        exportVal(root->right,arr, i);
    }
}

void TreeSet::copy(const TreeSet& set) {
	// TODO
	TreeSet::clear();
	int *arrCopy = new int[set.count];
    int i = 0;
    exportVal(set.root, arrCopy, &i);
    for(int i = 0 ; i < set.count; i++){
        TreeSet::addBST(root,arrCopy[i]);}
}

int TreeSet::first() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	AVLNode* pTempFindFirst;
	pTempFindFirst = root;
    while(pTempFindFirst->left != NULL){
        pTempFindFirst = pTempFindFirst->left;
    }
    return pTempFindFirst->key;
	// TODO
}

int TreeSet::last() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
    AVLNode* pTempFindLast;
	pTempFindLast = root;
    while(pTempFindLast->right != NULL){
        pTempFindLast = pTempFindLast->right;
    }

    return pTempFindLast->key;
	// TODO
}

AVLNode *searchHigher(AVLNode *root, int val){

    static AVLNode *pTempHigher = NULL;
    if(root == NULL){
        return NULL;
    }

    if(root->key > val){
        pTempHigher = root;
        AVLNode *returnSearch = pTempHigher;
        AVLNode *checkSearchHigher;
        checkSearchHigher = searchHigher(root->left, val);
        if(checkSearchHigher == NULL){pTempHigher = NULL; return returnSearch;}
    }

    else if(root->key < val){
        AVLNode *checkSearchHigher;
        AVLNode *returnSearch = pTempHigher;
        checkSearchHigher = searchHigher(root->right, val);
        if(checkSearchHigher == NULL){pTempHigher = NULL; return returnSearch;}
    }

    else if(root->key == val){
        if(root->right != NULL){
            root = root->right;
            while(root->left != NULL)
                root = root->left;
            return root;
        }

        else {
            AVLNode *returnSearch = pTempHigher;
            pTempHigher = NULL;
            return returnSearch;}
    }
}

int TreeSet::higher(int val) {
	// TODO
	AVLNode *checkHigher = searchHigher(root,val);
	if(checkHigher == NULL){return -1;}
	else{return checkHigher->key;}
}

AVLNode *searchLower(AVLNode *root, int val){

    static AVLNode *pTempLower = NULL;
    if(root == NULL){
        return NULL;
    }

    if(root->key > val){
        AVLNode *checkSearchLower;
        AVLNode *returnSearch = pTempLower;
        checkSearchLower = searchLower(root->left, val);
        if(checkSearchLower == NULL){pTempLower = NULL; return returnSearch;}
    }

    else if(root->key < val){
        pTempLower = root;
        AVLNode *returnSearch = pTempLower;
        AVLNode *checkSearchLower;
        checkSearchLower = searchLower(root->right, val);
        if(checkSearchLower == NULL){pTempLower = NULL; return returnSearch;}
    }

    else if(root->key == val){
        if(root->left != NULL){
            root = root->left;
            while(root->right != NULL)
                root = root->right;
            return root;
        }

        else {
            AVLNode *returnSearch = pTempLower;
            pTempLower = NULL;
            return returnSearch;}
    }
}

int TreeSet::lower(int val) {
	// TODO
    AVLNode *checkLower = searchLower(root,val);
	if(checkLower == NULL){return -1;}
	else{return checkLower->key;}

}

int TreeSet::remove(int val) {
	// TODO
	int success = 1;
	int shorter = 1;
	AVLDelete(root,val,shorter,success);
	if(success == 0){return 0;}
	else {count--; return 1;};
}

int partition(int *arrQuickSort, int first, int last)
{
    int i,index,pivot;
    index = first;
    pivot = arrQuickSort[last];
    for(i = first; i < last; i++)
    {
        if(arrQuickSort[i] <= pivot)
        {
            swap(arrQuickSort[index], arrQuickSort[i]);
            index++;
        }
    }
    swap(arrQuickSort[index], arrQuickSort[last]);
    return index;
}

int quickSort(int *arrQuickSort, int first, int last)
{
    int index;
    if(first >= last)
        return 0;
    {
        index = partition(arrQuickSort,first,last);
        quickSort(arrQuickSort, first, index - 1);
        quickSort(arrQuickSort, index + 1, last);
    }
}


TreeSet* TreeSet::subSet(int fromVal, int toVal) {
	// TODO
	TreeSet *subHead;
    static TreeSet subSet;
    subSet.clear();
    subHead = &subSet;

    int *setArr = new int[count];
    int i = 0;
    exportVal(root, setArr, &i);
    quickSort(setArr,0,count - 1);

    int minElement = TreeSet::higher(fromVal);
    int maxElement = TreeSet::lower(toVal);
    int minIndex =-1; int maxIndex = -1;
    int tempMinIndex;

    for(int i = 0 ; i < count ; i++){
        if(setArr[i] == fromVal){minIndex = i;}
        if(setArr[i] == minElement){tempMinIndex =  i;}
        if(setArr[i] == maxElement){maxIndex = i;}
    }
    if(minIndex == -1){minIndex = tempMinIndex;}

    for(int i = minIndex ; i < maxIndex + 1; i++){
        subHead->add(setArr[i]);
    }

    return subHead;
}

int TreeSet::size() {
	// TODO
	return count;
}

