//Daniil Belousov
//Id 2491827
//Practice assignment 2

#include<time.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int main(int argc, char* argv[]) {
    int *arrayinfo,*doctor, i, time=0, isanyfree, check,check1=0;
    struct record* list, *queue;
    struct Node *p, *t, *pl;  // pointers to iterate
    arrayinfo=parseInput(argv); // getting the numbers from the command line
    doctor=(int*)malloc(arrayinfo[1]*sizeof(int)); // allocating memory for the doctors array

    queue=initialiseSimulator(queue, doctor, arrayinfo); // creating the queue and setting the doctors busy-values
    list=createPatientList(arrayinfo);  // creating randomly the list of patients
    p=list->head->next; // the pointer that keeps track of the patient in the list

    t=queue->head;// the pointer that keeps track of the patient in the queue

    ///star of the loop
    //running the code until every person is served
    while( check1 ==0) {
        // checking whether the doctors are free, and it will always free the doctor based on the current time
        // so we won't face issues with the patients that are served first
        // running the loop aas many times as we have doctors so we check every doctor one by one
        for(i=0;i < arrayinfo[1];i++){
            check = 0;
            pl = queue->head->next;
            // if the
            while (pl!= NULL) {
                if (pl->doctorsid == i && (pl->servicestarttime + pl->servicetime) == time) {
                    check=1;
                }
                pl = pl->next;
            }
            if (check == 1) {
                doctor[i] = 1;
            }
        }
        // using the loop to add the patient to the queue and using the while loop
        // because there might be patients that come at the same time
        // doing the loop when the patients' arrival time is equal to time and the list of patients is not empty
        while (p!=NULL && p->arrivaltime == time) {
            queue = newPatient(p, queue);
            p = p->next;
        }
        // using the loop to serve patients because we might have the patients that came at the same time
        //using the extra function for checking is any doctor is free at all
        while (t->next != NULL && Isanyfree(doctor, arrayinfo) == 1 && t->next->servicestarttime == 0) {
            queue = servePatient(queue, doctor, time, arrayinfo, list);
            t = t->next;
        }
        // incrementing the time
        time++;
        // checking the main loop condition, if the list of patients is finished and the queue is empty
        // and the last person is served
        if (t->next==NULL && p==NULL && t->servicestarttime!=0)
            check1=1;
    }
    reportStatistics(list, arrayinfo);// reporting the statistics
    // setting all the allocated memory free
    free(list);
    free(queue);
    free(doctor);
    free(arrayinfo);
    return 0;
}





// getting every input from the command line, making them integers
// and storing in the array for further use
int* parseInput(char* argv[]){
    int *arrayinfo;
    arrayinfo=(int*)malloc(4*sizeof(int));
    int i;
    for (i=0;i<4;i++){
        *(arrayinfo+i)=atoi(argv[i+1]);
    }
    return arrayinfo;
}
// creating the patients randomly and storing the in the list
struct record* createPatientList(int *arrayinfo){
    struct record *list;
    struct Node *temp, *p;
    int i, check;
    srand(time(NULL));
    // creating the empty list to use
    list=(struct record*)malloc(sizeof(struct record));
    list->head=(struct Node*)malloc(sizeof(struct Node));
    list->head->next=NULL;
    list->length=*(arrayinfo); // the first inpur from the command line is providing the number of patients
    // that is why we use that value to create the list of the size we need
    // creating patients one by one
    for(i=0;i<list->length;i++){
        check=0;
        p=list->head;
        temp=(struct Node*)malloc(sizeof(struct Node));
        temp->arrivaltime=rand()%(*(arrayinfo+2))+1;
        temp->servicetime=rand()%(*(arrayinfo+3))+1;
        temp->servicestarttime=0;
        temp->doctorsid=0;
        temp->insurance=rand()%2;
        temp->Colorcode=rand()%4+1;
        temp->next=NULL;
        // placing the patients in the correct order based on their arrival time and also
        //checking their colorcodes if they arrived at the same time, so the more urgent patients are placed first
        if(p->next==NULL)
            p->next=temp;
        else{
            while(p->next!=NULL && check!=1){
                // checking the arrival time
                if(p->next->arrivaltime<temp->arrivaltime)
                    p=p->next;
                    // checking the colorcode
                else if(p->next->arrivaltime==temp->arrivaltime && p->next->Colorcode<temp->Colorcode){
                    p=p->next;
                }
                else{
                    //placing the patient at the appropriate place and stopping the loop
                    temp->next=p->next;
                    p->next=temp;
                    check=1;
                }
            }
            // if the patient shouldnt be inserted anywhere in the list and should just go to the end
            if(check!=1 && p->next==NULL){
                temp->next=p->next;
                p->next=temp;
            }

        }
    }
    return list; // returning the final list
}

