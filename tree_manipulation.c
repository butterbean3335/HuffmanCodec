#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

//functions being worked on --> ref Data Structures heapify
void heapify(Tree* tree, int index) {
    int smallest = index;
    int left = left_child(index);
    int right = right_child(index);

    if (left < tree->size && tree->nodes[left]->frequency < tree->nodes[smallest]->frequency)
        smallest = left;

    if (right < tree->size && tree->nodes[right]->frequency < tree->nodes[smallest]->frequency)
        smallest = right;

    if (index != smallest) {
        swap_nodes(&tree->nodes[smallest], &tree->nodes[index]);
        heapify(tree, smallest);
    }
}

void build_heap(Tree* tree) {
    int i, n = tree->size;

    for (i = parent(n); i >= 0; i--)
        heapify(tree, i);          //must heapify on every entry
}

Node* remove_min_node(Tree *tree) {    //min node equivalent to lowest node in sorted tree
    Node *tmp = tree->nodes[0];
    tree->nodes[0] = tree->nodes[tree->size - 1];
    tree->size--;
    heapify(tree, 0);

    return tmp;
}


void heapify(Tree* tree, int index);
void build_heap(Tree* tree);
Node* remove_min_node(Tree *tree);



int main(){
}
