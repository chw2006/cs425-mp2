namespace cpp mp2

enum ErrorType {
	NOT_FOUND,
	ALREADY_EXISTS
}

exception ReplicaError {
	1: ErrorType type,
	2: string name, 	// state machine name for the operation
	3: string message	// formatted error
}

service Replica {
	// create a state machine
	void create(1:string name, 2:string initialState, 3:list<i32> RMs, 4:bool fromFrontEnd) throws (1:ReplicaError e),

	// apply an operation on a state machine
	string apply(1:string name, 2:string operation, 3:bool fromFrontEnd) throws (1:ReplicaError e),

	// get the state of a state machine
	string getState(1: string name) throws (1:ReplicaError e),

	// remove a state machine
	void remove(1: string name, 2: bool fromFrontEnd) throws (1:ReplicaError e),

	// get number of state machines hosted by this manager
	i32 numMachines() throws (1:ReplicaError e),
	
	// check to see if the specified state machine already exists on this RM
	bool hasStateMachine(1: string name) throws (1:ReplicaError e),

	// exit / crash
	oneway void exit()
}
