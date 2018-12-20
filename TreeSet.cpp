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
void rotateLeft(AVLNode *&root){
    AVLNode *tempPtr = root;
    root = root->right;
    tempPtr->right = root->left;
    root->left = tempPtr;
}

void rotateRight(AVLNode *&root){
    AVLNode *tempPtr = root;
    root = root->left;
    tempPtr->left = root->right;
    root->right = tempPtr;
};

bool leftBalance(AVLNode *&root){
    AVLNode *leftTree = root->left;
    if(leftTree->balance == 1){
        rotateRight(root);
        root->balance = 0;
        root->right->balance = 0;
        return 0;
    }

    if(leftTree->balance == -1){
    	rotateLeft(root->left);
    	rotateRight(root);
    	if(root->balance == 1){
    		root->balance = 0;
    		root->left->balance = 0;
    		root->right->balance = -1;
    		return 0;
    	}
    	if(root->balance == 0){
    		root->balance = 0;
    		root->left->balance = 0;
    		root->right->balance = 0;
    		return 0;
    	}
    	if(root->balance == -1){
    		root->balance = 0;
    		root->left->balance = 1;
    		root->right->balance = 0;
    		return 0;
    	}
    }

    if(leftTree->balance == 0){
    	rotateRight(root);
    	root->balance = -1;
    	root->right->balance = 1;
    	return 1;
    }
}


bool rightBalance(AVLNode *&root){
    AVLNode *rightTree = root->right;
    if(rightTree->balance == -1){
        rotateLeft(root);
        root->balance = 0;
        root->left->balance = 0;
        return 0;
    }

    if(rightTree->balance == 1){
    	rotateRight(root->right);
    	rotateLeft(root);
    	if(root->balance == 1){
    		root->balance = 0;
    		root->left->balance = 0;
    		root->right->balance = -1;
    		return 0;
    	}
    	if(root->balance == 0){
    		root->balance = 0;
    		root->left->balance = 0;
    		root->right->balance = 0;
    		return 0;
    	}
    	if(root->balance == -1){
    		root->balance = 0;
    		root->left->balance = 1;
    		root->right->balance = 0;
    		return 0;
    	}
    }

    if(rightTree->balance == 0){
    	rotateLeft(root);
    	root->balance = 1;
    	root->left->balance = -1;
    	return 1;
    }

}


bool TreeSet::AVLInsert(AVLNode *&root, int inputData, bool &taller){

    if(root == NULL){
        root = new AVLNode(inputData);
        taller = true;
        return 1;
    }

    if(inputData == root->key){
        taller = false;
        duplicateData = true;
        return 0;
    }

    else if(inputData < root->key){
        int addData = AVLInsert(root->left, inputData, taller);
        if(!taller){return addData;}
        if(taller){
            if (root->balance == 0){
                root->balance = 1;
                return 1;
            }
            if(root->balance == -1)
            {
            	root->balance = 0; taller = false;
            	return 1;
            }

            taller = leftBalance(root);
            return 1;
        }
    }

    else{
        int addData = AVLInsert(root->right, inputData, taller);

        if(!taller){return addData;}
        if(taller){
            if(root->balance == 0){
                root->balance = -1;
                return 1;
            }
            if (root->balance == 1){
                root->balance = 0; taller = false;
                return 1;
            }

            taller = rightBalance(root);
            return 1;
        }
    }
};

bool TreeSet::AVLDelete(AVLNode *&root, int val, int &shorter){

    if(root == NULL){
        shorter = 0;
        return NULL;
    }

    if(val < root->key){
        int removeData = AVLDelete(root->left, val, shorter);
        if(!shorter){shorter = false; return removeData;}
        if(shorter){
            if(root->balance == 0){ root->balance = -1; shorter = false; return 1;}
            if(root->balance == 1){root->balance = 0; shorter = true; return 1;}
        	shorter = (!rightBalance(root));
        	return 1;
        }

    }
    if(val > root->key){
        int removeData = AVLDelete(root->right, val, shorter);
        if(!shorter){shorter = false; return removeData;}
        if(shorter){
            if(root->balance == 0){ root->balance = 1; shorter = false; return 1;}
            if(root->balance == -1){root->balance = 0; shorter = true; return 1;}
        	shorter = (!leftBalance(root));
        	return 1;
        }
    }

    else{
        if(root->left == NULL){
        	if(root->right == NULL){
        		free(root);
        		root = NULL;
        		shorter = 1;
        		return 1;
        	}
        	AVLNode *deleteAVLNode = root;
            root = root->right;
            free(deleteAVLNode);
            shorter = 1;
            return 1;
        }

        if(root->right == NULL){
        	AVLNode *deleteAVLNode = root;
            root = root->left;
            free(deleteAVLNode);
            shorter = 1;
            return 1;
        }
        else{
            AVLNode *exchPtr = root->left;
            while(exchPtr->right != NULL){
                exchPtr = exchPtr->right;
            }
            root->key = exchPtr->key;
            AVLDelete(root->left,exchPtr->key,shorter);
            if(!shorter){return 1;}
            if (shorter){
            	if(root->balance == 0){ root->balance = 1; shorter = false; return 1;}
                if(root->balance == -1){root->balance = 0; shorter = true; return 1;}
                shorter = (!leftBalance(root));
                return 1;
            }
        }
    }
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
	int shorter = 1;
	bool success;
	success = AVLDelete(root,val,shorter);
	if(success == 0){return 0;}
	else {count--; return 1;};
}

int TreeSet::higherEqual(int val) {
    if(root==NULL)
        return -1;

    AVLNode *pTemp = root;
    AVLNode *result = NULL;

    while(pTemp){
        if(pTemp->key > val){
            result = pTemp;
            pTemp = pTemp->left;
        }
        else if(pTemp->key == val){
            return val;
        }
        else
            pTemp = pTemp->right;
    }

    if(result != NULL)
        return result->key;
    return -1;
}

TreeSet* TreeSet::subSet(int fromVal, int toVal) {
	// TODO
	TreeSet *subTree = new TreeSet();
	if(root == NULL){
        return subTree;
	}

    if(fromVal > last() || toVal < first()){
        return subTree;
    }

    int newFromVal;

    while(true){
        if((newFromVal = higherEqual(fromVal)) >= toVal)
            break;
        else{
            if(newFromVal != -1){
                fromVal = newFromVal + 1;
                subTree->add(newFromVal);
            }

            else
                return subTree;
        }
    }

    return subTree;
}
int TreeSet::size() {
	// TODO
	return count;
}
