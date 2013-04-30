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
    	for(i = 1; i < RMs.size(); i++)
    	{
    	   // pass this message to the other RMs
    	   (*replicas)[RMs[i]].create(name, initialState, RMs, false);
    	}
   }
   // create the machine
   machines.insert(make_pair(name, factory.make(initialState)));
   boost::shared_ptr<boost::signals2::mutex> stateMachineMutex;
   mutexMap.insert(make_pair(name, stateMachineMutex));
   boost::shared_ptr<std::vector<int32_t> > groupVector;
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

void Replica::apply(string & result, const string & name, const string& operation, const bool fromFrontEnd) {
	checkExists(name);
	string result1;
	boost::shared_ptr<std::vector<int32_t> > groupVector;
	// if this command is from the front end, then you must pass this on to the other state machines
	if(fromFrontEnd)
	{
	   // apply this to other state machines
	   mutexMap[name]->lock();
	   groupVector = groupMap[name];
	   for(int i = 0; i < groupVector->size(); i++)
	   {
	      (*replicas)[(*groupVector)[i]].apply(result1, name, operation, false);
	   }
	   mutexMap[name]->unlock();  
	}
	// if it's from another RM, then just apply this to yourself
	mutexMap[name]->lock();
	result = machines[name]->apply(operation);
	mutexMap[name]->unlock();
	cout << "Applying operation: " << operation << endl;
}

void Replica::getState(string& result, const string &name) {
	checkExists(name);

	result = machines[name]->getState();
	cout << "Getting state: " << result << endl;
}

void Replica::remove(const string &name) {
	checkExists(name);
	mutexMap[name]->lock();
	machines.erase(name);
	groupMap.erase(name);
	mutexMap[name]->unlock();
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

/* DO NOT CHANGE THIS */
void Replica::exit(void) {
	clog << "Replica " << id << " exiting" << endl;
	::std::exit(0);	// no return
}

