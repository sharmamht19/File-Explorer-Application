#include<iostream>
#include<unistd.h>
#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stack>
#include <assert.h>
#include <signal.h>
#include<termios.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<dirent.h>
using namespace std;
//-----------------------------------------------|  Declaration of all the functions  |--------------------------------------------------
void Get_Win_Info(char const * );
void Set_Curr_Dir(char const * );
void Print_Details( void );
void Start_Non_Canon( void );
void Stop_Non_Canon();
void Mov_Cursor(int , int) ;
void Scroll_Up( void );
void Scroll_Down( void );
void Back( void );
void Forward( void );
void  Home( void );
void Enter(void );
void Level_Up( void );
void Enter( void );
void Command_Mode(void );
void Print_Alert(string);

//Variable Decalration...
int Max_Line=20;
int Curr_Col_size,Curr_row_size;
const string File_Color("\033[1;33m");     //Text color for file names...
const string Reset("\033[0m");
const string Cursor_P("\033[1;94m");  //Text color for current Pointer row..
struct winsize w;                     //to store Window size information of Terminal..
vector < dirent *> Files;             //to store Directory Files Dirent pointers..
string Root_Path;                     //Home Directory Path..
char Curr_Dir[1024];
size_t Dir_size=1024;
int Index_Start=0 , Index_End=Index_Start+Max_Line;
int In_Line,Out_Line,Mod_Line,Stat_Line, curs=1,Alert_Line,Cursor_Loc,CPATH_Line,Temp_Line;
static struct termios Init_Set, New_set;
stack<string > Back_Stack, Forward_Stack;
vector<string> Command_Tokens;

void Print_Alert(string s)                          //Print the line on Alert bar..
{
      Mov_Cursor(Alert_Line, 0);
      cout<<"\033[1;31m"<<"ALERT : "<<s<<"\033[0m";
      Mov_Cursor(curs, 0);
}

