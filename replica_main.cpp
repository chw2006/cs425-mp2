#include <string>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

#include "replica_handler.h"
#include "Replica.h"
#include "stringmachine.h"
#include "replicas.h"

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace std;
using namespace mp2;
using boost::shared_ptr;

int main(int argc, char **argv) {
	string pipedir;
	unsigned int myid;

	Replicas replicas(argc, argv, &myid);

	StringMachineFactory factory;
	shared_ptr<Replica> replica(new Replica(myid, factory));

	shared_ptr<TProcessor> processor(new ReplicaProcessor(replica));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(replicas.pipe_path(myid).c_str()));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();

	return 0;
}
