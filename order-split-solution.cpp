#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <iomanip>
using namespace std;


// I can also probably rewrite the replace task functionality to grab tasks by ID instead of value.
//also the way I did this I can probably make addTask just a push_back instead of maintaining the order, because I don't think there is any need for it.
// but also maybe the order is making it work better because it will make small changes first... i dont know. 


class Machine {

    private:
        //pair<taskSize, taskId>
        vector<pair<double,int>> tasks; //this will be ordered
        double completionTime;
        int id;

    public:

        double speed;

        Machine(double time, int idNum){
            completionTime = 0;
            speed = time;
            tasks.push_back({0,-1});
            id = idNum;
        }

        void addTask(pair<double,int> task){
            //I havent determined yet if maintainind the order of the tasks is beneficial or not, but one thing that could speed this up is if I stopped caring about the order
            //also in the prompts output it lists the tasks in order so maybe I keep it
            if(tasks.size() != 0){
                bool didInsert = false; //there is probably a better way of doing this, but when i tried doing tasks.end() - 1 i kept getting segfaults.
                for(auto it = tasks.begin(); it != tasks.end(); it++){
                    if(task.first <= it->first){ 
                        tasks.insert(it, task);
                        didInsert = true;
                        break;
                    }
                }
                if(!didInsert){
                    tasks.push_back(task);
                }
            }
            else{
                tasks.push_back(task);
            }
            completionTime += task.first/speed;
        }

        void replaceTask(pair<double,int> original, pair<double,int> replacement){
            //cout << endl << original << ", " << replacement << endl;
            completionTime -= (original.first/speed);
            if(original.second != -1){ // making sure every vector has an "empty" spot
                auto position = find(tasks.begin(), tasks.end(), original);
                if(position != tasks.end()){
                    tasks.erase(position);
                }
            }
            if(replacement.second != -1){ //this is to make sure we dont have like 50 zeros in one vector. 
                addTask(replacement);
            }
            
        }

        double evaluateReplacement(double original, double replacement){
            return (completionTime - (original/speed)) + (replacement/speed);
        }

        vector<pair<double,int>> getTasks(){
            return tasks;
        }
        int tasksSize(){ //i think this will speed up performance 
            return tasks.size();
        }

        int getId(){
            return id;
        }

        double getCompletionTime(){ 
            return completionTime;
        }
        
        void printTasks(){
            vector<pair<double, int>> tempTasks = tasks;
            sort(tempTasks.begin(), tempTasks.end(), 
                [](pair<double,int> smallerId, pair<double,int> largerId){
                    return smallerId.second < largerId.second;
                });
            for(pair<double, int> t : tempTasks){
                if(t.second >= 0)
                cout << t.second + 1 << " ";
            }
            cout << endl;
        }

        void print(){
            cout << endl << "Tasks: (" << tasks.size() - 1 << ")";
            for(auto i : tasks){
                cout << i.first << ", ";
            }
            cout << endl << "Speed: " << speed << " CompletionTime: " << completionTime << endl;
        }

};

void printTasks(vector<Machine> machines){
    cout << endl << "***************" << endl;
    for(Machine m : machines){
        cout << "S: " << m.speed << " CT: " << m.getCompletionTime() << " {";
        for(pair<double,int> task : m.getTasks()){
            cout << task.first << ",";
        }
        cout << "}" << endl;
    }
    cout << endl << "***************" << endl;
}

