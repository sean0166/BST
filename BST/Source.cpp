/*NOTE: WE SHOULD HAVE ROOT AS UNIQUE_PTR LINKING TO SHARED RIGHT AND LEFT PTRS*/

#include <memory>
#include <iostream>
#include <optional>
#include <queue>
#include <stack>

using namespace std;
template<typename T>
struct BSTNode
{
	explicit BSTNode(const T& val) : value(val) {};
	BSTNode() = default; 

	T value;
	shared_ptr<BSTNode> pLeft;
	shared_ptr<BSTNode> pRight;

};

template <typename T>
class BSTTree
{
	using BstNodePtr = shared_ptr<BSTNode<T>>;
	const size_t COUNT = 10;
	BstNodePtr m_pRoot;


	void printBranch(const BstNodePtr& node, int space)
	{
		// Base case  
		if (!node)
			return;

		// Increase distance between levels  
		space += COUNT;

		// Process right child first  
		printBranch(node->pRight, space);

		// Print current node after space  
		// count  
		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		cout << node->value << "\n";

		// Process left child  
		printBranch(node->pLeft, space);
	}

	BstNodePtr findNode(T val)const
	{
		auto traverseNode = m_pRoot;
		bool bFound = false;
		while (traverseNode && !bFound)
		{
			if (traverseNode->value > val)
			{
				traverseNode = traverseNode->pLeft;
			}
			else if (traverseNode->value < val)
			{
				traverseNode = traverseNode->pRight;
			}
			else
			{
				bFound = true;
			}
		}

		return bFound ? traverseNode : nullptr; 
	}

	BstNodePtr findPriorNode(T val)const
	{
		auto traversalNode = m_pRoot;
		bool bFound = false;

		//No prior node exist for root
		if (traversalNode->value == val) return nullptr; 
		
		while (traversalNode)
		{
			if ( (traversalNode->pLeft && traversalNode->pLeft->value == val) 
			   ||(traversalNode->pRight && traversalNode->pRight->value == val) )
			{
				return traversalNode;
			}
			else if (traversalNode->value > val)
			{
				traversalNode = traversalNode->pLeft;
			}
			else if (traversalNode->value < val)
			{
				traversalNode = traversalNode->pRight;
			}

		}

		return nullptr;
	}
	
	void breadthFirstSearchRecursiveLocal(const T& val, BstNodePtr& foundNode,  std::queue<BstNodePtr>& nodeQueue)const
	{
		if (!nodeQueue.empty())
		{
			auto curNode = nodeQueue.front();
			nodeQueue.pop();

			std::cout << curNode->value << " ";

			if (curNode->value == val) foundNode = curNode;

			if (curNode->pLeft) nodeQueue.push(curNode->pLeft);

			if (curNode->pRight) nodeQueue.push(curNode->pRight);

			breadthFirstSearchRecursiveLocal(val, foundNode, nodeQueue);
		}
	}

	void inorderSearchDFSLocal(const T& val, BstNodePtr& foundNode, const BstNodePtr& root) const
	{
		if (root->pLeft)
		{
			inorderSearchDFSLocal(val, foundNode, root->pLeft);
		}

		if (root->value == val) foundNode = root;
		std::cout << root->value << " ";


		if (root->pRight)
		{
			inorderSearchDFSLocal(val, foundNode, root->pRight);
		}
	}

	void preorderSearchDFSLocal(const T& val, BstNodePtr& foundNode, const BstNodePtr& root) const
	{
		if (root->value == val) foundNode = root;
		std::cout << root->value << " ";

		if (root->pLeft)
		{
			preorderSearchDFSLocal(val, foundNode, root->pLeft);
		}

		if (root->pRight)
		{
			preorderSearchDFSLocal(val, foundNode, root->pRight);
		}
	}

	void postorderSearchDFSLocal(const T& val, BstNodePtr& foundNode, const BstNodePtr& root) const
	{

		if (root->pLeft)
		{
			postorderSearchDFSLocal(val, foundNode, root->pLeft);
		}

		if (root->pRight)
		{
			postorderSearchDFSLocal(val, foundNode, root->pRight);
		}

		if (root->value == val) foundNode = root;
		std::cout << root->value << " ";
	}
public: 
	void insert(const T& val)
	{
		auto insertNode = make_shared<BSTNode<T>>(val);
		if (!m_pRoot)
		{
			m_pRoot = insertNode;
		}
		else
		{
			auto traversalNode = m_pRoot;
			bool bAdded = false;
			while (!bAdded)
			{
				if (traversalNode->value >= insertNode->value)
				{
					if (traversalNode->pLeft)
					{
						traversalNode = traversalNode->pLeft;
					}
					else
					{
						bAdded = true;
						traversalNode->pLeft = insertNode;
					}
				}
				else
				{
					if (traversalNode->pRight)
					{
						traversalNode = traversalNode->pRight;
					}
					else
					{
						bAdded = true;
						traversalNode->pRight = insertNode;
					}

				}
			}

		}
	}

