this is the new control method to control the arduino.

in my thought i want to build a cental control pannel to handle all the endpoint.

and make it smarter, more usefull...

on the other hand, i want to make my parents can use this HMI to grasp all the environment argument, and give it human decision.

## Data/goal.csv format
|temperature(C)|Humidity(%)|UV index|raspi Height(cm)|isAutoNet|check|
|-|-|-|-|-|-|
|25|50.0|6|100|1|55|
check = (((temperature%127 + int(Humidity)%127)%127 + (UV index)%127)%127 + (raspi Height)%127)%127 + isAutoNet
For any two numbers got mod by 127, form left to right.

## Data/history.csv
|Time stamp(sec)|machine ID[0-9]|temperature(C)|Humidity(%)|UV index|check|
|-|-|-|-|-|-|
|1581358631|0|27|70.0|5|102|