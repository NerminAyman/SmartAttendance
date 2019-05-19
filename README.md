**Brief Discription**:

Our Project’s is the smart attendance project. It is mainly concerned with facilitating the procedure of taking the attendance for students in a class.
The door is initially closed and does not open unless there is an ID that is swapped in front of the RFID sensor. When the instructor assigned to this specific slot swapped his/her ID, the door opens and the session starts on the system. Consequently, each student, who tends to enter the class, has to swap his/her ID; so that the door will be opened and the attendance of this student will be recorded. Also, the student cannot leave the room without swapping his ID again to let the door open. (door is simulated using two LEDs , a red led to indicate that flashes when the door opens , a yellow led that indicates that the door is closed) , In case of fire , the door opens and the buzzer is turned on and a message is displayed on LCD. At the end of the class, there will be an attendance sheet that store the students and their stat(whether “Attended” or “Absent”).
This approach will let us keep track of the exact duration the student has spent inside he class; so that the student that hasn’t spent more than 75% of the class duration will not be counted as “Attended”.

**The components and their functionalities:**

**The project components are:**

- Two LEDs (representing the door)
- Smoke sensor (detects smoke in case of fire)
- LCD (displays the number of students in a room)
- Buzzer (rings as warning in case of fire)
- RFID sensor (detects the IDs and their bar-code)
- Arduino Mega board.

**Libraries Used:**
- MFRC522: provides functions that are used to capture the reading of the RFID sensor to the cards.
- LiquidCrystal: provides functions that are used to display data.

**Inputs:**
- Card serial numbers: the RFID Sensor is connected to specific digital pins according to RFID library. During the process of scanning Id, data is sent through serial communication.
-Smoke: Smoke sensor is connected to analog input pin (A5); to detect the external smoke and then it is converted in the code to digital value using method (analogRead(smokeA0)).
Outputs:
- The LEDS lights: The output is configured using the method pinMode(LED Name attribute, OUTPUT). To control its state, we used the method digitalWrite(LED Name attribute, state), the state is either HIGH or LOW.
- Data displayed on LCD: we used the external library <LiquidCrystal.h>, that settles the screen as output source that handles the data we pass them to it using the method lcd.print(Data we pass).
- Buzzer tone: The output is configured like the two LEDs using the method pinMode(LED Name attribute, OUTPUT). To control its state, we used the method digitalWrite(LED Name attribute, state), the state is either HIGH or LOW.

**Limitations:**
There was no direct method to save the data printed in the serial monitor into a text file, so we used an external application called “TeraTerm”. It takes any output on Port “COM3” and saves it an external text file.

**How To Run:**
First we press the uploading icon on the arduino IDE. Then after it has done uploading, we open the TeraTerm, choose the serial option and press “OK”. Afterwards, we open the file drop down list, choose log and enter the (.txt) file name after choosing its destination.