void Get_Win_Info(char const * Dir_Path)            //Getting the Terminal Size Information..
{
      ioctl(STDOUT_FILENO, TIOCGWINSZ, & w);
      Curr_row_size=w.ws_row;
      Curr_Col_size=w.ws_col;
      Mod_Line=w.ws_row-6;
      Out_Line=Mod_Line+1;
      Stat_Line=Out_Line+1;
      In_Line=Stat_Line+1;
      Alert_Line=Mod_Line-2;
      Temp_Line=Mod_Line-3;
      CPATH_Line=Mod_Line-1;
}
void Mov_Cursor(int x, int y) {                      //Move the cursor to the  given location..
        cout<<"\033[" << x << ";" << y << "H";
        fflush(stdout);
}
void Clear_Comm_Mode_Line(){
      Mov_Cursor(Out_Line,0);
      cout<<"\033[1;36m"<<"Enter-Command ~$ "<<"\033[0m";
      Mov_Cursor(Out_Line,18);
      for(int i=Out_Line; i<=Stat_Line; i++) printf("\033[K");
      Mov_Cursor(Out_Line,18);
}
void Print_Status(string str){                        //Print the Conent on Status bar...
             Mov_Cursor(Stat_Line,0);
             printf("\033[K");
           cout<<"\033[1;22m"<<"STATUS : "<<str<<"\033[0m";
           Mov_Cursor(Out_Line,18);
}
bool comparator ( const dirent* a, const dirent* b)   //Comparator for sorting of the dirent vector..
{
    if (strcmp(a->d_name, b->d_name) < 0)
        return true;

    return false;
}
void Set_Line(int Cursor_Pointer){   
// if(Curr_row_size<15){
//        Max_Line=5;
//        Index_Start=0 ;
//        Index_End=Index_Start+Max_Line;
//        curs=1;
//     }
//     else  if(Curr_row_size<25){
//        Max_Line=10;
//        Index_Start=0 ;
//        Index_End=Index_Start+Max_Line;
//        curs=1;
//     }
//     else{
//        Max_Line=20;
//        Index_Start=0 ;
//        Index_End=Index_Start+Max_Line;
//     }                 //Highlight the Current Row Pointed by the cursor..
    struct stat F_Info;
   struct passwd * PW;
      lstat(Files[Cursor_Pointer+Index_Start-1]->d_name, & F_Info);
                 cout<<Cursor_P;
                 if(Curr_Col_size>45) {
                printf((S_ISDIR(F_Info.st_mode)) ? "d" : "-");
                printf((F_Info.st_mode & S_IRUSR) ? "r" : "-");
                printf((F_Info.st_mode & S_IWUSR) ? "w" : "-");
                printf((F_Info.st_mode & S_IXUSR) ? "x" : "-");
                printf((F_Info.st_mode & S_IRGRP) ? "r" : "-");
                printf((F_Info.st_mode & S_IWGRP) ? "w" : "-");
                printf((F_Info.st_mode & S_IXGRP) ? "x" : "-");
                printf((F_Info.st_mode & S_IROTH) ? "r" : "-");
                printf((F_Info.st_mode & S_IWOTH) ? "w" : "-");
                printf((F_Info.st_mode & S_IXOTH) ? "x" : "-");
                printf("\t");
                }
              cout<<Reset;
              if(Curr_Col_size>96) {
                PW = getpwuid(geteuid());
                if (PW) {
                       // printf("%s\t", PW->pw_name);
                        cout<<Cursor_P<<PW->pw_name<<Reset<<"\t";
                         cout<<Cursor_P<<PW->pw_name<<Reset<<"\t";
                        //printf("%s\t", PW->pw_name);

                }
                }
                if(Curr_Col_size>78) {
                string Last_Mod_Date = ctime( & F_Info.st_mtime);
                //cout<<Last_Mod_Date;
               for(unsigned int j=0;j<Last_Mod_Date.size()-1;j++)
                   {
                    //printf("%c",Last_Mod_Date[j]);
                    cout<<Cursor_P<<Last_Mod_Date[j]<<Reset;
                   }
                   printf("\t");
                   }
                   if(Curr_Col_size>60) {
                  double F_Size=F_Info.st_size;
                  if(F_Size/1048576 > 1)  printf("\033[1;94m%6.2f MB\033[0m",F_Size/1048576);
                  else if(F_Size/1024 > 1) printf("\033[1;94m%6.2f KB\033[0m",F_Size/1024);
                   else printf("\033[1;94m%6.2f  B\033[0m",F_Size);

                   printf("\t");
                   }
                     if(Curr_Col_size>20) {
              
                    cout<<Cursor_P;
                     printf("%.15s", Files[Cursor_Pointer+Index_Start-1]->d_name);
                     cout<<Reset;
                 }
                  
}
//---------------------------|  List the Files of Current Directory with  several information  |-------------------------------------
void Print_Details( ){
    // if(Curr_row_size<15){
    //    Max_Line=5;
    //    Index_Start=0 ;
    //    Index_End=Index_Start+Max_Line;
    //    curs=1;
    // }
    // else  if(Curr_row_size<25){
    //    Max_Line=10;
    //    Index_Start=0 ;
    //    Index_End=Index_Start+Max_Line;
    //    curs=1;
    // }
    // else{
    //  Max_Line=20;
    //    Index_Start=0 ;
    //    Index_End=Index_Start+Max_Line;
    // }
   printf("\033[H\033[J");
   struct stat F_Info;
   struct passwd * PW;
   //sort(Files.begin(),Files.end());
     sort(Files.begin(),Files.end(), comparator);
   for(int i=Index_Start;i<Index_End && i<Files.size();i++)
   {
        
      lstat(Files[i]->d_name, & F_Info);
                          
                  if(Curr_Col_size>45) {
                                              //Permissions..
                printf((S_ISDIR(F_Info.st_mode)) ? "d" : "-");
                printf((F_Info.st_mode & S_IRUSR) ? "r" : "-");
                printf((F_Info.st_mode & S_IWUSR) ? "w" : "-");
                printf((F_Info.st_mode & S_IXUSR) ? "x" : "-");
                printf((F_Info.st_mode & S_IRGRP) ? "r" : "-");
                printf((F_Info.st_mode & S_IWGRP) ? "w" : "-");
                printf((F_Info.st_mode & S_IXGRP) ? "x" : "-");
                printf((F_Info.st_mode & S_IROTH) ? "r" : "-");
                printf((F_Info.st_mode & S_IWOTH) ? "w" : "-");
                printf((F_Info.st_mode & S_IXOTH) ? "x" : "-");
                  printf("\t");
                  }
                  
                  if(Curr_Col_size>96) {
                   
                
                                                                        //owner....
                PW = getpwuid(geteuid());
                if (PW) {
                        printf("%s\t", PW->pw_name);
                        printf("%s", PW->pw_name);

                }
                cout<<"\t";  
            }
                     if(Curr_Col_size>78) {
                                                                      //Last modified date...
                string Last_Mod_Date = ctime( & F_Info.st_mtime);
                //cout<<Last_Mod_Date;
               for(unsigned int j=0;j<Last_Mod_Date.size()-1;j++)
                   {
                    printf("%c",Last_Mod_Date[j]);
                   }
                   printf("\t");
               }


                     if(Curr_Col_size>60) {
                  
                                                                        //Size of the Dir or file..
                  double F_Size=F_Info.st_size;
                  if(F_Size/1048576 > 1)  printf("%6.2f MB",F_Size/1048576);
                  else if(F_Size/1024 > 1) printf("%6.2f KB",F_Size/1024);
                   else printf("%6.2f  B",F_Size);
                  
                  printf("\t");
                   }
                    if(Curr_Col_size>20) {
                   
                                                                       //Name of the Directory...
                if ((S_ISDIR(F_Info.st_mode)))
                    //cout<<Files[i]->d_name;
                     printf("%.15s", Files[i]->d_name);
                 else{
                    cout<<File_Color;

                     printf("%.15s", Files[i]->d_name);
                      cout<<Reset;
                    }         //Name of the File...
                }

                   cout<<endl; 
       }
       Mov_Cursor(Mod_Line,0);
       cout<<"\033[0;35m#Normal Mode; \033[0m";
       cout << "\033[1;32mPress : to switch to Command Mode\033[0m\n";
        Print_Alert("Well-Come to File Explorer");
        Mov_Cursor(curs ,0);
        Set_Line(curs);
        Mov_Cursor(curs ,0);
        Mov_Cursor(CPATH_Line,0);
        cout<<"Current Directory :"<<"\033[1;38m"<<Curr_Dir<<"\033[0m";
        Mov_Cursor(curs,0);
                return;
               
  //cout<<"Print detail called"<<endl;
}
//-----------------------------------------------| Reading the Called Directory |-----------------------------------------------------
void Set_Curr_Dir(char const * Dir_Path){
      //cout<<"set curr dir  called"<<endl;
       DIR * Dir_;
       struct dirent* Store_Files;
       Dir_=opendir(Dir_Path);
       if(Dir_ == NULL){
            Print_Alert("!Error While Opening the Directory-> "+string(Dir_Path));
            return;
       }
       chdir(Dir_Path);
       
       getcwd(Curr_Dir,Dir_size);

       Files.clear();
       while((Store_Files=readdir(Dir_))!=NULL){
            Files.push_back(Store_Files);
      }
          closedir(Dir_);
         curs=Index_End=min(Max_Line, int(Files.size()));
         Index_Start=0;
         Print_Details();
        Mov_Cursor(curs,0);
        Mov_Cursor(CPATH_Line,0);
        Root_Path=getpwuid(getuid())->pw_dir;
        Mov_Cursor(curs,0);
       
      return;
}
//*******************************************************| NORMAL-MODE |**************************************************************//

 void Start_Non_Canon(){                                          //Starting Non-canonical mode....
      tcgetattr(0,& Init_Set);
      New_set=Init_Set;
      New_set.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
      New_set.c_cc[VMIN]=1;
      New_set.c_cc[VTIME]=0;
      tcsetattr(0,TCSANOW ,& New_set);
      return;
 }
 void Stop_Non_Canon(){                                          //Closing non-canonical mode...
      tcsetattr(0,TCSANOW, & Init_Set);
      return;
 }
 void Scroll_Up( void ){                                         //Scroll up on  ↑ Up Arrow key Pressing..
      if(curs>1)
      {
            curs--;
            Mov_Cursor(curs,0);
            Print_Details();
            Mov_Cursor(curs ,0);
      }
      else if(Index_Start!=0){
            Index_Start--;
            Index_End--;
            Print_Details();
            Mov_Cursor(curs ,0);

      }

 }
