
#include <stddef.h> // NULL
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free

#include <time.h> // clock

/**
 * The task is to reverse a linked list
 * 
 * Algo 1: you can use extra memory (heap or stack)
 * Algo 2: you cannot use heap memory
 * Algo 3: you cannot use heap and you cannot park data on the stack
 * 
 * For algo 3. Of course you can use stack memory to call a function but 
 * recursive approaches that scale heap usage with the length of the linked 
 * list is not allowed. Think of a device were heap memory is limited.
 */


struct node_t {
    int value;
    struct node_t *next;
};

typedef struct node_t node_t;

void algo_1_in_place_reverse_linked_list(node_t *head);
void algo_2_in_place_reverse_linked_list(node_t* n);
node_t *hop_forward(node_t *node, int n);
void flip(node_t *n1, node_t *n2);

void print_array(int *values, int length);
int *copy_list_to_array(node_t *head, int *length);
int get_length(node_t *head);
node_t *initialize_list(int length);
void print_list(node_t *head);


void algo_1_in_place_reverse_linked_list(node_t *head) {
    /* algorithm idea:
     *  - put all node values in an array
     *  - iterate forward over the list while taking the values out of the array in reverse order
     */
    int length = 0;
    int *values = copy_list_to_array(head, &length);
    for (int i = length-1; i >= 0; i--) {
        head->value = values[i];
        head= head->next;
    }
    free(values);
}

node_t *ghead = NULL;
void algo_2_in_place_reverse_linked_list(node_t* n) {
    /* algorithm idea:
     *  - instead of copy the values to an array we recurse and have the values on the stack
     *  - when leaving the recursion we move forward through the list and place the value in the current head
     */
    if (n == NULL) return;
    int n_val = n->value;
    algo_2_in_place_reverse_linked_list(n->next);
    // n->value = ghead->value; <-- we could do this and stop in the middle
    ghead->value = n_val;
    ghead = ghead->next;
}

void algo_3_in_place_reverse_linked_list(node_t *head) {
    /*
     * algorithm idea:
     *  - no scaling of memory usage with the size of the list allowed
     *  - so we first count the length of the list -> len(list)
     *  - loop i = 0  to len(list)/2:
     *     - run len(list)-i times forward -> n
     *     - flip(head, n)
     *     - head = head->next
     */
    int l = get_length(head);
    for (int i = 0; i < l/2; i++) {
        node_t *n = hop_forward(head, l - i*2 - 1); // always a bit tricky to get the - 1 right
        flip(head, n);
        
        head = head->next;
    }
}

node_t *hop_forward(node_t *node, int n) {
    for (int i = 0; i < n; i++) {
        if (node == NULL) return NULL;
        node = node->next;
    }
    return node;
}

void flip(node_t *n1, node_t *n2) {
    int tmp = n1->value;
    n1->value = n2->value;
    n2->value = tmp;
}


void print_array(int *values, int length) {
    for (int i =0; i < length; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");
}

int *copy_list_to_array(node_t *head, int *length) {

    int l = get_length(head);
    // printf("len: %d\n", l);
    int *values = malloc(l * sizeof(int));
    for (int i = 0; i < l; i++) {
        values[i] = head->value;
        head = head->next;
    }
    *length = l;
    return values;
}

int get_length(node_t *head) {
    int l = 0;
    while (head != NULL) {
        l++;
        head= head->next;
    }
    return l;
}

node_t *initialize_list(int length) {
    node_t *head = NULL;
    for (int i = length-1; i >= 0; i--) {
        node_t *n = (node_t *) malloc(sizeof(node_t));
        n->value = i;
        n->next = head;
        head = n;
    }
    return head;
}

void print_list(node_t *head) {

    return ;

    while (head != NULL) 
    {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

enum direction_t {
    up,
    down
};

typedef enum direction_t direction_t;

int check_array(node_t *head, direction_t direction) {
    switch (direction)
    {
    case up:
        {
            int val = 0;
            while (head != NULL) {
                if (head->value != val) { return 0; }
                head = head->next;
                val++;
            }
        }
        break;
    case down:
        {
            int val = get_length(head) - 1;
            while (head != NULL) {
                if (head->value != val) { return 0; }
                head = head->next;
                val--;
            }
        }
        break;
    } /* end switch */
    return 1; 
}

void test_algos(int list_len) {
    clock_t start_time = clock();
    node_t *head = initialize_list(list_len);
    clock_t end_time = clock();

    //__clock_t CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;
    //double timespan0 = (double) (end_time - start_time) / CLOCKS_PER_MS;
    clock_t timespan0 = (end_time - start_time);
    
    if (!check_array(head, up)) { printf("Error in initializing array\n"); }
    print_list(head);

    start_time = clock();
    algo_1_in_place_reverse_linked_list(head);
    end_time = clock();
    // double timespan1 = (double) (end_time - start_time) / CLOCKS_PER_MS;
    clock_t timespan1 = (end_time - start_time);

    if (!check_array(head, down)) { printf("Error in algo 1 \n"); }
    print_list(head);

    start_time = clock();
    ghead = head;
    algo_2_in_place_reverse_linked_list(head);
    end_time = clock();
    // double timespan2 = (double) (end_time - start_time) / CLOCKS_PER_MS;
    clock_t timespan2 = (end_time - start_time);
    
    if (!check_array(head, up)) { printf("Error in algo 2 \n"); }
    print_list(head);
    
    start_time = clock();
    algo_3_in_place_reverse_linked_list(head);
    end_time = clock();
    // double timespan3 = (double) (end_time - start_time) / CLOCKS_PER_MS;
    clock_t timespan3 = (end_time - start_time);

    if (!check_array(head, down)) { printf("Error in algo 3 \n"); }
    print_list(head);

    printf(
        // "%d,\t %f,\t %f,\t %f,\t %f\n", 
        "%6d,\t %6ld,\t %6ld,\t %6ld,\t %8ld\n", 
        list_len, timespan0, timespan1, timespan2, timespan3);

}

int main(int argc, char **argv) {
    fprintf(stderr, "Output numbers are in clock speed, i.e. one micro second (us = 1/1,000,000 seconds)\n");
    printf("   len,\t   init,\t algo_1,\t algo_2,\t algo_3\n");
    for (int i = 1; i < argc; i++) {
        int list_len = atoi(argv[i]);
        test_algos(list_len);
    }
    return 0;
}