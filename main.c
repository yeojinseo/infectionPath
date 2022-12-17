//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place)
{
	int i, timeMet, infester; 
	for (i=0;i<ifctdb_len();i++)
	{
		timeMet = isMet(patient_no, i);
		if (timeMet > 0) //만났다면 
		{
			if (timeMet < isMet(patient_no, i))
			{
				infester = i;
			}
		}

	}
	
	return infester;
}


int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    char placename[20];
    int placenum;
    int i;
    int minAge, maxAge, pAge;
    int infester, pFirst;
    int lastplace;
    int *detected_time;
	int *place;

    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    fp = fopen("patientInfo.txt", "r");
    
    while ( 3 == fscanf(fp, "%d %d %d", &pIndex, &age, &time))
    {
    	int i;
    	for (i=0;i<N_HISTORY;i++)
    	{
    		fscanf(fp, "%d", &placeHist[i]);
//    		printf("%5d", placeHist[i]);
    	}
    	ifct_element = ifctele_genElement(pIndex, age, time, placeHist);
    	ifctdb_addTail(ifct_element);
//    	printf("\n");
	}
	

/*
   {
    	int place1, place2; //첫번쨰, 두번쨰 장소 
    	
    	place1 = 3; //Los Angeles 
		place2 = 15; //Munich
		
		printf("The first place is %s\n", ifctele_getPlaceName(place1));
		printf("The second place is %s\n", ifctele_getPlaceName(place2));
	}
*/
/*
	{
		int i, j;
		for (i=0;i<5;i++)
		{
			printf("%d 번째 환자 감염 경로 : ", i);
			for (j=0;j<5;j++)
				printf("%s ", ifctele_getPlaceName(placeHist[j]));
				printf("\n");
		}
		printf("\n");
	}
*/

	
    //1-3. FILE pointer close //10주차 화 
    fclose(fp);
	
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT: 
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT: 
            	printf("Patient index : ");
            	scanf("%i", &pIndex);
                ifct_element = ifctdb_getData(pIndex);
                printf("--------------------------------------------\n");
                ifctele_printElement(ifct_element);
                printf("--------------------------------------------\n");
                break;
                
            case MENU_PLACE: //2
                printf("Place Name : ");
                scanf("%s", &placename);
                
				int cnt = 0;
                for (i=0;i<ifctdb_len();i++)
                {
                	ifct_element = ifctdb_getData(i);
                	placenum = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1);
                	if (strcmp(placename, ifctele_getPlaceName(placenum)) == 0)
                	{
                		printf("--------------------------------------------\n");
						ifctele_printElement(ifct_element);
                		printf("--------------------------------------------\n");
                		cnt++;
					}
				}
                printf("\nThere are %i patients detected in %s.\n", cnt, placename);
                break;
                
            case MENU_AGE: 
                printf("minimal age : ");
                scanf("%d", &minAge);
                printf("maximal age : ");
                scanf("%d", &maxAge);
            	
            	int cnt2;
            	for (i=0;i<ifctdb_len();i++)
				{
					ifct_element = ifctdb_getData(i);
					pAge = ifctele_getAge(ifct_element);
					if (pAge >= minAge && pAge <= maxAge)
					{
						printf("--------------------------------------------\n");
						ifctele_printElement(ifct_element);
                		printf("--------------------------------------------\n");
                		cnt2++;
					}
				}
                printf("\nThere are %i patients whose age is between %i and %i.\n", cnt2, minAge, maxAge);
                break;
                
            case MENU_TRACK: 
                printf("Patient index : ");
                scanf("%d", pIndex);
                
				ifct_element = ifctdb_getData(pIndex);
				lastplace = ifctele_getHistPlaceIndex(ifct_element, pIndex);
				
				detected_time = ifctele_getinfestedTime(ifct_element);
				place = ifctele_getHistPlaceIndex(ifct_element, 4);
                while (pIndex != -1)
                {
                	infester = trackInfester(pIndex, detected_time, place);
                	if (infester != 0)
                		printf("--> [TRACKING] patient %i is infected by %i (time : %i, place : %s)", pIndex, infester, 1, 1);
                	else
                		pFirst = pIndex;
                	pIndex = infester;
				}
                
                printf("The first infextor of %d is %d", pIndex, infester);
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}
