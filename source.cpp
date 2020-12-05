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

        Entry(K key, V value) : key(key), value(value) {}
    };

private:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) {}
    ~BKUTree() { this->clear(); }

    void add(K key, V value) ;
    void remove(K key);
    V search(K key, vector<K>& traversedList);

    void traverseNLROnAVL(void (*func)(K key, V value));
    void traverseNLROnSplay(void (*func)(K key, V value));

    void clear();
    bool check_keys(K key) {
      if(keys.)
    
    };  //check xem key da ton tai chua 

    class SplayTree {
    public:
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;

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

        void add(K key, V value) {
            Node* new_node = new Node();
            new_node->entry->key = key;
            new_node->entry->value = value;
            Node* insert(root, key);
        };
        void add(Entry* entry) {
            Node* new_node = new Node();
            new_node->entry->key = entry->key;
            new_node->entry->value = entry->value;
            Node* insert(root, key);
        };
        void remove(K key) { delete_Node_Tree_SPlay(root, key); };
        V search(K key) { splay(root, key); };

        void traverseNLR(void (*func)(K key, V value)) {};

        void clear() { deleteTree_Splay(&root); };
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
            if (root == NULL || root->key == key)
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
                else if (root->right->key < key)// Zag-Zag (Right Right)  
                {
                    root->right->right = splay(root->right->right, key);
                    root = leftRotate(root);
                }
                return (root->right == NULL) ? root : leftRotate(root);
            }
        }

        Node* insert(Node* root, K key)
        {

            if (root == NULL) return newNode(key);

            // Bring the closest leaf node to root  
            root = splay(root, key);

            // If key is already present, then return  
            if (root->entry->key == key) return root;

            Node* newnode = newNode(key);

            if (root->entry->key > key)
            {
                newnode->right = root;
                newnode->left = root->left;
                root->left = NULL;
            }

            else
            {
                newnode->left = root;
                newnode->right = root->right;
                root->right = NULL;
            }

            return newnode; // newnode becomes new root  
        }
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
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;

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

        void add(K key, V value) {
            Node* new_node = new Node();
            new_node->entry->key = key;
            new_node->entry->value = value;
            node_Insert_add(new_node, key);
        };
        void add(Entry* entry) {
            Node* new_node = new Node();
            new_node->entry->key = entry->key;
            new_node->entry->value = entry->value;
            node_Insert_add(new_node, key);
        };
        void remove(K key) { remove_node_Tree(root, key); };
        V search(K key) { search_Tree(root, key); };
       
        void traverseNLR(void (*func)(K key, V value)) {};

        void clear() { deleteTree(&root); };

        //
        int max(K a, K b) { return (a > b) ? a : b; };
        int getHeightRec(Node* node) {
            if (node == NULL)
                return 0;
            int lh = this->getHeightRec(node->pLeft);
            int rh = this->getHeightRec(node->pRight);
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
        Node* node_Insert_add(Node* node, K key) {
            // add
            if (node == NULL) return (new Node(key));
            if (key < node->entry->key) {
                node->left = node_Insert_add(node->left, key);
            }
            else if (key >= node->entry->key) {
                node->right = node_Insert_add(node->right, key);
            }
            // modify
            node->balance = getBalance(node);
            //case left_left
            if (node->balance > 1 && node->entry->key <= node->left->entry->key) { return rotate_Right(node); }
            //case  right_right
            if (balance < -1 && key >= node->right->entry->key)
                return rotate_Left(node);
            //case right_of_left
            if (node->balance < -1 && key <= node->right->entry->key)
            {
                node->right = rotate_Right(node->right);
                return rotate_Left(node);
            }
            //case left_of_right
            if (balance > 1 && key >= node->left->entry->key)
            {
                node->left = rotate_Left(node->left);
                return rotate_Right(node);
            }
            return node;

        };
        Node* maxValueNode(Node* node)
        {
            Node* current = node;

            while (current->right != NULL)
                current = current->right;

            return current;
        }
        Node* delete_Node(Node* node, K key) {

            if (node == NULL)
                return node;
            if (key < node->entry->key)
                node->left = delete_Node(node->left, key);

            else if (key > node->entry->key)
                node->right = delete_Node(node->right, key);

            else
            {
                if ((node->left == NULL) || (node->right == NULL))

                {
                    Node* temp = node->left ? node->left : node->right;
                    if (temp == NULL)
                    {
                        temp = node;
                        node = NULL;
                    }
                    else    *node = *temp;
                    free(temp);
                }
                else
                {

                    Node* temp = maxValueNode(node->left);
                    node->entry->key = temp->key;

                    node->left = delete_Node(node->left, temp->key);
                }
            }

            if (node == NULL)
                return node;

            int balance = getBalance(node);


            // Left Left Case  
            if (balance > 1 && getBalance(node->left) >= 0)

                return rotate_Right(node);

            // Left Right Case  
            if (balance > 1 && getBalance(node->left) < 0)

            {
                node->left = rotate_Left(node->left);
                return rotate_Right(node);
            }

            // Right Right Case  
            if (balance < -1 && getBalance(node->right) <= 0)

                return rotate_Left(node);

            // Right Left Case  
            if (balance < -1 && getBalance(node->right) > 0)

            {
                node->right = rotate_Right(node->right);
                return rotate_Left(node);
            }

            return node;
        }
        V search_Tree(Node* root, K key) {

            if (root == NULL)return -1;
            if (key == root->entry->key) return root->entry->value;
            else if (key > root->entry->key) {
                return search_Tree(root->right, key);
            }
            else return search_Tree(root->left, key);
            return -1;
        }
        void remove_node_Tree(Node* root, K key) {

            if (root == NULL)return -1;
            if (key == root->entry->key) { delete_Node(root, key); }
            else if (key > root->entry->key) {
                return remove_node_Tree(root->right, key);
            }
            else return remove_node_Tree(root->left, key);
            return -1;

        }
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

////hien thuc BKU Tree
template<class K , class V>
void BKUTree<K,V>::add(K key, V value) {
    if (keys.size >= maxNumOfKeys) { keys.pop(); }

    keys.push(key);
    splay->add(key, value);
    avl->add(key, value);


};
template<class K, class V>
void remove(K key) {};
template<class K, class V>
V search(K key, vector<K>& traversedList) {};
template<class K, class V>
void traverseNLROnAVL(void (*func)(K key, V value)) {};
template<class K, class V>
void traverseNLROnSplay(void (*func)(K key, V value)) {};
template<class K, class V>
void clear() {};

int main() {

}
