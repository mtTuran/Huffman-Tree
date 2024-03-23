typedef struct leaf_node
{
    char letter;
    int recurrence;
} leaf_node;

typedef struct tree_node
{
    int total_recurrence;
    union
    {
        struct tree_node* right_tree;
        struct leaf_node* right_leaf;
    } right;
    
    union
    {
        struct tree_node* left_tree;
        struct leaf_node* left_leaf;
    } left;
} tree_node;
