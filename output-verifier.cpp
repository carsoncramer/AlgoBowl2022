#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;



void printVec(vector<double> v){
    cout << "*";
    for(auto i : v){
        cout << "'" << i << "'" << " ";
    }
    cout << "*" << endl;
}



bool verifyOutput(string in, string out){

    ifstream input(in);
    ifstream output(out);

    int numTasks = 0, numMachines = 0;
    vector<double> taskRuntimes;
    vector<double> machineSpeeds;
    input >> numTasks >> numMachines;
    for(int i = 0; i < numTasks; i++){
        double temp = 0;
        input >> temp;
        taskRuntimes.push_back(temp);
    }
    for(int j = 0; j < numMachines; j++){
        double temp;
        input >> temp;
        machineSpeeds.push_back(temp);
    }
    //cout << numTasks << " " << numMachines << endl;
    //printVec(taskRuntimes);
    //printVec(machineSpeeds);

    string completionTime; 
    output >> completionTime;
    vector<vector<double>> scheduledTasks;
    while(!output.eof()){
        string line;
        getline(output, line);
        if(line.size() > 0){
            vector<double> tempVec;
        stringstream ss(line);
        double temp;
        while(ss >> temp){
            tempVec.push_back(temp);
        }
        scheduledTasks.push_back(tempVec);
        }  
    }
    input.close();
    output.close();

    // for(auto i : scheduledTasks){
    //     printVec(i);
    // }

    //now everything is inputted, test everything

    int amtOfTasks = 0;
    for(int i = 0; i < scheduledTasks.size(); i++){
        amtOfTasks += scheduledTasks[i].size();
    }
    if(amtOfTasks != numTasks){
        cout << "tasks sizes dont match up" << endl;
        return false;
    }
    if(completionTime[completionTime.size() - 3] != '.'){
        cout << "completion time doesn't have 2 decimals" << endl;
        return false;
    }
    

    //cout << 0 << " " << scheduledTasks[0].size() << endl;
    // for(auto i : scheduledTasks){
    //     cout << i.size() << endl;
    //     printVec(i);
    // }

    double maxRuntime = -1;
    for(int i = 0; i < machineSpeeds.size(); i++){
        double runtime = 0;
        
        for(int j = 0; j < scheduledTasks[i].size(); j++){
            //cout << taskRuntimes[scheduledTasks[i][j] - 1] << "/" << machineSpeeds[i] << endl; 
            runtime += taskRuntimes[scheduledTasks[i][j] - 1] / machineSpeeds[i]; 
        }
        if(runtime > maxRuntime){
            maxRuntime = runtime;
        }
    }

    int roundedTime = round(maxRuntime * 100);
    //cout << test << " " << roundedTime << endl;
    string twoDecPlaces = to_string(roundedTime);
    //cout << twoDecPlaces << endl;
    string maxTimeString = twoDecPlaces.substr(0, twoDecPlaces.size() - 2) + "." + twoDecPlaces.substr(twoDecPlaces.size() - 2, 2);

    if(completionTime != maxTimeString){
        cout << "max(" << maxTimeString << ") does not equal completionTime(" << completionTime << ")" << endl;
        return false;
    }
    

    return true;
}




int main(){

    cout << (verifyOutput("input_group370.txt", "output_370.txt") ? "good" : "bad") << endl;

    // double test = 2.0049;
    // cout << to_string(test) << endl;
    // int roundedTime = round(test * 100);
    // //cout << test << " " << roundedTime << endl;
    // string twoDecPlaces = to_string(roundedTime);
    // //cout << twoDecPlaces << endl;
    // cout << twoDecPlaces.substr(0, twoDecPlaces.size() - 2) << "." << twoDecPlaces.substr(twoDecPlaces.size() - 2, 2) << endl;
    


    return 0;
}