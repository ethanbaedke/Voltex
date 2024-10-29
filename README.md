# Voltex
The Voltex Engine is a tool used to rapdily prototype 2D platforming levels
Currently, the Voltex Engine only supports Windows(x64) and build settings are only set for Visual Studio

# Build Instructions
1) Download and unzip the repository on your Window(x64) machine
2) Run the CleanBuildProject.bat file
3) Open the generated Voltex.sln file
4) Right click the VoltexGame project and select "Set as Startup Project"

# Level Design Instructions
1) Right click the VoltexTool project and select "Set as Startup Project"
2) Run the VoltexTool and click boxes in the grid to design a room of a certain [roomType]
3) [Standard], [Start], and [End] rooms must have openings on the left and right, [Drop] rooms must have an opening on the bottom, [Catch] rooms must have an opening on the top
4) Save the room in VoltexProject/rooms/[roomType] where [roomType] is a premade folder holding rooms of that type
5) Right click the VoltexGame project and select "Set as Startup Project"
6) Run the VoltexGame and see your rooms being used in the generated level
