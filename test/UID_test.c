#include <stdio.h> /* printf */

#include "UID.h"

void TestAllFuncs();
void TestCreate();
void TestIsSame();

int main()
{
	TestAllFuncs();
	return (0);
}

void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestIsSame();
	printf("      ~END OF TEST FUNCTION~ \n");
}

void TestCreate()
{
	UID_t uid = UIDCreate();
	UID_t uid2 = UIDCreate();
	if(uid2.counter > uid.counter && uid.pid == uid2.pid)
	{
		printf("UIDCreate working!                                   V\n");
	}
	else
	{
		printf("UIDCreate NOT working!                               X\n");
	}

}

void TestIsSame()
{
	UID_t uid = UIDCreate();
	UID_t uid2 = uid;
	
	if((UIDIsSame(uid, uid2)) && (!UIDIsSame(uid, badUID)))
	{
		printf("UIDIsSame working!                                   V\n");
	}
	else
	{
		printf("UIDIsSame NOT working!                               X\n");
	}
}