	void remove(const T& val)
	{
		auto priorNode = findPriorNode(val);
		BstNodePtr foundNode;
		bool bIsLeft = false;
		if (priorNode)
		{
			if (priorNode->pLeft && priorNode->pLeft->value == val)
			{
				foundNode = priorNode->pLeft;
			}
			else if (priorNode->pRight && priorNode->pRight->value == val)
			{
				foundNode = priorNode->pRight;
			}
			else
			{
				foundNode = m_pRoot;
			}

		}
		else
		{
			foundNode = m_pRoot; 
		}

		if (foundNode->pRight)
		{
			if (foundNode->pRight->pLeft)
			{
				auto curRightLeftNode = foundNode->pRight->pLeft;
				auto curRightLeftNodeParent = foundNode->pRight;
				bool bFoundFarLeftNode = false;
				while (!bFoundFarLeftNode)
				{
					if (curRightLeftNode->pLeft)
					{
						curRightLeftNodeParent = curRightLeftNode; 
						curRightLeftNode = curRightLeftNode->pLeft;
					}
					else
					{
						bFoundFarLeftNode = true;
					}
				}
				curRightLeftNodeParent->pLeft = curRightLeftNode->pRight;
				foundNode->value = curRightLeftNode->value;
			}
			else
			{
				foundNode->value = foundNode->pRight->value;
				foundNode->pRight = foundNode->pRight->pRight;
				
			}
		}
		else if (foundNode->pLeft)
		{
			if (m_pRoot == foundNode)
			{
				m_pRoot = foundNode->pLeft;
			}
			foundNode = foundNode->pLeft;
		}
		else
		{
			if (priorNode->pLeft == foundNode)
			{
				priorNode->pLeft = nullptr;
			}
			else
			{
				priorNode->pRight = nullptr;

			}
		}

	}

	bool exist(const T& val)const
	{
		auto searchNode = findNode(val);

		return (searchNode != nullptr);
	}

	void printRoot()
	{
		printBranch(m_pRoot, 0);
	}


	BstNodePtr breadthFirstSearch(const T& val) const
	{
		auto curNode = m_pRoot;
		BstNodePtr foundNode;
		std::queue<BstNodePtr> nodeQueue;
		auto bFound = false;

		std::cout << "Starting BFS, here is the traversal:" << std::endl;
		nodeQueue.push(curNode);

		while (!nodeQueue.empty())
		{
			curNode = nodeQueue.front();
			nodeQueue.pop();

			std::cout << curNode->value << " ";

			if (curNode->value == val) foundNode = curNode;

			if (curNode->pLeft)
			{
				nodeQueue.push(curNode->pLeft);
			}
			if (curNode->pRight)
			{
				nodeQueue.push(curNode->pRight);
			}
		}
		std::cout << std::endl;
		
		return foundNode;
	}

	BstNodePtr breadthFirstSearchRecursive(const T& val) const
	{
		BstNodePtr foundNode;
		std::queue<BstNodePtr> nodeQueue;
		nodeQueue.push(m_pRoot);

		std::cout << "Starting BFS, here is the traversal:" << std::endl;
		breadthFirstSearchRecursiveLocal(val, foundNode, nodeQueue);
		std::cout << std::endl;

		return foundNode;
	}

	BstNodePtr inorderSearchDFS(const T& val) const
	{
		BstNodePtr foundNode;
		std::cout << "Starting BFS, here is the traversal:" << std::endl;
		inorderSearchDFSLocal(val, foundNode, m_pRoot);
		std::cout << std::endl;
		return foundNode;
	}

	BstNodePtr preorderSearchDFS(const T& val) const
	{
		BstNodePtr foundNode;
		std::cout << "Starting BFS, here is the traversal:" << std::endl;
		preorderSearchDFSLocal(val, foundNode, m_pRoot);
		std::cout << std::endl;
		return foundNode;
	}	

	BstNodePtr postorderSearchDFS(const T& val) const
	{
		BstNodePtr foundNode;
		std::cout << "Starting BFS, here is the traversal:" << std::endl;
		postorderSearchDFSLocal(val, foundNode, m_pRoot);
		std::cout << std::endl;
		return foundNode;
	}

	bool isValid()
	{
		bool isValidated = true;
		std::queue<BstNodePtr> pNodeQueue;

		pNodeQueue.push(m_pRoot);

		while (!pNodeQueue.empty() && isValidated)
		{
			auto pCurNode = pNodeQueue.front();
			pNodeQueue.pop();

			if (pCurNode->pLeft)
			{
				if (pCurNode->pLeft->value > pCurNode->value) isValidated = false;
				pNodeQueue.push(pCurNode->pLeft);
			}

			if (pCurNode->pRight)
			{
				if (pCurNode->pRight->value < pCurNode->value) isValidated = false;
				pNodeQueue.push(pCurNode->pRight);
			}

		}
		
		return isValidated; 
	}
};

int main()
{
	using IntBst = BSTTree<int>;

	IntBst bst;
	auto printTree = [&bst]() {
		cout << "Here is your tree: \n";
		bst.printRoot();
	};

	bst.insert(9);
	bst.insert(4);
	bst.insert(6);
	bst.insert(20);
	bst.insert(170);
	bst.insert(15);
	bst.insert(1);
	printTree(); 
	/*bst.remove(170);
	*/
	/*auto searchVal = 2;
	auto test = bst.exist(searchVal);
	cout << (test ? "Found "s : "Didn't Find "s) << searchVal << "!\n\n"
		<< "Here is your tree: \n";
	*/
	/*printTree();
	bst.remove(15);
	bst.remove(4);
	printTree();
	bst.remove(1);
	printTree();
	*/
	auto foundNode = bst.postorderSearchDFS(15);



	std::cout << "Found Node Value: " << foundNode->value << std::endl;
	
	std::cout << "Is Valid Bst: " << bst.isValid() << std::endl;

	printTree();
	return 0;
}