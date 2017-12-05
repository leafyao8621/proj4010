typedef struct LinkedList LinkedList;
typedef struct Node Node;

LinkedList* new_list(void);//creates a new list
int add_list(LinkedList* l, void* data);//adds a node to a list with the data
void* pop(LinkedList* l);//removes the last item in a list
void* dequeue(LinkedList* l);//removes the first item in a list
void* get(LinkedList* l, int ind);//gets an arbitrary item from a list
int remove_ind(LinkedList* l, int ind);//removes an arbitrary node from a list
int get_size(LinkedList* l, int* opt);//gets the size of a list
int free_list(LinkedList* l);//frees memory occupied
int print_list(LinkedList* l);//prints a representation
int handle_print(void* data);//handles printing representation of the data