void Scroll_Down( void ){                                        //Scroll up on ↑ Down Arrow key Pressing..
        if(curs<Max_Line && curs<Files.size())
      {
            curs++;
            Mov_Cursor(curs,0);
            Print_Details();
            Mov_Cursor(curs,0);
      }
      else if(Index_End!=Files.size()){
            Index_Start++;
            Index_End++;
            Print_Details();
            Mov_Cursor(curs,0);

      }

}
void Back(){                                                    //Go back on pressing ← Left arrow key..
 if(!Back_Stack.size())return;
  string Prev_Dir=Back_Stack.top();
 Back_Stack.pop();
 Forward_Stack.push(string(Curr_Dir));
 Set_Curr_Dir(Prev_Dir.c_str());
 return;
}
void Forward(){                                                 //Forward on pressing →Right arrow key...
  if(!Forward_Stack.size())return;
  string Fwd_Dir=Forward_Stack.top();
  Forward_Stack.pop();
  Back_Stack.push(string(Curr_Dir));
  Set_Curr_Dir(Fwd_Dir.c_str());
  return;
}
void Home(){                                                    //Go to Home on clicking H or h key...
string Home_Dir= getpwuid(getuid())->pw_dir;
Back_Stack.push(string(Curr_Dir));
Set_Curr_Dir(Home_Dir.c_str());
            //curs=1;
            Mov_Cursor(curs,0);
}
//------------| When backspace key clicked LevelUp Function called to go to one step parent directory  |----------------------------
void Level_Up(){
      if(Curr_Dir==Root_Path)
      {
            Print_Alert("You are already in Home Directory!");
            return;
      }
      Back_Stack.push(string(Curr_Dir));
      Set_Curr_Dir("../");
      
}
void Enter(){                                                //When Enter button clicked ,Directory or file will be opened..
      struct stat fileInfo;
      char *File_name=Files[curs+Index_Start-1]->d_name;
      lstat(File_name,& fileInfo);
      if(S_ISDIR(fileInfo.st_mode))
      {
         if(strcmp(File_name,"..")==0)
         {
                Level_Up();
                return;
         }
         else if(strcmp(File_name,".")==0) return;
         Back_Stack.push(string(Curr_Dir));
         Set_Curr_Dir((string(Curr_Dir) + '/' + string(File_name)).c_str());          //Go to the pointed Directory
      }
      else{
            pid_t pid=fork();
            if(pid==0){
                  Print_Alert("File Opened in Default Editor.                   ");
                  execl("/usr/bin/xdg-open","xdg-open",File_name ,NULL);              //Opening File on default editor...
                  exit(1);
            }
      }

}
//***********************************************************|  COMMAND MODE  |***********************************************************

