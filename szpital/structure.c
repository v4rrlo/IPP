#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    struct Node *next;
    void *value;
}node_t;

typedef struct Patient{
    struct Patient *next;   //pointer to next patient
    struct Node *diseases;  //pointer to a list of patient's diseases
    struct Node *tail;      //pointer to last of patient's diseases
    char *name;
}patient_t;

typedef struct Disease{
    struct Disease *next;   //pointer to next disease
    char *description;      //description of a disease
    int referenceCount;     //reference count of a disease
    int id;                 //unique id of disease needed for searching
}disease_t;

typedef struct Hospital{
    patient_t *patients;  //list of patients
    disease_t *diseases;  //list of all of the diseases
}hospital_t;

static hospital_t *hospital = NULL;
static disease_t *tailDiseases = NULL;
static patient_t *tailPatients = NULL;

static int debugMode = 0;
static int numberOfGlobalDiseases = 0;
static int counterOfDiseases = 0; //needed for giving unique ids to diseases

void increaseReferenceCount(disease_t *);
void decreaseReferenceCount(disease_t *);

/*  *********************************   */
/*  |     INITIALIZE STRUCTURE      |   */
/*  *********************************   */
void initStructure(int debug){
    if(debug){
        debugMode = 1;
    }
    if (hospital == NULL){
        hospital = malloc(sizeof(struct Hospital));
        hospital->patients = NULL;
        hospital->diseases = NULL;
        numberOfGlobalDiseases = 0;
    }
}

/*  *********************************   */
/*  |     TEARING DOWN STRUCTURE    |   */
/*  *********************************   */
void tearDownPatientsDiseases(node_t **list){
    node_t *current = *list;
    node_t *next;
    while(current != NULL){
        next = current->next;
        decreaseReferenceCount((disease_t *)current->value);
        free(current);
        current = next;
    }
    *list = NULL;
}

