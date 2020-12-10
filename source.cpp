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
    int maxNumOfKeys;
    friend class AVLTree;
    friend class SplayTree;
    queue<K> keys_remove;


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
            if (y->left != nullptr) {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == nullptr) {
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
                node->parent = nullptr;
                node->left = nullptr;
                node->right = nullptr;
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
        }
        void add(Entry* entry) {
            if (check_Node_have_or_nohave(this->root, entry->key) == 0) {
                Node* node = new Node(entry->key, entry->value);
                node->parent = nullptr;
                node->left = nullptr;
                node->right = nullptr;
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
        }
        //remove 
        void remove(K key) {
            if (check_Node_have_or_nohave(this->root, key) == 1) {
                deleteNodeHelper(this->root, key);
            }
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
            s = nullptr;
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
                this->root = searchTree(key);
                cout << root->entry->value;
                return 0;
            }
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

            Node* node = new Node(key, value);

            if (root->entry->key > node->entry->key) { search_Node(root->left, key, value); }
            else if (root->entry->key < node->entry->key) { search_Node(root->right, key, value); }
            return node;
        }
    };
    class AVLTree {
    public:
        friend class BKUTree;
        class Node {
        public:
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
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
              
            };
            void add(Entry * entry) {
                if (check_Node_have_or_nohave(this->root, entry->key) == 0) {
                    this->root = node_Insert_add(this->root, entry->key, entry->value);
                }
            };
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

                    return search_Tree(this->root, key);
                }
            };
            V search_Tree(Node * root, K key) {


                if (root->entry->key > key) { search_Tree(root->left, key); }

                else if (root->entry->key < key) { search_Tree(root->right, key); }
                else { cout << root->entry->value; return 0; }
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
               
                    Node* node = new Node(key, value);
                   
                        if (root->entry->key > node->entry->key) { search_Node(root->left, key, value); }
                        else if (root->entry->key < node->entry->key) { search_Node(root->right, key, value); }
                        return node;
                    
                
               
            } 

        };

        // xu ly queue
    void handle_add_queue(K key);
    void handle_remove_queue(K key);
    void handle_search_queue(K key);

    // in ra keys de check thu khong nop 

    void print_queue_keys() {
        //cout << "*";
        //if (this->keys.size() == 0) return;
        //cout << this->keys.front();  //top();
        //    this->keys.pop();
        //    print_queue_keys();
        while (!this->keys.empty()) {
            cout << this->keys.front();
            this->keys.pop();
        }
      
    }
  
    };

    // xu ly queue 
    template<class K , class V>
    void BKUTree<K,V>::handle_add_queue( K key){
        if ((int)this->keys.size() == (int)this->maxNumOfKeys) {
            this->keys.pop();
            this->keys.push(key);
        }
        else { this->keys.push(key);}
       
        
    }
    template<class K , class V>
    void BKUTree<K,V>::handle_remove_queue(K key) {
        if (this->keys.size() != 0) {
            if (this->keys.front() != key) {
                this->keys_remove.push(this->keys.front());
                this->keys.pop();
                handle_remove_queue(key);
            }
            else { this->keys.pop(); 
                handle_remove_queue(key);
            }
        }
        else {
            this->keys = this->keys_remove;
           // this->keys.push(this->splay->root);

        }

     }







    template<class K,class V>
    void BKUTree<K, V>::add(K key, V value) {
        this->handle_add_queue(key);
        this->avl->add(key, value);
        this->splay->add(key, value);
        typename BKUTree<K, V>::AVLTree::Node* avlNode = this->avl->search_Node(this->avl->root, key, value);

       typename BKUTree<K, V>::SplayTree::Node* splayNode = this->splay->search_Node(this->splay->root, key, value);
        avlNode->corr = splayNode;
        splayNode->corr = avlNode;
        
      

        return;
    };
    template<class K, class V>
    void BKUTree<K, V>::remove(K key) {
        this->avl->remove(key);
        this->splay->remove(key);
       // this->handle_remove_queue(key);
    
    };
    template<class K, class V>
    V BKUTree<K, V>::search(K key, vector<K>& traversedList) {};




void printKey(int key, int value) {
    cout << key << endl;
}


int main() {
    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i] * 10);
   
    tree->traverseNLROnSplay(printKey);
    tree->traverseNLROnAVL(printKey);

}
