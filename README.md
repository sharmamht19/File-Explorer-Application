# Linux based File Explorer
A file explorer for linux systems that runs on terminal
## Prerequisites

**1. Platform:** Linux 

**2. Software Requirement:**
* G++ compiler
   * **To install G++ :** ```sudo apt-get install g++```

## Steps to run the project
1. Open your terminal with the present working directory set as the project folder.
2. Run the **make** command to run the makefile:

    ```make```
3. Finally, run the project using the following command:
    ```./main```

## Assumptions

- Default mode is Normal mode, and you can switch to Command mode by pressing **:**
- Press **ESC** key to go back to normal mode from command mode.
- Press **q** key in normal mode to exit from the program.
- In Normal Mode, the maximum window size is assumed to be 10.
- In Command mode, use the backspace key to edit the command.
- For files/directories with spaces in their names, use a backslash (\\) as the delimiter. For example, if the file name is "os assignment" (without double quotes), you should enter "os\ assignment" (without double quotes).
- It is preferable to open the terminal in full size. If one record spans over more than one line, navigation may not work correctly.
- If **Enter** is pressed on a file and no default application is found, there will be no movement.

