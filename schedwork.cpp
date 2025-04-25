#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t& m,
    DailySchedule& sched,
    std::vector<Worker_T>& numShiftsList, // List of days scheduled for each worker based on ID
    Worker_T largestIDScheduled, // Last ID we scheduled because we go in order
    size_t currentDay // Current day we are working through because we go in order
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    for(size_t i = 0; i < avail.size(); i++){
        sched.push_back(std::vector<Worker_T>(dailyNeed, INVALID_ID)); // Build "empty" matrix of size n x d
    }
    std::vector<Worker_T> numShiftsList(avail[0].size(), 0); // List of number of days that each worker has worked
    return scheduleHelper(avail, maxShifts, sched, numShiftsList, INVALID_ID, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t& m,
    DailySchedule& sched,
    std::vector<Worker_T>& numShiftsList,
    Worker_T largestIDScheduled,
    size_t currentDay
)
{
    // Has anyone been scheduled more than m times?
    if(std::find(numShiftsList.begin(), numShiftsList.end(), m + 1) != numShiftsList.end())
        return false;

    Worker_T k = numShiftsList.size(); // Number of workers

    for(size_t i = currentDay; i < avail.size(); i++){
        // Is this day filled up yet?
        std::vector<Worker_T>::iterator it = std::find(sched[i].begin(), sched[i].end(), INVALID_ID);
        if(it != sched[i].end()){
            // Only go through IDs we haven't used for this day
            for(Worker_T j = largestIDScheduled + 1; j < k; j++){
                if(avail[i][j] == 1){ // Worker is available this day
                    *it = j;
                    numShiftsList[j]++;
                    if(!scheduleHelper(avail, m, sched, numShiftsList, j, i)){
                        numShiftsList[j]--; //Revert list then continue trying IDs
                        continue;
                    } else{
                        return true; // It did work
                    }
                }
            }
            *it = INVALID_ID;
        } 
        // If there's still an invalid ID return false
        if(std::find(sched[i].begin(), sched[i].end(), INVALID_ID) != sched[i].end()){
            return false;
        }
        largestIDScheduled = INVALID_ID; //Moving to the next day
    }
    return true;
}