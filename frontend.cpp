#include "frontend.h"
#include <boost/shared_ptr.hpp>
#include "statemachine.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace mp2;
using boost::shared_ptr;
using namespace apache::thrift;

class StateMachineStub : public mp2::StateMachine {
private:
	mutable ReplicaIf & replica;
	const string name;
	shared_ptr<Replicas> replicas;

public:
	StateMachineStub(ReplicaIf & replica, const string &name, shared_ptr<Replicas> replicas)
		: replica(replica), name(name), replicas(replicas) {}

	virtual string apply(const string & operation) {
		string result;
		// try to obtain state from last known alive RM
		try {
			replica.apply(result, name, operation, true);
			return result;
		} catch (TException e) {
			cerr << "RM failed. Searching for backup";
		}

		// last RM has failed, try to get state from a backup
		for(uint i = 0; i < replicas->numReplicas(); i++) {
			try {
				(*replicas)[i].apply(result, name, operation, true);
				replica = (*replicas)[i];
				return result;
			} catch (ReplicaError e) {
				cerr << "Can't apply op from machine " << name << " from RM #" << i << ": " << e.message << endl;
			} catch (TException e) {
				cerr << "Can't apply op from machine " << name << " from RM #" << i << " since it's dead" << endl;
			}
		}

		// failed to find any RM hosting desired machine
		cerr << "State machine " << name << " not found in network. Could not apply operation " << endl;
		return "";
		// string result;
		// replica.apply(result, name, operation, true);
		// return result;
	}

	// return current state of machine
	virtual string getState(void) const {
		string result;
		// try to obtain state from last known alive RM
		try {
			replica.getState(result, name);
			return result;
		} catch (TException e) {
			cerr << "RM failed. Searching for backup";
		}

		// last RM has failed, try to get state from a backup
		for(uint i = 0; i < replicas->numReplicas(); i++) {
			try {
				(*replicas)[i].getState(result, name);
				replica = (*replicas)[i];
				return result;
			} catch (ReplicaError e) {
				cerr << "Can't getState from machine " << name << " from RM #" << i << ": " << e.message << endl;
			} catch (TException e) {
				cerr << "Can't getState from machine " << name << " from RM #" << i << " since it's dead" << endl;
			}
		}

		// failed to find any RM hosting desired machine
		cerr << "State machine " << name << " not found in network" << endl;
		return "";
	}
};

FrontEnd::FrontEnd(boost::shared_ptr<Replicas> replicas) : replicas(replicas) {}

FrontEnd::~FrontEnd() { }

// create new state machine
shared_ptr<StateMachine> FrontEnd::create(const string &name, const string &initialState) {
	// loop through available managers and return the 3 least loaded ones
	// init reps vector
	vector<int> reps;
	reps.push_back(0);
	reps.push_back(1);
	reps.push_back(2);
	// find most loaded of the 3
	int maxidx = 0;
	if((*replicas)[0].numMachines() < (*replicas)[1].numMachines())
		maxidx = 1;
	if((*replicas)[reps[maxidx]].numMachines() < (*replicas)[2].numMachines())
		maxidx = 2;
	// iterate through all managers
	for(uint i = 3; i < replicas->numReplicas(); i++) {
		try {
			if((*replicas)[reps[maxidx]].numMachines() > (*replicas)[i].numMachines()) {
				reps[maxidx] = i;
				// update most loaded of 3 current RMS
				maxidx = 0;
				if((*replicas)[reps[0]].numMachines() < (*replicas)[reps[1]].numMachines())
					maxidx = 1;
				if((*replicas)[reps[maxidx]].numMachines() < (*replicas)[reps[2]].numMachines())
					maxidx = 2;
			}
		} catch (TException e) {
			cerr << "RM " << i << " is dead" << endl;
		}
	}

	// send create request to target RM
	// request contains name, initial state, flag indicating the frontEnd source,
	//    and a list of the 3 least loaded RMs that should store the newly created machine
	(*replicas)[reps[0]].create(name, initialState, reps, true);

	// return success
	return get(name);
}

// return a local state machine interface for the desired machine
shared_ptr<StateMachine> FrontEnd::get(const string &name) {
	// loop through existing RMs and designate the first positive response as the leader
	uint i;
	for(i = 0; i < replicas->numReplicas(); i++) {
		try {
			string temp;
			// will throw exception if RM is dead or does not have the desired state machine
			(*replicas)[i].getState(temp, name);
			break;
		} catch (ReplicaError e) {
			cerr << "Ignoring RM " << i << ": " << e.message << endl;
		}
	}
	// found desired machine, create new stub and return
	shared_ptr<StateMachine> result(new StateMachineStub((*replicas)[i], name, replicas));
	return result;
}

// remove a state machine from the network
void FrontEnd::remove(const string &name) {
	for(uint i = 0; i < replicas->numReplicas(); i++) {
		try {
			(*replicas)[i].remove(name);
		} catch (exception e) {
			cerr << "Can't remove machine " << name << " from RM #" << i << endl;
		}
	}
}


