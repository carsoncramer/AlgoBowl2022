#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;

int taskTests = 0;
// I can also probably rewrite the replace task functionality to grab tasks by ID instead of value.
//also the way I did this I can probably make addTask just a push_back instead of maintaining the order, because I don't think there is any need for it.
// but also maybe the order is making it work better because it will make small changes first... i dont know. 


//If I hold the Machines in a heap based on their completion time I can get the max in O(1)
// the sum(tasks)/sum(machines) could be the number we want to hit
class Machine {

    private:
        //pair<taskSize, taskId>
        vector<pair<double,int>> tasks; //this will be ordered
        int id;
        double completionTime;

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

        vector<pair<double,int>>& getTasksRef(){
            return tasks;
        }


        int tasksSize(){ //i think this will speed up performance 
            return tasks.size();
        }

        int getId(){
            return id;
        }

        double getCompletionTime() const { 
            return completionTime;
        }
        
        void printTasks(ofstream& file){
            vector<pair<double, int>> tempTasks = tasks;
            sort(tempTasks.begin(), tempTasks.end(), 
                [](pair<double,int> smallerId, pair<double,int> largerId){
                    return smallerId.second < largerId.second;
                });
            for(pair<double, int> t : tempTasks){
                if(t.second >= 0){
                    taskTests++;
                    file << t.second + 1 << " ";
                }
            }
            file << endl;
        }

        void print(){
            cout << endl << "Tasks: (" << tasks.size() - 1 << ")";
            for(auto i : tasks){
                cout << i.first << ", ";
            }
            cout << endl << "Speed: " << speed << " CompletionTime: " << setprecision(10) << completionTime << endl;
            int roundedTime = round(completionTime * 100);
            string twoDecPlaces = to_string(roundedTime);
            cout << endl << "OUTPUT:" << endl << twoDecPlaces.substr(0, twoDecPlaces.size() - 2) << "." << twoDecPlaces.substr(twoDecPlaces.size() - 2, 2) << endl;

        }

        bool operator < (const Machine& m) const {
            return completionTime < m.getCompletionTime();
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
        for(Machine &m : machines){ //O(h*n) 
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

//okay if i make a heap out of the vector, then I can always get the max in O(1), then whenever I make a swap I can check to see if it is
//still greater than the next two elements, if not then recursively move her down to maintain heap structure. 
// I am trying to basically redo the minimizeAverage algorithm but with a heap to make the speed for getting the max element faster.

void minimizeAverageHEAP(vector<Machine>& heap){
    cout << "in algo" << endl;
    bool noReplacements = false;
    while(!noReplacements){
        noReplacements = true;

        Machine& max = heap.front();
        vector<pair<double, int>> maxTasks = max.getTasksRef();
        pair<double, int> bestReplacement;
        for(int i = 1; i < heap.size(); i++){
            double bestReplacementCompletionTime = max.getCompletionTime();
            Machine& m = heap[i];
            vector<pair<double, int>> mTasks = m.getTasksRef();
            for(int j = 0; j < maxTasks.size(); j++){
                for(int k = 0; k < mTasks.size(); k++){
                    //cout << "max: " << maxTasks[j].first << " m: " << mTasks[k].first;
                    if(max.evaluateReplacement(maxTasks[j].first, mTasks[k].first) < bestReplacementCompletionTime && m.evaluateReplacement(mTasks[k].first, maxTasks[j].first) < max.getCompletionTime()){
                        // cout << endl << max.evaluateReplacement(maxTasks[j].first, mTasks[k].first) << " < " << bestReplacementCompletionTime << " && " << m.evaluateReplacement(mTasks[k].first, maxTasks[j].first) << " < " << max.getCompletionTime() << endl;
                        // cout << (max.evaluateReplacement(maxTasks[j].first, mTasks[k].first) < bestReplacementCompletionTime) << " && " << (m.evaluateReplacement(mTasks[k].first, maxTasks[j].first) < max.getCompletionTime()) << endl; 
                        // cout << "inside" << endl;
                        bestReplacementCompletionTime = max.evaluateReplacement(maxTasks[j].first, mTasks[k].first);
                        bestReplacement = mTasks[k];
                    }
                }
                if(bestReplacementCompletionTime < max.getCompletionTime()){
                    //cout << bestReplacementCompletionTime  << " " << max.getCompletionTime() << endl;
                    noReplacements = false;
                    max.replaceTask(maxTasks[j], bestReplacement);
                    m.replaceTask(bestReplacement, maxTasks[j]);
                }
                //cout << max.getCompletionTime() << endl;
                if(max.getCompletionTime() < heap[1].getCompletionTime() || max.getCompletionTime() < heap[2].getCompletionTime()){
                    //can possibly remove the two middle lines. 
                    pop_heap(heap.begin(), heap.end());
                    heap.pop_back();
                    heap.push_back(max);
                    push_heap(heap.begin(), heap.end());
                    goto forbiddenCommand;
                }
            }
        }
        forbiddenCommand:;
    }
    cout << "leaving algo" << endl;
}


/*
5
2
12 7 3 22 24
1 2
*/

vector<Machine> getInputAndSplitTasks(){
    ifstream file("input_group370.txt");
    int numTasks, numMachines;
    file >> numTasks >> numMachines;

    double addedTasks = 0;
    double addedMachines = 0;

    vector<pair<double, int>> tasks;
    for(int i = 0; i < numTasks; i++){
        double temp;
        file >> temp;
        tasks.push_back({temp, i});
        addedTasks += temp;
    }
    vector<Machine> machines;
    for(int i = 0; i < numMachines; i++){
        int temp;
        file >> temp;
        Machine m(temp, i);
        machines.push_back(m);
        addedMachines += temp;
    }
    cout << "&&&&& " << addedTasks / addedMachines << " &&&&&&" << endl;
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
    
    file.close();
    return machines;
}


void printOutput(vector<Machine> machines){
    ofstream file;
    file.open("output_370.txt");
    vector<Machine>::iterator iter = max_element(machines.begin(), machines.end(),[](Machine& lowerTime, Machine& higherTime){
        return lowerTime.getCompletionTime() < higherTime.getCompletionTime();
    });
    //I have to do it this way to avoid weird floating point rounding errors. 
    int roundedTime = round(iter->getCompletionTime() * 100);
    string twoDecPlaces = to_string(roundedTime);
    cout << endl << "MAX VALUE:" << endl << twoDecPlaces.substr(0, twoDecPlaces.size() - 2) << "." << twoDecPlaces.substr(twoDecPlaces.size() - 2, 2) << endl;
    file << twoDecPlaces.substr(0, twoDecPlaces.size() - 2) << "." << twoDecPlaces.substr(twoDecPlaces.size() - 2, 2) << endl;
    //sort machines by id
    sort(machines.begin(), machines.end(), 
        [](Machine smallerId, Machine largerId){
            return smallerId.getId() < largerId.getId();
        });
    for(Machine m : machines){
        m.printTasks(file);
    }
    file.close();
}



/*

10
5
12 45 67 13 2 4 6 9 9 1
1 4 6 7 3

5
2
12 7 3 22 24
1 2

*/

int main(){


    
    vector<Machine> machines = getInputAndSplitTasks();
    // 12069.95
    minimizeAverage(machines); 
    // cout << "before make heap" << endl;
    // make_heap(machines.begin(), machines.end());
    // cout << "before algo" << endl;
    // minimizeAverageHEAP(machines);
    
    
    //testing
    cout << endl << "======================" << endl;
    // for(auto m : machines){
    //     m.print();
    // }
    
    

    printOutput(machines);

    cout << taskTests << endl;

    return 0;
}
