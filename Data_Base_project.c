#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>

#define MAX_SIZE 2000
#define MEDIUM_SIZE MAX_SIZE/2
#define ELEMENT_SIZE 50

typedef struct Attribut{
    char *Attribut_name;
    int ID_Attribut;
    int Primary_Key;
}Attribut;

typedef struct fonct_depend{
    char *Dependancy_string;
    char *Determinant_string;
    char *Determiner_string;
    Attribut *Tab_determinants;
    Attribut *Tab_determiners;
    int Determinant_nbr;
    int Determiner_nbr;
}fonct_depend;

typedef struct Relation_info{
    char *Relation_Name;
    char *All_Attributs_string;
    int Number_of_Attributs;
    Attribut *Tab_Attributs;
    int Number_of_Primary_keys;
    int Number_of_fonc_depend;
    fonct_depend *Tab_fonc_depend;
    int * ID_Primary_Keys ;
}Relation_info;

int separate_Attributes(Relation_info * R);
void print_Relation(Relation_info *R);
fonct_depend* insert_fonc_depend(Relation_info * R);
fonct_depend* separate_Fonc_depend(Relation_info * R,int i);
void print_fonc_depend(Relation_info* R);
int verify_existance_deta(Relation_info* R,int i);
int verify_existance_dete(Relation_info* R,int i);
void primary_keys(Relation_info* R );
void know_keys(Relation_info* R);
int checkFunctionalDependency(Relation_info* R,char *combination);
void free_Relation_info(Relation_info *R);
void print_keys (Relation_info* R);

int separate_Attributes(Relation_info *R) {
    int nbr_Att = 0;
    char *All_Atts_copy = malloc(MAX_SIZE * sizeof(char));
    strcpy(All_Atts_copy, R->All_Attributs_string);
    char *tokens = strtok(All_Atts_copy, ",");
    R->Tab_Attributs = malloc(MAX_SIZE * sizeof(Attribut));
    while (tokens != NULL && nbr_Att < MAX_SIZE) {
        R->Tab_Attributs[nbr_Att].Attribut_name = strdup(tokens);
        R->Tab_Attributs[nbr_Att].ID_Attribut = nbr_Att + 1;
        tokens = strtok(NULL, ",");
        nbr_Att++;
    }
    free(All_Atts_copy);
    return nbr_Att;
}

void print_Relation(Relation_info *R) {
    printf("\t\t\t\t%s (%s)\n ", R->Relation_Name, R->All_Attributs_string);
}

fonct_depend* insert_fonc_depend(Relation_info *R) {
    R->Tab_fonc_depend = malloc(R->Number_of_fonc_depend * sizeof(fonct_depend));
    int i, o;
    int existe_deta, existe_dete;
    for (i = 0; i < R->Number_of_fonc_depend; i++) {
        R->Tab_fonc_depend[i].Tab_determinants = malloc(MAX_SIZE * sizeof(Attribut));
        R->Tab_fonc_depend[i].Tab_determiners = malloc(MAX_SIZE * sizeof(Attribut));
        o = 0;
        do {
            if (o == 1) {
                if (existe_deta == 0 && existe_dete != 0) {
                    printf("\t\t\t\t\aONE OF DETERMINANTS DOES NOT EXIST WITH THE ATTRIBUTS DECLARED PREVENTLY \n"
                           "\t\t\t\tYOU HAVE TO REENTER THE FUNCTIONAL DEPENDENCY : \n");

                } else if (existe_deta != 0 && existe_dete == 0) {
                    printf("\t\t\t\t\aONE OF DETERMINERS DOES NOT EXIST WITH THE ATTRIBUTS DECLARED PREVENTLY \n"
                           "\t\t\t\tYOU HAVE TO REENTER THE FUNCTIONAL DEPENDENCY : \n");
                } else {
                    printf("\t\t\t\t\aBOTH DETERMINANTS AND DETERMINERS DOES NOT EXIST WITH THE ATTRIBUTS DECLARED PREVENTLY \n"
                           "\t\t\t\tYOU HAVE TO REENTER THE FUNCTIONAL DEPENDENCY : \n");
                }
            }
            printf("\t\t\t\t\t(%d)  ", i + 1);
            R->Tab_fonc_depend[i].Dependancy_string = malloc(MAX_SIZE * sizeof(char));
            scanf("%s", R->Tab_fonc_depend[i].Dependancy_string);
            R->Tab_fonc_depend = separate_Fonc_depend(R, i);
            existe_deta = verify_existance_deta(R, i);
            existe_dete = verify_existance_dete(R, i);
            o = 1;
        } while (existe_deta == 0 || existe_dete == 0);
    }
    return R->Tab_fonc_depend;
}

