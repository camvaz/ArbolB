#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

#define MAX 4
#define MIN 2

struct btreeNode {
	int val[MAX + 1], count;
	btreeNode *link[MAX + 1];
};

btreeNode *root;

/* creating new node */
btreeNode * createNode(int val, btreeNode *child) {
	btreeNode *newNode = new btreeNode;
	newNode->val[1] = val;
	newNode->count = 1;
	newNode->link[0] = root;
	newNode->link[1] = child;
	return newNode;
}

/* Places the value in appropriate position */
void addValToNode(int val, int pos, btreeNode *node, btreeNode *child) {
	int j = node->count;
	while (j > pos) {
		node->val[j + 1] = node->val[j];
		node->link[j + 1] = node->link[j];
		j--;
	}
	node->val[j + 1] = val;
	node->link[j + 1] = child;
	node->count++;
}

/* split the node */
void splitNode(int val, int *pval, int pos, btreeNode *node,btreeNode *child, btreeNode **newNode) {
	int median, j;

	if (pos > MIN)
		median = MIN + 1;
	else
		median = MIN;

	*newNode = new btreeNode;
	j = median + 1;
	while (j <= MAX) {
		(*newNode)->val[j - median] = node->val[j];
		(*newNode)->link[j - median] = node->link[j];
		j++;
	}
	node->count = median;
	(*newNode)->count = MAX - median;

	if (pos <= MIN) {
		addValToNode(val, pos, node, child);
	}
	else {
		addValToNode(val, pos - median, *newNode, child);
	}
	*pval = node->val[node->count];
	(*newNode)->link[0] = node->link[node->count];
	node->count--;
}

/* sets the value val in the node */
int setValueInNode(int val, int *pval,btreeNode *node, btreeNode **child) {

	int pos;
	if (!node) {
		*pval = val;
		*child = NULL;
		return 1;
	}

	if (val < node->val[1]) {
		pos = 0;
	}
	else {
		for (pos = node->count;
		(val < node->val[pos] && pos > 1); pos--);
		if (val == node->val[pos]) {
			cout<<"Duplicates not allowed\n";
			return 0;
		}
	}
	if (setValueInNode(val, pval, node->link[pos], child)) {
		if (node->count < MAX) {
			addValToNode(*pval, pos, node, *child);
		}
		else {
			splitNode(*pval, pval, pos, node, *child, child);
			return 1;
		}
	}
	return 0;
}

/* insert val in B-Tree */
void insertion(int val) {
	int flag, i;
	btreeNode *child;

	flag = setValueInNode(val, &i, root, &child);
	if (flag)
		root = createNode(i, child);
}

/* copy successor for the value to be deleted */
void copySuccessor(btreeNode *myNode, int pos) {
	btreeNode *dummy;
	dummy = myNode->link[pos];

	for (; dummy->link[0] != NULL;)
		dummy = dummy->link[0];
	myNode->val[pos] = dummy->val[1];

}

/* removes the value from the given node and rearrange values */
void removeVal(btreeNode *myNode, int pos) {
	int i = pos + 1;
	while (i <= myNode->count) {
		myNode->val[i - 1] = myNode->val[i];
		myNode->link[i - 1] = myNode->link[i];
		i++;
	}
	myNode->count--;
}

/* shifts value from parent to right child */
void doRightShift(btreeNode *myNode, int pos) {
	btreeNode *x = myNode->link[pos];
	int j = x->count;

	while (j > 0) {
		x->val[j + 1] = x->val[j];
		x->link[j + 1] = x->link[j];
	}
	x->val[1] = myNode->val[pos];
	x->link[1] = x->link[0];
	x->count++;

	x = myNode->link[pos - 1];
	myNode->val[pos] = x->val[x->count];
	myNode->link[pos] = x->link[x->count];
	x->count--;
	return;
}

/* shifts value from parent to left child */
void doLeftShift(btreeNode *myNode, int pos) {
	int j = 1;
	btreeNode *x = myNode->link[pos - 1];

	x->count++;
	x->val[x->count] = myNode->val[pos];
	x->link[x->count] = myNode->link[pos]->link[0];

	x = myNode->link[pos];
	myNode->val[pos] = x->val[1];
	x->link[0] = x->link[1];
	x->count--;

	while (j <= x->count) {
		x->val[j] = x->val[j + 1];
		x->link[j] = x->link[j + 1];
		j++;
	}
	return;
}

