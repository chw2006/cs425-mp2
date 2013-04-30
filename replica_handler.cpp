#include "replica_handler.h"

using namespace mp2;
using namespace std;
using boost::shared_ptr;
using namespace apache::thrift;

#include <cstdlib>
#include <climits>
#include <iostream>

Replica::Replica(int myid, StateMachineFactory & factory, shared_ptr<Replicas> replicas) 
: factory(factory), id(myid), replicas(replicas) {
	// any initialization you need goes here
	pthread_mutex_init(&managerMutex, NULL);
}

void Replica::checkExists(const string &name) const throw (ReplicaError) {
	if (machines.find(name) == machines.end()) {
		ReplicaError error;
		error.type = ErrorType::NOT_FOUND;
		error.name = name;
		error.message = string("Cannot find machine ") + name;
		throw error;
	}	
}

void Replica::create(const string & name, const string & initialState, const std::vector<int32_t> & RMs, const bool fromFrontEnd) {
	//boost::shared_ptr<boost::thread::mutex> stateMachineMutex = new boost::thread::mutex();
	//mutexMap.insert(make_pair(name, stateMachineMutex));
	//mutexMap[name]->lock();
    // locals
    uint i;
    ReplicaError error;
    cout << "Attempting to get lock..." << endl;
    pthread_mutex_lock(&managerMutex);
    cout << "Lock acquired..." << endl;
    // check to see if this SM already exists in other RMs
    if(fromFrontEnd)
    {
       for(i = 0; i < replicas->numReplicas(); i++)
       {
            cout << "Checking if this SM is on any other machine"  << endl;
            if((*replicas)[i].hasStateMachine(name))
            {
			    error.type = ErrorType::ALREADY_EXISTS;
		    	 error.name = name;
		    	 error.message = string("Machine ") + name + (" already exists");
		    	throw error;
            }
        }
    }
   // check if this is a duplicate on this state on this RM, if it is, throw an error
   // also update group list to match given one
	if (machines.find(name) != machines.end()) {
		groupMap[name] = RMs;
		ReplicaError error;
		error.type = ErrorType::ALREADY_EXISTS;
		error.name = name;
		error.message = string("Machine ") + name + (" already exists");
		throw error;
	}
 	// see which RMs we need to pass this create message to
 	if(fromFrontEnd)
 	{
    	for(i = 1; i < RMs.size(); i++)
    	{
    	    cout << "Passing this message to other RMs" << endl;
    	    // pass this message to the other RMs
    	    (*replicas)[RMs[i]].create(name, initialState, RMs, false);
    	}
   }
   // create the machine
   cout << "Inserting state machine into factory..." << endl;
   machines.insert(make_pair(name, factory.make(initialState)));
   std::vector<int32_t> groupVector;
   for(i = 0; i < RMs.size(); i++)
   {
        if((unsigned int)RMs[i] != id)
        {
       		groupVector.push_back(RMs[i]);
        }
   }
   // TODO: LOCK!
   groupMap.insert(make_pair(name, groupVector));
   cout << "Creating machine " << name << " on RM #" << id << ". Now " << machines.size() << " here" << endl;
   pthread_mutex_unlock(&managerMutex);
}

void Replica::apply(string & result, const string & name, const string& operation, const bool fromFrontEnd) {
	checkExists(name);
	string result1;
	std::vector<int32_t> groupVector;
	// if this command is from the front end, then you must pass this on to the other state machines
	if(fromFrontEnd)
	{
	   // apply this to other state machines
	   pthread_mutex_lock(&managerMutex);
	   groupVector = groupMap[name];
	   for(uint i = 0; i < groupVector.size(); i++)
	   {
	      (*replicas)[groupVector[i]].apply(result1, name, operation, false);
	   }
	   pthread_mutex_unlock(&managerMutex);
	}
	// if it's from another RM, then just apply this to yourself
	pthread_mutex_lock(&managerMutex);
	result = machines[name]->apply(operation);
	pthread_mutex_unlock(&managerMutex);
	cout << "Applying operation: " << operation << endl;
}

void Replica::getState(string& result, const string &name) {
	checkExists(name);

	result = machines[name]->getState();
	cout << "Getting state: " << result << endl;
}

void Replica::remove(const string &name) {
	checkExists(name);
	pthread_mutex_lock(&managerMutex);
	machines.erase(name);
	groupMap.erase(name);
	pthread_mutex_unlock(&managerMutex);
	cout << "Removing machine: " << name << ". Now " << machines.size() << " here" << endl;
}

int32_t Replica::numMachines() {
	int32_t result = machines.size();
	cout << "Getting number of machines: " << result << endl;
	return result;
}

bool Replica::hasStateMachine(const std::string & name)
{
   if(machines.find(name) == machines.end()) 
   {
      return false; 
   }
   else
   {
      return true;
   }
}  

// TODO: caller must acquire lock before running this
void Replica::replaceRM(const std::string & name)
{
	// invalidate failed RMs
	std::vector<int>::iterator prev_it = groupMap[name].begin(); 
	for(std::vector<int>::iterator it = groupMap[name].begin(); it != groupMap[name].end(); ++it) {
		try {
			// will throw exception if RM has failed
			(*replicas)[*it].hasStateMachine(name);
		} catch (TException e) {
			// RM failed, remove from group
			groupMap[name].erase(it);
			it = prev_it;
		} catch (exception e) {
			cerr << "Unknown error in replaceRM()" << endl;
			throw e;
		}
	}

	// need to do this until 3 RMs have the state machine again
	while(groupMap[name].size() < 3) {
		int minload = INT_MAX;
		int rep;
		// find leasted loaded RM
		for(uint i = 0; i < replicas->numReplicas(); i++)
	    {
	        if(!(*replicas)[i].hasStateMachine(name))
	        {
	        	int num = (*replicas)[i].numMachines();
			    if(minload > num) {
			    	minload = num;
			    	rep = i;
			    }
	        }
	    }
	    // add new RM to the group list
	    groupMap[name].push_back(rep);
	}

	// send state machine to the new RMs and update everyone's group lists
	for(uint i = 0; i < groupMap[name].size(); i++)
    	(*replicas)[groupMap[name][i]].create(name, machines[name]->getState(), groupMap[name], false);

}

/* DO NOT CHANGE THIS */
void Replica::exit(void) {
	clog << "Replica " << id << " exiting" << endl;
	::std::exit(0);	// no return
}