int verify_existance_deta(Relation_info *R, int i) {
    int j, b;
    int f = 0;
    for (j = 0; j < R->Tab_fonc_depend[i].Determinant_nbr; j++) {
        for (b = 0; b < R->Number_of_Attributs; b++) {
            if (strcmp(R->Tab_Attributs[b].Attribut_name, R->Tab_fonc_depend[i].Tab_determinants[j].Attribut_name) ==0) {
                f++;
                break;
            }
        }
    }
    return ((f == R->Tab_fonc_depend[i].Determinant_nbr) ? 1 : 0);
}

int verify_existance_dete(Relation_info *R, int i) {
    int j, b;
    int f = 0;
    for (j = 0; j < R->Tab_fonc_depend[i].Determiner_nbr; j++) {
        for (b = 0; b < R->Number_of_Attributs; b++) {
            if (strcmp(R->Tab_Attributs[b].Attribut_name, R->Tab_fonc_depend[i].Tab_determiners[j].Attribut_name) ==0) {
                f++;
                break;
            }
        }
    }
    return ((f == R->Tab_fonc_depend[i].Determiner_nbr) ? 1 : 0);
}

fonct_depend* separate_Fonc_depend(Relation_info * R , int i) {
    int j;
    char *Single_depend = strdup(R->Tab_fonc_depend[i].Dependancy_string);
    // Separating determinants and determiners
    char* Half = strtok(Single_depend, "->");
    R->Tab_fonc_depend[i].Determinant_string = strdup(Half);
    char *determinants_copy= malloc(MEDIUM_SIZE* sizeof(char));
    strcpy(determinants_copy,R->Tab_fonc_depend[i].Determinant_string);

    Half = strtok(NULL, "->");
    R->Tab_fonc_depend[i].Determiner_string = strdup(Half);
    char *determiners_copy= malloc(MEDIUM_SIZE* sizeof(char));
    strcpy(determiners_copy,R->Tab_fonc_depend[i].Determiner_string);

    // Processing determinants
    char* Deta = strtok(determinants_copy, ",");
    int nbr_determinant = 0;
    while (Deta != NULL) {
        R->Tab_fonc_depend[i].Tab_determinants[nbr_determinant].Attribut_name = strdup(Deta);
        for(j=0 ;j<R->Number_of_Attributs ;j++) {
            if(strcmp(R->Tab_fonc_depend[i].Tab_determinants[nbr_determinant].Attribut_name,R->Tab_Attributs[j].Attribut_name)==0) {
                R->Tab_fonc_depend[i].Tab_determinants[nbr_determinant].ID_Attribut = R->Tab_Attributs[j].ID_Attribut;
                break;
            }
        }
        nbr_determinant++;
        Deta = strtok(NULL, ",");
    }
    R->Tab_fonc_depend[i].Determinant_nbr = nbr_determinant;
    // Processing determiners
    char* Dete = strtok(determiners_copy, ",");
    int nbr_determiner = 0;
    while (Dete != NULL) {
        R->Tab_fonc_depend[i].Tab_determiners[nbr_determiner].Attribut_name = strdup(Dete);
        for(j=0 ;j<R->Number_of_Attributs ;j++) {
            if(strcmp(R->Tab_fonc_depend[i].Tab_determiners[nbr_determiner].Attribut_name,R->Tab_Attributs[j].Attribut_name)==0) {
                R->Tab_fonc_depend[i].Tab_determiners[nbr_determiner].ID_Attribut = R->Tab_Attributs[j].ID_Attribut;
                break;
            }
        }
        nbr_determiner++;
        Dete = strtok(NULL, ",");
    }
    R->Tab_fonc_depend[i].Determiner_nbr = nbr_determiner;
    free(Single_depend); // Freeing the allocated memory for Single_depend
    free(determinants_copy);
    free(determiners_copy);
    return R->Tab_fonc_depend;
}

