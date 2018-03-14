const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;

//Wait for Press--------------------------------------------------
void waitForPress()
{
	while(nLCDButtons == 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------

//Wait for Release------------------------------------------------
void waitForRelease()
{
	while(nLCDButtons != 0){}
	wait1Msec(5);
}
//----------------------------------------------------------------
task main()
{
//Declare count variable to keep track of our choice
	int count = 0;

	//------------- Beginning of User Interface Code ---------------
	//Clear LCD
	clearLCDLine(0);
	clearLCDLine(1);
	//Loop while center button is not pressed
	bLCDBacklight = true;
	displayLCDCenteredString(0, "!!Welcome!!");
	displayLCDCenteredString(1, "Team 356B");
	wait1Msec(2000);

	while(nLCDButtons != centerButton)
	{
		//Switch case that allows the user to choose from 4 different options
		switch(count){
		case 0:
			//Display first choice
			displayLCDCenteredString(0, "Autonomous 1");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count = 4;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 1:
			//Display second choice
			displayLCDCenteredString(0, "Autonomous 2");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 2:
			//Display third choice
			displayLCDCenteredString(0, "Autonomous 3");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;
		case 3:
			//Display fourth choice
			displayLCDCenteredString(0, "Autonomous 4");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count++;
			}
			break;

		case 4:
			//Display fourth choice
			displayLCDCenteredString(0, "Autonomous 5");
			displayLCDCenteredString(1, "<		 Enter		>");
			waitForPress();
			//Increment or decrement "count" based on button press
			if(nLCDButtons == leftButton)
			{
				waitForRelease();
				count--;
			}
			else if(nLCDButtons == rightButton)
			{
				waitForRelease();
				count = 0;
			}
			break;
		default:
			count = 0;
			break;
		}
	}
}
