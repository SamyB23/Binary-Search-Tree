/***************************************************************
<Samy Batha>
<BinaryTreeSymbolTable>
<Program 3>
<Creates a symbol table that performs various operations with the structure of a Binary Search Tree. Contains functions such as put() remove() and get() with ordered notations that vary between O(logn) and O(n)>
***************************************************************/
#ifndef TREE_SYMBOL_TABLE_H
#define TREE_SYMBOL_TABLE_H

#include <algorithm>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BinaryTreeSymbolTable : public SymbolTable<Key, Value>
{
protected:

	struct BinaryTreeNode
	{
		Key _key;
		Value _value;
		BinaryTreeNode* _left;
		BinaryTreeNode* _right;
		unsigned _size;

		BinaryTreeNode(const Key& key = Key{},
			const Value& value = Value{},
			unsigned size = 0,
			BinaryTreeNode* ell = nullptr,
			BinaryTreeNode* r = nullptr)
			: _key{ key }, _value{ value }, _size{ size }, _left{ ell }, _right{ r } {}

		BinaryTreeNode(const BinaryTreeNode& that)
			: _key{ that._key }, _value{ that._value }, _size{ that._size }, _left{ that._left }, _right{ that._right } {}

		~BinaryTreeNode()
		{
			if (_left != nullptr)
			{
				delete _left;
				_left = nullptr;
			}
			if (_right != nullptr)
			{
				delete _right;
				_right = nullptr;
			}
			_size = 0;
		}
	};

	// Key value comparison (less than)
	bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

	// Equality of key values
	bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

	// Equality of key values
	bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}

	// The container of the <key, value> pairs
	BinaryTreeNode* _root;

