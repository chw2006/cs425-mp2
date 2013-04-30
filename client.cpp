#include "frontend.h"
#include "replicas.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace mp2;
using namespace std;
using boost::shared_ptr;

int main(int argc, char **argv) {
	shared_ptr<Replicas> replicas(new Replicas(argc, argv));

	FrontEnd frontEnd(replicas);

	try {
		shared_ptr<StateMachine> machine = frontEnd.create("testmachine", "initstate");

		cout << machine->getState() << endl;

		cout << machine->apply("testop") << endl;

		// (*replicas)[0].exit();

		cout << machine->getState() << endl;

		frontEnd.remove("testmachine");

		cout << "Prebuilt tests done!" << endl;

	} catch (ReplicaError e) {
			cerr << e.message << endl;
	}

	// get user input
	string input;
	map<string, shared_ptr<StateMachine> > machines;
	while(1) {
		try {
			// receive input from terminal
			getline(cin, input);

			// parse inputs
			vector<string> args;
		    istringstream iss(input);
		    do {
		        string sub;
		        iss >> sub;
		        if(sub != "")
		        	args.push_back(sub);
		    } while (iss);

		    // do operations and loop
		    if(args.size() < 1)
		    	continue;
		    // create new machine: create [name] [initial_state]
		    if(args[0] == "create") {
		    	if(args.size() != 3) {
		    		cout << "Expected 3 arguments" << endl;
		    	}else{
		    		cout << "Creating machine " << args[1] << endl;
		    		shared_ptr<StateMachine> temp = frontEnd.create(args[1], args[2]);
		    		machines.insert(pair<string, shared_ptr<StateMachine> >(args[1], temp));
		    	}
		    // get current state: get [name]
		    }else if(args[0] == "get") {
		    	if(args.size() != 2) {
		    		cout << "Expected 2 arguments" << endl;
		    	}else{
		    		cout << "Getting state from machine " << args[1] << endl;
		    		if(machines.find(args[1]) == machines.end()) {
		    			shared_ptr<StateMachine> temp = frontEnd.get(args[1]);
		    			machines.insert(pair<string, shared_ptr<StateMachine> >(args[1], temp));
		    		}
		    		cout << machines[args[1]]->getState() << endl;
		    	}
		    // apply operation: apply [name] [operation]
		    }else if(args[0] == "apply") {
		    	if(args.size() != 3) {
		    		cout << "Expected 3 arguments" << endl;
		    	}else{
		    		cout << "Applying operation " << args[2] << " to machine " << args[1] << endl;
		    		if(machines.find(args[1]) == machines.end()) {
		    			shared_ptr<StateMachine> temp = frontEnd.get(args[1]);
		    			machines.insert(pair<string, shared_ptr<StateMachine> >(args[1], temp));
		    		}
		    		cout << machines[args[1]]->apply(args[2]) << endl;
		    	}
		    // remove machine: remove [name]
		    }else if(args[0] == "remove") {
		    	if(args.size() != 2) {
		    		cout << "Expected 2 arguments" << endl;
		    		break;
		    	}
		    	cout << "Removing machine " << args[1] << endl;
		    	if(machines.find(args[1]) == machines.end()) {
		    			shared_ptr<StateMachine> temp = frontEnd.get(args[1]);
		    			machines.insert(pair<string, shared_ptr<StateMachine> >(args[1], temp));
		    		}
		    	frontEnd.remove(args[1]);
		    // invalid inputs
		    }else if(args[0] == "exit") {
		    	return 0;
		    }else{
		    	cout << "Invalid arguments" << endl;
		    }
		} catch (ReplicaError e) {
			cerr << e.message << endl;
		}
	}

	return 0;
}