void tearDownPatients(patient_t **patient){
    patient_t *current = *patient;
    patient_t *next;
    while(current != NULL){
        tearDownPatientsDiseases(&current->diseases);
        next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    *patient = NULL;
}

void tearDownDiseases(disease_t **disease){
    disease_t *current = *disease;
    disease_t *next;
    while(current != NULL){
        next = current->next;
        free(current->description);
        free(current);
        current = next;
    }
    *disease = NULL;
}

void tearDownStructure(){
    if (hospital != NULL){
        if(hospital->patients != NULL)
            tearDownPatients(&hospital->patients);
        if(hospital->diseases != NULL)
            tearDownDiseases(&hospital->diseases);
        free(hospital);
    }
}

/*  *********************************   */
/*  |  PATIENT-SPECIFIC FUNCTIONS   |   */
/*  *********************************   */
void printPatients(){
    puts("Printing patients");
    patient_t *current = hospital->patients;
    //puts("PRE");
    while(current != NULL){
        printf("\t%s\n", current->name);
        current = current->next;
    }
    //puts("AFTER");
}

patient_t *findPatient(char *name){
    patient_t *current = hospital->patients;
    while (current != NULL){
        if (strcmp(current->name, name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

patient_t *addNewPatient(char *name){
    patient_t **current = &hospital->patients;
    patient_t *new = malloc(sizeof(patient_t));
    new->next = NULL;
    new->diseases = NULL;
    new->tail = NULL;
    new->name = malloc((strlen(name) + 1) * sizeof(char));
    new->name = strcpy(new->name, name);
    if (*current == NULL){
        *current = new;
        tailPatients = new;
        return *current;
    }
    tailPatients->next = new;
    tailPatients = tailPatients->next;
    return tailPatients;
}

/*  *********************************   */
/*  |  DISEASE-SPECIFIC FUNCTIONS   |   */
/*  *********************************   */
disease_t *addNewDisease(char *description){
    disease_t *current = (disease_t *)&hospital->diseases;
    disease_t *new = malloc(sizeof(disease_t));
    new->next = NULL;
    new->referenceCount = 0;
    new->description = malloc((strlen(description) + 1) * sizeof(char));
    new->id = counterOfDiseases++;
    strcpy(new->description, description);
    numberOfGlobalDiseases++;
    if (current){
        while (current->next != NULL){
            current = current->next;
        }
        current->next = new;
    }
    else{
        current = new;
    }
    return new;
}

//THIS FUNCTION IS ALWAYS CALLED FROM INSIDE OF decreaseReferenceCount
//SO WE ARE SURE THAT SUCH A DISEASE EXISTS
void deleteDisease(int idDescription){
    disease_t *current = hospital->diseases;
    disease_t *previous = NULL;
    while (current->id != idDescription){
        previous = current;
        current = current->next;
    }
    //current == disease so we have to delete it
    if (previous){
        if (current->next){
            previous->next = current->next;
        }
        else{
            previous->next = NULL;
        }
        free(current->description);
        free(current);
    }
    else{   //previous == NULL, so we haven't got to the inside of while loop
            //it means that current is our target
        if(current->next){
            hospital->diseases = current->next;
        }
        else{
            hospital->diseases = NULL;
        }
        free(current->description);
        free(current);
    }
}

void increaseReferenceCount(disease_t *disease){
    disease->referenceCount++;
}

void decreaseReferenceCount(disease_t *disease){
    disease->referenceCount--;
    if (disease->referenceCount == 0){
        deleteDisease(disease->id);
        numberOfGlobalDiseases--;
    }
}

void addNewDiseaseToPatient(node_t **list, disease_t *disease){
    node_t *current = (node_t*)list;
    node_t *new = (node_t *)malloc(sizeof(node_t));
    new->next = NULL;
    new->value = (void *)disease;
    increaseReferenceCount(disease);
    if (current){
        while (current->next != NULL){
            current = current->next;
        }
        current->next = new;
    }
    else{
        current = new;
    }
}

disease_t *findLastAddedDisease(char *name){
    patient_t *current = findPatient(name);
    // we found given patient -- current != NULL because we are
    // sure that given patient exists
    node_t *currDisease = NULL;
    if (current->diseases != NULL){
        currDisease = (node_t *)&current->diseases;
    }
    else{
        return NULL;
    }

    while (currDisease->next != NULL){
        currDisease = currDisease->next;
    }
    // next is NULL, so we have found last patient's disease
    return (disease_t *)currDisease->value;
}

node_t *findNthPatientsDisease(node_t **list, int n){
    node_t *current = (node_t *)list;
    int i = 0;
    while (current != NULL){
        if (i == n){
            return current;
        }
        current = current->next;
        i++;
    }
    return NULL;
}

void printNumberOfGlobalDiseases(){
    if(debugMode){
        fprintf(stderr, "DESCRIPTIONS: %d\n", numberOfGlobalDiseases);
    }
}

/*  *********************************   */
/*  |         CORE FUNCTIONS        |   */
/*  *********************************   */
void newDiseaseEnterDescription(char *name, char *description){
    patient_t *patient = findPatient(name);
    disease_t *newDisease = addNewDisease(description); //creating new disease
    //patient = NULL because we haven't found him
    //we need to create him and add the disease
    if(patient == NULL){
        patient = addNewPatient(name);
    }
    addNewDiseaseToPatient(&patient->diseases, newDisease);
    printf("OK\n");
    printNumberOfGlobalDiseases();
}

void newDiseaseCopyDescription(char *name1, char *name2){
    patient_t *first = findPatient(name1);
    if (first == NULL){ //first patient not fount - create it
        first = addNewPatient(name1);
    }
    patient_t *second = findPatient(name2);
    if (first != NULL && second != NULL){ //if not found - both pointers are NULLs
        disease_t *foundDisease = findLastAddedDisease(name2);
        if(foundDisease != NULL){
            //we have to add found disease to the first patient
            addNewDiseaseToPatient(&first->diseases, foundDisease);
            printf("OK\n");
        }
        else{
            printf("IGNORED\n");
        }
    }
    else{   //either one or both of patients doesn't exist in the structure
        printf("IGNORED\n");
    }
    printNumberOfGlobalDiseases();
}


void changeDescription(char *name, int n, char *description){
    patient_t *patient = findPatient(name);
    node_t *nDisease = NULL;
    if (patient != NULL){
        nDisease = findNthPatientsDisease(&patient->diseases, n);
    }
    if (patient == NULL || nDisease == NULL){   //patient or disease not found
        printf("IGNORED\n");
    }
    else{
        disease_t *new = addNewDisease(description);
        decreaseReferenceCount((disease_t *)nDisease->value);
        nDisease->value = (void *)new;
        increaseReferenceCount(new);
        printf("OK\n");
    }
    printNumberOfGlobalDiseases();
}

void printDescription(char *name, int n){
    patient_t *patient = findPatient(name);
    node_t *pDisease = NULL;
    if (patient != NULL){
        pDisease = (node_t *)findNthPatientsDisease(&patient->diseases, n);
    }
    if (patient != NULL && pDisease != NULL){
        //found both patient and his nth disease
        disease_t *disease = (disease_t *)pDisease->value; //just for printing
        printf("%s\n", (char *)disease->description);
    }
    else{
        printf("IGNORED\n");
    }
    printNumberOfGlobalDiseases();
}

void deletePatientData(char *name){
    patient_t *patient = findPatient(name);
    if (patient != NULL){
        tearDownPatientsDiseases(&patient->diseases);
        printf("OK\n");
    }
    else{
        printf("IGNORED\n");
    }
    printNumberOfGlobalDiseases();
}
