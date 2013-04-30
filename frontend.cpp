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

   // apply()
	// Description: apply operation to distributed machines
	virtual string apply(const string & operation) {
	   // locals
		string result;
		
		// try to apply to last known living RM
		try {
			replica.apply(result, name, operation, true);
			return result;
		} 
		catch (TException e) {
			cerr << "RM apply failed. Searching for backup" << endl;
		} 
		catch (exception e) {
			cerr << "Unknown error in apply in FE" << endl;
		}
		
		// otherwise, last RM has failed, try to find a backup and apply
		for(uint i = 0; i < replicas->numReplicas(); i++) {
			try {
				(*replicas)[i].apply(result, name, operation, true);
				replica = (*replicas)[i];
				return result;
			} 
			catch (ReplicaError e) {
				cerr << "Can't apply op from machine " << name << " from RM #" << i << ": " << e.message << endl;
			} 
			catch (TException e) {
				cerr << "Can't apply op from machine " << name << " from RM #" << i << " since it's dead" << endl;
			} 
			catch (exception e) {
				cerr << "Other exception in frontend.cpp:StateMachineStub::apply()" << endl;
			}
		}
		
		// failed to find any RM hosting desired machine
		cerr << "State machine " << name << " not found in network. Could not apply operation " << endl;
		return "";
	}

   // getState()
	// Description: return current state of machine
	virtual string getState(void) const {
	   // locals
		string result;
		
		// try to obtain state from last known living RM
		try {
			replica.getState(result, name);
			return result;
		} 
		catch (TException e) {
			cerr << "RM getState failed. Searching for backup" << endl;
		} 
		catch (exception e) {
			cerr << "Unknown error in getState in FE" << endl;
		}

		// otherwise, last RM has failed, try to get state from a backup
		for(uint i = 0; i < replicas->numReplicas(); i++) {
			try {
				(*replicas)[i].getState(result, name);
				replica = (*replicas)[i];
				return result;
			} 
			catch (ReplicaError e) {
				cerr << "Can't getState from machine " << name << " from RM #" << i << ": " << e.message << endl;
			} 
			catch (TException e) {
				cerr << "Can't getState from machine " << name << " from RM #" << i << " since it's dead" << endl;
			} 
			catch (exception e) {
				cerr << "Unknown exception in frontend.cpp:getState()" << endl;
			}
		}

		// failed to find any RM hosting desired machine
		cerr << "State machine " << name << " not found in network" << endl;
		return "";
	}
};

// FrontEnd()
// Description: constructor
FrontEnd::FrontEnd(boost::shared_ptr<Replicas> replicas) : replicas(replicas) {}

// ~FrontEnd()
// Description: destructor
FrontEnd::~FrontEnd() { }

// create()
// Description: create new state machine and return a stub
shared_ptr<StateMachine> FrontEnd::create(const string &name, const string &initialState) {
	// keep trying to find a host RM until one succeeds
	while(1) {
		// find least loaded RM
		int minload = INT_MAX;
		vector<int> rep;
		rep.push_back(0);
		for(uint i = 0; i < replicas->numReplicas(); i++) {
	    	try {
		        if(!(*replicas)[i].hasStateMachine(name)) {
		        	int num = (*replicas)[i].numMachines();
				    if(minload > num) {
				    	minload = num;
				    	rep[0] = i;
				    }
		        }
		    } 
		    catch (exception e) {
		    	// do nothing
		    }
		}
		
		// send create request to single RM
		// destination RM will be responsible for propagating the machine to 2 secondaries
		try {
			(*replicas)[rep[0]].create(name, initialState, rep, true);
			break;
		} 
		catch (ReplicaError e) {
			cerr << "Failed to create: " << e.message << endl;
		} 
		catch (TException e) {
			cerr << "Failed to create: RM failed" << endl;
		} 
		catch (exception e) {
			cerr << "Unknown error2 in create()" << endl;
		}
    }

	// return success
	return get(name);
}

// get()
// Description: return a local state machine interface for the desired machine
shared_ptr<StateMachine> FrontEnd::get(const string &name) {
	// locals
	uint i;
	
	// loop through existing RMs and designate the first positive response as the primary
	for(i = 0; i < replicas->numReplicas(); i++) {
		try {
			// will throw exception if RM is dead
			if((*replicas)[i].hasStateMachine(name)) {
				break;
			}
		} 
		catch (TException e) {
			cerr << "Ignoring RM " << i << " in get() since it's dead" << endl;
		} 
		catch (exception e) {
			cerr << "Other exception in frontend.cpp:get()" << endl;
		}
	}
	
	// error check if machine was not found at all
	if(i >= replicas->numReplicas())
		i = replicas->numReplicas() - 1;
		
	// return stub
	shared_ptr<StateMachine> result(new StateMachineStub((*replicas)[i], name, replicas));
	return result;
}

// remove()
// Description: remove a state machine from the network
void FrontEnd::remove(const string &name) {
	// loop through existing RMs and call remove on each
	// This satisfies total ordering since the backend RMs will propagate the remove request
	// The target machine will be removed as long as at least 1 remove request reaches a hosting RM
	// Remove requests (either from the the frontend or other RMs) will simply be ignored on RMs that don't host the target machine
	for(uint i = 0; i < replicas->numReplicas(); i++) {
		try {
			(*replicas)[i].remove(name, true);
		} 
		catch (ReplicaError e) {
			cerr << "Can't remove machine " << name << " from RM #" << i << ": " << e.message << endl;
		} 
		catch (TException e) {
			cerr << "Can't remove machine " << name << " from RM #" << i << " since it's dead" << endl;
		} 
		catch (exception e) {
			cerr << "Can't remove machine " << name << " from RM #" << i << endl;
		}
	}
}


