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

<<<<<<< HEAD
void Replica::create(const string & name, const string & initialState, const std::vector<int32_t> & RMs) {
=======
void Replica::create(const string & name, const string & initialState, const std::vector<int> & RMs, bool & fromFrontEnd) {
   // check if this is a duplicate, if it is, throw a error
>>>>>>> 7d0df7815d88168913a6c10c91be2786bc6b6a9d
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
    	for(int i = 1; i < RMs.size(); i++)
    	{
    	   static int RM = RMs[i];
    	   // pass this message to the other RMs
    	   (*replicas)[RMs[i]].create(name, initialState, RMs, false);
    	}
   }
   // create the machine
   machines.insert(make_pair(name, factory.make(initialState)));
 	cout << "Creating machine " << name << "on RM" << id << endl;
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

	machines.erase(name);
	cout << "Removing machine: " << name << endl;
}

int32_t Replica::numMachines() {
	int32_t result = machines.size();
	cout << "Getting number of machines: " << result << endl;
	return result;
}


/* DO NOT CHANGE THIS */
void Replica::exit(void) {
	clog << "Replica " << id << " exiting" << endl;
	::std::exit(0);	// no return
}

