
#include "ConcatStringTree.h"
int Node::newid = 1;
ConcatStringTree& ConcatStringTree::operator=(ConcatStringTree& other) noexcept {
	this->root = other.root;
	//other.root = nullptr;
	return *this;
}
ConcatStringTree& ConcatStringTree::operator=(ConcatStringTree&& other) noexcept {
	this->root = other.root;
	other.root = nullptr;
	return *this;
}
ConcatStringTree::ConcatStringTree(ConcatStringTree& other) {
	this->root = other.root;
	//other.root = nullptr;
}
ConcatStringTree::ConcatStringTree(ConcatStringTree&& other) noexcept {
	this->root = other.root;
	other.root = nullptr;
}
Node::Node(int leftlength, int length, string data, Node* left, Node* right) {
	this->leftlength = leftlength;
	this->length = length;
	this->data = data;
	this->left = left;
	this->right = right;
	if (id > 70000) throw overflow_error("Id is overflow!");
	id = newid;
	//partree.root = new ADLNode(id,nullptr,nullptr,1);
	newid++;
	//cout << &newid << endl;
}
ConcatStringTree::ConcatStringTree(const char* s) {
	int len = 0;
	for (; s[len] != '\0'; len++) {}
	root = new Node(0, len, s, nullptr, nullptr);
	root->partree.root = new ADLNode(this->root->id, nullptr, nullptr, 1);
	root->partree.sizetree++;
	//newid++;
}
int ConcatStringTree::length() const {
	return root->length;
}
// chưa kiểm tra
char getrec(int index, Node*& root) {
	if (root->left == nullptr && root->right == nullptr) {
		return root->data[index];
	}
	if (index >= root->leftlength) {
		return getrec(index - root->leftlength, root->right);
	}
	else {
		return getrec(index, root->left);
	}
}
char ConcatStringTree::get(int index) {
	if (index >= root->length) {
		throw out_of_range("Index of string is invalid!");
	}
	return getrec(index, root);
}
int myfind(string& s, char& c) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] == c) {
			return i;
		}
	}
	return -1;
}
int indrec(char& c, Node*& root) {
	if (root->left == nullptr && root->right == nullptr) {//data của string luôn nằm ở các leaf
		return myfind(root->data, c);
	}
	if (root->left != nullptr) {//if node left is null thì không có để qua left nên buộc phải qua right để tìm kiếm
		int index = indrec(c, root->left);
		if (index != -1) {
			return index;
		}
	}
	int rightidx= indrec(c, root->right);
	if (rightidx != -1)
		return root->leftlength+indrec(c, root->right);// không thể xảy ra trường hợp left và right bị null do ở trên đã xét trước rồi
	else return -1;
}
int ConcatStringTree::indexOf(char c) {
	return indrec(c, this->root);
}
ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {
	//Node* newroot = new Node(this->root->length,this->root->length+otherS.root->length,"", this->root, otherS.root);
	ConcatStringTree newtree("");
	newtree.root->leftlength = this->root->length;
	newtree.root->data = "";
	newtree.root->right = otherS.root;
	newtree.root->left = this->root;
	newtree.root->length = this->root->length + otherS.root->length;
	this->root->partree.root = this->root->partree.insert(this->root->partree.root, newtree.root->id);
	otherS.root->partree.root = otherS.root->partree.insert(otherS.root->partree.root, newtree.root->id);
	return newtree;
}
string toStringrec(Node*& root) {
	if (root == nullptr) return "";
	string s = root->data;
	s += toStringrec(root->left);
	s += toStringrec(root->right);
	return s;
}
string ConcatStringTree::toString() const {
	Node* temp = this->root;
	string s = "ConcatStringTree[";
	s += 34;
	s += toStringrec(temp) + '"' + "]";
	return s;
}
string prerec(Node* root) {
	if (root == nullptr) return "";
	string s;
	s = "(LL=" + to_string(root->leftlength) + ",L=" + to_string(root->length) + ",";
	if (root->data == "") {
		s += "<NULL>";
	}
	else {
		s += '"';
		s += root->data;
		s += '"';
	}
	s += ");";
	/*if (root->left == nullptr && root->right == nullptr) {
		return s;
	}*/
	s += prerec(root->left);//trarverse the left node
	s += prerec(root->right);//traverse the right node
	return s;

}
string ConcatStringTree::toStringPreOrder() const {
	string s = prerec(root);
	if (s.size() != 0) {
		s[s.size() - 1] = ']';
		return "ConcatStringTree[" + s;
	}
	else return  "ConcatStringTree[]";
}
void subrec(Node* roottemp, Node* newroot, int& from, int& to) {//sai khúc cập nhật data của length và left length
	if (roottemp != nullptr) {
		if (roottemp->leftlength <= from// không traverse về bên trái nếu from >=leftlength do thế nên nếu left length =0 thì tự động pass (những trường hợp data ="")
			&& from != 0) {// nếu from đã bằng 0 thì ta sẽ chỉ đơn giản làm copy đến vị trí to
			from -= roottemp->leftlength;
			to -= roottemp->leftlength;
		}
		else {
			Node* newleft;
			if (roottemp->left != nullptr) {
				if (roottemp->left->left == nullptr && roottemp->left->right == nullptr) {
					newleft = new Node(0, 0, roottemp->left->data.substr(from, to - from), nullptr, nullptr);//hmmmmm
				}
				else {
					newleft = new Node(0, 0, roottemp->left->data, nullptr, nullptr);//hmmmmm
				}
				from -= roottemp->left->data.size();
				to -= roottemp->left->data.size();
				if (from < 0) {
					from = 0;
				}
				if (to < 0) {
					to = 0;
				}
				//newleft->leftlength = roottemp->left->leftlength - from;
				//newleft->length = newleft->data.size();
			}
			else newleft = nullptr;
			newroot->left = newleft;
			if (newroot->left != nullptr) newroot->left->partree.root = newroot->left->partree.insert(newroot->left->partree.root, newroot->id);
			subrec(roottemp->left, newroot->left, from, to);
		}
		if (to == 0) {//update the length of node
			if (newroot->left != nullptr && newroot->right != nullptr) { newroot->length = newroot->left->length + newroot->right->length + newroot->data.size(); newroot->leftlength = newroot->left->length; }
			else if (newroot->left != nullptr && newroot->right == nullptr) { newroot->length = newroot->left->length + newroot->data.size(); newroot->leftlength = newroot->left->length; }
			else if (newroot->right != nullptr && newroot->left == nullptr) { newroot->length = newroot->right->length + newroot->data.size(); newroot->leftlength = 0; }
			else {
				newroot->length = newroot->data.size(); newroot->leftlength = 0;
			}
			return;
		}
		else {
			Node* newright;
			if (roottemp->right != nullptr) {
				if (roottemp->right->left == nullptr && roottemp->right->right == nullptr) {
					newright = new Node(0, 0, roottemp->right->data.substr(from, to - from), nullptr, nullptr);
				}
				else {
					newright = new Node(0, 0, roottemp->right->data, nullptr, nullptr);
				}
				from -= roottemp->right->data.size();
				to -= roottemp->right->data.size();
				if (from < 0) {
					from = 0;
				}
				if (to < 0) {
					to = 0;
				}
				//newright->leftlength = roottemp->right->leftlength - from;
				//newright->length = newright->data.size();
			}
			else newright = nullptr;
			newroot->right = newright;
			if (newroot->right != nullptr) newroot->right->partree.root = newroot->right->partree.insert(newroot->right->partree.root, newroot->id);
			subrec(roottemp->right, newroot->right, from, to);
		}
		if (newroot->left != nullptr && newroot->right != nullptr) { newroot->length = newroot->left->length + newroot->right->length + newroot->data.size(); newroot->leftlength = newroot->left->length; }//update the length of node
		else if (newroot->left != nullptr && newroot->right == nullptr) { newroot->length = newroot->left->length + newroot->data.size(); newroot->leftlength = newroot->left->length; }
		else if (newroot->right != nullptr && newroot->left == nullptr) { newroot->length = newroot->right->length + newroot->data.size(); newroot->leftlength = 0; }
		else {
			newroot->length = newroot->data.size(); newroot->leftlength = 0;
		}
	}
	else return;
}
ConcatStringTree ConcatStringTree::subString(int from, int to) const {
	if (from < 0 || from >= this->root->length || to > this->root->length) {
		throw  out_of_range("Index of string is invalid!");
	}
	if (from >= to) {
		throw  logic_error("Invalid range!");
	}
	ConcatStringTree newtree("");
	if (this->root->data == "") newtree.root->data = this->root->data;
	else newtree.root->data = this->root->data.substr(from, to - from);
	newtree.root->length = to - from;
	newtree.root->leftlength = this->root->leftlength - from;
	subrec(this->root, newtree.root, from, to);
	return newtree;
}
string reverse_string(string s) {
	string ret = "";
	for (int i = s.length() - 1; i >= 0; i--) {
		ret += s[i];
	}
	return ret;
}
Node* revcopy(Node* roottemp) {
	if (roottemp == nullptr) return nullptr;
	if (roottemp != nullptr) {
		Node* newnode = new Node();
		newnode->data = reverse_string(roottemp->data);//reverse the data
		newnode->left = revcopy(roottemp->right);
		newnode->right = revcopy(roottemp->left);
		if (newnode->left != nullptr && newnode->right != nullptr) {
			newnode->length = newnode->left->length + newnode->right->length + newnode->data.size();
			newnode->leftlength = newnode->left->length;
			//newnode->left->partree.root= newnode->left->partree.insert(newnode->left->partree.root,newnode->id);//insert into the ADL tree for node left of the right above node
			//newnode->right->partree.root = newnode->right->partree.insert(newnode->right->partree.root, newnode->id); //insert into the ADL tree for node right of the right above node
		}
		else if (newnode->left != nullptr && newnode->right == nullptr) {
			newnode->length = newnode->left->length + newnode->data.size();
			newnode->leftlength = newnode->left->length;
			//newnode->left->partree.root = newnode->left->partree.insert(newnode->left->partree.root, newnode->id);//insert into the ADL tree for node left of the right above node
		}
		else if (newnode->left == nullptr && newnode->right != nullptr) {
			newnode->length = newnode->right->length + newnode->data.size();
			newnode->leftlength = 0;
			//newnode->right->partree.root = newnode->right->partree.insert(newnode->right->partree.root, newnode->id); //insert into the ADL tree for node right of the right above node
		}
		else {
			newnode->length = newnode->data.size();
			newnode->leftlength = 0;
		}
		return newnode;
	}
}

