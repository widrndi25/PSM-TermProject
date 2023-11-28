#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

// extra people count
int extra_count = 1;

// Data structure Define
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

// Linked list node structure
typedef struct _Node
{
    Data data;
    struct _Node *next;
} Node;

// This function opens file and returns file pointer
FILE *open_file(char *address, char *mode)
{
    FILE *fp = fopen(address, mode);
    if (fp == NULL)
    {
        printf("Error opening %s file.\n", address);
        exit(1);
    }

    return fp;
}

// P1
// This function splits raw data into data structure
void split_data(char raw_data[MAX][1000], Data data[MAX])
{
    for (int i = 0; i < MAX; i++)
    {
        int count = 0;

        // Split raw data by '/'
        char *token = strtok(raw_data[i], "/");
        char tokens[7][100] = {0};

        while (count < 7)
        {
            strcpy(tokens[count], token);
            token = strtok(NULL, "/");
            count++;
        }

        data[i].tag = atoi(tokens[0]);
        strcpy(data[i].date, tokens[1]);
        strcpy(data[i].fee_paid, tokens[2]);
        strcpy(data[i].name, tokens[3]);
        data[i].age = atoi(tokens[4]);
        strcpy(data[i].organization, tokens[5]);
        strcpy(data[i].job, tokens[6]);
    }
}

// P1
// This function sorts data by age using bubble sort
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
    if (new == NULL)
    {
        printf("Error allocating memory. function: insert_node\n");
        exit(1);
    }
    // Insert data into new node
    new->data = data;
    new->next = NULL;
    ptr = head;

    // Find the last node
    while (ptr->next != NULL)
        ptr = ptr->next;

    ptr->next = new;
}

// P3
// This function finds fee-paid people
void is_paid(Node *head)
{
    printf("P2 \n");
    printf("Fee Paid List\n_________________________________________________________________\n");

    Node *current = head->next;

    while (current != NULL)
    {
        if (strcmp(current->data.fee_paid, "yes") == 0)
            printf("%d/%s/%s/%s/%d/%s/%s", current->data.tag, current->data.date, current->data.fee_paid,
                   current->data.name, current->data.age, current->data.organization, current->data.job);

        current = current->next;
    }

    printf("_________________________________________________________________\n");
}

// P5
// This function adds extra human's information
void add_human(Node *head)
{
    // open file
    FILE *fp = open_file("data/registration_data.txt", "a");
    if (fp == NULL)
    {
        printf("Error Opening the file. (function: add_human) \n");
        exit(1);
    }

    // extra human's data
    Data extra = {MAX + extra_count, "2022-11-30", "yes", "Kang", 25, "Gachon University", "Student"};

    // 문제파악 오류 파일에 쓰는 것이 아닌 Linked list에 추가후 출력해야함.
    fprintf(fp, "%d/%s/%s/%s/%d/%s/%s", extra.tag, extra.date, extra.fee_paid, extra.name, extra.age, extra.organization, extra.job);

    extra_count++;

    fclose(fp);

    // extra human's node
    Node *extra_node = (Node *)malloc(sizeof(Node));
    if (extra_node == NULL)
    {
        printf("Error allocating memory! (extra_node) \n");
        exit(1);
    }

    extra_node->data = extra, extra_node->next = NULL;

    Node *ptr = head->next, *prev_ptr = head;

    // extra human's age check
    // FIX sort 함수를 call 해서 사용하면 더 좋을듯 합니다.
    while (ptr)
    {
        if ((ptr->next->data.age) < (extra_node->data.age))
        {
            prev_ptr = ptr;
            ptr = ptr->next;
        }
        else if ((ptr->next->data.age) >= (extra_node->data.age))
        {
            break;
        }
    }

    prev_ptr->next = extra_node;
    extra_node->next = ptr;
}

// This function makes free memory allocated for the linked list
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
    char raw_data[30][1000];
    Data data[MAX];
    FILE *fp = open_file("data/registration_data.txt", "r");
    FILE *sorted_fp = open_file("data/P1.txt", "w");

    // Read data from file
    for (int i = 0; i < MAX; i++)
        fgets(raw_data[i], 1000, fp);

    split_data(raw_data, data);
    sort_data(data);

    // Write data to file
    for (int i = 0; i < MAX; i++)
        fprintf(sorted_fp, "%d/%s/%s/%s/%d/%s/%s", data[i].tag, data[i].date, data[i].fee_paid, data[i].name, data[i].age, data[i].organization, data[i].job);

    // Close file
    fclose(fp);
    fclose(sorted_fp);

    // Set dummy Head
    Node *head = (Node *)malloc(sizeof(Node));
    if (head == NULL)
    {
        printf("Error allocating memory. location: Set dummy head)\n");
        exit(1);
    }
    head->next = NULL;

    // Making the linked list
    for (int i = 0; i < MAX; i++)
        insert_node(head, data[i]);

    // function call
    is_paid(head);
    add_human(head);
    free_linked_list(head);

    return 0;
}
