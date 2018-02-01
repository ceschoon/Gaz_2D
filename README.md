# Gaz_2D
A program that simulates a gaz by implementing elastical collisions between particles in a 2D box. The velocity distribution is shown.

![alt text](Screenshot-brownian.png?raw=true "Screenshot - Brownian Motion")
![alt text](Screenshot-velocities.png?raw=true "Screenshot - Distribution of Velocities")

Usage (the Windows executable as to stay in the same folder than the dll's):

1. Open a command prompt and go the the location of this file (by typing "cd <path>")

2. Run the command "Gaz_2D.exe" to execute the program

3. You can set some parametres by addings arguments after the command. 

E.g:  "Gaz_2D.exe --number=20 --noDistribution" 
will call the program such as the number of particles to 20 and no window for the velocities distribution will appear.

4. Typing "Gaz_2D.exe --help" will show a list of all the possible parameters that can be used.