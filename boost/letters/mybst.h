#ifndef MYBST_H
#define MYBST_H

/**
 * C++ 语言: 二叉查找树
 *
 * @author skywang
 * @date 2013/11/07
 */
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

template <class T>
class BSTNode{
	public:
		T key;            // 关键字(键值)
		BSTNode *left;    // 左孩子
		BSTNode *right;    // 右孩子
		BSTNode *parent;// 父结点

		BSTNode(T value, BSTNode *p, BSTNode *l, BSTNode *r):
			key(value),parent(),left(l),right(r) {}
};

template <class T>
class BSTree {
	private:
		BSTNode<T> *mRoot;    // 根结点

	public:
		BSTree();
		~BSTree();

		// 前序遍历"二叉树"
		void preOrder();
		// 中序遍历"二叉树"
		void inOrder();
		// 后序遍历"二叉树"
		void postOrder();

		// (递归实现)查找"二叉树"中键值为key的节点
		BSTNode<T>* search(T key);
		// (非递归实现)查找"二叉树"中键值为key的节点
		BSTNode<T>* iterativeSearch(T key);

		// 查找最小结点：返回最小结点的键值。
		T minimum();
		// 查找最大结点：返回最大结点的键值。
		T maximum();

		// 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
		BSTNode<T>* successor(BSTNode<T> *x);
		// 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
		BSTNode<T>* predecessor(BSTNode<T> *x);

		// 将结点(key为节点键值)插入到二叉树中
		void insert(T key);

		// 删除结点(key为节点键值)
		void remove(T key);

		// 销毁二叉树
		void destroy();

		// 打印二叉树
		void print();
		void save();
		void save(vector< T >  &lt);

		int depth();

		size_t LeafSize();

	private:
		// 前序遍历"二叉树"
		void preOrder(BSTNode<T>* tree) const;
		// 中序遍历"二叉树"
		void inOrder(BSTNode<T>* tree) const;
		// 后序遍历"二叉树"
		void postOrder(BSTNode<T>* tree) const;

		// (递归实现)查找"二叉树x"中键值为key的节点
		BSTNode<T>* search(BSTNode<T>* x, T key) const;
		// (非递归实现)查找"二叉树x"中键值为key的节点
		BSTNode<T>* iterativeSearch(BSTNode<T>* x, T key) const;

		// 查找最小结点：返回tree为根结点的二叉树的最小结点。
		BSTNode<T>* minimum(BSTNode<T>* tree);
		// 查找最大结点：返回tree为根结点的二叉树的最大结点。
		BSTNode<T>* maximum(BSTNode<T>* tree);

		// 将结点(z)插入到二叉树(tree)中
		void insert(BSTNode<T>* &tree, BSTNode<T>* z);

		// 删除二叉树(tree)中的结点(z)，并返回被删除的结点
		BSTNode<T>* remove(BSTNode<T>* &tree, BSTNode<T> *z);

		// 销毁二叉树
		void destroy(BSTNode<T>* &tree);

		// 打印二叉树
		void print(BSTNode<T>* tree, T key, int direction);
		int depth(BSTNode<T>* tree);

		vector<string> l_s = {"A","B","C","D","E","F","G","H","I","J","K", "L","M", "N","O","P","Q","R","S","T","U","V","W","X",
							   "Y","Z","1","2","3","4","5"};
		ofstream outdata;
		void save(BSTNode<T>* tree, T key, int direction);
		void save(vector< T >  &lt,BSTNode<T>* tree, T key, int direction);
		size_t _LeafSize(BSTNode<T> *root);


};

/*
 * 构造函数
 */
template <class T>
BSTree<T>::BSTree():mRoot(NULL)
{
}

/*
 * 析构函数
 */
template <class T>
BSTree<T>::~BSTree()
{
	destroy();
}

/*
 * 前序遍历"二叉树"
 */
