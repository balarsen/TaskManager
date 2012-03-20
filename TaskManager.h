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
#ifndef TASKMANAGER_h
#define TASKMANAGER_h

#include <stdint.h>

/* useful constants
  F_CPU - CPU frequency
 */

class Task {
  public:
    // uint8_t taskNum;   // should a task know its own number?
    Task( void (*fn)(), uint32_t ticks);
    Task operator++();    //Prefix decrement operator  (++x)
    Task operator++(int); //Postfix decrement operator (x++)
  private:
    void (*function)();
    uint32_t counter;
    uint32_t max_cnt;
};

class TaskManager {
  public:
    TaskManager(uint8_t nTasks, uint32_t useconds);  // nTasks is the number of task you will have
    void attach( Task *task, uint8_t taskNum );
    void start();
    void stop();
//    void overflow();
    Task* tasks;
    uint8_t nTasks;
  private:
    uint32_t useconds;
};


#endif
