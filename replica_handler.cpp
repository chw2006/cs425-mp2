#include "replica_handler.h"

using namespace mp2;
using namespace std;
using boost::shared_ptr;

#include <cstdlib>
#include <iostream>

Replica::Replica(int myid, StateMachineFactory & factory, shared_ptr<Replicas> replicas) 
: factory(factory), id(myid), replicas(replicas) {
	// any initialization you need goes here
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
   // locals
   uint i;
   ReplicaError error;
   // check to see if this SM already exists in other RMs
   if(fromFrontEnd)
   {
      for(i = 0; i < replicas->numReplicas(); i++)
      {
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
	if (machines.find(name) != machines.end()) {
		ReplicaError error;
		error.type = ErrorType::ALREADY_EXISTS;
		error.name = name;
		error.message = string("Machine ") + name + (" already exists");
		throw error;
	}
 	// see which RMs we need to pass this create message to
 	if(fromFrontEnd)
 	{
 		// build group list for this state machine
 		// TODO: LOCK THIS THING
 		pair<int, int> group = pair<int, int>(RMs[0], RMs[1]);
 		groups.insert(make_pair(name, group));

    	for(i = 1; i < RMs.size(); i++)
    	{
    	   // pass this message to the other RMs
    	   (*replicas)[RMs[i]].create(name, initialState, RMs, false);
    	}
   }
   // create the machine
   machines.insert(make_pair(name, factory.make(initialState)));
   pthread_mutex_t * stateMachineMutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
   mutexMap.insert(make_pair(name, stateMachineMutex));
   std::vector<int32_t> groupVector = new std::vector<int32_t>(2);
   for(i = 0; i < RMs.size(); i++)
   {
      if(RMs[i] != id)
      {
         groupVector->push_back(RMs[i]);
      }
   }
   groupMap.insert(make_pair(name, groupVector));
   cout << "Creating machine " << name << " on RM #" << id << ". Now " << machines.size() << " here" << endl;
}

void Replica::apply(string & result, const string & name, const string& operation) {
	checkExists(name);
	result = machines[name]->apply(operation);
	cout << "Applying operation: " << operation << endl;
}

void Replica::getState(string& result, const string &name) {
	checkExists(name);

	result = machines[name]->getState();
	cout << "Getting state: " << result << endl;
}

void Replica::remove(const string &name) {
	checkExists(name);

	// TODO: LOCK!
	machines.erase(name);
	groups.erase(name);
	cout << "Removing machine: " << name << ". Now " << machines.size() << " here" << endl;
}

int32_t Replica::numMachines() {
	int32_t result = machines.size();
	cout << "Getting number of machines: " << result << " ~~~ " << id << endl;
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

/* DO NOT CHANGE THIS */
void Replica::exit(void) {
	clog << "Replica " << id << " exiting" << endl;
	::std::exit(0);	// no return
}

