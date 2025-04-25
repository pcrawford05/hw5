#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    std::string& in, 
    std::string& floating, 
    const std::set<std::string>& dict, 
    std::set<std::string>& retSet, //Set of words we're returning
    size_t& numDashes, 
    size_t idx,   // Index of letter that we're working through
    size_t& nonFloatCount
);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> retSet;
    std::string inputCopy = in;
    std::string floatingCopy = floating;
    size_t numDashes = 0;
    size_t nonFloatCount = 0;
    // Count number of dashes
    for(size_t i = 0; i < in.size(); i++){
        if(in[i] == '-')
            numDashes++;
    }

    wordleHelper(inputCopy, floatingCopy, dict, retSet, numDashes, 0, nonFloatCount);
    return retSet;

}

// Define any helper functions here
void wordleHelper(
    std::string& in,
    std::string& floating,
    const std::set<std::string>& dict,
    std::set<std::string>& retSet,
    size_t& numDashes,
    size_t idx,
    size_t& nonFloatCount
)
{
    if(numDashes == 0){ //The word is full
        if(dict.find(in) != dict.end()) //It is actually a word
            retSet.insert(in); // Insert into set
    } else{
        //Locate first dash
        while(idx < in.size() - 1 && in[idx] != '-'){
            idx++;
        }
        //Try all floating numbers here
        for(size_t j = 0; j < floating.size(); j++){
            if(floating[j] != ' '){
                in[idx] = floating[j];
                numDashes--;
                floating[j] = ' ';
                wordleHelper(in, floating, dict, retSet, numDashes, ++idx, nonFloatCount);
                
                //Revert changes
                floating[j] = in[--idx];
                in[idx] = '-';
                numDashes++;
            }
        }
        //Count how many floats are left
        size_t floatRemaining = 0;
        for(size_t y = 0; y < floating.size(); y++){
            if(floating[y] != ' ')
                floatRemaining++;
        }
        //Only start adding other letters if there's available space
        if(numDashes > floatRemaining){
            for(size_t k = 0; k < 26; k++){
                in[idx] = 'a' + k;
                numDashes--;
                wordleHelper(in, floating, dict, retSet, numDashes, ++idx, ++nonFloatCount);
                
                //Revert changes
                nonFloatCount--;
                in[--idx] = '-';
                numDashes++;
            }
        }
    }
}