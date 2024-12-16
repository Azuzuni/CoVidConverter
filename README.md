# CoVidConverter

Console Video Converter
An attempt to make application that converts video file into console output


# Project Build

Required Cmake and OpenCV
After downloading OpenCV and adding it to your path
try to compile the project, in case it does not compile check CmakeLists.txt in order to edit how OpenCV is included in the project.
I have been using GCC 12 to compile this project

# How to use

After launching, the program will search ../video folder to find mp4 then display options in terminal
after choosing the video, program will start converting it in real time
the screen size of video can be accessed via variable in myApp.h 
FPS cap is based on initial video FPS count
The program uses SetConsoleCursorPosition to change symbols in correct places therefore if the terminal window is too small the video will mess up the way it looks.