// creating the empty queue with dummy node
struct record* initialiseSimulator(struct record* queue,int *doctor, int *arrayinfo){
    int i;
    for(i=0;i<arrayinfo[1];i++){
        *(doctor+i)=1;
    }
    queue=(struct record*)malloc(sizeof(struct record));
    queue->head=(struct Node*)malloc(sizeof(struct Node));
    queue->tail=queue->head;
    queue->length=0;
    queue->head->next=NULL;
    return queue;
}
// adding the new patient to the queue
struct record* newPatient(struct Node *newpatient, struct record *queue) {
    struct Node *temp, *p;
    int check=0;
    // allocating memmory for the patient and copping the values from the patients list
    temp=(struct Node*)malloc(sizeof(struct Node));
    temp->arrivaltime=newpatient->arrivaltime;
    temp->Colorcode=newpatient->Colorcode;
    temp->servicetime=newpatient->servicetime;
    temp->insurance=newpatient->insurance;
    temp->next=NULL;
    queue->length++;
    // if the queue is empty inserting the patient straight to the queue even though the patient is served straight away
    // the queue hold every patients detail
    if (queue->head->next == NULL) {
        queue->head->next = temp;
    }
        // if there is already a record in the queue we start searching for the place to insert the newpatient
    else{
        p=queue->head;
        // firstly skipping all the patients that were served
        while(p->next!=NULL && check!=1){
            if(p->next->servicestarttime!=0)
                p=p->next;
            else
                check=1;
        }
        check=0;
        // secondly based on the colorcode inserting the patient in the correct order
        // so even if there is another patient that needs to be served, if the newpatients colorcode is more urgent
        // we inserted him before the other patients but not if the patients in the queue came befor and hacs the same or
        // equally importand colorcode
        while(p->next!=NULL && check!=1) {
            if(p->next->Colorcode<=temp->Colorcode && p->next->arrivaltime<=temp->arrivaltime)
                p = p->next;
            else
                check=1;
        }
        // inserting the patient in the proper space
        temp->next=p->next;
        p->next = temp;
    }
    // returning the queue after the patients insertion
    return queue;
}


// serving the patients in the queue
struct record*servePatient(struct record *queue, int *doctor, int time, int* arrayinfo, struct record *list){
    int id, check=0;
    struct Node *p,*t;
    p=queue->head->next;
    // finding the patient that was not served based on the queue
    while(p->arrivaltime<=time && p->servicestarttime!=0 && p->next!=NULL)
        p=p->next;
    // choosing the doctor randomly for the patient
    do{
        id=rand()%arrayinfo[1];
    }while(doctor[id]!=1);

    doctor[id]=0; // setting the doctors that was chosen busy
    // assigning the patient the values for the serving time and doctors id
    p->servicestarttime=time;
    p->doctorsid=id;
    queue->length--;
    t=list->head->next;
    //finding the value in the list and assigning the values to it
    while(check!=1 && t!=NULL){
        if (t->arrivaltime==p->arrivaltime && t->Colorcode==p->Colorcode && t->insurance==t->insurance && t->servicestarttime==0){
            t->servicestarttime=p->servicestarttime;
            t->doctorsid=p->doctorsid;
            check=1;
        }
        else
            t=t->next;
    }
    return queue;
}