// Modifying the given shorthands to path to actual path..
bool Set_Path(){                                        
    int Len=Command_Tokens.size();
    string PATH=Command_Tokens[Len-1];
    if(PATH[0]=='~'){
        string Home_Dir= getpwuid(getuid())->pw_dir;
        PATH.replace(0,1,Home_Dir);
    }
    else {
        char Buffer[PATH_MAX];
            char *Response=realpath(PATH.c_str(),Buffer);
            if(Response)
                 {
                    PATH=string(Buffer);
                 } else {
                          char* errStr = strerror(errno);
                          printf("error string: %s\n", errStr);
                          perror("realpath");
                          return false;
                         }

    }
     Command_Tokens[Len-1]=PATH;
    return true;
}
//---------------------------------------------------------| Create Method |------------------------------------------------------
void create_file(){
 if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
    int Len=Command_Tokens.size();
    string PATH=Command_Tokens[Len-1];

    for(int i=1; i<Len-1; i++){
        string name = Command_Tokens[i];
        open((PATH + '/' + name).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    }
}
void create_dir(){
 if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
     int Len=Command_Tokens.size();
    string PATH=Command_Tokens[Len-1];
    for(int i=1; i<Len-1; i++){
        string name = Command_Tokens[i];
        mkdir((PATH + '/' + name).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
    }
}
//---------------------------------------------------------------| GO-TO |------------------------------------------------------
void Goto(){
    if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
    string Dest_PATH=Command_Tokens[1];
    Set_Curr_Dir(Dest_PATH.c_str());
    Print_Status("GOTO Successfull..");
}
//--------------------------------------------------------------| RENAME FILE |-------------------------------------------------
void rename_(){
    string N_NAME=Command_Tokens[2];
    Command_Tokens.pop_back();
    if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
    string O_NAME=Command_Tokens[1];

    if(!rename(O_NAME.c_str(),N_NAME.c_str())){
         Set_Curr_Dir(Curr_Dir);
         Print_Status("Renamed Successfull..");

    }
    else{
        Print_Status("Renamed Failed..");
    }
}

//----------------------------------------------------------| COPY FILE / DIRECTORY |-----------------------------------------
//Copy the file content..
void copy_file(string name ,string path){
    char C_[1024];
    int File_Out, File_In, R;
    File_In=open(name.c_str(), O_RDONLY);
    File_Out=open((path).c_str(), O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR );
    while((R=read(File_In, C_, sizeof(C_)))>0)
    {
        write(File_Out,C_,R);
    }
    struct stat k;
 stat (name.c_str(),&k);
 chmod(path.c_str(),k.st_mode);
 chown(path.c_str(),k.st_uid,k.st_gid);
}
//Copy the Directory..
void Paste_Data(string Path_to_Copy , string Dest_PATH)
{ 
  DIR * Dir_to_copy;
  struct dirent* dirent_to_copy;
  struct stat F_info_to_copy;
  if(!(Dir_to_copy=opendir(Path_to_Copy.c_str())))
    {
        Print_Alert("can't open the Directory-> "+Path_to_Copy);
        Print_Status("copy Failed..");
        return ;
    }
    chdir(Path_to_Copy.c_str());
     while((dirent_to_copy = readdir(Dir_to_copy))!= NULL){
        lstat(dirent_to_copy->d_name, &F_info_to_copy);
        string name=string(dirent_to_copy->d_name);
        if(S_ISDIR(F_info_to_copy.st_mode)){
            if(name == "." || name == "..") continue;
            else{
                mkdir((Dest_PATH + '/' + name).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
                Paste_Data(name,Dest_PATH + "/" + name);
            }
        }
        else
            copy_file(name,Dest_PATH + "/" + name);
    }
}
void copy_dir(int i){
    int Len=Command_Tokens.size();
    string Dest_PATH=Command_Tokens[Len-1];
    string DIR_NAME=Command_Tokens[i];
   int ret=mkdir((Dest_PATH+'/'+DIR_NAME).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
       if(!ret){
          struct stat k;
          stat (DIR_NAME.c_str(),&k);
          chmod((Dest_PATH+'/'+DIR_NAME).c_str(),k.st_mode);
        chown((Dest_PATH+'/'+DIR_NAME).c_str(),k.st_uid,k.st_gid);
    }
    else{
             Print_Status("copy Failed..");
         return;
    }
    Paste_Data(string(Curr_Dir)+ "/" + DIR_NAME, Dest_PATH+ "/" + DIR_NAME );

}

void copy_(){
 Print_Status("copied Successfull..");
int Len=Command_Tokens.size();
if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
string Dest_PATH=Command_Tokens[Len-1];
struct stat F_Info;
 for(int i=1;i<Len-1;i++){
   string Path=string(Curr_Dir) + "/" + Command_Tokens[i];
 lstat(Path.c_str(), &F_Info);
 if(S_ISDIR(F_Info.st_mode))
     copy_dir(i);
 else{
    string name=Command_Tokens[i];
    copy_file(name,Dest_PATH + "/" + name);
     }
 }
 //Set_Curr_Dir(Curr_Dir);
// Print_Status("copied Successfull..");
}

//---------------------------------------------------| SEARCH FILE / DIRECTORY |------------------------------------------------
//search for the file or directory recursively in the given Directory..
bool Search_FD(string Path_To_Search,string FNAME)
{
    //cout<<"function called...."<<Path_To_Search;
    DIR *D_to_search;
    struct dirent *Dirent_TO_Search;
    struct stat File_Info_to_Search;
    if(!(D_to_search=opendir(Path_To_Search.c_str())))
    {
        Print_Alert("can't open the Directory-> "+Path_To_Search);
        return false;
    }
    chdir(Path_To_Search.c_str());
    while((Dirent_TO_Search = readdir(D_to_search))!= NULL){
        lstat(Dirent_TO_Search->d_name, &File_Info_to_Search);
        string name=string(Dirent_TO_Search->d_name);
        //if(name[0] != '.') continue;
        if(FNAME == name)
        {
            Set_Curr_Dir(Path_To_Search.c_str());
            return true;
        }
        if(S_ISDIR(File_Info_to_Search.st_mode) && name !="." && name != ".." && name[0] != '.'){
              bool rep=Search_FD((string(Path_To_Search) + '/' + string(name)).c_str(), FNAME);
                if(rep) return true;
        }
    }
    chdir("../");
    closedir(D_to_search);
    return false;   
}
//----------------------------------------------------| MOVE FILE / DIRECTORY |--------------------------------------------------
//move the Directory or File ..
void move_(){
    int Len=Command_Tokens.size();
     if(!Set_Path())           //Checking for a valid Path..
    {
     Print_Status("Invailid PATH!.."); return;   
    }
    string N_NAME_Temp;
    string N_NAME=Command_Tokens[Len-1];
    for(int j=0;j<Len-2;j++){
        Command_Tokens.pop_back();
        int L=Command_Tokens.size();
    if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return;   
    }
    string O_NAME=Command_Tokens[L-1];
    string Dir_Name="";
   int l=O_NAME.length();
    for (int i = 0; i <l; ++i)
    {
        if(O_NAME[i]=='/')
        {
            Dir_Name="";
            continue;
       }
          Dir_Name.push_back(O_NAME[i]);
    }
      N_NAME_Temp=N_NAME+'/'+Dir_Name;
    if(!rename(O_NAME.c_str(),N_NAME_Temp.c_str())){
        Set_Curr_Dir(Curr_Dir);
        Print_Status("Moved Successfull..");

    }
    else{
        Print_Status("Move Failed..");
    }
}
    
}

//---------------------------------------------------| DELETE FILE / DIRECTORY |----------------------------------------------------
//Delete the File contents and remove the  File..
int delete_file(){
    string File_to_del = Command_Tokens[1];
    int Ret= unlink(File_to_del.c_str());
    return Ret;
}
//Delete recursively all the directory and files inside the called Directory.....
void Delete_Rec(string Path_To_Search ){
 DIR *D_to_search;
    struct dirent *Dirent_TO_Search;
    struct stat File_Info_to_Search;
    if(!(D_to_search=opendir(Path_To_Search.c_str())))
    {
        Print_Alert("can't open the Directory-> "+Path_To_Search);
        return ;
    }
    chdir(Path_To_Search.c_str());
   while((Dirent_TO_Search = readdir(D_to_search))){
        lstat(Dirent_TO_Search->d_name,&File_Info_to_Search);
        if(S_ISDIR(File_Info_to_Search.st_mode)){
            if(strcmp(".",Dirent_TO_Search->d_name)==0 || strcmp("..",Dirent_TO_Search->d_name)==0){
                continue;
            }
            Delete_Rec(Dirent_TO_Search->d_name);
            rmdir(Dirent_TO_Search->d_name);
        }
        else{
            unlink(Dirent_TO_Search->d_name);
        }
    }
    chdir("..");
    closedir(D_to_search);

}
int delete_dir(){
    if(!Set_Path())
    {
     Print_Status("Invailid PATH!.."); return 0;   
    }
    string Dir_to_del = Command_Tokens[1];
    if(Dir_to_del==Curr_Dir){
        Print_Alert("Error : Directory is Opened some where, you can't delete!..");
        return 0;
    }
    Delete_Rec(Dir_to_del);
    rmdir(Dir_to_del.c_str());
    return 1;
}

//To get the Input on buffer from  the keyboard.
//------------------------------------------------------------|  COMMAND-INPUT  |-------------------------------------------------
string Get_Command(){
      char Ch;
      string Command;
      Cursor_Loc=18;
      while(true){
            Ch=cin.get();
           if(Ch==10){
                  Cursor_Loc=18;
                  Mov_Cursor(Out_Line,Cursor_Loc);
                  return  Command;
           }
          /* else if(ch==65 ||ch==66 ||ch==67 ||ch==68){
         continue;
           }*/
           else if(Ch==27){
              Print_Alert("--Well-Come back to Normal mode--");
                  return "esc";
           }
           else if(Ch==127){
                     if(Cursor_Loc==18)
                        continue;
                      Command.pop_back();
                      Cursor_Loc--;
                      Clear_Comm_Mode_Line();
                      cout<<Command;
                      Mov_Cursor(Out_Line,Command.size()+18);

           }
           else{
                 Command.push_back(Ch);
                      cout<<Ch;
                      Cursor_Loc++;
                 Mov_Cursor(Out_Line,Cursor_Loc);
           }

      }
}
//main function to  command mode methods in non Canonical Mode..
void Command_Mode(){
        Mov_Cursor(Mod_Line,0);
        cout<<"\033[0;35m#Command Mode; \033[0m";
        cout << "\033[1;32mPress ESC to switch to Normal  Mode\033[0m\n";
        Mov_Cursor(1,0);
        while(true){
        Clear_Comm_Mode_Line();
        Command_Tokens.clear();
        string Command=Get_Command();
        if(Command=="esc"){
            Mov_Cursor(Mod_Line,0);
      cout<<"\033[0;35m#Normal Mode; \033[0m";
      cout <<"\033[1;32mPress : to switch to Command  Mode\033[0m\n";
            Mov_Cursor(Out_Line,0);
            printf("\033[K");
            Mov_Cursor(Stat_Line,0);
            printf("\033[K");
            Mov_Cursor(1,0);
           break;
        }
        else{
            printf("\033[K");
        }
        string Temp_String;
         int len=Command.length();
         int flag=1;
         for(int i=0;i<len;i++)
         {
            if(flag && Command[i]==' ' ) continue; 
             else if((Command[i]==' ' && i<len-1 && Command[i+1]=='/') || (Temp_String[0]!='/' && Command[i]==' ') ){
                     Command_Tokens.push_back(Temp_String);
                     Temp_String="";
                     flag=1;
             } 
            else
            { 
               Temp_String.push_back(Command[i]);
               flag=0;
            }
         }
         Command_Tokens.push_back(Temp_String);
         string Task=Command_Tokens[0];
         if(Task=="copy"){
              copy_();                                                      //Call to copy_ Function...
         }
         else if(Task=="move"){
            move_();                                                        //Call to move_ Function...
         }
         else if(Task=="rename"){
             rename_();                                                     //Call to rename Function...
             Set_Curr_Dir(Curr_Dir);
         }
        else if(Task=="create_file"){
            create_file();                                                  //Call to create_file Function...
            Print_Status("File Creation Successfull..");
         }
         else if(Task=="create_dir"){
            create_dir();                                                   //Call to create_dir Function...
            Print_Status("Directory Creation Successfully..");
         }
         else if(Task=="delete_file"){                                      //Call to delete_file Function...
             if(!delete_file()){
                   Set_Curr_Dir(Curr_Dir);
                    Print_Status("File deleted Successfully..");
             }
             else
            Print_Status("deletion Failed..");
         }
          else if(Task=="delete_dir"){                                      //Call to delete_dir Function...
            if(delete_dir())
            {
                   Set_Curr_Dir(Curr_Dir);
                    Print_Status("File deleted Successfully..");
            }
            else
            Print_Status("deletion Failed..");
         }
         else if(Task=="goto"){
                    Goto();                                                 //Call to Goto Function...
         }
          else if(Task=="quit"){
            Mov_Cursor(Mod_Line,0);
            cout<<"\033[0;35m#Normal Mode; \033[0m";
            cout <<"\033[1;32mPress : to switch to Command  Mode\033[0m\n";
            Mov_Cursor(Out_Line,0);
            printf("\033[K");
            Mov_Cursor(Stat_Line,0);
            printf("\033[K");
            Mov_Cursor(1,0);  
            break;                                                           //Quit from Command Mode...
         }
         else if(Task=="search"){
            string  FNAME=Command_Tokens[1];
            bool k=Search_FD(string(Curr_Dir),FNAME);                        //Call to search Function...
            if(k)
            Print_Status("True");
        else
            Print_Status("False");
         }
         else{
           Mov_Cursor(Stat_Line,0);
           printf("\033[K");
           cout<<"\033[0;31m"<<"STATUS : Invailid Command! \033[0m";   //Print Status on bar if command is Invailid..
           Mov_Cursor(Out_Line,18);
         }


        }
}
static void sig_handler(int sig)
{
  if (SIGWINCH == sig) {
    struct winsize winsz;

    ioctl(0, TIOCGWINSZ, &winsz);
    Curr_Col_size=winsz.ws_col;
    Curr_row_size=winsz.ws_row;
     // int colm=winsz.ws_col;
     // prnt(colm);
    
            Print_Details();
            
  }

} // sig_handler

//********************************************************************|  MAIN FUNCTION |************************************************
int main()
{

  signal(SIGWINCH, sig_handler);
    printf("\033[H\033[J");
    char Path_Dir[1024];
    size_t S=1024;
    getcwd(Path_Dir,S);
   
     Get_Win_Info(Path_Dir);
     Set_Curr_Dir(Path_Dir);
    printf("\033[H\033[J");
     Set_Curr_Dir(Path_Dir);
     //Set_Curr_Dir("~/");
     Start_Non_Canon();
    //fflush(stdin);
        char inp[3];
        
        while(inp[0]!= 'q'){
        
        inp[0] = cin.get();
        if (inp[0] == 65 ){
        Scroll_Up();
        }
        else if (inp[0] == 66) 
        {
        Scroll_Down();
        }
        else if(inp[0] == 67)
        {
           Back();
        }
        else if(inp[0] == 68)
        {
         Forward();
        }
         else if(inp[0] == 'h' || inp[0] == 'H'){
            Home();
            }
        else if(inp[0] == 10){
        //cout<<"Enter Pressed";
            Enter();
        }
        else if(inp[0] == 127){
            Level_Up();
        }
        else if(inp[0] == ':'){
            Command_Mode();
        }
        }
    Stop_Non_Canon();
    printf("\033[H\033[J");
    return 0;
}
//*********************************************************~\  THE END  /~***************************************************************//