/* merge nodes */
void mergeNodes(btreeNode *myNode, int pos) {
	int j = 1;
	btreeNode *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

	x2->count++;
	x2->val[x2->count] = myNode->val[pos];
	x2->link[x2->count] = myNode->link[0];

	while (j <= x1->count) {
		x2->count++;
		x2->val[x2->count] = x1->val[j];
		x2->link[x2->count] = x1->link[j];
		j++;
	}

	j = pos;
	while (j < myNode->count) {
		myNode->val[j] = myNode->val[j + 1];
		myNode->link[j] = myNode->link[j + 1];
		j++;
	}
	myNode->count--;
	free(x1);
}

/* adjusts the given node */
void adjustNode(btreeNode *myNode, int pos) {
	if (!pos) {
		if (myNode->link[1]->count > MIN) {
			doLeftShift(myNode, 1);
		}
		else {
			mergeNodes(myNode, 1);
		}
	}
	else {
		if (myNode->count != pos) {
			if (myNode->link[pos - 1]->count > MIN) {
				doRightShift(myNode, pos);
			}
			else {
				if (myNode->link[pos + 1]->count > MIN) {
					doLeftShift(myNode, pos + 1);
				}
				else {
					mergeNodes(myNode, pos);
				}
			}
		}
		else {
			if (myNode->link[pos - 1]->count > MIN)
				doRightShift(myNode, pos);
			else
				mergeNodes(myNode, pos);
		}
	}
}

/* delete val from the node */
int delValFromNode(int val,btreeNode *myNode) {
	int pos, flag = 0;
	if (myNode) {
		if (val < myNode->val[1]) {
			pos = 0;
			flag = 0;
		}
		else {
			for (pos = myNode->count;
			(val < myNode->val[pos] && pos > 1); pos--);
			if (val == myNode->val[pos]) {
				flag = 1;
			}
			else {
				flag = 0;
			}
		}
		if (flag) {
			if (myNode->link[pos - 1]) {
				copySuccessor(myNode, pos);
				flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
				if (flag == 0) {
					cout<<"Given data is not present in B-Tree\n";
				}
			}
			else {
				removeVal(myNode, pos);
			}
		}
		else {
			flag = delValFromNode(val, myNode->link[pos]);
		}
		if (myNode->link[pos]) {
			if (myNode->link[pos]->count < MIN)
				adjustNode(myNode, pos);
		}
	}
	return flag;
}

/* delete val from B-tree */
void deletion(int val,btreeNode *myNode) {
	btreeNode *tmp;
	if (!delValFromNode(val, myNode)) {
		cout<<"Given value is not present in B-Tree\n";
		return;
	}
	else {
		if (myNode->count == 0) {
			tmp = myNode;
			myNode = myNode->link[0];
			free(tmp);
		}
	}
	root = myNode;
	return;
}

/* search val in B-Tree */
void searching(int val, int *pos,btreeNode *myNode) {
	if (!myNode) {
		return;
	}

	if (val < myNode->val[1]) {
		*pos = 0;
	}
	else {
		for (*pos = myNode->count;
		(val < myNode->val[*pos] && *pos > 1); (*pos)--);
		if (val == myNode->val[*pos]) {
			cout << "Given data is Found\n";
			return;
		}
	}
	searching(val, pos, myNode->link[*pos]);
	return;
}

/* B-Tree Traversal */
void traversal(btreeNode *myNode) {
	int i;
	if (myNode) {
		for (i = 0; i < myNode->count; i++) {
			traversal(myNode->link[i]);
			cout<< myNode->val[i + 1]<<' ';
		}
		traversal(myNode->link[i]);
	}
}

