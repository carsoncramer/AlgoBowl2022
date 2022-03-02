#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
class check {
private:
    int numTasks, numMachines;
    vector<int> tasks;
    vector<int> machineSpeeds;
    string outputString;
    float output;
    vector<vector<int>> assignedTasks;
public:
    bool checkInput() {
        ifstream fileOpen;
        string inputFilename;
        cout << "Enter input filename" << endl;
        cin >> inputFilename;
        fileOpen.open(inputFilename);
        string temp;
        if (fileOpen.is_open()) {
            fileOpen >> temp;
            stringstream taskNum(temp);
            taskNum >> numTasks;
        } else {
            cerr << "File not opened";
            return false;
        }
        if (fileOpen.is_open()) {
            fileOpen >> temp;
            stringstream machineNum(temp);
            machineNum >> numMachines;
        } else {
            cerr << "File not correct";
            return false;
        }
        int taskToAdd;
        for (int i = 0; i < numTasks; i++) {
            if (fileOpen.is_open()) {
                fileOpen >> temp;
                stringstream task(temp);
                task >> taskToAdd;
                tasks.push_back(taskToAdd);
            } else {
                cerr << "Assigned number of tasks does not match given number of tasks";
                return false;
            }
        }
        int machToAdd;
        for (int i = 0; i < numMachines; i++) {
            if (fileOpen.is_open()) {
                fileOpen >> temp;
                stringstream mach(temp);
                mach >> machToAdd;
                machineSpeeds.push_back(machToAdd);
            } else {
                cerr << "Assigned number of tasks does not match given number of tasks";
                return false;
            }
        }
        return true;
    }
    void printInput() {
        cout << numTasks << endl;
        cout << numMachines << endl;
        for (int i = 0; i < numTasks; i++) {
            cout << tasks.at(i) << " ";
        }
        cout << endl;
        for (int i = 0; i < numMachines; i++) {
            cout << machineSpeeds.at(i) << " ";
        }
        cout << endl;
    }
    bool checkOutput() {
        ifstream fileOpen;
        string outputFilename;
        cout << "Enter output filename" << endl;
        cin >> outputFilename;
        fileOpen.open(outputFilename);
        string temp;
        if (fileOpen.is_open()) {
            fileOpen >> temp;
            outputString = temp;
            //Check if output is a double calculated to hundredths place
            if (temp[temp.size()-3] != '.') {
                cerr << "Not a valid output, must round to hundredths place";
                return false;
            }
            output = stof(temp);
        } else {
            cerr << "File not opened";
            return false;
        }
        for (int i = 0; i <= numMachines; i++) {
            string line;
            int back = 0;
            int taskToAdd;
            vector<int> machineAndTasks;
            getline(fileOpen,line);
            if (line != "") {
                //Extract all tasks assigned to a machine
                line = line + " ";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == ' ') {
                        temp = line.substr(back,i);
                        stringstream task(temp);
                        task >> taskToAdd;
                        machineAndTasks.push_back(taskToAdd);
                        back = i;
                    }
                }
                assignedTasks.push_back(machineAndTasks);
            }
        }
        return true;
    }
    void printOutput() {
        cout << outputString << endl;
        for (int i = 0; i < numMachines; i++) {
            for (int j = 0; j < assignedTasks[i].size(); j++) {
                cout << assignedTasks[i][j] << " ";
            }
            cout << endl;
        }
    }
    bool checkValid() {
        float max = 0;
        int summation, index;
        float average;
        for (int i = 0; i < numMachines; i++) {
            summation = 0;
            average = 0;
            for (int j = 0; j < assignedTasks[i].size(); j++) {
                index = assignedTasks[i][j];
                summation += tasks[index-1];
            }
            average = summation/machineSpeeds[i];
            if (average > max) {
                max = average;
            }
        }
        //cout << "MAX: " << max << endl;
        //cout << "OUT: " << output << endl;
        if (max == output) {
            return true;
        }
        else return false;
    }
};



int main() {
    check Check;
    Check.checkInput();
    //Check.printInput();
    Check.checkOutput();
    //Check.printOutput();
    cout << Check.checkValid() << endl;
}
