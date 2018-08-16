#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include "LinuxDARwIn.h"
#include <ctime>

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../../Data/motion_4096.bin"
#endif

     
#define INI_FILE_PATH       "../../../../Data/config.ini"
#define U2D_DEV_NAME        "/dev/ttyUSB0"
     
using namespace Robot;
     
     
void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
       chdir(dirname(exepath));
}
     
    int main(void)
    {
	clock_t start, total; 
        printf( "\n===== Walk Backward Tutorial for DARwIn =====\n\n");
     
        change_current_dir();
     
        minIni* ini = new minIni(INI_FILE_PATH);
     
    	//////////////////// Framework Initialize ////////////////////////////
    	LinuxCM730 linux_cm730(U2D_DEV_NAME);
    	CM730 cm730(&linux_cm730);
    	if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    	{
    		printf("Fail to initialize Motion Manager!\n");
    			return 0;
    	}
            //Port initialization and opening, dynamixel power on
     
            MotionManager::GetInstance()->LoadINISettings(ini);
            Walking::GetInstance()->LoadINISettings(ini);
            //Load default settings for MotionManager and Walking module
   //1234 
    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
//123
 
    	MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());
    	MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());
            LinuxMotionTimer *motion_timer = new LinuxMotionTimer(MotionManager::GetInstance());
            motion_timer->Start();
            //Create MotionManager object and registers head and walking modules, then timers are initialized.
    	/////////////////////////////////////////////////////////////////////
     
            /////////////////////////Capture Motor Position//////////////////////

//12343

    MotionManager::GetInstance()->LoadINISettings(ini);

    int firm_ver = 0;
    if(cm730.ReadByte(JointData::ID_HEAD_PAN, MX28::P_VERSION, &firm_ver, 0)  != CM730::SUCCESS)
    {
        fprintf(stderr, "Can't read firmware version from Dynamixel ID %d!! \n\n", JointData::ID_HEAD_PAN);
        exit(0);
    }

    if(0 < firm_ver && firm_ver < 27)
    {
#ifdef MX28_1024
        Action::GetInstance()->LoadFile(MOTION_FILE_PATH);
#else
        fprintf(stderr, "MX-28's firmware is not support 4096 resolution!! \n");
        fprintf(stderr, "Upgrade MX-28's firmware to version 27(0x1B) or higher.\n\n");
        exit(0);
#endif
    }
    else if(27 <= firm_ver)
    {
#ifdef MX28_1024
        fprintf(stderr, "MX-28's firmware is not support 1024 resolution!! \n");
        fprintf(stderr, "Remove '#define MX28_1024' from 'MX28.h' file and rebuild.\n\n");
        exit(0);
#else
        Action::GetInstance()->LoadFile((char*)MOTION_FILE_PATH);
#endif
    }
    else
        exit(0);




Action::GetInstance()->m_Joint.SetEnableBody(true, true);
    MotionManager::GetInstance()->SetEnable(true);
//124132


    LinuxActionScript::PlayMP3("../../../../Data/mp3/Yes.mp3");
    Action::GetInstance()->Start(16);
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);


    	int n = 0;
    	int param[JointData::NUMBER_OF_JOINTS * 5];
    	int wGoalPosition, wStartPosition, wDistance;

    	LinuxActionScript::PlayMP3("../../../../Data/mp3/Yes.mp3");
     
    	for(int id=JointData::ID_R_SHOULDER_PITCH; id<JointData::NUMBER_OF_JOINTS; id++)
    	{
    		wStartPosition = MotionStatus::m_CurrentJoints.GetValue(id);
    		wGoalPosition = Walking::GetInstance()->m_Joint.GetValue(id);
    		if( wStartPosition > wGoalPosition )
    			wDistance = wStartPosition - wGoalPosition;
    		else
    			wDistance = wGoalPosition - wStartPosition;
     
    		wDistance >>= 2;
    		if( wDistance < 8 )
    			wDistance = 8;
     
    		param[n++] = id;
    		param[n++] = CM730::GetLowByte(wGoalPosition);
    		param[n++] = CM730::GetHighByte(wGoalPosition);
    		param[n++] = CM730::GetLowByte(wDistance);
    		param[n++] = CM730::GetHighByte(wDistance);
    	}
    	cm730.SyncWrite(MX28::P_GOAL_POSITION_L, 5, JointData::NUMBER_OF_JOINTS - 1, param);
            //Capture initial position of Darwin OP 2's motor position
     
    	printf("Press the ENTER key to begin!\n");
    	getchar();

    Action::GetInstance()->Start(16);
    	while(Action::GetInstance()->IsRunning()) usleep(8*1000);
    LinuxActionScript::PlayMP3(“../../../../Data/mp3/UTSA.mp3");



         usleep(500000);


//------------------------Begin School Song dance-------------------------//
        Action::GetInstance()->m_Joint.SetEnableBody(true, true);
        MotionManager::GetInstance()->SetEnable(true);
        Action::GetInstance()->Start(65);
        while(Action::GetInstance()->IsRunning()) usleep(8*1000);

        LinuxActionScript::PlayMP3(“../../../../Data/mp3/FinalStage.mp3");
        usleep(500000); //may need to adjust, prevents falling


//-------------------------BEgin Workout Stage 2-------------------------//

	start = clock();
	total = 0; 	
	while(total < 10) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);        
	   Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC; 

            Walking::GetInstance()->Start();

	       if(total < 2) {
		Walking::GetInstance()->X_MOVE_AMPLITUDE = 15.0;
                Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
	       }
	       else if( total >=2 && total < 4) {
		 Walking::GetInstance()->X_MOVE_AMPLITUDE = -15.0;
                 Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
	       }
	       else if( total >=4 && total < 7) {
                 Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Y_MOVE_AMPLITUDE = 35.0;
               }
	       else if( total >=7 && total < 9) {
                 Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Y_MOVE_AMPLITUDE = -35.0;
               }
	}


        Walking::GetInstance()->Stop();
	usleep(1000000);

        Action::GetInstance()->m_Joint.SetEnableBody(true, true);
        MotionManager::GetInstance()->SetEnable(true);
        Action::GetInstance()->Start(61);
        while(Action::GetInstance()->IsRunning()) usleep(8*1000);


	start = clock();
        total = 0;
        while(total < 2) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
           Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC;


            Walking::GetInstance()->Start();
                Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->Z_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
        }

	Walking::GetInstance()->Stop();
        usleep(1000000);




	 Action::GetInstance()->m_Joint.SetEnableBody(true, true);
         MotionManager::GetInstance()->SetEnable(true);
	 Action::GetInstance()->Start(34);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);

	start = clock();
        total = 0;
        while(total < 2) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
           Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC;


            Walking::GetInstance()->Start();
		Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->Z_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
        }


	Walking::GetInstance()->Stop();
        usleep(1000000);

         Action::GetInstance()->m_Joint.SetEnableBody(true, true);
         MotionManager::GetInstance()->SetEnable(true);
         Action::GetInstance()->Start(34);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);

	 Action::GetInstance()->Start(89);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);




