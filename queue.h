//Daniil Belousov
//Id 2491827
//Practice assignment 2

struct record{
    struct Node *head;
    struct Node *tail;
    int length;
};
struct Node{
    int arrivaltime;
    int servicetime;
    int servicestarttime;
    int doctorsid;
    int insurance;
    int Colorcode;
    struct Node *next;
};

int* parseInput(char* argv[]);
struct record* createPatientList(int *);
struct record* initialiseSimulator(struct record* ,int *, int *);
struct record* newPatient(struct Node *, struct record *);
struct record*servePatient(struct record *, int *, int , int* , struct record *);
void printqueue(struct record *queue);
int Isanyfree(int*, int*);
void reportStatistics(struct record* , int* );
