#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

// extra people count.
int extra_count = 1;

// Data structure Define.
typedef struct _Data
{
    int tag;
    char date[11];
    char fee_paid[5];
    char name[20];
    int age;
    char organization[50];
    char job[20];
} Data;

// Linked list node structure.
typedef struct _Node
{
    Data data;
    struct _Node *next;
} Node;

// This function verifies file to prevent error.
void verify_file(FILE *fp, char *address)
{
    if (fp == NULL)
    {
        printf("Error opening file.\naddress: %s\n", address);
        exit(1);
    }
}

// This function verifies memory to prevent segmentation fault.
void verify_memory(void *ptr)
{
    if (ptr == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }
}

// This function opens file and returns file pointer.
FILE *open_file(char *address, char *mode)
{
    FILE *fp = fopen(address, mode);
    verify_file(fp, address);

    return fp;
}

// This function prints data.
void print_data(Data data)
{
    printf("%d/%s/%s/%s/%d/%s/%s \n", data.tag, data.date, data.fee_paid, data.name, data.age, data.organization, data.job);
}

// P1
// This function read raw data from file and store it in the data structure.
void read_data(FILE *fp, Data data[MAX])
{
    for (int i = 0; i < MAX; i++)
        fscanf(fp, "%d/%[^/]/%[^/]/%[^/]/%d/%[^/]/%s", &data[i].tag, data[i].date, data[i].fee_paid, data[i].name, &data[i].age, data[i].organization, data[i].job);
}

// P1
// This function write data to file.
void write_data(FILE *fp, Data data)
{
    fprintf(fp, "%d/%s/%s/%s/%d/%s/%s \n", data.tag, data.date, data.fee_paid, data.name, data.age, data.organization, data.job);
}

// P1
// This function sorts data by age using bubble sort.
void sort_data(Data data[MAX])
{
    Data temp;

    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX - i - 1; j++)
        {
            if (data[j].age > data[j + 1].age)
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// P2
// This function inserts a node into the linked list.
void insert_node(Node *head, Data data)
{
    Node *new = (Node *)malloc(sizeof(Node)), *ptr;
    verify_memory(new);

    // Insert data into new node
    new->data = data;
    new->next = NULL;
    ptr = head;

    // Find the last node
    while (ptr->next != NULL)
        ptr = ptr->next;

    ptr->next = new;
}

// P2
// This function displays the linked list using the sorted data.
void print_linked_list(Node *head)
{
    Node *current = head->next;

    while (current != NULL)
    {
        print_data(current->data);
        current = current->next;
    }

    printf("_________________________________________________________________\n\n");
}

// P3
// This function finds and displays fee-paid people.
void is_paid(Node *head)
{
    printf("_________________________________________________________________\n");
    printf("P3 \n");
    printf("Fee Paid List\n_________________________________________________________________\n");

    Node *current = head->next;

    while (current != NULL)
    {
        if (strcmp(current->data.fee_paid, "yes") == 0)
            print_data(current->data);

        current = current->next;
    }

    printf("_________________________________________________________________\n");
}

// P4
// This function deletes nodes with a specific job (ex: "staff").
void delete_nodes_with_job(Node *head, char *job)
{
    Node *current = head->next;
    Node *prev = head;

    while (current != NULL)
    {
        if (strcmp(current->data.job, job) == 0)
        {
            prev->next = current->next;
            free(current);
            current = prev->next;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

// P5
// This function adds extra human's information.
void add_human(Node *head, Data extra)
{
    extra_count++;

    // extra human's node.
    Node *extra_node = (Node *)malloc(sizeof(Node));
    verify_memory(extra_node);

    extra_node->data = extra, extra_node->next = NULL;

    Node *ptr = head->next, *prev_ptr = head;

    // extra human's node insert ( use linked list )
    while (ptr)
    {
        if ((ptr->data.age) < (extra_node->data.age))
        {
            prev_ptr = ptr;
            ptr = ptr->next;
        }
        else if ((ptr->next->data.age) >= (extra_node->data.age))
            break;
    }

    prev_ptr->next = extra_node;
    extra_node->next = ptr;
}

// This function makes free memory allocated for the linked list.
void free_linked_list(Node *head)
{
    Node *current = head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

int main()
{
    Data data[MAX];
    FILE *fp = open_file("registration_data.txt", "r");
    FILE *sorted_fp = open_file("output/P1.txt", "w");

    // P1
    read_data(fp, data);
    sort_data(data);

    // Write data to file
    for (int i = 0; i < MAX; i++)
        write_data(sorted_fp, data[i]);

    // Close file
    fclose(fp);
    fclose(sorted_fp);

    // P2
    //  Set dummy Head.
    Node *head = (Node *)malloc(sizeof(Node));
    verify_memory(head);
    head->next = NULL;

    // Making the linked list.
    for (int i = 0; i < MAX; i++)
        insert_node(head, data[i]);

    // Print the linked list.
    printf("_________________________________________________________________\n");
    printf("P2\n");
    printf("Linked List using the sorted data\n");
    printf("_________________________________________________________________\n");
    print_linked_list(head);

    // P3
    is_paid(head);

    // P4
    printf("_________________________________________________________________\n");
    printf("P4\n");
    printf("Remove staff\n");
    printf("_________________________________________________________________\n");
    delete_nodes_with_job(head, "staff");
    print_linked_list(head);

    // P5
    // extra human's data.
    Data extra = {MAX + extra_count, "2022-11-30", "yes", "Kang", 25, "Gachon University", "Student"};
    printf("_________________________________________________________________\n");
    printf("P5 \n");
    printf("Add human's data\n");
    printf("_________________________________________________________________\n");
    add_human(head, extra);
    print_linked_list(head);
    free_linked_list(head);
    printf("_________________________________________________________________\n");

    return 0;
}