//#include <iostream>
//#include <stdlib.h>
//
//using namespace std;
//
//const int MAX = 4 ;
//const int MIN = 2 ;
//struct btnode
//{
//	int count ;
//	int value[MAX + 1] ;
//	btnode *child[MAX + 1] ;
//} ;
//class btree
//{
//	private :
//		btnode *root ;
//	public :
//		btree( ) ;
//		void insert ( int val ) ;
//		int setval ( int val, btnode *n, int *p, btnode **c ) ;
//		static btnode * search ( int val, btnode *root, int *pos ) ;
//		static int searchnode ( int val, btnode *n, int *pos ) ;
//		void fillnode ( int val, btnode *c, btnode *n, int k ) ;
//		void split ( int val, btnode *c, btnode *n,
//				int k, int *y, btnode **newnode ) ;
//		void del ( int val ) ;
//		int delhelp ( int val, btnode *root ) ;
//		void clear ( btnode *root, int k ) ;
//		void copysucc ( btnode *root, int i ) ;
//		void restore ( btnode *root, int i ) ;
//		void rightshift ( int k ) ;
//		void leftshift ( int k ) ;
//		void merge ( int k ) ;
//		void show( ) ;
//		static void display ( btnode *root ) ;
//		static void deltree ( btnode *root ) ;
//		~btree( ) ;
//} ;
//
//btree :: btree( )
//{
//	root = NULL ;
//}
//void btree :: insert ( int val )
//{
//	int i ;
//	btnode *c, *n ;
//	int flag ;
//	flag = setval ( val, root, &i, &c ) ;
//	if ( flag )
//	{
//		n = new btnode ;
//		n -> count = 1 ;
//		n -> value[1] = i ;
//		n -> child[0] = root ;
//		n -> child[1] = c ;
//		root = n ;
//	}
//}
//int btree :: setval ( int val, btnode *n, int *p, btnode **c )
//{
//	int k ;
//	if ( n == NULL )
//	{
//		*p = val ;
//		*c = NULL ;
//		return 1 ;
//	}
//	else
//	{
//		if ( searchnode ( val, n, &k ) )
//			cout << endl << "Key value already exists." << endl ;
//		if ( setval ( val, n -> child[k], p, c ) )
//		{
//			if ( n -> count < MAX )
//			{
//				fillnode ( *p, *c, n, k ) ;
//				return 0 ;
//			}
//			else
//			{
//				split ( *p, *c, n, k, p, c ) ;
//				return 1 ;
//			}
//		}
//		return 0 ;
//	}
//}
//btnode * btree :: search ( int val, btnode *root, int *pos )
//{
//	if ( root == NULL )
//		return NULL ;
//	else
//	{
//		if ( searchnode ( val, root, pos ) )
//			return root ;
//		else
//			return search ( val, root -> child[*pos], pos ) ;
//	}
//}
//int btree :: searchnode ( int val, btnode *n, int *pos )
//{
//	if ( val < n -> value[1] )
//	{
//		*pos = 0 ;
//		return 0 ;
//	}
//	else
//	{
//		*pos = n -> count ;
//		while ( ( val < n -> value[*pos] ) && *pos > 1 )
//			( *pos )-- ;
//		if ( val == n -> value[*pos] )
//			return 1 ;
//		else
//			return 0 ;
//	}
//}
//void btree :: fillnode ( int val, btnode *c, btnode *n, int k )
//{
//	int i ;
//	for ( i = n -> count ; i > k ; i-- )
//	{
//		n -> value[i + 1] = n -> value[i] ;
//		n -> child[i + 1] = n -> child[i] ;
//	}
//	n -> value[k + 1] = val ;
//	n -> child[k + 1] = c ;
//	n -> count++ ;
//}
//void btree :: split ( int val, btnode *c, btnode *n,
//		int k, int *y, btnode **newnode )
//{
//	int i, mid ;
//
//	if ( k <= MIN )
//		mid = MIN ;
//	else
//		mid = MIN + 1 ;
//
//	*newnode = new btnode ;
//
//	for ( i = mid + 1 ; i <= MAX ; i++ )
//	{
//		( *newnode ) -> value[i - mid] = n -> value[i] ;
//		( *newnode ) -> child[i - mid] = n -> child[i] ;
//	}
//
//	( *newnode ) -> count = MAX - mid ;
//	n -> count = mid ;
//
//	if ( k <= MIN )
//		fillnode ( val, c, n, k ) ;
//	else
//		fillnode ( val, c, *newnode, k - mid ) ;
//
//	*y = n -> value[n -> count] ;
//	( *newnode ) -> child[0] = n -> child[n -> count] ;
//	n -> count-- ;
//}
//void btree :: del ( int val )
//{
//	btnode * temp ;
//
//	if ( ! delhelp ( val, root ) )
//		cout << endl << "Value " << val << " not found." ;
//	else
//	{
//		if ( root -> count == 0 )
//		{
//			temp = root ;
//			root = root -> child[0] ;
//			delete temp ;
//		}
//	}
//}
//int btree :: delhelp ( int val, btnode *root )
//{
//	int i ;
//	int flag ;
//
//	if ( root == NULL )
//		return 0 ;
//	else
//	{
//		flag = searchnode ( val, root, &i ) ;
//		if ( flag )
//		{
//			if ( root -> child[i - 1] )
//			{
//				copysucc ( root, i ) ;
//				flag = delhelp ( root -> value[i], root -> child[i] ) ;
//				if ( !flag )
//					cout << endl << "Value " << val << " not found." ;
//			}
//			else
//				clear ( root, i ) ;
//		}
//		else
//			flag = delhelp ( val, root -> child[i] ) ;
//		if ( root -> child[i] != NULL )
//		{
//			if ( root -> child[i] -> count < MIN )
//				restore ( root, i ) ;
//		}
//		return flag ;
//	}
//}
//void btree :: clear ( btnode *root, int k )
//{
//	int i ;
//	for ( i = k + 1 ; i <= root -> count ; i++ )
//	{
//		root -> value[i - 1] = root -> value[i] ;
//		root -> child[i - 1] = root -> child[i] ;
//	}
//	root -> count-- ;
//}
//void btree :: copysucc ( btnode *root, int i )
//{
//	btnode *temp = root -> child[i] ;
//
//	while ( temp -> child[0] )
//		temp = temp -> child[0] ;
//
//	root -> value[i] = temp -> value[1] ;
//}
//void btree :: restore ( btnode *root, int i )
//{
//	if ( i == 0 )
//	{
//		if ( root -> child [1] -> count > MIN )
//			leftshift ( 1 ) ;
//		else
//			merge ( 1 ) ;
//	}
//	else
//	{
//		if ( i == root -> count )
//		{
//			if ( root -> child[i - 1] -> count > MIN )
//				rightshift ( i ) ;
//			else
//				merge ( i ) ;
//		}
//		else
//		{
//			if ( root -> child[i - 1] -> count > MIN )
//				rightshift ( i ) ;
//			else
//			{
//				if ( root -> child[i + 1] -> count > MIN )
//					leftshift ( i + 1 ) ;
//				else
//					merge ( i ) ;
//			}
//		}
//	}
//}
//void btree :: rightshift ( int k )
//{
//	int i ;
//	btnode *temp ;
//
//	temp = root -> child[k] ;
//
//	for ( i = temp -> count ; i > 0 ; i-- )
//	{
//		temp -> value[i + 1] = temp -> value[i] ;
//		temp -> child[i + 1] = temp -> child[i] ;
//	}
//
//	temp -> child[1] = temp -> child[0] ;
//	temp -> count++ ;
//	temp -> value[1] = root -> value[k] ;
//	temp = root -> child[k - 1] ;
//	root -> value[k] = temp -> value[temp -> count] ;
//	root -> child[k] -> child [0] = temp -> child[temp -> count] ;
//	temp -> count-- ;
//}
//void btree :: leftshift ( int k )
//{
//	btnode *temp ;
//
//	temp = root -> child[k - 1] ;
//	temp -> count++ ;
//	temp -> value[temp -> count] = root -> value[k] ;
//	temp -> child[temp -> count] = root -> child[k] -> child[0] ;
//	temp = root -> child[k] ;
//	root -> value[k] = temp -> value[1] ;
//	temp -> child[0] = temp -> child[1] ;
//	temp -> count-- ;
//	for ( int i = 1 ; i <= temp -> count ; i++ )
//	{
//		temp -> value[i] = temp -> value[i + 1] ;
//		temp -> child[i] = temp -> child[i + 1] ;
//	}
//}
//void btree :: merge ( int k )
//{
//	btnode *temp1, *temp2 ;
//	int i;
//	temp1 = root -> child[k] ;
//	temp2 = root -> child[k - 1] ;
//	temp2 -> count++ ;
//	temp2 -> value[temp2 -> count] = root -> value[k] ;
//	temp2 -> child[temp2 -> count] = root -> child[0] ;
//	for ( int i = 1 ; i <= temp1 -> count ; i++ )
//	{
//		temp2 -> count++ ;
//		temp2 -> value[temp2 -> count] = temp1 -> value[i] ;
//		temp2 -> child[temp2 -> count] = temp1 -> child[i] ;
//	}
//	for ( i = k ; i < root -> count ; i++ )
//	{
//		root -> value[i] = root -> value[i + 1] ;
//		root -> child[i] = root -> child[i + 1] ;
//	}
//	root -> count-- ;
//	delete temp1 ;
//}
//void btree :: show( )
//{
//	display ( root ) ;
//}
//void btree :: display ( btnode *root )
//{
//	int i;
//	if ( root )
//	{
//		for ( int i = 0 ; i < root -> count ; i++ )
//		{
//			display ( root -> child[i] ) ;
//			cout << root -> value[i + 1] << "\t" ;
//		}
//		display ( root -> child[i] ) ;
//	}
//}
//void btree :: deltree ( btnode *root )
//{
//	int i;
//	if ( root != NULL )
//	{
//		for ( int i = 0 ; i < root -> count ; i++ )
//		{
//			deltree ( root -> child[i] ) ;
//			delete ( root -> child[i] ) ;
//		}
//		deltree ( root -> child[i] ) ;
//		delete ( root -> child[i] ) ;
//	}
//}
//
//btree :: ~btree( )
//{
//	deltree ( root ) ;
//}