public:

	BinaryTreeSymbolTable() : _root{ nullptr } {}

	virtual ~BinaryTreeSymbolTable() { delete _root; }

	// Puts key-value pair into the table
	virtual void put(const Key& key, const Value& val = Value{})
	{
		_root = putHelper(_root, key, val);
	}

	// acquire the value paired with key
	virtual bool get(const Key& key, Value& val = Value{}) const
	{
		if (empty())
			return false;

		//Node created as a basis
		BinaryTreeNode* node = getHelper(_root, key);

		if (node == nullptr) return false;

		//If the key found at the node during the iteration is equal to key set the value of that node equal to val
		if (node->_key == key)
		{
			val = node->_value;
			return true;
		}

		else return false;
	}

	// remove key (and its value) from table
	virtual void remove(const Key& key)
	{
		//Sets _root to the node reutrned from remove helper because it is removing through replacement rather than deletion
		_root = removeHelper(_root, key);
	}

	// Is there a value paired with key?
	virtual bool contains(const Key& key) const
	{
		if (empty())
			return false;

		//Checks if the key exists in the tree
		if (getHelper(_root, key)->_key == key)
			return true;

		else return false;
	}

	// Is the table empty?
	virtual bool empty() const
	{
		if (_root == nullptr)
			return true;

		return false;
	}

	// Number of key-value pairs.
	virtual int size() const
	{
		return size(_root);
	}

	// Smallest key
	virtual bool min(Key& key = Key{}) const
	{
		if (empty())
			return false;
		
		key = minHelper(_root)->_key;
		return true;
	}

	// Largest key
	virtual bool max(Key& key = Key{}) const
	{
		if (empty())
			return false;

		key = maxHelper(_root)->_key;
		return true;
	}

	// Largest key less than or equal to key
	virtual bool floor(const Key& key, Key& floorKey) const
	{
		if (empty())
			return false;

		BinaryTreeNode* node = floorHelper(_root, key);

		if (node == nullptr) return false;
		
		floorKey = node->_key;

		return true;

	}

	// Smallest key greater than or equal to key
	virtual bool ceiling(const Key& key, Key& ceilingKey) const
	{
		if (empty())
			return false;

		BinaryTreeNode* node = ceilingHelper(_root, key);

		if (node == nullptr) return false;

		ceilingKey = node->_key;

		return true;
	}

	// Number of keys less than key
	virtual int rank(const Key& key) const
	{
		if (empty())
			return 0;

		return rankHelper(_root, key);
	}

	// key of rank k
	virtual bool select(int k = 0, Key& key = Key{}) const
	{
		if (empty())
			return false;

		key = selectHelper(_root, k)->_key;

		return true;
	}

	// Delete the smallest key
	virtual bool deleteMin()
	{
		if (empty())
			return false;

		if (_root == nullptr)
			return false;

		//Uses replacement to delete
		_root = deleteMinHelper(_root);

		return true;
	}

	// Delete the largest key
	virtual bool deleteMax()
	{
		if (empty())
			return false;

		if (_root == nullptr)
			return false;

		//Uses replacement to delete
		_root = deleteMaxHelper(_root);

		return true;
	}

	// number of keys in [low, high] (including low, high)
	virtual int size(const Key& low, const Key& high) const
	{
		if (empty())
			return 0;

		//Tree could be bounded by high but not contain, so need to check for that condition
		if (contains(high))
			return rank(high) - rank(low) + 1;
		else
			return rank(high) - rank(low);
	}

	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{
		return keysHelper(_root, low, high);
	}

	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
		Key minimum, maximum;
		min(minimum);
		max(maximum);
		return keys(minimum, maximum);
	}

	// Removes all elements from the table
	virtual void clear()
	{
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

private:

	BinaryTreeNode* putHelper(BinaryTreeNode* node, const Key& key, const Value& value)
	{
		//Null check
		if (node == nullptr) return new BinaryTreeNode(key, value, 1);

		//If the key is less than the root, traverse the left subtree
		else if (keyLessThan(key, node->_key)) node->_left = putHelper(node->_left, key, value);

		//If the key is greater than the root, traverse the right subtree
		else if (keyLessThan(node->_key, key)) node->_right = putHelper(node->_right, key, value);

		//Either duplicate value found or the value is equal to the root
		else node->_value = value; // Duplicate: overwrite the value

		//Size update for every node put in
		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

	BinaryTreeNode* getHelper(BinaryTreeNode* node, const Key& key) const
	{
		if (empty())
			return nullptr;

		if (node == nullptr)
			return nullptr;

		//If key at that node is equal to key, found the right node, return it
		if (node->_key == key)
			return node;

		//If the left child is not null and the key is less than key at node, continue to traverse left tree and key is found
		if(node->_left != nullptr)
		if (node->_key > key) return getHelper(node->_left, key);

		//If the right child is not null and the key is greater than key at node, continue to traverse left tree and key is found
		if(node->_right != nullptr)
		if (node->_key < key) return getHelper(node->_right, key);
	}

	BinaryTreeNode* removeHelper(BinaryTreeNode* node, Key key)
	{
		if (node == nullptr)
			return nullptr;

		//If key is less than root node traverse left subtree
		if (keyLessThan(key, node->_key)) node->_left = removeHelper(node->_left, key);

		//Else traverse right subtree
		else if (keyLessThan(node->_key, key)) node->_right = removeHelper(node->_right, key);

		//Traverse until _root is equal to node that needs to be deleted, than go through each extraneous case
		else
		{
			//Both if statements handle the case where there is only one child or no children
			//The node that isn't null is that node that gets returned to replace the deleted node, also known as _root in this case
			if (node->_left == nullptr) {
				BinaryTreeNode* temp = node->_right;
				return temp;
			}
			else if (node->_right == nullptr) {
				BinaryTreeNode* temp = node->_left;
				return temp;
			}

			//If there is a node with 2 children get the smallest value from the right subtree, the inorder successor
			BinaryTreeNode* temp = minHelper(node->_right);

			//Store the value of the key at the node to be deleted to the temp node
			node->_key = temp->_key;

			//Remove the inorder successor at this point as it is being reassigned
			node->_right = removeHelper(node->_right, node->_key);
		}

		//Update the size every time the tree is being updated
		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

	BinaryTreeNode* minHelper(BinaryTreeNode* node) const
	{
		if (node == nullptr)
			return nullptr;

		BinaryTreeNode* current = node;

		//Keep traversing the left child of the left subtree until you reach the node with no lesser value
		while (current->_left != nullptr)
			current = current->_left;

		//Return the leaf of the left subtree
		return current;
	}

	BinaryTreeNode* maxHelper(BinaryTreeNode* node) const
	{
		if (node == nullptr)
			return nullptr;

		BinaryTreeNode* current = node;

		//Keep traversing the right child of the right subtree until you reach the node with no greater value
		while (current->_right != nullptr)
			current = current->_right;

		return current;
	}

	BinaryTreeNode* floorHelper(BinaryTreeNode* node, const Key& key) const
	{
		if (node == nullptr)
			return nullptr;

		//If the key at the node is equal to the key parameter the node floors itself
		if (node->_key == key)
			return node;

		//recursively traverse the left subtree looking for the floor
		if (key < node->_key)
			return floorHelper(node->_left, key);

		//In the case where the floor exists in the right subtree
		BinaryTreeNode* right = floorHelper(node->_right, key);
		
		//Found it
		if (right != nullptr)
			return right;
		else 
			return node;
	}

	BinaryTreeNode* ceilingHelper(BinaryTreeNode* node, const Key& key) const
	{
		if (node == nullptr)
			return nullptr;

		//If the key at the node is equal to the key parameter the node ceilings itself
		if (node->_key == key)
			return node;

		//recursively traverse the left subtree looking for the floor
		if (node->_key < key)
			return ceilingHelper(node->_right, key);
		
		//In the case where the ceiling exists in the left subtree
		BinaryTreeNode* left = ceilingHelper(node->_left, key);

		//Found it
		if (left != nullptr)
			return left;
		else
			return node;

	}

	int rankHelper(BinaryTreeNode* node, const Key& key) const
	{
		//Counter to check how many values are less than the node key pair pushed in
		//Start at rank 0
		int count = 0;

		//While the node isn't pointing to null perform the following if conditions
		while (node != nullptr) {
			//If the key is less than the key at the node the rank stops increasing
			if (key < node->_key)
				node = node->_left;
			//When the key is greater and you begin going through the elements in the right subtree
			//you add up all the elements from the left subtree and the root and include it in the rank
			//You then keep incremeneting count until the next key is not greater than the key at the node
			else if (key > node->_key) {
				count += size(node->_left) + 1;
				node = node->_right;
			}
			else
				return count + size(node->_left);
		}
		//Return the rank of the node
		return count;
	}

	BinaryTreeNode* selectHelper(BinaryTreeNode* node, int k) const
	{
		if (node == nullptr)
			return nullptr;

		//Create the int variable that gets the rank of the node to the left of root
		int compare = rankHelper(node->_left, node->_key);

		//If the rank of the compare value is greater than the rank k given, the node with rank k is less than the compare value
		if (compare > k) {
			return selectHelper(node->_left, k);
		}
		//If the rank is greater than the rank of the node, traverse right until the element of rank k is found
		else if (compare < k)
			return selectHelper(node->_right, k - compare - 1);
		else
			return node;
	}

	std::vector<Key> keysHelper(BinaryTreeNode* node, Key low, Key high) const
	{
		//Decalre vector
		std::vector<Key> v;

		//Return empty vector if node is null
		if (node == nullptr)
			return{};

		//Iterate throught the values between low and high
		//Any value that is between low and high is pushed onto the stack, inclusively.
		for (int i = low; i <= high; i++) {
			if (contains(i))
				v.push_back(i);
		}
		//Return the vector with the values
		return v;
	}

	BinaryTreeNode* deleteMinHelper(BinaryTreeNode* node)
	{
		//If left is null the smallest element is the right
		if (node->_left == nullptr)
			return node->_right;

		//Traverse the left subtree until you find the node that matches the if statement above
		node->_left = deleteMinHelper(node->_left);
		
		//Update the size because deletion
		node->_size = 1 + size(node->_left) + size(node->_right);

		//In the deleteMin() function, the node being returned replaces the "_root" of the traversal, similar to removeHelper()
		return node;
	}

	//The same story as deleteMinHelper except you are searching for the max node to be deleted/replaced
	BinaryTreeNode* deleteMaxHelper(BinaryTreeNode* node)
	{
		if (node->_right == nullptr)
			return node->_left;

		node->_right = deleteMaxHelper(node->_right);

		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

	// Returns the height of the BST (for debugging).
	// @return the height of the BST (a 1-node tree has height 0)
	int height() const { return height(root); }
	int height(BinaryTreeNode* node) const
	{
		if (node == nullptr) return -1;

		return 1 + std::max(height(node->_left), height(node->_right));
	}

	// Non-recursive node deletion.
	BinaryTreeNode* Delete(BinaryTreeNode* node) const
	{
		node->_left = nullptr;
		node->_right = nullptr;
		delete node;
		node = nullptr;
		return node;
	}

	int size(const BinaryTreeNode* const node) const
	{
		return node == nullptr ? 0 : node->_size;
	}

	//
	///////////////////////////////////////////////////////////////////////////////
	// Check integrity of BST data structure.
	///////////////////////////////////////////////////////////////////////////////
	//
	bool check() const
	{
		if (!isBST())            std::cout << "Not in symmetric order" << std::endl;
		if (!isSizeConsistent()) std::cout << "Subtree counts not consistent" << std::endl;
		if (!isRankConsistent()) std::cout << "Ranks not consistent" << std::endl;

		return isBST() && isSizeConsistent() && isRankConsistent();
	}

	//
	// does this binary tree satisfy symmetric order?
	// Note: this test also ensures that data structure is a binary tree since order is strict
	//
	bool isBST() const { return isBST(_root, nullptr, nullptr); }
	// is the tree rooted at x a BST with all keys strictly between min and max
	// (if min or max is null, treat as empty constraint)
	bool isBST(BinaryTreeNode* node, Key* min, Key* max) const
	{
		if (node == nullptr) return true;

		if (min != nullptr && keyLessThanOrEquals(node->_key, *min)) return false;

		if (max != nullptr && keyLessThanOrEquals(*max, node->_key)) return false;

		return isBST(node->_left, min, &(node->_key)) && isBST(node->_right, &(node->_key), max);
	}

	// are the size fields correct?
	bool isSizeConsistent() const { return isSizeConsistent(_root); }
	bool isSizeConsistent(BinaryTreeNode* node) const
	{
		if (node == nullptr) return true;

		// The size of this need must equate to the sum of its children (plus itself)
		if (node->_size != size(node->_left) + size(node->_right) + 1) return false;

		return isSizeConsistent(node->_left) && isSizeConsistent(node->_right);
	}

	// check that ranks are consistent
	bool isRankConsistent() const
	{
		// The i th node should be rank i
		for (int i = 0; i < size(); i++)
		{
			Key key;
			select(i, key);
			if (i != rank(key)) return false;
		}

		// All keys must equate to the key acquired at its rank 
		for (Key key : keys())
		{
			Key acquired;
			select(rank(key), acquired);

			if (!keyEquals(key, acquired)) return false;
		}

		return true;
	}
};

#endif