//--------------------Begin Dancing Stage-------------------------------//
     usleep(1400000);



//------------------------


	for(int i = 0; i <2; i++) {
        start = clock();
        total = 0;
        while(total < 8) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
           Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC;


            Walking::GetInstance()->Start();


	       if(total < 4) {
                 Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Z_MOVE_AMPLITUDE = 45.0;
                 Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Y_MOVE_AMPLITUDE = 45.0;
               }
               else if( total >=4) {
                 Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Z_MOVE_AMPLITUDE = 45.0;
                 Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
                 Walking::GetInstance()->Y_MOVE_AMPLITUDE = -45.0;

               }



        }


        Walking::GetInstance()->Stop();
	usleep(1000000);

//kick kick 


         Action::GetInstance()->m_Joint.SetEnableBody(true, true);
         MotionManager::GetInstance()->SetEnable(true);
         Action::GetInstance()->Start(20);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);
         Action::GetInstance()->Start(85);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);
         Action::GetInstance()->Start(86);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);
         Action::GetInstance()->Start(85);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);





        start = clock();
        total = 0;
        while(total < 4) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
           Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC;


            Walking::GetInstance()->Start();
	       if(total< 2)
                Walking::GetInstance()->A_MOVE_AMPLITUDE = 55.0;
	       else if(total >=2 && total <= 3) 
	                Walking::GetInstance()->A_MOVE_AMPLITUDE = -42.0;
		else
                    Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;



        }
        usleep(500000);


     }

         Action::GetInstance()->m_Joint.SetEnableBody(true, true);
         MotionManager::GetInstance()->SetEnable(true);
         Action::GetInstance()->Start(20);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);

/*
//-------------------------------Testing------------------------------
        Action::GetInstance()->m_Joint.SetEnableBody(true, true);
        MotionManager::GetInstance()->SetEnable(true);
//-------------------------------Testing------------------------------
*/

	for(int i = 0; i<4; i++) { 
         Action::GetInstance()->Start(95);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);
         usleep(650000);
         }

        for(int i = 0; i<2; i++) {
         Action::GetInstance()->Start(102);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);
         usleep(650000);
         }

        usleep(650000);

         Action::GetInstance()->Start(106);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);

/*
//-------------------------------Testing------------------------------
        Action::GetInstance()->m_Joint.SetEnableBody(true, true);
        MotionManager::GetInstance()->SetEnable(true);
//-------------------------------Testing------------------------------
*/

        start = clock();
        total = 0;
        while(total < 1) {
           Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
           Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);

            total = (double)(clock() - start) / CLOCKS_PER_SEC;


            Walking::GetInstance()->Start();
                Walking::GetInstance()->X_MOVE_AMPLITUDE = 20.0;
                Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
                Walking::GetInstance()->Z_MOVE_AMPLITUDE = 40.0;
                Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
        }

        Walking::GetInstance()->Stop();
        usleep(1000000);

//-------------------------------Testing------------------------------
        Action::GetInstance()->m_Joint.SetEnableBody(true, true);
        MotionManager::GetInstance()->SetEnable(true);
//-------------------------------Testing------------------------------

         Action::GetInstance()->Start(119);
         while(Action::GetInstance()->IsRunning()) usleep(8*1000);

     
        return 0;
    }
