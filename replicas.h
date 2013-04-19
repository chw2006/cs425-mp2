#ifndef REPLICAS_H
#define REPLICAS_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "Replica.h"

namespace mp2 {

/* this class is used to contact other replicas. 
Usage is simple: replicas[i].apply(...) */
class Replicas {
private:
	std::string pipedir;
	unsigned int numreplicas;
	std::vector<boost::shared_ptr<ReplicaClient> > replicas;

public:
	std::string pipe_path(unsigned int id) const;

	Replicas(int argc, char **argv, unsigned int * myid = NULL);

	ReplicaIf & operator [](unsigned int);

	unsigned int numReplicas() const { return numreplicas;}
};

} // namespace

#endif