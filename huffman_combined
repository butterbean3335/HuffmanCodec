/*
Ben Walsh and Max Savage
Huffman Encoding
Lab 4
Embedded Computing
March 5, 2024
*/
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
void makeMinHeap(Tree* tree);
Node* remove_min_node(Tree *tree);
void printBinary(unsigned char byte);

void printBinary(unsigned char byte) {
    for(int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}


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
    if(left < tree->size && tree->nodes[left]->frequency < tree->nodes[smallest]->frequency)
        smallest = left;
    else if(right < tree->size && tree->nodes[right]->frequency < tree->nodes[smallest]->frequency)
        smallest = right;
    if (index != smallest) {
        swap_nodes(&tree->nodes[smallest], &tree->nodes[index]);
        heapify(tree, smallest);
    }
}

void makeMinHeap(Tree* tree) {
    int i, n = tree->size;
    for (i = parent(n); i >= 0; i--)
        heapify(tree, i);
}

Node* remove_min_node(Tree *tree) {
    Node *tmp = tree->nodes[0];
    tree->nodes[0] = tree->nodes[tree->size - 1];
    tree->size--;
    heapify(tree, 0);
    return tmp;
}

void generate_codes(Node* current_node, char code[], int position) {
    if (current_node->left_child) {
        code[position] = '0';                                                   //left traversal is 0
        generate_codes(current_node->left_child, code, position + 1);
    }

    if (current_node->right_child) {
        code[position] = '1';                                                   //right traversal is 1
        generate_codes(current_node->right_child, code, position + 1);
    }

    if (is_leaf(current_node)) {
        code[position] = '\0';
        strcpy(character_codes[current_node->character].information, code);     // writes code to information member of character_codes
        character_codes[current_node->character].bits = position;               //bits are = position because if you end in the third position, say the numbers 101, thats 3 bits
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
    unsigned char C;
    character_frequencies = (unsigned int*)calloc(sizeof(unsigned int),MAX);
    priority_queue = (Node**)calloc(sizeof(Node*), MAX);
    character_codes = (Code*)calloc(sizeof(Code),MAX);
    if(compress == true){
        //      -> need to find freq of each character
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
        tree->nodes = (Node**)calloc(sizeof(Node*),MAX);        //max changed from num_c to ignore 0 frequencie and make tree make nodes even for 0 freq
        for(i = 0; i < num_c; i++)
        {
            tree->nodes[i] = create_node(priority_queue[i]->character, priority_queue[i]->frequency, NULL, NULL);
        }
        
        tree->size = num_c;


        //      ->convert tree to minHeap, lowest values are to be removed and added together, and put into a node
        makeMinHeap(tree);

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
            insert_node(tree, sumNode);
        }
        root_node = remove_min_node(tree);

        //output file will be formatted as frequencies first, followed by the codes
        //first step is to write frequencies to output file --> for testing can printf to write to stdout
        for(i = 0; i < MAX; i++){
            fwrite(&character_frequencies[i], sizeof(unsigned int), 1, outputFile);
            printf("Freq for i = %d (0x%02X): %d\n", i, i, character_frequencies[i]);
            //if i = 65, i is character A, and the frequency of occurance is stored at character_frequencies[i]
        }

        //we have written all frequencies, and traversed to the end of inputFile, need to rewind to now do codes
        rewind(inputFile);
        
        //need to traverse to generate codes
        char generatedCode[MAX];
        int position = 0;
        
        //seg fault is occuring right here
        generate_codes(root_node, generatedCode, position);         //-> has put codes into character_codes.info

        //now need to write codes
        // we have character_codes which is storing the info, being the code, and the bits for each code, or the length
        unsigned char readInBuffer;
        bool bit;
        int codeLength = 0;
        int bufferLength = 0;
        int lengthStepper = 0;
        // Print character codes
        for (int i = 0; i < MAX; i++) {
            if (character_codes[i].bits > 0) {
                printf("Character: %c, Code: %s\n", i, character_codes[i].information);
            }
        }

        while(fread(&C, sizeof(unsigned char), 1, inputFile) > 0){
            //first step is to get the code to reference
            strcpy(generatedCode, character_codes[C].information);
            codeLength = character_codes[C].bits;
            //number of bits is equal to length of coded msg
            while(bufferLength < 8){
                //8 bits for an ascii character
                if(lengthStepper >= codeLength){
                    //we have stepped through the whole code, this code didnt take up 8 bits
                    lengthStepper = 0;  //set to 0 for next code
                    break;      //break causes program to go back to fread and gets next character to reference code table
                }
                readInBuffer  = readInBuffer << 1;  //shift left 1 to read in next bit of code
                if(generatedCode[lengthStepper] == '1')
                    bit = 1;
                else
                    bit = 0;
                readInBuffer |= bit;        //set the bit in place of buffer, will need to shift now
                bufferLength++;
                //we added a bit to buffer, need to increase it's length
                lengthStepper++;
                //current step in length needs to be incremented, next bit of code needed
                if(bufferLength >=8){
                    //buffer is full, shouldnt encounter greater than case, but includes all values of bufferLength now
                    fwrite(&readInBuffer, sizeof(unsigned char), 1, outputFile);
                    readInBuffer = 0x00;       //clear the buffer, done reading 
                    bufferLength = 0;           //set bufferLength to 0
                }
            }
        }
        //now done reading from file, need to make sure buffer is empty
        if(bufferLength != 0){   //things were left in the buffer
            //need to shift buffer over however many bits are left over
            readInBuffer = readInBuffer << (8-bufferLength);    
            fwrite(&readInBuffer, sizeof(unsigned char), 1, outputFile);
        }
    }
    else{
        // decompression
        character_frequencies = (unsigned int*)calloc(sizeof(unsigned int), MAX);
        unsigned int freq;
        priority_queue = (Node**)calloc(sizeof(Node*), MAX);
        int num_c = 0;
        int result;

        // first 256 unsigned ints worth of data in the file is the character frequencies
        for (int c=0; c<256; c++) {
            if (result = fread(&freq, sizeof(unsigned int), 1, inputFile) > 0) {
                character_frequencies[c] = freq;

                // fill the priority queue with all characters included in the compressed file
                if (character_frequencies[c] != 0) {
                    priority_queue[num_c] = create_node(c, freq, NULL, NULL);
                    num_c++;
                }
                printf(".%d ", c);
            }
        }

        // reallocate to get rid of unnecessarily allocated memory
        priority_queue = (Node**)realloc(priority_queue, sizeof(Node*) * num_c); 

        // sort priority queue
        Node* tmp;
        for (int l=0; l<num_c-1; l++) {
            for (int m=0; m<num_c-l-1; m++) {
                // check if this character has a higher frequency than the next
                if (character_frequencies[priority_queue[m]->character] > character_frequencies[priority_queue[m+1]->character]) {
                    // swap these two nodes in the priority queue
                    tmp = priority_queue[m];
                    priority_queue[m] = priority_queue[m+1];
                    priority_queue[m+1] = tmp;
                }
                
                // check if this character has a greater ASCII value than the next
                else if (priority_queue[m]->character > priority_queue[m+1]->character) {
                    // swap these two nodes in the priority queue
                    tmp = priority_queue[m];
                    priority_queue[m] = priority_queue[m+1];
                    priority_queue[m+1] = tmp;
                }
            }
        }

        // fill frequency information into the priority_queue's nodes (prob should have just done this before sorting ngl)
        for (int c=0; c<num_c; c++) {
            priority_queue[c]->frequency = character_frequencies[priority_queue[c]->character];
        }

        // construct binary search tree
        Tree *tree = (Tree*)calloc(sizeof(Tree), 1);  // should be same size as priority_queue right?
        tree->size = 0;
        tree->nodes = (Node**)calloc(sizeof(Node*), MAX);
        for(int c=0; c<num_c; c++)
        {
            tree->nodes[c] = create_node(priority_queue[c]->character, priority_queue[c]->frequency, NULL, NULL);
        }
        tree->size = num_c;

        // convert tree to minheap to facilitate node combinations
        makeMinHeap(tree);

        // do node combinations as required by huffman codec
        Node *lowest;
        Node *secondLowest;
        Node *sumNode;
        int sumFrequency = 0;

        while(tree->size > 0)  { 
            lowest = remove_min_node(tree); // remove lowest item in the tree
            secondLowest = remove_min_node(tree);   // remove second lowest item in the tree
            sumFrequency = lowest->frequency + secondLowest->frequency;
            sumNode = create_node(0, sumFrequency, lowest, secondLowest);

            // if only one more node in the tree, that node is the root_node
            if(tree->size == 1){
                root_node = remove_min_node(tree);
            }
            printf(".");
        }

        // the next byte of data begins the codes
        unsigned char readIn;
        unsigned char buffer;
        bool bit;
        int lengthStepper = 7;
        int bufferLength = 0;
        Node *rover = root_node;

        while (fread(&readIn, sizeof(unsigned char), 1, inputFile) > 0) {
            while (lengthStepper >= 0) {
                // once lengthStepper gets below zero, read in next byte
                // if not finished with current byte keep checking bits in byte
                buffer = readIn >> lengthStepper;

                // check if the bit at position lengthStepper is 0 or 1
                if (buffer & 0b00000001 == 0b00000001) {
                    bit = 1;
                } else {
                    bit = 0;
                }

                if (bit == 1) {
                    // traverse to the right
                    rover = rover->right_child;

                } else if (bit == 0) {
                    // traverse to the left
                    rover = rover->left_child;
                }
                printf("%d ", lengthStepper);

                // check if rover has any children, if it does, still more bits in code
                if (rover->right_child == NULL && rover->left_child == NULL) {
                    // rover does not have any children, this is the character we want to print
                    printf("!");
                    fwrite(&(rover->character), sizeof(unsigned char), 1, outputFile);

                    // reset rover
                    rover = root_node;
        
                }
                lengthStepper--;

            }

            // reset lengthStepper for next byte
            lengthStepper = 7;
        }
        
        free(tree->nodes);
        free(tree);
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