void minimizeAverage(vector<Machine>& machines){
    bool noReplacements = false;
    while(!noReplacements){ //this is an attempt to find a new max every time
        noReplacements = true;
        vector<Machine>::iterator iter = max_element(machines.begin(), machines.end(),[](Machine& lowerTime, Machine& higherTime){
            return lowerTime.getCompletionTime() < higherTime.getCompletionTime();
        });
        Machine& max = *iter; 
        //we need to find the new max each time instead of iterating through
        //printTasks(machines);
        for(Machine &m : machines){
            for(int i = 0; i < max.tasksSize(); i++){//original 
                vector<pair<double,int>> maxTasks = max.getTasks();
                vector<pair<double,int>> mTasks = m.getTasks();
                double bestReplacementCompletionTime = max.getCompletionTime();
                pair<double,int> bestReplacement;
                for(int j = 0; j < m.tasksSize(); j++){//replacement
                    if(max.evaluateReplacement(maxTasks[i].first, mTasks[j].first) < bestReplacementCompletionTime && m.evaluateReplacement(mTasks[j].first, maxTasks[i].first) < max.getCompletionTime() && max.evaluateReplacement(maxTasks[i].first, mTasks[j].first) < max.getCompletionTime()){
                        //cout << "max: " << maxTasks[i] << " m: " << mTasks[j];
                        //cout << endl << max.evaluateReplacement(maxTasks[i], mTasks[j]) << " < " << bestReplacementCompletionTime << " && " << m.evaluateReplacement(mTasks[j], maxTasks[i]) << " < " << max.getCompletionTime() << " && " << max.evaluateReplacement(maxTasks[i], mTasks[j]) << " < " << max.getCompletionTime() << endl; 
                        bestReplacementCompletionTime = max.evaluateReplacement(maxTasks[i].first, mTasks[j].first);
                        bestReplacement = mTasks[j];
                    }
                }
                if(bestReplacementCompletionTime < max.getCompletionTime()){
                    noReplacements = false;
                    max.replaceTask(maxTasks[i], bestReplacement);
                    m.replaceTask(bestReplacement, maxTasks[i]);
                }
            }
        }
    }
}


/*
5
2
12 7 3 22 24
1 2
*/

vector<Machine> getInputAndSplitTasks(){
    int numTasks, numMachines;
    cin >> numTasks >> numMachines;

    vector<pair<double, int>> tasks;
    for(int i = 0; i < numTasks; i++){
        double temp;
        cin >> temp;
        tasks.push_back({temp, i});
    }
    vector<Machine> machines;
    for(int i = 0; i < numMachines; i++){
        int temp;
        cin >> temp;
        Machine m(temp, i);
        machines.push_back(m);
    }
    //sort tasks by size
    sort(tasks.begin(), tasks.end(), 
        [](pair<double,int> smallerTask, pair<double,int> largerTask){
            return smallerTask.first > largerTask.first;
        });
    //sort machines by speed
    sort(machines.begin(), machines.end(),
        []( Machine& lowerSpeed,  Machine& higherSpeed){
            return lowerSpeed.speed > higherSpeed.speed;
    });

    //assigning tasks to machines but better
    int baseTaskSize = tasks.size() / machines.size();
    int remainingTasks = tasks.size() % machines.size();
    int index = 0;
    int bound = 0;
    for(int i = 0; i < machines.size(); i++){
        bound += (i == 0 ? baseTaskSize + remainingTasks : baseTaskSize);
        for(int j = index; j < bound && j < tasks.size(); j++){
            machines[i].addTask(tasks[j]);
        }
        index = bound;
    }

    // testing code
    /*
    for(auto t : tasks){
        cout << t.first << " ";
    }

    for(auto m : machines){
        m.print();
    }
    */
    

    return machines;
}


void printOutput(vector<Machine> machines){
    vector<Machine>::iterator iter = max_element(machines.begin(), machines.end(),[](Machine& lowerTime, Machine& higherTime){
        return lowerTime.getCompletionTime() < higherTime.getCompletionTime();
    });
    //I have to do it this way to avoid weird floating point rounding errors. 
    int roundedTime = round(iter->getCompletionTime() * 100);
    string twoDecPlaces = to_string(roundedTime);
    cout << endl << "OUTPUT:" << endl << twoDecPlaces.substr(0, twoDecPlaces.size() - 2) << "." << twoDecPlaces.substr(twoDecPlaces.size() - 2, 2) << endl;

    //sort machines by id
    sort(machines.begin(), machines.end(), 
        [](Machine smallerId, Machine largerId){
            return smallerId.getId() < largerId.getId();
        });
    for(Machine m : machines){
        m.printTasks();
    }
}



/*

10
5
12 45 67 13 2 4 6 9 9 1
1 4 6 7 3

5
2
12 7 3 22 4
1 2

*/

int main(){

    vector<Machine> machines = getInputAndSplitTasks();

    minimizeAverage(machines);

    /*
    //testing
    cout << endl << "======================" << endl;
    for(auto m : machines){
        m.print();
    }
    */
    

    printOutput(machines);


    return 0;
}