

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#ifdef WIN32
#include<conio.h>
#else
#include <unistd.h>
#endif

using namespace std;

using namespace cv;

VideoCapture vc;

#ifdef WIN32
	char separator[] = "\\";
#else
	char separator[] = "/";
#endif
const char* dirname = "~/INKERS";
char *absPath = NULL;

class MyVideo
{
    
    char inputFname[50];
    char outputFname[50];
    char inputPath[150];
    char outputPath[150];
    public:
     MyVideo(char *fn)
     {
        strcpy(inputFname,fn);
        //strcpy(outputFname,fnp);
        
        strcat (inputFname,".avi");
        // puts(inputFname);
    
        strcpy(outputFname,"processed_");
        strcat(outputFname,inputFname);
     

        //puts(outputFname);
      
        if (absPath)
        {
            strcpy(inputPath, absPath);
            strcpy(outputPath,absPath);
        }
        else
        {
            strcpy(inputPath,dirname);
            strcpy(outputPath,dirname);

        }
        strcat(inputPath,separator);
        strcat(outputPath,separator);
        strcat(inputPath,inputFname);
        strcat(outputPath,outputFname);
        //strcat (path,".avi");
    }
        
        Mat RedExtraction(Mat );
        Mat GaussFilter(Mat );
        char*getInputFname();
        char*getOutputFname();
        int getCapture();

};
	
char* MyVideo::getInputFname()
{
    return inputFname;
}
	
char * MyVideo::getOutputFname()
{
    return outputFname;
}
	
 Mat MyVideo::RedExtraction(Mat frame)
{
    Mat planes[3];
    Mat gaussFrame;
    split(frame,planes);  // planes[2] is the red channel
    Mat redFrame = planes[2];
              
    return redFrame;            
}

Mat MyVideo::GaussFilter(Mat frame)
{
    Mat gaussFrame;
    GaussianBlur(frame, gaussFrame, Size(3,3), 0.5, 0.1, BORDER_DEFAULT);
    
    return gaussFrame;       
}




int MyVideo::getCapture()
{
  VideoCapture vc(0);
    if(!vc.isOpened())
    {
        cout<<"Unable to open camera"<<endl;
        return -1;
    }
	// For saving the frame
	Mat frame;

	// Get vc resolution
	int frameWidth = vc.get(CAP_PROP_FRAME_WIDTH);
	int frameHeight = vc.get(CAP_PROP_FRAME_HEIGHT);
	
	vc.set(CAP_PROP_FPS, 60);
	double fps = vc.get(CAP_PROP_FPS);
	cout<<"FPS="<<fps<<endl;
    
	
	// Create vc writer object
	cout<<"InputPath"<<inputPath<<endl;
	cout<<"outputPath"<<outputPath<<endl;
	VideoWriter output(inputPath, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(frameWidth, frameHeight));
	VideoWriter output1(outputPath, VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(frameWidth, frameHeight));
	
	
    int maxCount = 10*fps;
	int count =0; 
	// Loop through available frames
	while (vc.read(frame) && count< maxCount) {
        // Display the frame
        imshow("Video feed", frame);
		Mat redFrame =RedExtraction(frame);
        imshow("Red", redFrame);// Show extracted red frame
		 
		Mat gaussFrame = GaussFilter(redFrame); 
		 
	    imshow("Gauss", gaussFrame);
	    Mat planes[3];
	 
	    split(frame,planes);  // planes[2] is the red channel
        planes[2] = gaussFrame;
        Mat newFrame;
        merge(planes, 3, newFrame);    // Merge all planes of new frame.

        imshow(inputFname, newFrame);   // Show orignal frame
              
        output1.write(newFrame);  // Write new frame to Video
              
		// Write vc frame to output
		output.write(frame);
        
        count++;
		// For breaking the loop
		if (waitKey(1) >=0) 
		    continue;// break;

	} // end while (vc.read(frame))

	output.release();
	output1.release();
	vc.release();

	// Destroy all windows
	destroyAllWindows();

  

}



