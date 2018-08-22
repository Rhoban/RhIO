! Rhio Shell in the context of Rhoban robots

   USAGE: rhio [IP_ADDRESS]
   If you don't input the IP_ADDRESS parameter, the default value will be 127.0.0.1 (localhost=your computer)
   Otherwise you should input the robot's IP_ADDRESS.

   Inside the rhio shell, data are organised as Commands, Directories and Files. You can navigate through the rhio virtual filesystem using the standard *ls* and *cd* commands.
   You can also use the autocompletion using the TAB key.

   The main directories are:
   The */lowlevel* directory contains everything related to the low level system i.e the motors and sensors.
   The */moves* directory contains everything related to the moves (one sub directory for every move).
   The */Vision* directory contains everything related to the vision (this directory doesn't exist in novision mode) i.e. one sub directory for every vision filter.
   The */localisation* directory contains everything related to the localisation and the Particule Filter.

   The main rhio shell commands:

* help:
     shows help on rhio command

* pad [GAMEPAD_CONFIG.json]: 
     If run without argument it will display the buttons of the plugged gamepad (useful to configure a new gamepad)
     If run with a filename it will run the gamepad control. Example of configuration files are located in workspace/env/rhio/
     To use the configuration file you should put it into the directory ~/.rhio/ (create it if it doesn't exist)
     example: pad xbox_walk.json (have a look at the content of the file for more explanations)

* delay: 
     Usage: delay [n secs] [command]
     Delays a command
     example: delay 5 start walk (will start the walk move in 5 seconds)

* repeat: 
     Usage: repeat [command]
     Repeats a command

* diff: 
     Shows the what parameters have changed compared to the stored values

* save:
     Store the parameters.
     The values will be saved in the workspace/env/YOUR_ROBOT/rhio following the same filesystem hierarchy.

* load: 
     Loads the stored parameters values

* plot:
     Plot paramters
     Example: plot lowlevel/imu/yaw lowlevel/head_pitch/position 

* log: 
     Usage: log [param1 [param2 [param3 ...]]] [> filename]
     Log values to a csv file. Very similar to plot, but instead of plotting it will save the data.
     Example: log lowlevel/imu/yaw lowlevel/head_pitch/position > tmplog (will save the values of imu/yaw and head_pitch/position into the local file tmplog)
     Example: If you want to log the time series of the actual motors position during the walk, you should first initiate the robot and run the walk, then
              you can log all the motors you need with for example: log lowlevel/right_knee/position lowlevel/right_ankle_pitch/position 
	      you can also log the motors commands: log lowlevel/right_knee/goalPosition lowlevel/right_ankle_pitch/goalPosition
	      Notice that in fake mode, only the goalPosition is updated.

* plot2d: 
     Usage: x-var y1 [y2 [y3...]]
     Plot 2D parameters
     Example: plot2d lowlevel/right_pressure/x lowlevel/right_pressure/y

* plot3d:
     Usage: x-var y1 y2 [y3 [y4...]]
     Plot 3D parameters

* tune: 
     Usage: tune [var1] [var2] [var3]...
     Run the tuner graphical interface to show/change values

* watch: 
     Continuously watch parameters from the current node

* em:
     Emergency stop. Will power off all motors.

* init:
     Initialize the robot to the zero position on all motors

* infoPlaying:
     Tells informations about the referee (are we playing?)

* team: (from /teamplay/)
     Display information about the current robot state and referee state. These are the same informations than showed in the MonitoringRobocup software.

* tare:
     Performs a pressure sensors calibration. Very important to do while the robot is off the ground.

* gyroTare:
     Performs an IMU sensor calibration. Very important to do while the robot is in walk mode on the ground and without moving.

* stats:
     Gives timings statistics about the MoveScheduler

* view:
     Usage: view [frame]
     View a frame stream. Only in vision mode, in each Vision filter sub directory there is a *out* frame that you can display.
     Example: view /Vision/green/out (will show the result of the green filter)
     Some special frames also exists: view /Vision/TaggedImg (shows a summary of the vision computation)
   
   Some rhio shell commands related to the localisation particule filter:

* view /localisation/TopView: shows a summary of the localisation particule filter

* resetPosition: will reset the particule filter with all uniform random positions.

* customReset [X] [Y] [Orientation]: will reset the particule filter at a given X Y Orientation position. Default values are X=0, Y=0, Orientation=0.

* borderReset: will reset the particule filter on the border line on the team side (after a pickup)