template <class T>
void BSTree<T>::preOrder(BSTNode<T>* tree) const
{
	if(tree != NULL)
	{
		//cout<< tree->key << " " ;
		for (auto  th : tree->key) cout << th << " ";
		cout << endl;
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template <class T>
void BSTree<T>::preOrder()
{
	preOrder(mRoot);
}

/*
 * 中序遍历"二叉树"
 */
template <class T>
void BSTree<T>::inOrder(BSTNode<T>* tree) const
{
	if(tree != NULL)
	{
		inOrder(tree->left);
		//cout<< tree->key << " " ;
		//cout<< tree->key << " " ;
		for (auto  th : tree->key) cout << th << " ";
		cout << endl;
		inOrder(tree->right);
	}
}

template <class T>
void BSTree<T>::inOrder()
{
	inOrder(mRoot);
}

/*
 * 后序遍历"二叉树"
 */
template <class T>
void BSTree<T>::postOrder(BSTNode<T>* tree) const
{
	if(tree != NULL)
	{
		postOrder(tree->left);
		postOrder(tree->right);
		//cout<< tree->key << " " ;
		//cout<< tree->key << " " ;
		for (auto  th : tree->key) cout << th << " ";
		cout << endl;
	}
}

template <class T>
void BSTree<T>::postOrder()
{
	postOrder(mRoot);
}

/*
 * (递归实现)查找"二叉树x"中键值为key的节点
 */
template <class T>
BSTNode<T>* BSTree<T>::search(BSTNode<T>* x, T key) const
{
	if (x==NULL || x->key==key)
		return x;

	if (key < x->key)
		return search(x->left, key);
	else
		return search(x->right, key);
}

template <class T>
BSTNode<T>* BSTree<T>::search(T key)
{
	return search(mRoot, key);
}

/*
 * (非递归实现)查找"二叉树x"中键值为key的节点
 */
template <class T>
BSTNode<T>* BSTree<T>::iterativeSearch(BSTNode<T>* x, T key) const
{
	while ((x!=NULL) && (x->key!=key))
	{
		if (key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

template <class T>
BSTNode<T>* BSTree<T>::iterativeSearch(T key)
{
	iterativeSearch(mRoot, key);
}

/*
 * 查找最小结点：返回tree为根结点的二叉树的最小结点。
 */
template <class T>
BSTNode<T>* BSTree<T>::minimum(BSTNode<T>* tree)
{
	if (tree == NULL)
		return NULL;

	while(tree->left != NULL)
		tree = tree->left;
	return tree;
}

template <class T>
T BSTree<T>::minimum()
{
	BSTNode<T> *p = minimum(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

/*
 * 查找最大结点：返回tree为根结点的二叉树的最大结点。
 */
template <class T>
BSTNode<T>* BSTree<T>::maximum(BSTNode<T>* tree)
{
	if (tree == NULL)
		return NULL;

	while(tree->right != NULL)
		tree = tree->right;
	return tree;
}

template <class T>
T BSTree<T>::maximum()
{
	BSTNode<T> *p = maximum(mRoot);
	if (p != NULL)
		return p->key;

	return (T)NULL;
}

/*
 * 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
 */
template <class T>
BSTNode<T>* BSTree<T>::successor(BSTNode<T> *x)
{
	// 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
	if (x->right != NULL)
		return minimum(x->right);

	// 如果x没有右孩子。则x有以下两种可能：
	// (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
	// (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
	BSTNode<T>* y = x->parent;
	while ((y!=NULL) && (x==y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
 * 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
 */
template <class T>
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T> *x)
{
	// 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
	if (x->left != NULL)
		return maximum(x->left);

	// 如果x没有左孩子。则x有以下两种可能：
	// (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
	// (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
	BSTNode<T>* y = x->parent;
	while ((y!=NULL) && (x==y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

/*
 * 将结点插入到二叉树中
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 插入的结点
 */
template <class T>
void BSTree<T>::insert(BSTNode<T>* &tree, BSTNode<T>* z)
{
	BSTNode<T> *y = NULL;
	BSTNode<T> *x = tree;

	// 查找z的插入位置
	while (x != NULL)
	{
		y = x;
		if (z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	z->parent = y;
	if (y==NULL)
		tree = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
}

/*
 * 将结点(key为节点键值)插入到二叉树中
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     key 插入结点的键值
 */
template <class T>
void BSTree<T>::insert(T key)
{
	BSTNode<T> *z=NULL;

	// 如果新建结点失败，则返回。
	if ((z=new BSTNode<T>(key,NULL,NULL,NULL)) == NULL)
		return ;

	insert(mRoot, z);
}

/*
 * 删除结点(z)，并返回被删除的结点
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 删除的结点
 */
template <class T>
BSTNode<T>* BSTree<T>::remove(BSTNode<T>* &tree, BSTNode<T> *z)
{
	BSTNode<T> *x=NULL;
	BSTNode<T> *y=NULL;

	if ((z->left == NULL) || (z->right == NULL) )
		y = z;
	else
		y = successor(z);

	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;

	if (x != NULL)
		x->parent = y->parent;

	if (y->parent == NULL)
		tree = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	if (y != z)
		z->key = y->key;

	return y;
}

/*
 * 删除结点(z)，并返回被删除的结点
 *
 * 参数说明：
 *     tree 二叉树的根结点
 *     z 删除的结点
 */
template <class T>
void BSTree<T>::remove(T key)
{
	BSTNode<T> *z, *node;

	if ((z = search(mRoot, key)) != NULL)
		if ( (node = remove(mRoot, z)) != NULL)
			delete node;
}

/*
 * 销毁二叉树
 */
template <class T>
void BSTree<T>::destroy(BSTNode<T>* &tree)
{
	if (tree==NULL)
		return ;

	if (tree->left != NULL)
		return destroy(tree->left);
	if (tree->right != NULL)
		return destroy(tree->right);

	delete tree;
	tree=NULL;
}

template <class T>
void BSTree<T>::destroy()
{
	destroy(mRoot);
}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
template <class T>
void BSTree<T>::print(BSTNode<T>* tree, T key, int direction)
{
	if(tree != NULL)
	{
		if(direction==0)    // tree是根节点
		{
			//cout << setw(2) << tree->key << " is root" << endl;
			for (auto  th : tree->key) cout << setw(2)<<th << " ";
			cout << setw(9) << " is root" << endl;
			cout << endl;
		}
		else
			{// tree是分支节点
			for (auto  th : tree->key) cout << setw(2)<< th << " ";
			cout << setw(12) << (direction==1?"right child" : "left child") << endl;
			cout << endl;
		}

		print(tree->left, tree->key, -1);
		print(tree->right,tree->key,  1);
	}
}

template <class T>
void BSTree<T>::print()
{
	cout << endl;
	cout << endl;
	if (mRoot != NULL)
		print(mRoot, mRoot->key, 0);
}


template <class T>
void BSTree<T>::save()
{
	outdata.open(  "file_nothread_d.csv", ios::app);
	if (mRoot != NULL)
		save(mRoot, mRoot->key, 0);
	outdata.close();
}

template <class T>
void BSTree<T>::save(BSTNode<T>* tree, T key, int direction)
{
	if(tree != NULL)
	{
		string one_line_data = "";
		for (auto  th : tree->key)
			one_line_data+=   to_string(th)  + ",";
		outdata << one_line_data <<endl;
		save(tree->left, tree->key, -1);
		save(tree->right,tree->key,  1);
	}
}

template <class T>
void BSTree<T>::save(vector<T> &lt)
{
	if (mRoot != NULL)
		save(lt, mRoot, mRoot->key, 0);
}

template <class T>
void BSTree<T>::save(vector<T> &lt,BSTNode<T>* tree, T key, int direction)
{
	if(tree != NULL)
	{
		string one_line_data = "";
		lt.push_back(tree->key);
		
		outdata << one_line_data <<endl;
		save(lt, tree->left, tree->key, -1);
		save(lt, tree->right,tree->key,  1);
	}
}

template <class T>
int BSTree<T>::depth(BSTNode<T>* tree)
{
	if (tree == NULL)
    return 0;
  else if (tree->left == NULL && tree->right == NULL)
    return 1;
  else
  {
    int leftDepth = depth(tree->left) + 1;
    int rightDepth = depth(tree->right) + 1;
    return leftDepth > rightDepth ? leftDepth : rightDepth;
  }

}


template <class T>
int BSTree<T>::depth()
{
	

	return depth(mRoot);
	
}

template <class T>
size_t BSTree<T>::LeafSize()
  {
    return _LeafSize(mRoot);
  }
 template <class T>
  size_t BSTree<T>::_LeafSize(BSTNode<T> *root)
  {
    if (root == NULL)
      return 0;
    else if (root->_left == NULL && root->_right == NULL)
      return 1;
    else
      return _LeafSize(root->left) + _LeafSize(root->right);
  }


#endif // MYBST_H