ConcatStringTree ConcatStringTree::reverse()const {
	ConcatStringTree newtree("");
	newtree.root->partree.deleteADLNode(newtree.root->partree.root, newtree.root->id);
	Node::newid--;
	delete newtree.root;
	newtree.root = revcopy(this->root);
	//revcopy(this->root, newtree.root);
	newtree.root->partree.root = newtree.root->partree.insert(newtree.root->partree.root, newtree.root->id);
	return newtree;
}
ParentsTree::ParentsTree() {
	root = nullptr;
	sizetree = 0;
}
int ParentsTree::size()const {
	return this->sizetree;
}
string partostringrec(ADLNode* ADLNode) {
	if (ADLNode == nullptr) return "";
	string s = "(id=" + to_string(ADLNode->ADLid) + ");";
	s += partostringrec(ADLNode->left);
	s += partostringrec(ADLNode->right);
	return s;
}
string ParentsTree::toStringPreOrder()const {
	string s = partostringrec(this->root);
	if (s.size() != 0) {
		s[s.size() - 1] = ']';
		return "ParentsTree[" + s;
	}
	else return "ParentsTree[]";
}
int ParentsTree::height(ADLNode* N)
{
	if (N == NULL)
		return 0;
	return N->height;
}
// A utility function to get maximum
		// of two integers