void print_fonc_depend(Relation_info* R) {
    int i ;
    for (i=0;i<R->Number_of_fonc_depend;i++){
        printf("\t\t\t\t\t(%d) %s ==> %s\n",i+1,R->Tab_fonc_depend[i].Determinant_string,R->Tab_fonc_depend[i].Determiner_string);
    }
}

// Function to print all combinations of attributes
void know_keys(Relation_info* R) {
    char *combinations[1 << R->Number_of_Attributs]; // Array to store combinations
    int num_combinations = 0;
    int shortest_length = INT_MAX; // Initialize shortest length to a large value
    for (int i = 1; i < (1 << R->Number_of_Attributs); i++) { // Iterate over all possible combinations
        char combination[100] = ""; // Buffer to hold the combination
        for (int j = 0; j < R->Number_of_Attributs; j++) {
            if (i & (1 << j)) { // Check if jth bit is set in the current combination
                strcat(combination, R->Tab_Attributs[j].Attribut_name); // Append the attribute
                strcat(combination, ","); // Append comma to separate attributes
            }
        }
        combinations[num_combinations++] = strdup(combination); // Store the combination
    }
    // Iterate over combinations to find the shortest length
    for (int i = 0; i < num_combinations; i++) {
        if (checkFunctionalDependency(R, combinations[i]) == 1) {
            int length = strlen(combinations[i]);
            if (length < shortest_length) {
                shortest_length = length; // Update shortest length if a shorter combination is found
            }
        }
    }
    // Print combinations with the shortest length
    printf("\t\t\t\tTHE KEYS ARE : \n");
    for (int i = 0; i < num_combinations; i++) {
        if (strlen(combinations[i]) == shortest_length && checkFunctionalDependency(R, combinations[i]) == 1) {
            printf("\t\t\t\t\t(%s\b)\n", combinations[i]); // Print the combination
        }
        free(combinations[i]); // Free memory allocated for the combination
    }
}

// Function to check if a combination satisfies a functional dependency
int checkFunctionalDependency(Relation_info* R,char *combination) {
    int nbr_Att = 0;
    int * tmp_IDs = malloc(sizeof(int )*R->Number_of_Attributs);
    char *All_Atts_copy = malloc(MAX_SIZE * sizeof(char));
    strcpy(All_Atts_copy, combination);
    char *tokens = strtok(All_Atts_copy, ",");
    while (tokens != NULL && nbr_Att <= R->Number_of_Attributs) {
        for(int i =0;i<R->Number_of_Attributs;i++){
            if(strcmp(R->Tab_Attributs[i].Attribut_name,tokens)==0){
                tmp_IDs[nbr_Att++]= R->Tab_Attributs[i].ID_Attribut;
            }
        }
        tokens = strtok(NULL, ",");
    }
    int modified = 1;
    while (modified) {
        modified = 0;
        for (int i = 0; i < R->Number_of_fonc_depend; i++) {
            int all_determinant_matched = 1; // Flag to track if all determinants are present
            for (int j = 0; j < R->Tab_fonc_depend[i].Determinant_nbr; j++){
                int determinant_matched = 0;
                for (int k = 0; k < nbr_Att; k++){
                    if (tmp_IDs[k] == R->Tab_fonc_depend[i].Tab_determinants[j].ID_Attribut) {
                        // If any determinant is missing in the combination, set the flag to 0
                        determinant_matched = 1;
                        break;
                    }
                }
                // If determinant is not in the closure, mark the flag and break
                if (!determinant_matched) {
                    all_determinant_matched = 0;
                    break;
                }
            }
            if(all_determinant_matched){
                for(int j=0;j<R->Tab_fonc_depend[i].Determiner_nbr;j++){
                    int determiner_already_existe =0 ;
                    // Check if the determiner is already in the closure
                    for(int k=0; k<nbr_Att;k++){
                        if(tmp_IDs[k] == R->Tab_fonc_depend[i].Tab_determiners[j].ID_Attribut){
                            determiner_already_existe =1 ;
                            break;
                        }
                    }
                    // If the determiner is not already in the closure, add it
                    if(!determiner_already_existe){
                        tmp_IDs[nbr_Att++]=R->Tab_fonc_depend[i].Tab_determiners[j].ID_Attribut;
                        modified = 1;
                    }
                }
            }
        }
    }
    int determines_relation = (nbr_Att == R->Number_of_Attributs)?1:0;
    free(tmp_IDs);
    free(All_Atts_copy);
    // If no functional dependency is satisfied, return 0
    return determines_relation;
}
void free_Relation_info(Relation_info *R) {
    // Free memory allocated for relation name and attribute strings
    free(R->Relation_Name);
    free(R->All_Attributs_string);

    // Free memory allocated for attributes
    for (int i = 0; i < R->Number_of_Attributs; i++) {
        free(R->Tab_Attributs[i].Attribut_name);
    }
    free(R->Tab_Attributs);

    // Free memory allocated for functional dependencies
    for (int i = 0; i < R->Number_of_fonc_depend; i++) {
        free(R->Tab_fonc_depend[i].Dependancy_string);
        free(R->Tab_fonc_depend[i].Determinant_string);
        free(R->Tab_fonc_depend[i].Determiner_string);
        free(R->Tab_fonc_depend[i].Tab_determinants);
        free(R->Tab_fonc_depend[i].Tab_determiners);
    }
    free(R->Tab_fonc_depend);
    // Free memory allocated for primary and secondary keys arrays
    free(R->ID_Primary_Keys);
    free(R);
}

