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
	
	return ptr->placeHist[pIndex];
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
		printf("%s(%i)  ", ifctele_getPlaceName(ptr->placeHist[i]), (ptr->time)-(N_HISTORY-i-1));
	}
//	printf("Path History : : %c(%i)-> %c(%i)-> %c(%i)-> %c(%i)-> %c(%i)", );

	printf("\n");
	
}



//return country name pointer from country number
char* ifctele_getPlaceName(int placeIndex)
{
	return countryName[placeIndex];
}



int isMet(int pIndex, int pTest) //현재환자, i번쨰 환자 
{
	int i, j;
	int pIndex_place;
	int time, pTest_place, placeindex;
	int detected_time, pTest_detected_time;
	int timeMet = -1;
	int placeArray[N_HISTORY];
	
	void *pIndex_element; //
	void *pTest_element;

	pIndex_element = ifctdb_getData(pIndex);
	pTest_element = ifctdb_getData(pTest);
	
	detected_time = ifctele_getinfestedTime(pIndex_element);
	pTest_detected_time = ifctele_getinfestedTime(pTest_element);
	
	for (j=0;j<N_HISTORY;j++) //현재환자의 장소 배열 만들기  
	{
		pIndex_place = ifctele_getHistPlaceIndex(pIndex_element, j);
		placeArray[j] = pIndex_place;
	}
	
	for (i=0;i<N_HISTORY-2;i++)
	{	
		//pIndex의 placeHist[i]의 시간(날짜)  //현재환자의 i번째 이동장소 시점 계산 
		time = detected_time - (N_HISTORY - i - 1); //현재환자의 i번째 이동장소의 날짜 알아내기 
		
		//pTest의 time에서 위치    //계산된 시점에서의 대상환자 이동장소 계산
		placeindex = convertTimeToIndex(time, pTest_detected_time); 
		if (placeindex >= N_HISTORY)
		{
			return -1;
		}
		else
		{
			pTest_place = ifctele_getHistPlaceIndex(pTest_element, placeindex);
		}
		//testplace = ifctele_getHistPlaceIndex(pIndex_element, placeindex);
		if (placeArray[i] == pTest_place)
		{
			timeMet = time;
		}
	}
	
	return timeMet;
}

 

int convertTimeToIndex(int time, int infestedTime)
{
	int index = -1;
	if (time <= infestedTime && time > infestedTime-N_HISTORY)
	{
		index = N_HISTORY-(infestedTime - time)-1;
	}
	
	return index;
}
