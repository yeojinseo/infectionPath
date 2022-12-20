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
	int i, j, timeMet;
	int who = -1;
	int timearray[ifctdb_len()];
	
	for (i=0;i<ifctdb_len();i++)
	{
		timeMet = isMet(patient_no, i);
		timearray[i] = timeMet;
		if (timeMet > 0) //만났다면 
		{
			for (j=0;j<ifctdb_len();j++) //지금까지 환자 중 만난시간이 가장 이른가?
			{
				if (timeMet <= timearray[j])
					who = i;
			}
		}
	}
	return who;
}


int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    int i;
    char placeInput[20];
    int placenum;
    int minAge, maxAge, pAge;
    
    int infester = -1;
	int pFirst, index;
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
    while ( 3 == fscanf(fp, "%d %d %d", &pIndex, &age, &time))
    {
    	int i;
    	for (i=0;i<N_HISTORY;i++)
    	{
    		fscanf(fp, "%d", &placeHist[i]);
    	}
    	ifct_element = ifctele_genElement(pIndex, age, time, placeHist);
    	ifctdb_addTail(ifct_element);
	}

    //1-3. FILE pointer close
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
                
            case MENU_PLACE:
                printf("Place Name : ");
                scanf("%s", &placeInput);
                
				int cnt = 0;
                for (i=0;i<ifctdb_len();i++) //환자수만큼 반복 
                {
                	ifct_element = ifctdb_getData(i); //i환자의 정보 가져오기 
                	placenum = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1); //i환자의 발병확인장소
                	if (strcmp(placeInput, ifctele_getPlaceName(placenum)) == 0) //입력받은 장소에서 발병된 환자라면 (입력받은장소 = i환자의 발병확인장소) 
                	{
                		printf("--------------------------------------------\n");
						ifctele_printElement(ifct_element); 					//해당환자정보 출력 
                		printf("--------------------------------------------\n");
                		cnt++;
					}
				}
                printf("\nThere are %i patients detected in %s.\n", cnt, placeInput);
                break;
                
            case MENU_AGE: 
                printf("minimal age : ");
                scanf("%d", &minAge);
                printf("maximal age : ");
                scanf("%d", &maxAge);
            	
            	int cnt2 = 0;
            	for (i=0;i<ifctdb_len();i++) //환자수만큼 반복 
				{
					ifct_element = ifctdb_getData(i); //i환자의 정보 가져오기 
					pAge = ifctele_getAge(ifct_element); //i환자의 나이 
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
                scanf("%i", &index);
                
                pIndex = index;
                
				ifct_element = ifctdb_getData(index);
				
				int j, getplace;
				int placeArray[N_HISTORY];
				
				for (j=0;j<N_HISTORY;j++)
				{
					getplace = ifctele_getHistPlaceIndex(ifct_element, j);
					placeArray[j] = getplace;
				}

				int detected_time, place;
				int* ptrtime = &detected_time;
				int* ptrplace = &place;
				
				detected_time = ifctele_getinfestedTime(ifct_element);
				place = placeArray[N_HISTORY];
				
                while (index >= 0 && index < ifctdb_len()) //현재한자가 누군가 있음  
                {
                	infester = trackInfester(index, ptrtime, ptrplace);
                	if (infester != -1)
                		printf(" --> [TRACKING] patient %i is infected by %i (time : %i, place : %i)\n", index, infester, 1, 1);
                	else 
                	{
                		pFirst = index;
                		//printf("%i is the first infector!!\n", index); 
                		break;
					}
                	index = infester;
                	index = -1;
				}
                printf("The first infester of %d is %d\n", pIndex, pFirst);
                break; 
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}
