//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Jordan Dayedes
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Binary Tree in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"
#include <chrono>
#include <thread>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    int height;
    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);

    int calculateHeight(Node* node);
    int getBalance(Node* node);

    Node* removeNode(Node* node, string bidId);

    Node* findMin(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);

};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // FixMe (1): initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */

//destructor helper function
void deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}
// Destructor
BinarySearchTree::~BinarySearchTree() {
    // calls recursive fucntion to the root deleting every node
    deleteTree(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // FixMe (2): In order root
    // call inOrder fuction and pass root
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // FixMe (3): Post order root
    // postOrder root
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // FixMe (4): Pre order root
    // preOrder root
    preOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // FIXME (5) Implement inserting a bid into the tree
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(bid);
    }
    else {
        // add Node root and bid
        cout << "Inserting bid with ID: " << bid.bidId << endl;
        addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // FIXME (6) Implement removing a bid from the tree
    // remove node root bidID
    root = removeNode(root, bidId);
}

// Remove a bid helper function
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // Check if tree is empty and return null
    if (node == nullptr) {
        return nullptr;
    }
    // If tree is not empty search for node to be removed
    if (bidId < node->bid.bidId) {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId > node->bid.bidId) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        // matching node found, remove node

        //remove node with only left child
        if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        // remove node with only right child
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        // remove if node has two children
        Node* temp = findMin(node->right);
        // current node becomes successor
        node->bid = temp->bid;
        // remove inorder successor 
        node->right = removeNode(node->right, temp->bid.bidId);
    }
    return node;

}

Node* BinarySearchTree::findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FIXME (7) Implement searching the tree for a bid
    // set current node equal to root
    Node* node = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (node != nullptr) {
        // if match found, return current bid
        if (bidId == node->bid.bidId) {
            return node->bid;
        }
    }
        // if bid is smaller than current node then traverse left
    if (node != nullptr && bidId < node->bid.bidId) {
        node = node->left;
        }
        // else larger so traverse right
    else {
        node = (node != nullptr) ? node->right : nullptr;
    }
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    int bidIdInt = stoi(bid.bidId);
    int nodeBidIdInt = stoi(node->bid.bidId);

    // Calculate the height of the tree
    int leftHeight = (node->left != nullptr) ? node->left->height : 0;
    int rightHeight = (node->right != nullptr) ? node->right->height : 0;

    // Determine balance factor
    int balanceFactor = leftHeight - rightHeight;

    // Set param for balance threshold
    const int balanceThreshold = 1;

    // If tree is left-heavy, balance to the right
    if (balanceFactor > balanceThreshold) {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }

    // If tree is right-heavy, balance to the left
    else if (balanceFactor < -balanceThreshold) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }

    // If tree is balanced and the parent node is larger, add to the left
    else if (bidIdInt < nodeBidIdInt) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }

    // If tree is balanced and the parent node is smaller, add to the right
    else if (bidIdInt > nodeBidIdInt) {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }

    // Update the height of the current node
    node->height = 1 + max(leftHeight, rightHeight);
    cout << "New height of node with Bid ID " << bid.bidId << ": " << node->height << endl;
}

void BinarySearchTree::inOrder(Node* node) {
    // FixMe (9): Pre order root
    if (node == nullptr) {
        return;
    }
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);

        //output bidID, title, amount, fund
        cout << "bid ID: " << node->bid.bidId << " | " << node->bid.title << " | " << node->bid.amount <<
            " | " << node->bid.fund << endl;
        //InOder right
        inOrder(node->right);
    }
}

void BinarySearchTree::postOrder(Node* node) {
      // FixMe (10): Pre order root
      //if node is not equal to null ptr
    if (node != nullptr) {
        //postOrder left
        postOrder(node->left);
        //postOrder right
        postOrder(node->right);
      //output bidID, title, amount, fund
        cout << "bid ID: " << node->bid.bidId << " | " << node->bid.title << " | " << node->bid.amount <<
            " | " << node->bid.fund << endl;
      }
}

void BinarySearchTree::preOrder(Node* node) {
      // FixMe (11): Pre order root
      //if node is not equal to null ptr
    if (node != nullptr) {
        //output bidID, title, amount, fund
        cout << "bid ID: " << node->bid.bidId << " | " << node->bid.title << " | " << node->bid.amount <<
            " | " << node->bid.fund << endl;
        //postOrder left
        preOrder(node->left);
        //postOrder right
        preOrder(node->right);
      }   
}

int BinarySearchTree::calculateHeight(Node* node) {
    return (node == nullptr) ? 0 : node->height;
    }
    /*if (node == nullptr) {
        return 0;
    }
    else {
        int leftHeight = calculateHeight(node->left);
        int rightHeight = calculateHeight(node->right);
        return 1 + std::max(leftHeight, rightHeight);
    }*/

int BinarySearchTree::getBalance(Node* node) {
    return (node == nullptr) ? 0 : calculateHeight(node->left) - calculateHeight(node->right);
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        //csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            //bst->PostOrder();
            //bst->PreOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