int showList()
{
    char cmd[150];
#ifdef WIN32
	strcpy(cmd, "dir ");
#else	
	strcpy(cmd,"ls ");
#endif
    if(absPath)
        strcat(cmd, absPath);
    else
        strcat(cmd, dirname);
	strcat(cmd, separator);
	strcat(cmd,"*.avi");
    int result = system(cmd);
    return result;
}
  
      
int init()
{
    char cmd[150];
    absPath = getenv("INKERS_PATH");
#ifdef WIN32
    strcpy(cmd, "md ");
#else
	strcpy(cmd, "mkdir -p ");
#endif
    //absPath = realpath(dirname, NULL);
    if(absPath)
    {   
	    cout<<"Found absolute path at"<<absPath<<endl;

        strcat(cmd, absPath);
    }
    else
    {
        cout<<"Could not find absolute path at"<<endl;
        strcat(cmd, dirname);
    }
    printf("Running command: %s\n",cmd);
    system(cmd);
    
#ifndef WIN32
	strcpy(cmd, "chmod -R 777 ");

    if(absPath)
        strcat(cmd, absPath);
    else
        strcat(cmd, dirname);
    printf("Running command: %s\n",cmd);
    system(cmd);
#endif
    return 0;
}




      
class User
{
    int isLoggedIn=0;

    const char *orgLogin = "inkers";
	const char *orgPwd = "inkers123";
	char usrLogin[20];
	char usrPwd[20];
	char *usr;
	char *pwd;
    public:
	User(){}
        User(char *login, char *passwd)
        {
            strcpy(usrLogin, login);
            strcpy(usrPwd, passwd);
        }
        
        void getLogin()
        {
	        printf("Login:");	
	        scanf("%s",usrLogin);

#ifdef WIN32
			char ch = '0';
			int pos = -1;
			cout << "Enter Password:";
			while ((ch = getch()) != '\r')
			{
				usrPwd[++pos] = ch;
			}
			usrPwd[++pos] = '\0';
#else
			pwd = getpass("Enter Password: ");
			strcpy(usrPwd, pwd);
#endif	        	
        }

        int verifyLogin()
        {
	        if(strcmp(usrLogin,orgLogin)==0 && strcmp(usrPwd,orgPwd)==0)
	            isLoggedIn = 1;	     
	        else
		        isLoggedIn = 0;
		    return isLoggedIn;
        }
};






class SubMenu
{
    int ch;
    public:
    
    SubMenu(){}
    
    void DisplayMenu()
        {
            printf("\n1.Record\n2.Show List\n3.Logout\n");       
        }

    
        int getChoice()
        {
	        
	        cout<<"Enter the choice  : "<<endl;
	        cin>>ch;
	        return ch;
        }
    
        int processChoice()
        {
            MyVideo *mv;
            
            char inputFname[50];
            switch(ch)
            {
                    case 1:  
                             cout<<"Enter the file name "<<endl;
                             cin>>inputFname;
                             mv = new MyVideo(inputFname);
                             mv->getCapture();
                                delete mv;
                          
                             break;
                             
                     case 2 : showList();
                     
                                break;
                                
                     case 3: cout<<"Logging Out"<<endl;
                                break;
					 default: cout << " Invalid Choice  " << endl;
            
            }
			return 0;
        
        }
};



class Menu
{
	int ch;
	User usr;
public:

	void DisplayMenu()
	{
		printf("1.Login\n2.Exit\n");
	}


	int getChoice()
	{

		cout << "Enter the choice  : " << endl;
		cin >> ch;
		return ch;
	}

	int processChoice()
	{
		SubMenu sm;
		int subch = 0;
		switch (ch)

		{
		case 1:
			usr.getLogin();
			if (usr.verifyLogin())
			{
				do
				{

					sm.DisplayMenu();
					subch = sm.getChoice();
					sm.processChoice();

				} while (subch != 3);


			}
			else
				printf("try again\n");
					   			 		  		  
			break;


		case 2: break;


		default: cout << " Invalid Choice  " << endl;
		}

		return 0;
	}
};


int main() {


    Menu menu;
	char strLogin[20],strPwd[20];
	int ch;
	User usr;
	
	init();
	do
	{
	    menu.DisplayMenu();
	    ch = menu.getChoice();
	    menu.processChoice();
	
	}while(ch!=2);
	//cout<<"Wait for login"<<endl; 
	
				
   

		return 0;

}
