#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define left_child(x) (x * 2 + 1)
#define right_child(x) (x * 2 + 2)
#define parent(x) ((x - 1) / 2)
#define MAX 256 

typedef struct node {
    unsigned char character;
    unsigned int frequency;
    struct node* left_child;
    struct node* right_child;
} Node;

typedef struct tree {
    int size;
    Node** nodes; 
} Tree;

typedef struct code {
    char information[MAX];
    unsigned int bits;
} Code;

unsigned int *character_frequencies;
Code *character_codes;
Node **priority_queue;
Node *root_node;

void generate_codes(Node* current_node, char code[], int position);
Node* create_node(unsigned char character, unsigned int frequency, Node* left, Node* right);
void insert_node(Tree* tree, Node* node);
bool is_leaf(Node* node);
void swap_Nodes(Node **a, Node **b);
void heapify(Tree* tree, int index);
void build_heap(Tree* tree);
Node* remove_min_node(Tree *tree);

void swap_nodes(Node **a, Node **b)
{
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify(Tree* tree, int index) {
    int smallest = index;
    int left = left_child(index);
    int right = right_child(index);
    smallest = (left < tree->size && tree->nodes[left]->frequency < tree->nodes[smallest]->frequency) ? left : smallest;
    smallest = (right < tree->size && tree->nodes[right]->frequency < tree->nodes[smallest]->frequency) ? right : smallest;
    if (index != smallest) {
        swap_nodes(&tree->nodes[smallest], &tree->nodes[index]);
        heapify(tree, smallest);
    }
}

void build_heap(Tree* tree) {
    int i, n = tree->size;
    for (i = parent(n); i >= 0; i--)
        heapify(tree, i);
}

Node* remove_min_node(Tree *tree) {
    tree->size--;
    Node *tmp = tree->nodes[0];
    tree->nodes[0] = tree->nodes[tree->size - 1];
    heapify(tree, 0);
    return tmp;
}

void generate_codes(Node* current_node, char code[], int position) {
    if (current_node->left_child) {
        code[position] = '0';
        generate_codes(current_node->left_child, code, position + 1);
    }

    if (current_node->right_child) {
        code[position] = '1';
        generate_codes(current_node->right_child, code, position + 1);
    }

    if (is_leaf(current_node)) {
        code[position] = '\0';
        strcpy(character_codes[current_node->character].information, code);
        character_codes[current_node->character].bits = position;
    }
}

Node* create_node(unsigned char character, unsigned int frequency, Node* left, Node* right) {
    Node* new_node = (Node*)calloc(sizeof(Node), 1);
    new_node->character = character;
    new_node->frequency = frequency;
    new_node->left_child = left;
    new_node->right_child = right;
    return new_node;
}

void insert_node(Tree* tree, Node* node) {
    int index = tree->size;
    tree->size++;
    while (index && node->frequency < tree->nodes[parent(index)]->frequency) {
        tree->nodes[index] = tree->nodes[parent(index)];
        index = parent(index);
    }
    tree->nodes[index] = node;
}

bool is_leaf(Node* node) {
    return (node->left_child == NULL && node->right_child == NULL);
}



int main(int argc, char *argv[]) {
    int i;
    char outputFileName[50];
    int filenameLength;
    //start with input validation
    if (argc != 3 || ((strcmp(argv[1], "compress")) & (strcmp(argv[1], "decompress"))) != 0) {
        printf("Usage: ./huffman [compress or decompress] [filename]\n");
        return 1;
    }
    //open for read binary
    FILE *inputFile = fopen(argv[2], "rb");
    if (inputFile == NULL) {
        printf("File cannot be opened for reading\n");
        return 1;
    }
    // copy from argument into filename to store name to create output file's name
    strcpy(outputFileName, argv[2]);
    filenameLength = strlen(outputFileName);
    outputFileName[filenameLength - 3] = '\0';  //removes .--- extension for our own to be added.
    bool compress;
    FILE *outputFile;
    if (strcmp(argv[1], "compress") == 0) {
        strcat(outputFileName, "huff");      //only difference in this and else if is the ending of the file we create, based on argument
        outputFile = fopen(outputFileName, "wb");
        compress = true;
    } 
    else{                       //can use else because we already input validated
        strcat(outputFileName, "dhuff");
        outputFile = fopen(outputFileName, "wb");
        compress = false;
    } 
    if (outputFile == NULL) {
        printf("Cannot open file for writing\n");
        fclose(inputFile);
        return 1;
    }


    int result = 0;
    int num_c = 0;
    if(compress == true){
        //      -> need to find freq of each character
        unsigned char C;
        character_frequencies = (unsigned int*)calloc(sizeof(unsigned int),MAX);
        priority_queue = (Node**)calloc(sizeof(Node*), MAX);
        character_codes = (Code*)calloc(sizeof(Code),MAX);

        //      -> let character_frequencies store the frequencies of each character
        while((result = fread(&C, sizeof(unsigned char),1,inputFile)) > 0)
        {
            if(!character_frequencies[C]) 
            { 
                priority_queue[num_c] = create_node(C,0,NULL,NULL);
                num_c++; 
            }
            character_frequencies[C]++;    
        }

        //      -> need to make priority queue
        priority_queue = (Node**)realloc(priority_queue, sizeof(Node*)*num_c);   


        //      -> need to sort priority queue
        int j;
        Node* tmp;
        for(i = 0; i < num_c-1; i++) {
            for(j = 0; j < num_c-i-1; j++) {
                if(character_frequencies[priority_queue[j]->character] > character_frequencies[priority_queue[j+1]->character]) {       //if greater swap
                    tmp = priority_queue[j];
                    priority_queue[j] = priority_queue[j+1];
                    priority_queue[j+1] = tmp;
                } 
                else if (character_frequencies[priority_queue[j]->character] == character_frequencies[priority_queue[j+1]->character]){     //character frequencies equal, characters themselves are compared
                    if(priority_queue[j]->character > priority_queue[j+1]->character) {     //if greater swap
                        tmp = priority_queue[j];
                        priority_queue[j] = priority_queue[j+1];
                        priority_queue[j+1] = tmp;    
                    }
                }
            }
        }

        //      ->assign frequencies into the priority queue
        for(i = 0; i < num_c; i++) {
            priority_queue[i]->frequency = character_frequencies[priority_queue[i]->character];
        }


        //      -> make the tree
        Tree *tree = (Tree*)calloc(sizeof(Tree),1);
        tree->size = 0;
        tree->nodes = (Node**)calloc(sizeof(Node*),MAX);
        for(i = 0; i < num_c; i++)
        {
            tree->nodes[i] = create_node(priority_queue[i]->character, priority_queue[i]->frequency, NULL, NULL);
        }
        tree->size = num_c;

        //      ->convert tree to minHeap, lowest values are to be removed and added together, and put into a node
        build_heap(tree);

        //      ->remove two nodes
        Node *firstFreqToAdd;
        Node *secondFreqToAdd;
        Node *sumNode;
        while(tree->size > 1)  {    //while items are in the tree
            firstFreqToAdd = remove_min_node(tree); //lowst item should be removed and returned to firstFreqToAdd
            secondFreqToAdd = remove_min_node(tree);   //now lowest two removed, sum them together
            int sumFrequency = firstFreqToAdd->frequency+ secondFreqToAdd->frequency;
            sumNode = create_node(0, sumFrequency, firstFreqToAdd, secondFreqToAdd);
            //second freq should be greater than first freq, therefore right child
        }
        

        //      ->now tree size is 1, need to remove that node, and set it as root_node
        root_node = remove_min_node(tree);

        //output file will be formatted as frequencies first, followed by the codes
        //first step is to write frequencies to output file --> for testing can printf to write to stdout
        for(i = 0; i < MAX; i++)
            fwrite(&character_frequencies[i], sizeof(unsigned int), 1, outputFile);

        //we have written all frequencies, and traversed to the end of fpt, need to rewind to now do codes
        rewind(inputFile);


        //need to traverse to generate codes
        char generatedCode[MAX];
        int position;
        generate_codes(root_node, generatedCode, position);

        //now need to write codes


    /*
        -> start writing codes  
    */
    }
    else{
        /*  if decompressing
            ->
            ->
            ->                      */
    }



    fclose(inputFile);
    fclose(outputFile);
    for(i = 0; i < num_c; i++){
        free(priority_queue[i]);
    }
    free(priority_queue);
    free(character_frequencies);
    return 0;
}