int ParentsTree::max(int a, int b)
{
	return (a > b) ? a : b;
}
ADLNode* ParentsTree::newNode(int key)
{
	ADLNode* node = new ADLNode(key, nullptr, nullptr, 1);// new node is initially
	// added at leaf
	return(node);
}
ADLNode* ParentsTree::rightRotate(ADLNode* y)
{
	ADLNode* x = y->left;
	ADLNode* T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left),
		height(y->right)) + 1;
	x->height = max(height(x->left),
		height(x->right)) + 1;

	// Return new root
	return x;
}
ADLNode* ParentsTree::leftRotate(ADLNode* x)
{
	ADLNode* y = x->right;
	ADLNode* T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left),
		height(x->right)) + 1;
	y->height = max(height(y->left),
		height(y->right)) + 1;

	// Return new root
	return y;
}
int ParentsTree::getBalance(ADLNode* N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}
ADLNode* ParentsTree::insert(ADLNode* node, int key)
{
	/* 1. Perform the normal BST insertion */
	if (node == NULL) {
		sizetree++;
		return(newNode(key));
	}
	if (key < node->ADLid)
		node->left = insert(node->left, key);
	else if (key > node->ADLid)
		node->right = insert(node->right, key);
	else // Equal keys are not allowed in BST
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
		height(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->ADLid)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->ADLid)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->ADLid)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->ADLid)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}
