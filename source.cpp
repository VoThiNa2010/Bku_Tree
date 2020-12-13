#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) { }
    };

    //private:
public:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    queue<K> keys_remove;
    queue<K> keys_search;

   
    vector<K> save_Node_SearchBKU;
    int maxNumOfKeys;
    friend class AVLTree;
    friend class SplayTree;
   


public:
    BKUTree(int maxNumOfKeys = 5) {
       
        this->avl = new  AVLTree();
        this->splay = new SplayTree();
        this->maxNumOfKeys = maxNumOfKeys;
         }
   
    ~BKUTree() { this->clear(); }

   
    void add(K key, V value); 
    void remove(K key);
    V search(K key, vector<K>& traversedList);

    void traverseNLROnAVL(void (*func)(K key, V value)) {

        this->avl->traverseNLR(*func);
    };
    void traverseNLROnSplay(void (*func)(K key, V value)) {
        this->splay->traverseNLR(*func);
    };

    void clear() {
        this->avl->clear();
        this->splay->clear();

    };



    class SplayTree {
    public:
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;

          typename AVLTree::Node* corr;

            Node(K key, V value) {
                this->entry = new Entry(key, value);

                this->right = NULL;
                this->left = NULL;
                this->parent = NULL;
                this->corr = NULL;

            }

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }
        };

    public:
        vector<K> search_keys_Splay;
        Node* root;
        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };
        bool check_Node_have_or_nohave(Node* root, K key) {

            if (root == NULL) { return 0; }

            if (key == root->entry->key) { return 1; }
            else if (key > root->entry->key) {
                return check_Node_have_or_nohave(root->right, key);
            }
            else return check_Node_have_or_nohave(root->left, key);

            return 0;

        }

        //add
        void leftRotate(Node* x) {
            Node* y = x->right;
            x->right = y->left;
            if (y->left != NULL) {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == NULL) {
                this->root = y;

            }
            else if (x == x->parent->left) {
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }
            y->left = x;
            x->parent = y;
        }
        void rightRotate(Node* x) {
            Node* y = x->left;

            x->left = y->right;
            if (y->right != NULL) {
                y->right->parent = x;

            }
            y->parent = x->parent;
            if (x->parent == NULL) {
                this->root = y;
            }
            else if (x == x->parent->right) {
                x->parent->right = y;
            }
            else {
                x->parent->left = y;
            }
            y->right = x;
            x->parent = y;
        }
        void splay(Node* x) {
            while (x->parent) {
                if (!x->parent->parent) {
                    if (x == x->parent->left) {
                        // zig rotation
                        rightRotate(x->parent);
                    }
                    else {
                        // zag rotation
                        leftRotate(x->parent);
                    }
                }
                else if (x == x->parent->left && x->parent == x->parent->parent->left) {
                    // zig-zig rotation
                    rightRotate(x->parent->parent);
                    rightRotate(x->parent);
                }
                else if (x == x->parent->right && x->parent == x->parent->parent->right) {
                    // zag-zag rotation
                    leftRotate(x->parent->parent);
                    leftRotate(x->parent);
                }
                else if (x == x->parent->right && x->parent == x->parent->parent->left) {
                    // zig-zag rotation
                    leftRotate(x->parent);
                    rightRotate(x->parent);
                }
                else {
                    // zag-zig rotation
                    rightRotate(x->parent);
                    leftRotate(x->parent);
                }
            }
        }
        void add(K key, V value) {
            if (check_Node_have_or_nohave(this->root, key) == 0) {
                // normal BST insert
                Node* node = new Node(key, value);
                node->parent = NULL;
                node->left = NULL;
                node->right = NULL;
                node->entry->key = key;
                Node* y = NULL;
                Node* x = this->root;

                while (x != NULL) {
                    y = x;
                    if (node->entry->key < x->entry->key) {
                        x = x->left;
                    }
                    else {
                        x = x->right;
                    }
                }

                // y is parent of x
                node->parent = y;
                if (y == NULL) {
                    root = node;
                }
                else if (node->entry->key < y->entry->key) {
                    y->left = node;
                }
                else {
                    y->right = node;
                }

                // splay the node
                splay(node);
            }
            else throw "Duplicate key";
        }
        void add(Entry* entry) {
            if (check_Node_have_or_nohave(this->root, entry->key) == 0) {
                Node* node = new Node(entry->key, entry->value);
                node->parent = NULL;
                node->left = NULL;
                node->right = NULL;
                node->entry->key = entry->key;
                Node* y = NULL;
                Node* x = this->root;

                while (x != NULL) {
                    y = x;
                    if (node->entry->key < x->entry->key) {
                        x = x->left;
                    }
                    else {
                        x = x->right;
                    }
                }

                // y is parent of x
                node->parent = y;
                if (y == NULL) {
                    root = node;
                }
                else if (node->entry->key < y->entry->key) {
                    y->left = node;
                }
                else {
                    y->right = node;
                }

                // splay the node
                splay(node);
            }
            else throw "Duplicate key";
        }
        //remove 
        void remove(K key) {
            if (check_Node_have_or_nohave(this->root, key) == 1) {
                deleteNodeHelper(this->root, key);
            }
            else throw "Not found";

        }
        void deleteNodeHelper(Node* node, K key) {
            Node* x = NULL;
            Node* t;
            Node* s;
            while (node != NULL) {
                if (node->entry->key == key) {
                    x = node;
                }

                if (node->entry->key < key) {
                    node = node->right;
                }
                else {
                    node = node->left;
                }
            }

            if (x == NULL) {
                //cout << "Couldn't find key in the tree" << endl;
                return;
            }
            split(x, s, t); // split the tree
            if (s->left) { // remove x
                s->left->parent = NULL;
            }
            root = join(s->left, t);
            delete(s);
            s = NULL;
        }
        Node* join(Node* s, Node* t) {
            if (!s) {
                return t;
            }

            if (!t) {
                return s;
            }
            Node* x = maximum(s);
            splay(x);
            x->right = t;
            t->parent = x;
            return x;
        }

        // splits the tree into s and t
        void split(Node*& x, Node*& s, Node*& t) {
            splay(x);
            if (x->right) {
                t = x->right;
                t->parent = NULL;
            }
            else {
                t = NULL;
            }
            s = x;
            s->right = NULL;
            x = NULL;
        }
        // find the node with the minimum key
        Node* minimum(Node* node) {
            while (node->left != NULL) {
                node = node->left;
            }
            return node;
        }

        // find the node with the maximum key
        Node* maximum(Node* node) {
            while (node->right != NULL) {
                node = node->right;
            }
            return node;
        }

        //search
        Node* searchTree(K key) {
            Node* x = searchTreeHelper(this->root, key);
            if (x) {
                splay(x);
            }
            return x;
        }
        V search(K key) {
            if (check_Node_have_or_nohave(this->root, key) == 1) {
                return searchTree(key)->entry->key;
                
            }
            else throw "Not found";
        }
        Node* searchTreeHelper(Node* node, K key) {
            if (node == NULL || key == node->entry->key) {
                return node;
            }

            if (key < node->entry->key) {
                return searchTreeHelper(node->left, key);
            }
            return searchTreeHelper(node->right, key);
        }
        //clear


        // print 
        void traverseNLR(void (*func)(K key, V value)) {
            print_Pre_order_Traversal(this->root, func);

        };
        void print_Pre_order_Traversal(Node* _root, void (*func)(K key, V value)) {
            if (_root == NULL) { return; }

            (func)(_root->entry->key, _root->entry->value);
            print_Pre_order_Traversal(_root->left, *func);
            print_Pre_order_Traversal(_root->right, *func);

        }
        //clear
        void clear() { deleteTree_Splay(&root); };
        void _deleteTree_Splay(Node* node)
        {
            if (node == NULL) return;

            _deleteTree_Splay(node->left);
            _deleteTree_Splay(node->right);

            delete node;
        }

        void deleteTree_Splay(Node** node_ref)
        {
            _deleteTree_Splay(*node_ref);
            *node_ref = NULL;
        }

        // dung trong cay BKU TREE
        Node* search_Node(Node* root, K key, V value) {

            //Node* node = new Node(key, value);

            if (root->entry->key > key) { search_Node(root->left, key, value); }
            else if (root->entry->key < key) { search_Node(root->right, key, value); }
            else return root;
        }

        //splay 1 lan
        void search_splay_1_Splay_BKU(Node* x , K _key) {
            this->search_keys_Splay.push_back(x->entry->key);
            if (x->entry->key == _key) {
                //splay 1 lan
                if (x->parent != NULL) {
                    if (!x->parent->parent) {
                        if (x == x->parent->left) {
                            // zig rotation
                            rightRotate(x->parent);
                        }
                        else {
                            // zag rotation
                            leftRotate(x->parent);
                        }
                    }
                    else if (x == x->parent->left && x->parent == x->parent->parent->left) {
                        // zig-zig rotation
                        rightRotate(x->parent->parent);
                        rightRotate(x->parent);
                    }
                    else if (x == x->parent->right && x->parent == x->parent->parent->right) {
                        // zag-zag rotation
                        leftRotate(x->parent->parent);
                        leftRotate(x->parent);
                    }
                    else if (x == x->parent->right && x->parent == x->parent->parent->left) {
                        // zig-zag rotation
                        leftRotate(x->parent);
                        rightRotate(x->parent);
                    }
                    else {
                        // zag-zig rotation
                        rightRotate(x->parent);
                        leftRotate(x->parent);
                    }
                }


            }
            else if (x->entry->key > _key) { return search_splay_1_Splay_BKU(x->left, _key); }
            else return search_splay_1_Splay_BKU(x->right, _key);
           
        }
    };
    class AVLTree {
    public:
         class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent; int balance;
            
            typename SplayTree::Node* corr;
            Node(K key, V value) {
                this->entry = new Entry(key, value);
                this->balance = 0;
                this->right = NULL;
                this->left = NULL;
                this->corr = NULL;
                
            }

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {

                this->entry = entry;

                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
              

            }
        };

    public:
        vector<K> keys_search_AVL;
        Node* root;
       
        AVLTree() : root(NULL) {};
        ~AVLTree() { this->clear(); };
        //
        
        int max(K a, K b) { return (a > b) ? a : b; };
        int getHeightRec(Node* node) {
            if (node == NULL)
                return 0;
            int lh = this->getHeightRec(node->left);
            int rh = this->getHeightRec(node->right);
            return (lh > rh ? lh : rh) + 1;
        };
        
        Node* rotate_Right(Node* node) {
            Node* new_node1 = node->left;
            Node* new_node2 = new_node1->right;
            new_node1->right = node;
            node->left = new_node2;
            return new_node1;
        };
        Node* rotate_Left(Node* node) {
            Node* y = node->right;
            Node* T2 = y->left;
            y->left = node;
            node->right = T2;
            return y;
        };;
        int getBalance(Node* node) {
            if (node == NULL) return 0;
            return getHeightRec(node->left) - getHeightRec(node->right);

        };
        // print 
        void traverseNLR(void (*func)(K key, V value)) {
            print_Pre_order_Traversal(this->root, func);

        };
        void print_Pre_order_Traversal(Node* _root, void (*func)(K key, V value)) {
            if (_root == NULL) { return; }

            (func)(_root->entry->key, _root->entry->value);
            print_Pre_order_Traversal(_root->left, *func);
            print_Pre_order_Traversal(_root->right, *func);

        }
        //kiem tra xem key da co trong tree chua
        bool check_Node_have_or_nohave(Node* root, K key) {

            if (root == NULL) { return 0; }

            if (key == root->entry->key) { return 1; }
            else if (key > root->entry->key) {
                return check_Node_have_or_nohave(root->right, key);
            }
            else return check_Node_have_or_nohave(root->left, key);

            return 0;

        }
        // add

       // void add(K key, V value) {
            void add(K key, V value) {
                if (check_Node_have_or_nohave(this->root, key) == 0) {

                    this->root = node_Insert_add(this->root, key, value);
                }
                else throw "Duplicate key";
              
            }
            void add(Entry * entry) {
                if (check_Node_have_or_nohave(this->root, entry->key) == 0) {
                    this->root = node_Insert_add(this->root, entry->key, entry->value);
                }
                else throw "Duplicate key";
            }
            Node* node_Insert_add(Node * root, K key, V value) {
                
                
                if (root == NULL) { 
                    Node* new_node = new Node(key, value); 
                    return new_node;
                }

                if (key < root->entry->key) {
                    root->left = node_Insert_add(root->left, key, value);
                }
                else if (key > root->entry->key) {
                    root->right = node_Insert_add(root->right, key, value);
                }
                

                //case left_left
               
                if (getBalance(root) > 1 && key < root->left->entry->key) { return rotate_Right(root); }
                //case  right_right
                if (getBalance(root) < -1 && key > root->right->entry->key)
                    return rotate_Left(root);
                //case right_of_left
                if (getBalance(root) < -1 && key < root->right->entry->key)
                {
                    root->right = rotate_Right(root->right);
                    return rotate_Left(root);
                }
                //case left_of_right
                if (getBalance(root) > 1 && key > root->left->entry->key)
                {
                    root->left = rotate_Left(root->left);
                    return rotate_Right(root);
                }
                return root;

            };
            // remove
            void remove(K key) {
                if (check_Node_have_or_nohave(this->root, key) == 1) {
                    this->root = remove_node_Tree(root, key);
                }
                else throw "Not found";
            };
            Node* remove_node_Tree(Node * root, K key) {
                if (root == NULL)
                    return root;
                if (key < root->entry->key)
                    root->left = remove_node_Tree(root->left, key);
                else if (key > root->entry->key)
                    root->right = remove_node_Tree(root->right, key);
                else
                {
                    if ((root->left == NULL) || (root->right == NULL))

                    {
                        Node* temp = root->left ? root->left : root->right;
                        if (temp == NULL)  // khong co node con nao 
                        {
                            temp = root;
                            root = NULL;
                        }
                        else {
                            *root = *temp;
                            free(temp);
                        }
                    }
                    else      // co ca 2 node con ben phai va ben trai
                    {
                        Node* temp = maxValueNode(root->left);
                        root->entry->key = temp->entry->key;
                        root->left = remove_node_Tree(root->left, temp->entry->key);

                    }
                }
                if (root == NULL)
                    return root;
                int balance = getBalance(root);
                // Left Left Case  
                if (balance > 1 && getBalance(root->left) >= 0)

                    return rotate_Right(root);

                // Left Right Case  
                if (balance > 1 && getBalance(root->left) < 0)

                {
                    root->left = rotate_Left(root->left);
                    return rotate_Right(root);
                }

                // Right Right Case  
                if (balance < -1 && getBalance(root->right) <= 0)

                    return rotate_Left(root);

                // Right Left Case  
                if (balance < -1 && getBalance(root->right) > 0)

                {
                    root->right = rotate_Right(root->right);
                    return rotate_Left(root);
                }

                return root;

            }
            Node* maxValueNode(Node * node)
            {
                Node* current = node;

                while (current->right != NULL)
                    current = current->right;

                return current;
            }
            //search
            V search(K key) {
                if (check_Node_have_or_nohave(this->root, key) == 1) {
                   
                    return search_Tree(this->root, key)->entry->value;
                     
                }
                else throw "Not found";
            };
            Node* search_Tree(Node * root, K key) {


                if (root->entry->key > key) { search_Tree(root->left, key); }

                else if (root->entry->key < key) { search_Tree(root->right, key); }
                else { return root; }
            }

            //clear 
            void clear() { deleteTree(&root); };

            void _deleteTree(Node * node)
            {
                if (node == NULL) return;

                _deleteTree(node->left);
                _deleteTree(node->right);

                delete node;
            }

            void deleteTree(Node * *node_ref)
            {
                _deleteTree(*node_ref);
                *node_ref = NULL;
            }

            //dung trong cay BKUTREE
            Node* search_Node(Node * root, K key, V value) {
               
                        //Node* node = new Node(key, value);
                   
                        if (root->entry->key > key) { search_Node(root->left, key, value); }
                        else if (root->entry->key < key) { search_Node(root->right, key, value); }
                        else return root;
                    
                
               
            } 
            //dung de search tren key_value cay con cuar AVL 
            Node* search_Child_AVL_Tree(Node* _key_rootChild,Node* _rootChild, K _key) {
                if (_key_rootChild != NULL) {
                    this->keys_search_AVL.push_back(_key_rootChild->entry->key);

                    if (_key_rootChild->entry->key == _key) {
                        return _key_rootChild;
                    }
                    else if (_key_rootChild->entry->key < _key) return  search_Child_AVL_Tree(_key_rootChild->right, _rootChild, _key);
                    else return search_Child_AVL_Tree(_key_rootChild->left, _rootChild, _key);


                }
                else return search_BKU_Tree_AVL(this->root,_rootChild,  _key);
            }

            Node* search_BKU_Tree_AVL(Node* root, Node* _rootChild, K _key) {
                this->keys_search_AVL.push_back(root->entry->key);
                if (root != NULL) {
                    if (root->entry->key == _rootChild->entry->key) { throw"Not found"; }
                    else if (root->entry->key == _key) return root;
                    else if (root->entry->key > _key)  return search_BKU_Tree_AVL(root->left, _rootChild, _key);
                    else if (root->entry->key < _key) return search_BKU_Tree_AVL(root->right, _rootChild, _key);
                    else return root;
                }
                else throw "Not found";

            }
            
                
             

        };

       
        // xu ly queue
    void handle_add_queue(K key);
    void handle_remove_queue(K key , K root);
    bool handle_search_queue(K key);   //check xem node caanf duyeet co nam trong hang doi khong

    // in ra keys de check thu khong nop 

    void print_queue_keys() {
       
        while (!this->keys.empty()) {
            cout << this->keys.front() <<"*";
            this->keys.pop();
        }
      
    }




    ////////
    void checkNode(vector<K>& nodeAVL, vector<K>& nodeSPLAY) {
        if (avl == NULL) return;
        checkTrueNode(nodeAVL, nodeSPLAY, avl->root);
    }
    void checkTrueNode(vector<K>& nodeAVL, vector<K>& nodeSPLAY, typename AVLTree::Node* f) {
        if (!f)  return;
        typename SplayTree::Node* r = f->corr;
        if (r->entry->key != f->entry->key) {
            nodeAVL.push_back(f->entry->key);
            nodeSPLAY.push_back(r->entry->key);
        }
        checkTrueNode(nodeAVL, nodeSPLAY, f->left);
        checkTrueNode(nodeAVL, nodeSPLAY, f->right);
    }
  
    };

    // xu ly queue 
    template<class K, class V>
    void BKUTree<K, V>::handle_add_queue(K key) {
        if ((int)this->keys.size() == (int)this->maxNumOfKeys) {
            this->keys.pop();
            this->keys.push(key);
        }
        else { this->keys.push(key); }


    }
    template<class K, class V>
    void BKUTree<K, V>::handle_remove_queue(K key, K _root) {
        //xoa key da tim kiem gan nhat neu co 

        if (this->keys.size() != 0) {
            if (this->keys.front() != key) {

                this->keys_remove.push(this->keys.front());
                this->keys.pop();
                handle_remove_queue(key, _root);
            }
            else {

                this->keys.pop();
                handle_remove_queue(key, _root);
            }
        }
        else {

            while (this->keys_remove.size() != 0) {
                K temp = this->keys_remove.front();
                this->keys.push(temp);
                this->keys_remove.pop();
            }
            // cout << _root << "njsfdjh";
            this->keys.push(_root);
            if (this->keys.size() > this->maxNumOfKeys) { this->keys.pop(); }

        }

    }
    template<class K, class V>
    bool BKUTree<K, V>::handle_search_queue(K key) {
        //tra ve neu co key can duyet co trong hang doi
        int temp1 = 0; //dung de dem so lan xuat hien trong queue
        if (this->keys.size() != 0) {
            if (this->keys.front() != key) {
                
                this->keys_search.push(this->keys.front());
                this->keys.pop();
                handle_search_queue(key);
            }
            else {
                temp1++;
                this->keys_search.push(this->keys.front());
                this->keys.pop();
                handle_search_queue(key);
            }
        }
        else {

            while (this->keys_search.size() != 0) {
                
                this->keys.push(this->keys_search.front());
                this->keys_search.pop();
            }
        }
        if (temp1 > 0) return 1;
        else return 0;
    }


