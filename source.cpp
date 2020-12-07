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

public:
    BKUTree(int maxNumOfKeys = 5) {}
    ~BKUTree() { this->clear(); }
   

    //void add(K key, V value) ;
    //void remove(K key);
    //V search(K key, vector<K>& traversedList);

    //void traverseNLROnAVL(void (*func)(K key, V value)) {
    //    this->avl->traverseNLR(*func);
    //};
    //void traverseNLROnSplay(void (*func)(K key, V value));

    //void clear();
    
   
    class SplayTree {
    public:
        class Node {
        public : 
            Entry* entry;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;
            Node(K key, V value) {
                this->entry = new Entry(key, value);
                //this->balance = 0;
                this->right = NULL;
                this->left = NULL;
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
       
        ////
        Node* rightRotate(Node* x)
        {
            Node* y = x->left;
            x->left = y->right;
            y->right = x;
            return y;
        }

        Node* leftRotate(Node* x)
        {
            Node* y = x->right;
            x->right = y->left;
            y->left = x;
            return y;
        }
        Node* splay(Node* root, K key)
        {
            if (root == NULL || root->entry->key == key)
                return root;

            if (root->entry->key > key)
            {
                if (root->left == NULL) return root;

                // Zig-Zig (Left Left)  
                if (root->left->entry->key > key)
                {
                    root->left->left = splay(root->left->left, key);
                    root = rightRotate(root);
                }
                else if (root->left->entry->key < key) // Zig-Zag (Left Right)  
                {

                    root->left->right = splay(root->left->right, key);

                    if (root->left->right != NULL)
                        root->left = leftRotate(root->left);
                }
                return (root->left == NULL) ? root : rightRotate(root);
            }
            else
            {
                if (root->right == NULL) return root;

                // Zig-Zag (Right Left)  
                if (root->right->entry->key > key)
                {

                    root->right->left = splay(root->right->left, key);

                    if (root->right->left != NULL)
                        root->right = rightRotate(root->right);
                }
                else if (root->right->entry->key < key)// Zag-Zag (Right Right)  
                {
                    root->right->right = splay(root->right->right, key);
                    root = leftRotate(root);
                }
                return (root->right == NULL) ? root : leftRotate(root);
            }
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
        //add
        void add(K key, V value) {
            if (check_Node_have_or_nohave(root, key) == 0) {
                this->root = insert(this->root, key, value);
           }
        };
        void add(Entry* entry) {
            if (check_Node_have_or_nohave(root, entry->key) == 0) {
                this->root = insert(this->root, entry->key, entry->value);
            }
        };
        Node* insert(Node* root,  K key, V value)
        {
            Node* new_node = new Node(key , value);
            if (root == NULL) return new_node;
            root = splay(root, key);
            if (root->entry->key == key) return root;

            if (root->entry->key > key)
            {
                new_node->right = root;
                new_node->left = root->left;
                root->left = NULL;
            }

            else
            {
                new_node->left = root;
                new_node->right = root->right;
                root->right = NULL;
            }

            return new_node; // newnode becomes new root  
        }
        // remove 
        void remove(K key) { delete_Node_Tree_SPlay(root, key); };
        Node* delete_Node_Tree_SPlay(Node* root, int key)
        {
            Node* temp = new Node();
            if (!root)
                return NULL;
            root = splay(root, key);
            if (key != root->key)
                return root;
            if (!root->left)
            {
                temp = root;
                root = root->right;
            }

            else
            {
                temp = root;
                root = splay(root->left, key);
                root->right = temp->right;
            }
            free(temp);

            return root;

        }
        //search
        V search(K key) { splay(root, key); };
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

            _deleteTree(node->left);
            _deleteTree(node->right);

            delete node;
        }

        void deleteTree_Splay(Node** node_ref)
        {
            _deleteTree(*node_ref);
            *node_ref = NULL;
        }
       
 
    }; 
   

    class AVLTree {
    public:
        friend class BKUTree;
        class Node {
        public : 
            Entry* entry;
             Node* left;
            Node* right;
            int balance;
           // typename SplayTree::Node* corr;
            Node(  K key , V value ) {
                  this->entry = new Entry(key, value);
                  this->balance = 0;
                  this->right = NULL;
                  this->left = NULL;
             }

            Node(Entry* entry =NULL , Node* left = NULL, Node* right = NULL) {
              
                this->entry = entry; 
               
                this->left = left;
                this->right = right;
                this->balance = 0;
              //  this->corr = NULL;
                
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
        int getHeight() {
            return this->getHeightRec(this->root);
        }
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
            
            if (root == NULL) {  return 0; }
            
            if (key == root->entry->key) {  return 1; }
            else if (key > root->entry->key) {
                return check_Node_have_or_nohave(root->right, key);
            }
            else return check_Node_have_or_nohave(root->left, key);
           
            return 0;
            
        }
        // add
        void add(K key, V value) { 
            if (check_Node_have_or_nohave(this->root, key) == 0) { 
                
                this->root = node_Insert_add(this->root, key, value);
            } 
        };
        void add(Entry* entry) {
            if (check_Node_have_or_nohave(this->root, entry->key) == 0) {
                this->root = node_Insert_add(this->root, entry->key, entry->value);
            }
        };
        Node* node_Insert_add(Node* root, K key, V value) {

            Node* new_node = new Node(key, value);
            if (root == NULL) return new_node;

            if (key < root->entry->key) {
                root->left = node_Insert_add(root->left, key, value);
            }
            else if (key > root->entry->key) {
                root->right = node_Insert_add(root->right, key, value);
            }
            // modify
            root->balance = getBalance(root);
            //case left_left
            if (getBalance(root) > 1 && root->entry->key <= root->left->entry->key) { return rotate_Right(root); }
            //case  right_right
            if (getBalance(root) < -1 && key >= root->right->entry->key)
                return rotate_Left(root);
            //case right_of_left
            if (getBalance(root) < -1 && key <= root->right->entry->key)
            {
                root->right = rotate_Right(root->right);
                return rotate_Left(root);
            }
            //case left_of_right
            if (getBalance(root) > 1 && key >= root->left->entry->key)
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
        Node* remove_node_Tree(Node* root, K key) {
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
        Node* maxValueNode(Node* node)
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
        V search_Tree(Node* root, K key) {


            if (root->entry->key > key) { search_Tree(root->left, key); }

            else if (root->entry->key < key) { search_Tree(root->right, key); }
            else { cout << root->entry->value; return 0; }
        }

        //clear 
        void clear() { deleteTree(&root); };

        void _deleteTree(Node* node)
        {
            if (node == NULL) return;

            _deleteTree(node->left);
            _deleteTree(node->right);

            delete node;
        }

        void deleteTree(Node** node_ref)
        {
            _deleteTree(*node_ref);
            *node_ref = NULL;
        }


    };
   
};

//////hien thuc BKU Tree
//template<class K , class V>
//void BKUTree<K,V>::add(K key, V value) {
//   //if (keys.size() == maxNumOfKeys) { keys.pop(); }
//    //this->keys.push(key);
//    //this->splay->add(key, value);
//    this->avl->add(key, value);
//
//
//};
//template<class K, class V>
//void BKUTree<K, V>::remove(K key) {
//
//};
//template<class K, class V>
//V BKUTree<K, V>::search(K key, vector<K>& traversedList) {};
////template<class K, class V>
////void BKUTree<K, V>::traverseNLROnAVL(void (*func)(K key, V value)) {
////
////};
//template<class K, class V>
//void BKUTree<K, V>::traverseNLROnSplay(void (*func)(K key, V value)) {};
//template<class K, class V>
//void BKUTree<K, V>::clear() {
//    this->avl->clear();
//    this->splay->clear();
//};


void printKey(int key, int value) {
    cout << key << endl;
}


int main() {
   
    BKUTree<int, int>::SplayTree* tree = new BKUTree<int, int>::SplayTree();
    int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    //tree->traverseNLROnAVL(printKey);
    //tree->traverseNLROnSplay(printKey);
    //for (int i = 0; i < 9; i++) tree->search(keys[i]);
    //tree->add(100, 128);
    //tree->remove(100);
   
    //tree->clear();
    cout << "------------------------------" << endl;
    //tree->search(100);
    tree->traverseNLR(printKey); 
    //
    //BKUTree<int, int>::AVLTree* avl = new BKUTree<int, int>::AVLTree();
    //int keys[] = { 1, 3, 5, 7, 9, 2, 4 };
    //for (int i = 0; i < 7; i++) avl->add(keys[i], keys[i]);
    ////avl.printTreeStructure();
    //avl->traverseNLR(printKey);


}