ADLNode* maxValueNode(ADLNode* node)
{
	ADLNode* current = node;

	/* loop down to find the rightmost leaf */
	while (current->right != NULL)
		current = current->right;

	return current;
}
ADLNode* ParentsTree::deleteADLNode(ADLNode* root, int key) {
	// STEP 1: PERFORM STANDARD BST DELETE 
	if (root == NULL)
		return root;

	// If the key to be deleted is smaller 
	// than the root's key, then it lies
	// in left subtree 
	if (key < root->ADLid)
		root->left = deleteADLNode(root->left, key);

	// If the key to be deleted is greater 
	// than the root's key, then it lies 
	// in right subtree 
	else if (key > root->ADLid)
		root->right = deleteADLNode(root->right, key);

	// if key is same as root's key, then 
	// This is the node to be deleted 
	else
	{
		// node with only one child or no child 
		if ((root->left == NULL) ||
			(root->right == NULL))
		{
			ADLNode* temp = root->left ?
				root->left :
				root->right;

			// No child case 
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case 
				*root = *temp; // Copy the contents of 
			// the non-empty child 
			free(temp);
			temp = nullptr;
		}
		else
		{
			// node with two children: Get the inorder 
			// successor (smallest in the left subtree) 
			ADLNode* temp = maxValueNode(root->left);

			// Copy the inorder successor's 
			// data to this node 
			root->ADLid = temp->ADLid;

			// Delete the inorder successor 
			//root->right = deleteADLNode(root->left, temp->ADLid); không rõ
			root->left = deleteADLNode(root->left, temp->ADLid);
		}
	}

	// If the tree had only one node
	// then return 
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
	root->height = 1 + max(height(root->left),
		height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF 
	// THIS NODE (to check whether this 
	// node became unbalanced) 
	int balance = getBalance(root);

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case 
	if (balance > 1 &&
		getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case 
	if (balance < -1 &&
		getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case 
	if (balance < -1 &&
		getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}
int ConcatStringTree::getParTreeSize(const string& query) const {
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] != 'l' && query[i] != 'r') {
			throw runtime_error("Invalid character of query");
		}
	}
	Node* temp = this->root;
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] == 'l') temp = temp->left;
		else if (query[i] == 'r') temp = temp->right;
		if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->partree.size();
}
string ConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] != 'l' && query[i] != 'r') {
			throw runtime_error("Invalid character of query");
		}
	}
	Node* temp = this->root;
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] == 'l') temp = temp->left;
		else if (query[i] == 'r') temp = temp->right;
		if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->partree.toStringPreOrder();
}
void deletenode(Node*& root) {
	if (root->partree.root == nullptr) {
		if (root->left != nullptr) {
			root->left->partree.root = root->left->partree.deleteADLNode(root->left->partree.root, root->id);
			root->left->partree.sizetree--;
			deletenode(root->left);
		}
		if (root->right != nullptr) {
			root->right->partree.root = root->right->partree.deleteADLNode(root->right->partree.root, root->id);
			root->right->partree.sizetree--;
			deletenode(root->right);
		}
		delete root;
		root = nullptr;
	}
	else {
		return;
	}
}
ConcatStringTree::~ConcatStringTree() {
	//cout << "ok" << endl;
	if (root == nullptr) return;
	this->root->partree.root = this->root->partree.deleteADLNode(this->root->partree.root, this->root->id);
	this->root->partree.sizetree--;
	deletenode(this->root);
}
HashConfig::HashConfig(int p, double c1, double c2, double lambda, double alpha, int initsize) {
	this->p = p;
	this->c1 = c1;
	this->c2 = c2;
	this->lambda = lambda;
	this->alpha = alpha;
	this->initSize = initsize;
}
LitStringHash::LitStringHash(const HashConfig& hashConfig) {
	this->config.alpha = hashConfig.alpha;
	this->config.c1 = hashConfig.c1;
	this->config.c2 = hashConfig.c2;
	this->config.initSize = hashConfig.initSize;
	this->config.lambda = hashConfig.lambda;
	this->config.p = hashConfig.p;
	this->lastidx = -1;
	this->hash_table = new litString * [hashConfig.initSize];
	size = hashConfig.initSize;
	is_tomb = new bool[hashConfig.initSize];
	count = 0;
	for (int i = 0; i < hashConfig.initSize; i++) {//cho tất cả các pointer đều trỏ đến nullptr
		this->hash_table[i] = nullptr;
		is_tomb[i] = 0;//set tomb là 0
	}
	//this->hash_table;

}
void LitStringHash::rehash() {
	litString** newhash;
	long long tempsize = config.alpha * size;
	bool* new_tomb;
	newhash = new litString * [tempsize];
	for (int i = 0; i < tempsize; i++) {//initialize tất cả mọi cái thành nullptr
		newhash[i] = nullptr;
	}
	bool changelastidx = 0;
	for (int i = 0; i < size; i++) {//hash qua cái newhash
		if (hash_table[i] != nullptr) {
			long long hs = 0;//hàm băm cho cái newhash
			long long ptemp = 1;
			string s = hash_table[i]->s;//string của cái hash cũ
			for (long long j = 0; s[j] != '\0'; j++) {
				hs = (hs + s[j] * ptemp) % tempsize;//phải là size mới của cái newhash mới
				ptemp = (ptemp * config.p) % tempsize;
			}
			long long hp = 0;
			bool nopossslot = 1;
			for (long long j = 0; j < tempsize; j++) {
				hp = (hs + (long long)((config.c1 * j) + (config.c2 * j * j)) % tempsize) % tempsize;
				//if (hp > tempsize) break;
				if (newhash[hp] == nullptr) {
					newhash[hp] = hash_table[i];//dời cái node của hash table cũ vào newhash
					//cout << newhash[hp] << endl;
					newhash[hp]->indxpos = hp;//cập nhật indxpos mới
					//hash_table[i] = nullptr;
					if (lastidx == i && changelastidx == 0) {
						changelastidx = 1;
						lastidx = hp;
					}
					nopossslot = 0;
					break;
				};
			}
			if (nopossslot == 1) {
				for (int k = 0; k < tempsize; k++) {
					if (newhash[k] != nullptr) {
						delete newhash[k];//hmmmmmmmm
					}
				}
				throw runtime_error("No possible slot");
			}
		}
	}
	delete is_tomb;
	is_tomb = new bool[tempsize];
	for (int i = 0; i < tempsize; i++) {
		is_tomb[i] = 0;
	}
	delete hash_table;
	this->size = tempsize;
	hash_table = newhash;//gán lại cái hash table mới
}
int LitStringHash::getLastInsertedIndex() const {
	return this->lastidx;
}
string LitStringHash::toString() const {
	string s = "LitStringHash[";
	for (int i = 0; i < this->size; i++) {
		if (this->hash_table[i] == nullptr) {
			s += "();";
		}
		else {
			s += "(litS=";
			s += '"';
			s += this->hash_table[i][0].s;
			s += '"';
			s += ");";
		}
	}
	if (s[s.size() - 1] == ';') s[s.size() - 1] = ']';
	else s += "]";
	return s;
}
LitStringHash::~LitStringHash() {
	if (hash_table == nullptr) return;
	if (is_tomb != nullptr) {
		delete is_tomb;
		is_tomb = nullptr;
	}
	for (int i = 0; i < size; i++) {
		if (hash_table[i] != nullptr) {
			delete hash_table[i];
		}
	}
	delete hash_table;
	hash_table = nullptr;
}
reducenode::reducenode(int leftlength, int length, reducenode* left, reducenode* right) {
	//this->is_leaf = is_leaf;
	this->left = left;
	this->right = right;
	this->leftlength = leftlength;
	this->length = length;
	if (newidred > 10000000) throw overflow_error("Id is overflow!");
	this->id = newidred;
	newidred++;
}
ReducedConcatStringTree::ReducedConcatStringTree(ReducedConcatStringTree& other) {
	this->root = other.root;
	this->litStringHash = other.litStringHash;
	//return *this;
}
ReducedConcatStringTree::ReducedConcatStringTree(ReducedConcatStringTree&& other) {
	this->root = other.root;
	other.root = nullptr;
	this->litStringHash = other.litStringHash;
	other.litStringHash = nullptr;
	//return *this;
}
ReducedConcatStringTree& ReducedConcatStringTree:: operator =(ReducedConcatStringTree& other) noexcept {
	this->root = other.root;
	this->litStringHash = other.litStringHash;
	return *this;
}
ReducedConcatStringTree& ReducedConcatStringTree:: operator =(ReducedConcatStringTree&& other) noexcept {
	this->root = other.root;
	this->litStringHash = other.litStringHash;
	other.root = nullptr;
	other.litStringHash = nullptr;
	return *this;
}
ReducedConcatStringTree::ReducedConcatStringTree(const char* s, LitStringHash* litStringHash) {
	//if (s[0] != '\0') {
	this->litStringHash = litStringHash;
	if (this->litStringHash->hash_table == nullptr) {
		this->litStringHash->size = this->litStringHash->config.initSize;
		this->litStringHash->hash_table = new litString * [this->litStringHash->config.initSize];
		this->litStringHash->is_tomb = new bool[this->litStringHash->config.initSize];
		for (int i = 0; i < this->litStringHash->size; i++) {//cho tất cả các pointer đều trỏ đến nullptr
			this->litStringHash->hash_table[i] = nullptr;
			this->litStringHash->is_tomb[i] = 0;
		}
	}
	int len = 0;
	for (; s[len] != '\0'; len++) {}
	this->root = new reducenode(0, len, nullptr, nullptr);//tạo node mới
	long long hs = 0;//hàm băm
	long long ptemp = 1;
	for (long long i = 0; s[i] != '\0'; i++) {
		hs = (hs + s[i] * ptemp) % litStringHash->size;
		ptemp = (ptemp * litStringHash->config.p) % litStringHash->size;
	}
	long long hp = 0;
	bool nopossslot = 1;
	bool first_suitable = 0;// is tomb 
	int first_suitablepos = -1;
	for (long long i = 0; i < litStringHash->size; i++) {
		hp = (hs + (long long)((litStringHash->config.c1 * i) + (litStringHash->config.c2 * i * i)) % litStringHash->size) % litStringHash->size;
		//if (hp > litStringHash->size) break;
		if (litStringHash->hash_table[hp] == nullptr) {
			if (litStringHash->is_tomb[hp] == 1) {
				if (first_suitable == 0) {
					first_suitable = 1;
					first_suitablepos = hp;
				}
				continue;
			}
			else {//tomb=0 and litString is null so it is completly empty
				if (first_suitable == 1) {
					hp = first_suitablepos;
					litStringHash->is_tomb[hp] = 0;
				}
				litString* newlit = new litString();//tạo nên 1 litString mới
				newlit->numnode++;
				newlit->s = s;
				newlit->indxpos = hp;
				this->root->data = newlit;
				litStringHash->hash_table[hp] = newlit;//chèn litString mới vào trong hashtable
				litStringHash->lastidx = hp;//update last index
				nopossslot = 0;
				litStringHash->count++;
				break;
			}
		}
		else if (litStringHash->hash_table[hp]->s == s) {
			this->root->data = litStringHash->hash_table[hp];
			litStringHash->hash_table[hp]->numnode++;//cộng thêm nếu cùng data
			nopossslot = 0;
			break;
		}
	}
	if (nopossslot == 1)
	{
		//cout << "ok" << endl;
		delete this->root;
		this->root = nullptr;
		this->litStringHash = nullptr;
		throw runtime_error("No possible slot");
	}
	//this->litStringHash->hash_table[hs] = newlit;// updata litString vào bên trong hash table
	//this->litStringHash->hash_table[hp]
	//litStringHash->count++;
	if ((double)litStringHash->count / double(litStringHash->size) > litStringHash->config.lambda) { litStringHash->rehash(); }//rehash nếu số count / size lớn hơn lambda
	this->root->partree.root = new ADLNode(this->root->id, nullptr, nullptr, 1);//updata partree
	this->root->partree.sizetree++;
	//}
	/*else {
		this->root = new reducenode(0, 0, nullptr, nullptr);
	}*/
}
ReducedConcatStringTree::ReducedConcatStringTree(string s) {
	this->root = new reducenode(0, 0, nullptr, nullptr);
}
ReducedConcatStringTree ReducedConcatStringTree::concat(const ReducedConcatStringTree& otherS) const {
	ReducedConcatStringTree newtree("");
	newtree.root->leftlength = this->root->length;
	newtree.root->data = nullptr;
	newtree.root->right = otherS.root;
	newtree.root->left = this->root;
	newtree.root->length = otherS.root->length + this->root->length;
	newtree.litStringHash = this->litStringHash;
	newtree.root->partree.root = newtree.root->partree.insert(newtree.root->partree.root, newtree.root->id);
	this->root->partree.root = this->root->partree.insert(this->root->partree.root, newtree.root->id);
	otherS.root->partree.root = otherS.root->partree.insert(otherS.root->partree.root, newtree.root->id);
	return newtree;

}
int ReducedConcatStringTree::length() const {
	return this->root->length;
}
char getrecreduce(int index, reducenode*& root) {
	if (root->left == nullptr && root->right == nullptr) {
		return root->data->s[index];
	}
	if (index >= root->leftlength) {
		return getrecreduce(index - root->leftlength, root->right);
	}
	else {
		return getrecreduce(index, root->left);
	}
}
char ReducedConcatStringTree::get(int index) {
	if (index >= root->length) {
		throw out_of_range("Index of string is invalid!");
	}
	return getrecreduce(index, root);
}
//int myfind(string& s, char& c) {//ở trên có rồi
//	for (int i = 0; s[i] != '\0'; i++) {
//		if (s[i] == c) {
//			return i;
//		}
//	}
//	return -1;
//}
int indrecreduce(char& c, reducenode*& root) {
	if (root->left == nullptr && root->right == nullptr) {//data của string luôn nằm ở các leaf
		return myfind(root->data->s, c);
	}
	if (root->left != nullptr) {//if node left is null thì không có để qua left nên buộc phải qua right để tìm kiếm
		int index = indrecreduce(c, root->left);
		if (index != -1) {
			return index;
		}
	}
	return indrecreduce(c, root->right);// không thể xảy ra trường hợp left và right bị null do ở trên đã xét trước rồi
}
int ReducedConcatStringTree::indexOf(char c) {
	return indrecreduce(c, this->root);
}
string prerecred(reducenode* root) {
	if (root == nullptr) return "";
	string s;
	s = "(LL=" + to_string(root->leftlength) + ",L=" + to_string(root->length) + ",";
	if (root->data == nullptr) {
		s += "<NULL>";
	}
	else {
		s += '"';
		s += root->data->s;
		s += '"';
	}
	s += ");";
	/*if (root->left == nullptr && root->right == nullptr) {
		return s;
	}*/
	s += prerecred(root->left);//trarverse the left node
	s += prerecred(root->right);//traverse the right node
	return s;

}
string ReducedConcatStringTree::toStringPreOrder() const {
	string s = prerecred(root);
	if (s.size() != 0) {
		s[s.size() - 1] = ']';
		return "ConcatStringTree[" + s;
	}
	else return  "ConcatStringTree[]";
}
string to_stringrec(reducenode*& root) {
	if (root == nullptr) return "";
	string s;
	if (root->data == nullptr) { s = ""; }
	else { s += root->data->s; }
	s += to_stringrec(root->left);
	s += to_stringrec(root->right);
	return s;
}
string ReducedConcatStringTree::toString() const {
	reducenode* temp = this->root;
	string s = "ConcatStringTree[";
	s += '"';
	s += to_stringrec(temp);
	s += '"';
	s += "]";
	return s;
}
int ReducedConcatStringTree::getParTreeSize(const string& query) const {
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] != 'l' && query[i] != 'r') {
			throw runtime_error("Invalid character of query");
		}
	}
	reducenode* temp = this->root;
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] == 'l') temp = temp->left;
		else if (query[i] == 'r') temp = temp->right;
		if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->partree.size();
}
string ReducedConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] != 'l' && query[i] != 'r') {
			throw runtime_error("Invalid character of query");
		}
	}
	reducenode* temp = this->root;
	for (int i = 0, l = query.size(); i < l; i++) {
		if (query[i] == 'l') temp = temp->left;
		else if (query[i] == 'r') temp = temp->right;
		if (temp == nullptr) throw runtime_error("Invalid query: reaching NULL");
	}
	return temp->partree.toStringPreOrder();
}
void ReducedConcatStringTree::deletenodered(reducenode*& root) {
	if (root->partree.root == nullptr) {
		if (root->left != nullptr) {
			root->left->partree.root = root->left->partree.deleteADLNode(root->left->partree.root, root->id);
			root->left->partree.sizetree--;
			deletenodered(root->left);
		}
		if (root->right != nullptr) {
			root->right->partree.root = root->right->partree.deleteADLNode(root->right->partree.root, root->id);
			root->right->partree.sizetree--;
			deletenodered(root->right);
		}
		if (root->data != nullptr) {
			root->data->numnode--;
			if (root->data->numnode == 0) {
				//hashtable[i]==nullptr
				//if (this->litStringHash->lastidx == root->data->indxpos) //this->litStringHash->lastidx = -1;
				this->litStringHash->hash_table[root->data->indxpos] = nullptr;
				this->litStringHash->is_tomb[root->data->indxpos] = 1;//set the tomb to be=1
				this->litStringHash->count--;
				delete root->data;
				root->data = nullptr;
			}//how the fuck do we update the hash table ??
		}
		delete root;
		root = nullptr;
	}
	else {
		return;
	}
}
void ReducedConcatStringTree::free_litStringhash(LitStringHash* litStringHash) {
	if (litStringHash->hash_table == nullptr) return;
	delete litStringHash->is_tomb;
	for (int i = 0; i < litStringHash->size; i++) {
		if (litStringHash->hash_table[i] != nullptr) delete litStringHash->hash_table[i];
	}
	delete litStringHash->hash_table;
	//litStringHash->count = 0;
	litStringHash->is_tomb = nullptr;
	litStringHash->size = 0;
	litStringHash->lastidx = -1;
	litStringHash->hash_table = nullptr;
}
ReducedConcatStringTree::~ReducedConcatStringTree() {
	if (root == nullptr) return;
	this->root->partree.root = this->root->partree.deleteADLNode(this->root->partree.root, this->root->id);
	this->root->partree.sizetree--;
	deletenodered(this->root);
	if (this->litStringHash->count == 0) {
		free_litStringhash(this->litStringHash);
	}
}