//BKU
    template<class K,class V>
    void BKUTree<K, V>::add(K key, V value) {
        
        this->avl->add(key, value);
        this->splay->add(key, value);
        typename AVLTree::Node* avlNode = this->avl->search_Node( avl->root,key,value);
        typename SplayTree::Node* splayNode = this->splay->search_Node(splay->root, key, value);
        splayNode->corr = avlNode;
        avlNode->corr = splayNode;
        this->handle_add_queue(key);
        return;
    }
    template<class K, class V>
    void BKUTree<K, V>::remove(K key) {
        this->avl->remove(key);
        this->splay->remove(key);
        this->handle_remove_queue(key, this->splay->root->entry->key);
    
    };
    template<class K, class V>
    V BKUTree<K, V>::search(K key, vector<K>& traversedList) {
        if (this->splay->root->entry->key == key) return this->splay->root->entry->value;
        else if (this->handle_search_queue(key) == 1) {
            this->splay->search_splay_1_Splay_BKU(splay->root, key);

            // dung 1 vector de luu gia tri ma no da di qua 
            traversedList = this->splay->search_keys_Splay;
            this->splay->search_keys_Splay.clear();
        }
       
        else {
            

            // thao khao den cay AVL con co goc r'

            this->splay->search_splay_1_Splay_BKU(splay->root , (this->avl->search_Child_AVL_Tree( this->splay->root->corr, this->splay->root->corr, key))->corr->entry->key);

            //traversedList
            /*while (this->avl->keys_search_AVL.size() != 0) {
                traversedList.push_back(this->avl->keys_search_AVL.front());
                    this->avl->keys_search_AVL.pop();

            }*/
            traversedList = this->avl->keys_search_AVL;
            this->avl->keys_search_AVL.clear();

        }
    
        this->keys.push(key);
        if (this->keys.size() > this->maxNumOfKeys) { this->keys.pop(); }
    
    
    
    };


//
    void printKey(int key, int value) {
    cout << key << endl;
    }

////
  int main() {
    try {
        BKUTree<int, int>* tree = new BKUTree<int, int>();
        vector<int> abc;
        int keys[] = { 1, 3, 5, 7, 9, 2, 4 ,45 };
        for (int i = 0; i < 8; i++) tree->add(keys[i], keys[i] * 10);
        
        tree->search(5, abc);
        for (auto i = abc.begin(); i != abc.end(); i++) {
            cout << *i << ",";
        }
        cout << tree->avl->keys_search_AVL.size();
       
       /*
        tree->search(1, abc);
        for (auto i = abc.begin(); i != abc.end(); i++) {
            cout << *i << ",";
        }
        tree->print_queue_keys();*/
        //cout << tree->splay->root->corr->entry->key << "hfuh";

    }
    catch (const char* e) {
        cerr << e << '\n';
    }
}