void primary_keys(Relation_info* R) {
    int i, j, k;
    R->ID_Primary_Keys = malloc(sizeof(int) * R->Number_of_Attributs);
    for (i = 0; i < R->Number_of_Attributs; i++) {
        R->ID_Primary_Keys[i] = 0;
        R->Tab_Attributs[i].Primary_Key = 0;
    }
    int f = 0;
    for (i = 0; i < R->Number_of_Attributs; i++) {
        int determined_by_others = 0;
        for (j = 0; j < R->Number_of_fonc_depend; j++) {
            for (k = 0; k < R->Tab_fonc_depend[j].Determiner_nbr; k++) {
                if (strcmp(R->Tab_Attributs[i].Attribut_name,R->Tab_fonc_depend[j].Tab_determiners[k].Attribut_name) == 0) {
                    determined_by_others = 1;
                    break;
                }
            }
        }
        if (!determined_by_others) {
            R->Tab_Attributs[i].Primary_Key = 1;
            R->Number_of_Primary_keys = f+1;
            R->ID_Primary_Keys[f++] = R->Tab_Attributs[i].ID_Attribut;
        }
    }
}

void print_keys (Relation_info* R){
    int i ;
    printf("\t\t\t\tNUMBER OF PRIMARY KEYS : %d\n\t\t\t\t\t", R->Number_of_Primary_keys);
    printf("(");
    for(i=0;i<R->Number_of_Primary_keys;i++){
        printf("%s,",R->Tab_Attributs[R->ID_Primary_Keys[i]-1].Attribut_name);
    }
    printf("\b)\n\n");
}

int main() {
    int m ;
    Relation_info R;
    do {
        system("cls");
        printf("\t\t\t\tHELLO TO ENTER THE PROGRAM TYPE 1 ELSE TYPE ANY NUMBER :");
        scanf("%d",&m);
        if(m==1){
            
            printf("\t\t\t\tENTER THE NAME OF THE RELATION : ");
            R.Relation_Name = malloc(ELEMENT_SIZE * sizeof(char));
            scanf("%s", R.Relation_Name);
            printf("\t\t\t\tENTER THE ATTRIBUTES SEPARATED BY COMMAS : ");
            R.All_Attributs_string = malloc(MAX_SIZE * sizeof(char));
            scanf("%s", R.All_Attributs_string);
            printf("\t\t\t\t ENTER THE NUMBER OF FUNCTIONAL DEPENDENCIES : ");
            scanf("%d", &R.Number_of_fonc_depend);
            R.Number_of_Attributs = separate_Attributes(&R);
            R.Tab_fonc_depend = insert_fonc_depend(&R);
            printf("\t\t\t\tRELATION :\n");
            print_Relation(&R);
            know_keys(&R);
            primary_keys(&R);
            print_keys(&R);
            printf("\t\t\t\tNUMBER OF FUNCTIONAL DEPENDENCIES : %d\n", R.Number_of_fonc_depend);
            printf("\t\t\t\tFUNCTIONAL DEPENDENCIES :\n");
            print_fonc_depend(&R);
            system("pause");
            
        }
        else{
            printf("\n\n\n\n\t\t\t\tGOODBYE :)\n\n\n\n\n ");
            
            break;
        }
        
    }while(m==1);
    free_Relation_info(&R); // Free allocated memory before exiting
    return 0;
}
