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
		replica.apply(result, name, operation);
		return result;
	}

	virtual string getState(void) const {
		string result;
		try {
			replica.getState(result, name);
			return result;
		} catch (TException e) {
			cerr << "RM failed. Searching for backup";
		}
		// try {
		// 	replica = (*replicas)[0].get(name);
		// 	replica.getState(result, name);
		// 	return result;
		// } catch (exception e) {;
		// 	cerr << "State machine " << name << " not found in network: " << e << endl;
		// }
		for(uint i = 0; i < replicas->numReplicas(); i++) {
			try {
				(*replicas)[i].getState(result, name);
				replica = (*replicas)[i];
				return result;
			} catch (ReplicaError e) {
				cerr << "Can't getState from machine " << name << " from RM #" << i << ": " << e.message << endl;
			}
		}
		cerr << "State machine " << name << " not found in network" << endl;
		return "";
	}
};

FrontEnd::FrontEnd(boost::shared_ptr<Replicas> replicas) : replicas(replicas) {}

FrontEnd::~FrontEnd() { }

// TODO: comment on interface and request protocol
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
	(*replicas)[reps[0]].create(name, initialState, reps, true);

	// return success
	return get(name);
}

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
	shared_ptr<StateMachine> result(new StateMachineStub((*replicas)[i], name, replicas));
	return result;
}

void FrontEnd::remove(const string &name) {
	for(uint i = 0; i < replicas->numReplicas(); i++) {
		try {
			(*replicas)[i].remove(name);
		} catch (exception e) {
			cerr << "Can't remove machine " << name << " from RM #" << i << endl;
		}
	}
}


