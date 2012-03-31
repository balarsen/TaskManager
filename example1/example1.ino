/*
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See GitHub project https://github.com/balarsen/TaskManager for latest 
 *      version, please feel free to fork and/or post patches
 */

#include <TaskManager.h>

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define SERIAL_DEBUG

////////////////////////////////////////////
// Demo functions
////////////////////////////////////////////

class Blink : public Task
{
public:
  Blink(int time) : _time(time) {}
  void operator()() {
    digitalWrite(greenLEDpin, digitalRead(greenLEDpin) ^ 1);
    #ifdef SERIAL_DEBUG
      Serial.println("in Blink");
    #endif

  }
}

TaskManager TM = TaskManager(1); // number of tasks

Blink blink(500); // runs every 500 milliseconds

void setup()
{
  #ifdef SERIAL_DEBUG
    Serial.begin(19200);
  #endif
  pinMode(greenLEDpin, OUTPUT);   
  TM.add(blink);  // add the task to the TM
}

void loop()
{
}

