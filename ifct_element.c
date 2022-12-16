//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};


typedef struct ifct_ele {
	int pIndex; //number
	int age; //age
	int time; //time
	place_t placeHist[N_HISTORY]; //place(N_HISTORY개)
} ifct_ele_t;



void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])
{
	int i;
	ifct_ele_t* ptr;
	
	ptr = (void*)malloc(sizeof(ifct_ele_t));
	ptr->pIndex = index;
	ptr->age = age;
	ptr->time = detected_time;
	for (i=0;i<N_HISTORY;i++)
	{
		ptr->placeHist[i] = history_place[i]; //(ifct_ele_t.placeHist, ) ??
	}

	//여기에서 free하면 안됨. 
	return ptr;
}



int ifctele_getAge(void* obj)
{
	ifct_ele_t* ptr = (ifct_ele_t*)obj;
	
	return ptr->age;
}



int ifctele_getHistPlaceIndex(void* obj, int pIndex) //place는 몇번쨰 장소인지도 고려. 
{
	ifct_ele_t* ptr = (ifct_ele_t*)obj;
	
	return ptr->placeHist[N_HISTORY-1];
}



unsigned int ifctele_getinfestedTime(void* obj)
{
	ifct_ele_t* ptr = (ifct_ele_t*)obj;
	
	return ptr->time;
}
 


void ifctele_printElement(void* obj)
{
	int i;
	
	ifct_ele_t* ptr = (ifct_ele_t*)obj;
	
	printf("Patient Index : %i\n", ptr->pIndex);
	printf("Patient Age : %i\n", ptr->age);
	printf("Detected Time : %i\n", ptr->time);
	printf("Path History : ");
	for(i=0;i<N_HISTORY;i++)
	{
		printf("%s ", ifctele_getPlaceName(ptr->placeHist[i]));
	}

	printf("\n");
	
//	printf("Path History : : %c(%i)-> %c(%i)-> %c(%i)-> %c(%i)-> %c(%i)", );
}



//return country name pointer from country number
char* ifctele_getPlaceName(int placeIndex)
{
	return countryName[placeIndex];
}

