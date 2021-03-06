#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    struct Node *next;
    void *value;
}node_t;

typedef struct Patient{
    struct Patient *next;   //pointer to next patient
    node_t *diseases;  //pointer to a list of patient's diseases
    node_t *tail;      //pointer to last of patient's diseases
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
static disease_t *tailDiseases = NULL;  //pointer to last disease in hospital
static patient_t *tailPatients = NULL;  //pointer to last patient in hospital

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
        current->tail = NULL;
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
    disease_t **current = &hospital->diseases;
    disease_t *new = malloc(sizeof(disease_t));
    new->next = NULL;
    new->referenceCount = 0;
    new->description = malloc((strlen(description) + 1) * sizeof(char));
    new->id = counterOfDiseases++;
    strcpy(new->description, description);
    numberOfGlobalDiseases++;
    if (*current == NULL){
        *current = new;
        tailDiseases = new;
        return *current;
    }
    tailDiseases->next = new;
    tailDiseases = tailDiseases->next;
    return tailDiseases;
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
            tailDiseases = previous; //current is last node, so after deleting it
                                     //previous is going to be the last one
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
            tailDiseases = hospital->diseases;
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

void addNewDiseaseToPatient(patient_t **patient, node_t **list, disease_t *disease){
    node_t **current = list;
    node_t *new = malloc(sizeof(node_t));
    new->next = NULL;
    new->value = (void *)disease;
    increaseReferenceCount(disease);
    if(*current == NULL){
        *current = new;
        (*patient)->tail = new;
        return;
    }
    (*patient)->tail->next = new;
    (*patient)->tail = (*patient)->tail->next;
    return;
}

disease_t *findLastAddedDisease(patient_t **patient){
    if((*patient)->tail != NULL){
        return (disease_t *)((*patient)->tail->value);
    }
    else{
        return NULL;
    }
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
    addNewDiseaseToPatient(&patient, &patient->diseases, newDisease);
    printf("OK\n");
    printNumberOfGlobalDiseases();
}

void newDiseaseCopyDescription(char *name1, char *name2){
    patient_t *target = findPatient(name1);
    if (target == NULL){ //target patient not fount - create it
        target = addNewPatient(name1);
    }
    patient_t *source = findPatient(name2);
    if (target != NULL && source != NULL){ //if not found - both pointers are NULLs
        disease_t *foundDisease = findLastAddedDisease(&source);
        if(foundDisease != NULL){
            //we have to add found disease to the target patient
            addNewDiseaseToPatient(&target, &target->diseases, foundDisease);
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
        patient->tail = NULL;
        printf("OK\n");
    }
    else{
        printf("IGNORED\n");
    }
    printNumberOfGlobalDiseases();
}
