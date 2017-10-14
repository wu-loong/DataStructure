#include <iostream>
#include <queue>
#include <memory>
#include <cmath>
using namespace std;

class Tree
{
public:
	Tree(Tree& tree) = delete;
	Tree(){}
	void Insert(int data)
	{
		if (IsEmpty())root_.reset(new Node(data));
		else{root_->Insert(data);}		
		size_++;
	}
	bool IsEmpty()const
	{
		return root_ == nullptr;
	}
	void LevelOrderTraversal()const
	{
		if(root_==nullptr)return;
		queue<Node*> q;
		q.push(root_.get());
		cout << root_->data;
		while (!q.empty())
		{
			if (q.front()->left != nullptr)q.push(q.front()->left);
			if (q.front()->right != nullptr)q.push(q.front()->right);
			q.pop();
			if(!q.empty())cout <<" "<< q.front()->data;
		}
	}
	void Balance()
	{
		const auto temp = root_->Balance(size_);
		root_.release();
		root_.reset(temp);
	}
private:
	struct Node
	{
		int data;
		Node* left{};
		Node* right{};
		explicit  Node(int d) :data(d){}
		Node* ToRightList()
		{
			auto parent = new Node(1);
			const auto tmp = parent;
			parent->right = this;
			auto root=this;
			while (root->left!=nullptr){root = root->left;}
			while (parent->right != nullptr)
			{
				if (parent->right->left != nullptr)
				{
					parent->right = parent->right->LeftRotation();
				}
				else
				{
					parent = parent->right;
				}
			}
			delete tmp;
			return root;
		}
		Node* RightRotation()
		{
			if (right == nullptr)return this;
			auto temp = right;
			right = temp->left;
			temp->left = this;
			return temp;
		}
		Node* RightRotationN(int n)
		{
			if (n < 1)return this;
			const auto root = RightRotation();
			auto temp=root;
			for (int i = 1; i < n; ++i)
			{
				temp->right = temp->right->RightRotation();
				temp = temp->right;
			}
			return root;
		}
		Node* LeftRotation()
		{
			auto tmp = left;
			left = tmp->right;
			tmp->right = this;
			return tmp;
		}
		void Insert(int data)
		{
			if (data > this->data)
			{
				if (right == nullptr)right = new Node(data);
				else { right->Insert(data); }
			}
			else if (data < this->data)
			{
				if (left == nullptr)left = new Node(data);
				else	{left->Insert(data);}
			}
		}
		Node* Balance(int size)
		{
			int difference = pow(2, floor(log(size + 1) / log(2))) - 1;;
			auto root = ToRightList()->RightRotationN(size-difference);
			while (difference>1)
			{
				difference /=2;
				root = root->RightRotationN(difference);
			}
			return root;
		}
	};
	unique_ptr<Node,void(*)(Node*)> root_{nullptr,[](Node*p)
	{
		queue<Node*> q;
		q.push(p);
		while (!q.empty())
		{
			if (q.front()->left != nullptr)q.push(q.front()->left);
			if (q.front()->right != nullptr)q.push(q.front()->right);
			delete q.front();
			q.pop();
		}
	}};
	int size_{};
};
int main()
{
	int nodeCount;
	cin >> nodeCount;
	Tree tree;
	for (int i = 0; i < nodeCount; ++i)
	{
		int data;
		cin >> data;
		tree.Insert(data);
	}
	tree.Balance();
	tree.LevelOrderTraversal();
}