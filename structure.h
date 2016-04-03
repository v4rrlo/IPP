#ifndef STRUCTURE_H_   /* Include guard */
#define STRUCTURE_H_


void initStructure();
void tearDownStructure();
void newDiseaseEnterDescription(char *, char *);
/*  0.check if name exists
    1.find if name is already in structure
     1a.if not, then addNewPatient(name);
    2.  addNewDiseaseToPatient(name, description);
        create new disease with given description
            add new node to global list of diseases
            initialize new node with diseases description updated
            and it's referenceCount set to 0
        add new Node to patients name diseaseList
        set new Node's *value pointer to address of created disease
        increase referenceCount of new Disease by 1
*/
void newDiseaseCopyDescription(char *, char *);
/*  0.check if name2 and name1 exists
    1.find last added disease to name2
        findLastAddedDisease(name) -> returns address to that disease
    2.add that disease to name1 (by pointing and increasing referenceCount)
        add new node to patients name1 diseaseList
        set its *value pointer to that found disease from step 1
        set that pointer to given address
*/
void changeDescription(char *, int, char *);
/*  0.check if name exists
    1.Create new Disease with given description
    2.add it to global list of diseases
    3.find n-th disease in name's diseaseList (if exists)
     3a.if doesn't exist -> IGNORED
     3b.otherwise set its value pointer to new disease
    4.decrease referenceCount of that n-th found disease
*/
void printDescription(char *, int);
/*  0.check if name exists
    1.find n-th description in name's list (if exists)
    2.print n-th description
*/
void deletePatientData(char *);
/*  0. check if name exists
    1. iterating through name's diseaseList free all Nodes and decrease
        referenceCount of diseases on the list
*/
#endif // STRUCTURE_H_