/* the function that can be used to see the records
void printqueue(struct record *queue){
    struct Node *p;
    p=queue->head->next;
    printf("\n!!!!!!!!!!!!!!!!!!!!!!\n");
    while(p!=NULL){
        printf("arrivaltime->%d\n", p->arrivaltime);
        printf("servicetime->%d\n", p->servicetime);
        printf("servicestarttime->%d\n", p->servicestarttime);
        printf("doctorsid->%d\n", p->doctorsid+1); // in the array
        printf("insurance->%d\n", p->insurance);
        printf("Colorcode->%d\n", p->Colorcode);
        printf("\n");
        p=p->next;
    }
    printf("???????????????????????\n");
}
*/

// the function that checks if any doctor is free
int Isanyfree(int*doctor, int*arrayinfo){
    int isanyfree, j, k=0;
    for (j = 0; j < arrayinfo[1]; j++) {
        if (*(doctor + j) == 0)
            k++; // if value k is the same as the number of doctors then all the doctors are currently busy
        if (k == arrayinfo[1])
            isanyfree = 0;
        else
            isanyfree = 1;
    }
    return isanyfree;
}

// outputting the final operations data
void reportStatistics(struct record* queue, int* arrayinfo){
    printf("****************Report*****************\n");
    int k, z, maxwait=0, insurance=0, complition=0;
    float average=0;
    struct Node *temp;
    // iterating through the queue to find average, maximum waiting time, counting the insurances, and completion time
    temp=queue->head->next;
    while(temp!=NULL){
        average+=temp->servicestarttime-temp->arrivaltime;
        if (maxwait<(temp->servicestarttime-temp->arrivaltime))
            maxwait=temp->servicestarttime-temp->arrivaltime;
        if (complition<temp->servicestarttime+temp->arrivaltime)
            complition=temp->servicestarttime+temp->arrivaltime;
        insurance+=temp->insurance;
        temp=temp->next;
    }
    printf("The number of patients: %d\n", arrayinfo[0]);
    printf("The number of patients: %d\n", arrayinfo[1]);
    printf("Number of patients for each patient type:\n");
    // going through the list several times to see how many patients per each colorcode we had and displaying the numbers
    for(k=1;k<=arrayinfo[0];k++){
        z=0;
        temp=queue->head->next;
        while(temp!=NULL){
            if(k==temp->Colorcode)
                z++;
            temp=temp->next;
        }
        if(k==1)
            printf("\tred: %d\n",z);
        if(k==2)
            printf("\torange: %d\n",z);
        if(k==3)
            printf("\tyellow: %d\n",z);
        else if(k==4)
            printf("\tgreen: %d\n",z);
    }
    // going through the queue one more time to display the values of the doctors served patients
    printf("Number of patients for each Doctor:\n");
    for(k=0;k<arrayinfo[1];k++){
        z=0;
        temp=queue->head->next;
        while(temp!=NULL){
            if(k==temp->doctorsid)
                z++;
            temp=temp->next;
        }
        printf("\tDoctor %d:%d\n",k+1,z); //k+1 because in the program i used the value 0 for the first doctor
                                          // but to understand easier i output the doctors id starting for 1 not 0
    }
    // displaying the average, maximum waiting time, number of insurances, and completion time
    printf("Completion time: %d\n", complition);
    printf("Average time spent in the queue: %.1f\n",average/arrayinfo[0]);
    printf("Maximum waiting time: %d\n", maxwait);
    printf("Number of patients with insurance: %d\n", insurance);
    printf("Number of patients without insurance: %d\n",arrayinfo[0]-insurance);


}










