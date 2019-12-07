//
//  q6(i).cpp
//  Data Structure
//
//  Created by Mohd Shoaib Rayeen on 19/03/18.
//  Copyright © 2018 Shoaib Rayeen. All rights reserved.
//

#include<iostream>
using namespace std;

struct bnode {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : creating structure for binary tree
     
     ------------------------------------------------------------------------------------------------
     input parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :  structure defines a node structure which contains data ( int type ) and
                    left and right ( bnode types for storing address of left and right elements )
     
     ------------------------------------------------------------------------------------------------
     */
    int data;
    bnode *left;
    bnode *right;
};

class btree {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : binary tree class
     
     ------------------------------------------------------------------------------------------------
     input parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   class defines constructor and functions which can be accessed publicly such as :-
                     1. btree() for initializing the pointer
                     2. insert() for insertion in binary tree
                     3. display() for displaying binary tree
                     4. path() for calculating path from root to every leaf node
                     5. printpath() for displaying path
                     6. newNode() for creating new node
                     7. preorder() for displaying preorder of tree
                     8. postorder() for displaying postorder of tree
                     9. inorder() for displaying inorder of tree
                     10. creating root node for binary tree
                     11. arr[] for storing path and pathlen for storing length of path for every leaf node
     ------------------------------------------------------------------------------------------------
     */
public:
    bnode *root;
    int arr[20] ,pathlen;
    btree();
    void insert();
    void display();
    void path(bnode* , int);
    void printpath(int* , int);
    bnode* newNode(int);
    void preorder(bnode*);
    void postorder(bnode*);
    void inorder(bnode*);
};
btree::btree() {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : initializing root as NULL and pathlen as zero
     
     ------------------------------------------------------------------------------------------------
     input parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   assigning NULL to root and 0 to pathlen
     
     ------------------------------------------------------------------------------------------------
     */
    root = NULL;
    pathlen = 0;
}
bnode* btree:: newNode(int value) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : creating newnode and assigning it to given value
     
     ------------------------------------------------------------------------------------------------
     input parameter : value which has to inserted into binary tree
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : new node which is created
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   creating a bnode type node and assiging its left and right as NULL and data as value
     
     ------------------------------------------------------------------------------------------------
     */
    bnode* temp=new bnode;
    temp->data=value;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
}
void btree:: insert() {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : insertion in binary tree
     
     ------------------------------------------------------------------------------------------------
     input parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   inserting nodes from root , then its right & left and so on
     
     ------------------------------------------------------------------------------------------------
     */
    root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->right->left = newNode(6);
    root->right->right = newNode(7);
    root->right->right->left = newNode(8);
    root->left->right->left = newNode(9);
}
void btree::display() {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : displaying binary tree and path
     
     ------------------------------------------------------------------------------------------------
     input parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   calling insert() , inorder() , preorder() , postorder and path() functions
     
     ------------------------------------------------------------------------------------------------
     */
    insert();
    cout << "\nInorder\t\t:\t";
    inorder(root);
    cout << "\nPostorder\t:\t";
    postorder(root);
    cout << "\nPreorder\t:\t";
    preorder(root);
    cout << "\nPossible Paths\n";
    path(root , 0);
}
void btree :: postorder(bnode* root) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : displaying tree in postorder recursively
     
     ------------------------------------------------------------------------------------------------
     input parameter : root of tree
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   calling this function recursively in following sequence :-
                     - left , right and then value of pointer
     
     ------------------------------------------------------------------------------------------------
     */
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << "\t";
}

void btree :: preorder(bnode* root) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : displaying tree in preorder recursively
     
     ------------------------------------------------------------------------------------------------
     input parameter : root of tree
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   calling this function recursively in following sequence :-
                     - value of pointer , left and right
     
     ------------------------------------------------------------------------------------------------
     */
    if (root == NULL)
        return;
    cout << root->data << "\t";
    preorder(root->left);
    preorder(root->right);
}
void btree :: inorder(bnode* root) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : displaying tree in inorder recursively
     
     ------------------------------------------------------------------------------------------------
     input parameter : root of tree
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   calling this function recursively in following sequence :-
                    - left , value of pointer and right
     
     ------------------------------------------------------------------------------------------------
     */
    if (root == NULL)
        return;
    preorder(root->left);
    cout << root->data << "\t";
    preorder(root->right);
}

void btree :: path(bnode* root ,int pathlen) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : calculating path from root to everyleaf node
     
     ------------------------------------------------------------------------------------------------
     input parameter : root of tree and pathlen
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   this function calls itself for left and right node recursively till the left
                     and right of the current node is not NULL and store the path in array .
                     When the left and right of the current node is NULL prints path for the same.
     
     ------------------------------------------------------------------------------------------------
     */
    if (root==NULL)
        return;
    arr[pathlen] = root->data;
    pathlen++;
    if (root->left==NULL && root->right==NULL) {
        printpath(arr, pathlen);
    }
    else {
        path(root->left , pathlen);
        path(root->right , pathlen);
    }
}

void btree ::printpath(int arr[], int len) {
    /*
     ------------------------------------------------------------------------------------------------
     
     objective : printing path for current node
     
     ------------------------------------------------------------------------------------------------
     input parameter :  array which stores path from root to leaf node and len which stores length of
                        the path
     
     ------------------------------------------------------------------------------------------------
     
     output parameter : none
     
     ------------------------------------------------------------------------------------------------
     
     approach    :   prints path stored in array using for loop
     
     ------------------------------------------------------------------------------------------------
     */
    cout << "\n";
    
    for (int i=0; i<len; i++) {
        cout << arr[i] << "\t";
    }
    cout << "\n";
}
