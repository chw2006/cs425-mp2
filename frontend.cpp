#include "frontend.h"
#include <boost/shared_ptr.hpp>
#include "statemachine.h"

using namespace std;
using namespace mp2;
using boost::shared_ptr;

class StateMachineStub : public mp2::StateMachine {
private:
	ReplicaIf & replica;
	const string name;

public:
	StateMachineStub(ReplicaIf & replica, const string &name)
		: replica(replica), name(name) {}
	virtual string apply(const string & operation) {
		string result;
		replica.apply(result, name, operation);
		return result;
	}

	virtual string getState(void) const {
		string result;
		replica.getState(result, name);
		return result;
	}
};

FrontEnd::FrontEnd(boost::shared_ptr<Replicas> replicas) : replicas(replicas) {}

FrontEnd::~FrontEnd() { }

shared_ptr<StateMachine> FrontEnd::create(const string &name, const string &initialState) {
	// Always uses machine 0
	(*replicas)[0].create(name, initialState);
	return get(name);
}

shared_ptr<StateMachine> FrontEnd::get(const string &name) {
	shared_ptr<StateMachine> result(new StateMachineStub((*replicas)[0], name));
	return result;
}

void FrontEnd::remove(const string &name) {
	(*replicas)[0].remove(name);
}


