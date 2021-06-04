
//*****************************************************//
//		MyKey.c
//*****************************************************//


#include "MyAll.h"


int KeyValue;

int KeyUpFlag;
int KeyUpStep;
int KeyUpContinuous;

int KeyDownFlag;
int KeyDownStep;
int KeyDownContinuous;



void KeyInit(void)
{

	KeyValue = KEY_VALUE_NULL;

	KeyUpFlag = 0;
	KeyUpStep = 0;
	KeyUpContinuous = 0;

	KeyDownFlag = 0;
	KeyDownStep = 0;
	KeyDownContinuous = 0;

	if(GetKeyUp() == 0)
	{
		Debuger = Debuger;

	}
	else
	{
		Debuger = Debuger;

	}

	

	if(GetKeyDown() == 0)
	{
		Debuger = Debuger;

	}
	else
	{
		Debuger = Debuger;

	}
	


}



void Key10ms(void)
{

	if(GetKeyUp() == 0)
	{
		if(KeyUpStep == 0)
		{
			KeyUpStep++;

		}
		else if(KeyUpStep < 2)
		{
			KeyUpStep++;

		}
		else if(KeyUpStep == 2)
		{
			KeyUpStep++;

			KeyUpFlag = 1;
		}
		else if(KeyUpStep < 100)
		{
			KeyUpStep++;

		}
		else
		{
			KeyUpContinuous = 1;

		}

	}
	else
	{
		KeyUpStep = 0;

	}
	if(GetKeyDown() == 0)
	{
		if(KeyDownStep == 0)
		{
			KeyDownStep++;

		}
		else if(KeyDownStep < 2)
		{
			KeyDownStep++;

		}
		else if(KeyDownStep == 2)
		{
			KeyDownStep++;

			KeyDownFlag= 1;
		}
		else if(KeyDownStep < 100)
		{
			KeyDownStep++;

		}
		else
		{
			KeyDownContinuous = 1;

		}

	}
	else
	{
		KeyDownStep = 0;

	}


	if(KeyUpFlag > 0)
	{
		KeyUpFlag = 0;

		KeyValue = KEY_VALUE_UP;

	}
	else if(KeyDownFlag > 0)
	{
		KeyDownFlag = 0;

		KeyValue = KEY_VALUE_DOWN;

	}
	else
	{
		KeyValue = KEY_VALUE_NULL;

	}





}



void Key100ms(void)
{

	if(KeyUpContinuous > 0)
	{
		KeyUpContinuous = 0;

		KeyValue = KEY_VALUE_UP;
		
	}
	else if(KeyDownContinuous > 0)
	{
		KeyDownContinuous= 0;

		KeyValue = KEY_VALUE_DOWN;

	}
	else
	{
		KeyValue = KEY_VALUE_NULL;

	}



}







