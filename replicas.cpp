#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include "replicas.h"

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace std;
using namespace mp2;

using boost::shared_ptr;
namespace po = boost::program_options;


string Replicas::pipe_path(unsigned int id) const {
	stringstream result;
	result << pipedir << "/pipe" << id;
	return result.str();
}

Replicas::Replicas(int argc, char **argv, unsigned int *myid) {
	string config_file;

	po::options_description cmd_only, config_options, 
	hidden("Hidden options");

	config_options.add_options()
	("pipedir,D", po::value<string>(&pipedir)->default_value("./.pipes"), 
		"directory where named pipes should be created")
	("numreplicas,N", po::value<unsigned int>(&numreplicas)->default_value(1), "Number of replicas");

	cmd_only.add_options() 
	("config,C", po::value<string>(&config_file)->default_value("config"));

	po::options_description cmd_options;
	cmd_options.add(cmd_only).add(config_options);

	try {
		po::variables_map vm;
		if (myid != NULL) {		
				cmd_options.add_options()
					("myid", po::value<unsigned int>(myid), "Replica ID");
			po::positional_options_description posd;
			posd.add("myid", 1);
			po::store(po::command_line_parser(argc, argv).
				options(cmd_options).positional(posd).run(), vm);
		} else {
			po::store(po::parse_command_line(argc, argv, cmd_options), vm);
		}
		notify(vm);

		ifstream config_stream(config_file.c_str());

		if (config_stream.good()) {
			po::store(po::parse_config_file(config_stream, config_options), vm);
			notify(vm);
		}

	} catch (po::error e) {
		po::options_description usage("Allowed options");
		usage.add(cmd_only).add(config_options);
		cerr << "Usage: " << argv[0] << " [options] replicaid" << endl << usage << endl;
		exit(1);
	}
	replicas.resize(numreplicas);
}

ReplicaIf & Replicas::operator[](unsigned int id) {
	if (id >= numreplicas) {
		throw "Invalid replica number";
	}

	if (replicas[id] == NULL) {
		shared_ptr<TSocket> socket(new TSocket(pipe_path(id).c_str()));
		shared_ptr<TTransport> transport(new TBufferedTransport(socket));
		shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

		replicas[id].reset(new ReplicaClient(protocol));
		transport->open();
	}
	return *replicas[id];
}
