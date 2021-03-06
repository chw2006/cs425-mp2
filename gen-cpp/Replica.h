/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Replica_H
#define Replica_H

#include <thrift/TDispatchProcessor.h>
#include "replica_types.h"

namespace mp2 {

class ReplicaIf {
 public:
  virtual ~ReplicaIf() {}
  virtual void create(const std::string& name, const std::string& initialState, const std::vector<int32_t> & RMs, const bool fromFrontEnd) = 0;
  virtual void apply(std::string& _return, const std::string& name, const std::string& operation, const bool fromFrontEnd) = 0;
  virtual void getState(std::string& _return, const std::string& name) = 0;
  virtual void remove(const std::string& name, const bool fromFrontEnd) = 0;
  virtual int32_t numMachines() = 0;
  virtual bool hasStateMachine(const std::string& name) = 0;
  virtual void exit() = 0;
};

class ReplicaIfFactory {
 public:
  typedef ReplicaIf Handler;

  virtual ~ReplicaIfFactory() {}

  virtual ReplicaIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ReplicaIf* /* handler */) = 0;
};

class ReplicaIfSingletonFactory : virtual public ReplicaIfFactory {
 public:
  ReplicaIfSingletonFactory(const boost::shared_ptr<ReplicaIf>& iface) : iface_(iface) {}
  virtual ~ReplicaIfSingletonFactory() {}

  virtual ReplicaIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ReplicaIf* /* handler */) {}

 protected:
  boost::shared_ptr<ReplicaIf> iface_;
};

class ReplicaNull : virtual public ReplicaIf {
 public:
  virtual ~ReplicaNull() {}
  void create(const std::string& /* name */, const std::string& /* initialState */, const std::vector<int32_t> & /* RMs */, const bool /* fromFrontEnd */) {
    return;
  }
  void apply(std::string& /* _return */, const std::string& /* name */, const std::string& /* operation */, const bool /* fromFrontEnd */) {
    return;
  }
  void getState(std::string& /* _return */, const std::string& /* name */) {
    return;
  }
  void remove(const std::string& /* name */, const bool /* fromFrontEnd */) {
    return;
  }
  int32_t numMachines() {
    int32_t _return = 0;
    return _return;
  }
  bool hasStateMachine(const std::string& /* name */) {
    bool _return = false;
    return _return;
  }
  void exit() {
    return;
  }
};

typedef struct _Replica_create_args__isset {
  _Replica_create_args__isset() : name(false), initialState(false), RMs(false), fromFrontEnd(false) {}
  bool name;
  bool initialState;
  bool RMs;
  bool fromFrontEnd;
} _Replica_create_args__isset;

class Replica_create_args {
 public:

  Replica_create_args() : name(), initialState(), fromFrontEnd(0) {
  }

  virtual ~Replica_create_args() throw() {}

  std::string name;
  std::string initialState;
  std::vector<int32_t>  RMs;
  bool fromFrontEnd;

  _Replica_create_args__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_initialState(const std::string& val) {
    initialState = val;
  }

  void __set_RMs(const std::vector<int32_t> & val) {
    RMs = val;
  }

  void __set_fromFrontEnd(const bool val) {
    fromFrontEnd = val;
  }

  bool operator == (const Replica_create_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(initialState == rhs.initialState))
      return false;
    if (!(RMs == rhs.RMs))
      return false;
    if (!(fromFrontEnd == rhs.fromFrontEnd))
      return false;
    return true;
  }
  bool operator != (const Replica_create_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_create_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_create_pargs {
 public:


  virtual ~Replica_create_pargs() throw() {}

  const std::string* name;
  const std::string* initialState;
  const std::vector<int32_t> * RMs;
  const bool* fromFrontEnd;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_create_result__isset {
  _Replica_create_result__isset() : e(false) {}
  bool e;
} _Replica_create_result__isset;

class Replica_create_result {
 public:

  Replica_create_result() {
  }

  virtual ~Replica_create_result() throw() {}

  ReplicaError e;

  _Replica_create_result__isset __isset;

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_create_result & rhs) const
  {
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_create_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_create_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_create_presult__isset {
  _Replica_create_presult__isset() : e(false) {}
  bool e;
} _Replica_create_presult__isset;

class Replica_create_presult {
 public:


  virtual ~Replica_create_presult() throw() {}

  ReplicaError e;

  _Replica_create_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Replica_apply_args__isset {
  _Replica_apply_args__isset() : name(false), operation(false), fromFrontEnd(false) {}
  bool name;
  bool operation;
  bool fromFrontEnd;
} _Replica_apply_args__isset;

class Replica_apply_args {
 public:

  Replica_apply_args() : name(), operation(), fromFrontEnd(0) {
  }

  virtual ~Replica_apply_args() throw() {}

  std::string name;
  std::string operation;
  bool fromFrontEnd;

  _Replica_apply_args__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_operation(const std::string& val) {
    operation = val;
  }

  void __set_fromFrontEnd(const bool val) {
    fromFrontEnd = val;
  }

  bool operator == (const Replica_apply_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(operation == rhs.operation))
      return false;
    if (!(fromFrontEnd == rhs.fromFrontEnd))
      return false;
    return true;
  }
  bool operator != (const Replica_apply_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_apply_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_apply_pargs {
 public:


  virtual ~Replica_apply_pargs() throw() {}

  const std::string* name;
  const std::string* operation;
  const bool* fromFrontEnd;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_apply_result__isset {
  _Replica_apply_result__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_apply_result__isset;

class Replica_apply_result {
 public:

  Replica_apply_result() : success() {
  }

  virtual ~Replica_apply_result() throw() {}

  std::string success;
  ReplicaError e;

  _Replica_apply_result__isset __isset;

  void __set_success(const std::string& val) {
    success = val;
  }

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_apply_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_apply_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_apply_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_apply_presult__isset {
  _Replica_apply_presult__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_apply_presult__isset;

class Replica_apply_presult {
 public:


  virtual ~Replica_apply_presult() throw() {}

  std::string* success;
  ReplicaError e;

  _Replica_apply_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Replica_getState_args__isset {
  _Replica_getState_args__isset() : name(false) {}
  bool name;
} _Replica_getState_args__isset;

class Replica_getState_args {
 public:

  Replica_getState_args() : name() {
  }

  virtual ~Replica_getState_args() throw() {}

  std::string name;

  _Replica_getState_args__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  bool operator == (const Replica_getState_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    return true;
  }
  bool operator != (const Replica_getState_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_getState_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_getState_pargs {
 public:


  virtual ~Replica_getState_pargs() throw() {}

  const std::string* name;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_getState_result__isset {
  _Replica_getState_result__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_getState_result__isset;

class Replica_getState_result {
 public:

  Replica_getState_result() : success() {
  }

  virtual ~Replica_getState_result() throw() {}

  std::string success;
  ReplicaError e;

  _Replica_getState_result__isset __isset;

  void __set_success(const std::string& val) {
    success = val;
  }

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_getState_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_getState_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_getState_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_getState_presult__isset {
  _Replica_getState_presult__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_getState_presult__isset;

class Replica_getState_presult {
 public:


  virtual ~Replica_getState_presult() throw() {}

  std::string* success;
  ReplicaError e;

  _Replica_getState_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Replica_remove_args__isset {
  _Replica_remove_args__isset() : name(false), fromFrontEnd(false) {}
  bool name;
  bool fromFrontEnd;
} _Replica_remove_args__isset;

class Replica_remove_args {
 public:

  Replica_remove_args() : name(), fromFrontEnd(0) {
  }

  virtual ~Replica_remove_args() throw() {}

  std::string name;
  bool fromFrontEnd;

  _Replica_remove_args__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_fromFrontEnd(const bool val) {
    fromFrontEnd = val;
  }

  bool operator == (const Replica_remove_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(fromFrontEnd == rhs.fromFrontEnd))
      return false;
    return true;
  }
  bool operator != (const Replica_remove_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_remove_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_remove_pargs {
 public:


  virtual ~Replica_remove_pargs() throw() {}

  const std::string* name;
  const bool* fromFrontEnd;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_remove_result__isset {
  _Replica_remove_result__isset() : e(false) {}
  bool e;
} _Replica_remove_result__isset;

class Replica_remove_result {
 public:

  Replica_remove_result() {
  }

  virtual ~Replica_remove_result() throw() {}

  ReplicaError e;

  _Replica_remove_result__isset __isset;

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_remove_result & rhs) const
  {
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_remove_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_remove_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_remove_presult__isset {
  _Replica_remove_presult__isset() : e(false) {}
  bool e;
} _Replica_remove_presult__isset;

class Replica_remove_presult {
 public:


  virtual ~Replica_remove_presult() throw() {}

  ReplicaError e;

  _Replica_remove_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Replica_numMachines_args {
 public:

  Replica_numMachines_args() {
  }

  virtual ~Replica_numMachines_args() throw() {}


  bool operator == (const Replica_numMachines_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Replica_numMachines_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_numMachines_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_numMachines_pargs {
 public:


  virtual ~Replica_numMachines_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_numMachines_result__isset {
  _Replica_numMachines_result__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_numMachines_result__isset;

class Replica_numMachines_result {
 public:

  Replica_numMachines_result() : success(0) {
  }

  virtual ~Replica_numMachines_result() throw() {}

  int32_t success;
  ReplicaError e;

  _Replica_numMachines_result__isset __isset;

  void __set_success(const int32_t val) {
    success = val;
  }

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_numMachines_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_numMachines_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_numMachines_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_numMachines_presult__isset {
  _Replica_numMachines_presult__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_numMachines_presult__isset;

class Replica_numMachines_presult {
 public:


  virtual ~Replica_numMachines_presult() throw() {}

  int32_t* success;
  ReplicaError e;

  _Replica_numMachines_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Replica_hasStateMachine_args__isset {
  _Replica_hasStateMachine_args__isset() : name(false) {}
  bool name;
} _Replica_hasStateMachine_args__isset;

class Replica_hasStateMachine_args {
 public:

  Replica_hasStateMachine_args() : name() {
  }

  virtual ~Replica_hasStateMachine_args() throw() {}

  std::string name;

  _Replica_hasStateMachine_args__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  bool operator == (const Replica_hasStateMachine_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    return true;
  }
  bool operator != (const Replica_hasStateMachine_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_hasStateMachine_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_hasStateMachine_pargs {
 public:


  virtual ~Replica_hasStateMachine_pargs() throw() {}

  const std::string* name;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_hasStateMachine_result__isset {
  _Replica_hasStateMachine_result__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_hasStateMachine_result__isset;

class Replica_hasStateMachine_result {
 public:

  Replica_hasStateMachine_result() : success(0) {
  }

  virtual ~Replica_hasStateMachine_result() throw() {}

  bool success;
  ReplicaError e;

  _Replica_hasStateMachine_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  void __set_e(const ReplicaError& val) {
    e = val;
  }

  bool operator == (const Replica_hasStateMachine_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(e == rhs.e))
      return false;
    return true;
  }
  bool operator != (const Replica_hasStateMachine_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_hasStateMachine_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Replica_hasStateMachine_presult__isset {
  _Replica_hasStateMachine_presult__isset() : success(false), e(false) {}
  bool success;
  bool e;
} _Replica_hasStateMachine_presult__isset;

class Replica_hasStateMachine_presult {
 public:


  virtual ~Replica_hasStateMachine_presult() throw() {}

  bool* success;
  ReplicaError e;

  _Replica_hasStateMachine_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Replica_exit_args {
 public:

  Replica_exit_args() {
  }

  virtual ~Replica_exit_args() throw() {}


  bool operator == (const Replica_exit_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Replica_exit_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Replica_exit_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Replica_exit_pargs {
 public:


  virtual ~Replica_exit_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class ReplicaClient : virtual public ReplicaIf {
 public:
  ReplicaClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  ReplicaClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void create(const std::string& name, const std::string& initialState, const std::vector<int32_t> & RMs, const bool fromFrontEnd);
  void send_create(const std::string& name, const std::string& initialState, const std::vector<int32_t> & RMs, const bool fromFrontEnd);
  void recv_create();
  void apply(std::string& _return, const std::string& name, const std::string& operation, const bool fromFrontEnd);
  void send_apply(const std::string& name, const std::string& operation, const bool fromFrontEnd);
  void recv_apply(std::string& _return);
  void getState(std::string& _return, const std::string& name);
  void send_getState(const std::string& name);
  void recv_getState(std::string& _return);
  void remove(const std::string& name, const bool fromFrontEnd);
  void send_remove(const std::string& name, const bool fromFrontEnd);
  void recv_remove();
  int32_t numMachines();
  void send_numMachines();
  int32_t recv_numMachines();
  bool hasStateMachine(const std::string& name);
  void send_hasStateMachine(const std::string& name);
  bool recv_hasStateMachine();
  void exit();
  void send_exit();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ReplicaProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ReplicaIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ReplicaProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_create(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_apply(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_getState(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_remove(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_numMachines(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_hasStateMachine(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_exit(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ReplicaProcessor(boost::shared_ptr<ReplicaIf> iface) :
    iface_(iface) {
    processMap_["create"] = &ReplicaProcessor::process_create;
    processMap_["apply"] = &ReplicaProcessor::process_apply;
    processMap_["getState"] = &ReplicaProcessor::process_getState;
    processMap_["remove"] = &ReplicaProcessor::process_remove;
    processMap_["numMachines"] = &ReplicaProcessor::process_numMachines;
    processMap_["hasStateMachine"] = &ReplicaProcessor::process_hasStateMachine;
    processMap_["exit"] = &ReplicaProcessor::process_exit;
  }

  virtual ~ReplicaProcessor() {}
};

class ReplicaProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ReplicaProcessorFactory(const ::boost::shared_ptr< ReplicaIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ReplicaIfFactory > handlerFactory_;
};

class ReplicaMultiface : virtual public ReplicaIf {
 public:
  ReplicaMultiface(std::vector<boost::shared_ptr<ReplicaIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ReplicaMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ReplicaIf> > ifaces_;
  ReplicaMultiface() {}
  void add(boost::shared_ptr<ReplicaIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void create(const std::string& name, const std::string& initialState, const std::vector<int32_t> & RMs, const bool fromFrontEnd) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->create(name, initialState, RMs, fromFrontEnd);
    }
    ifaces_[i]->create(name, initialState, RMs, fromFrontEnd);
  }

  void apply(std::string& _return, const std::string& name, const std::string& operation, const bool fromFrontEnd) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->apply(_return, name, operation, fromFrontEnd);
    }
    ifaces_[i]->apply(_return, name, operation, fromFrontEnd);
    return;
  }

  void getState(std::string& _return, const std::string& name) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getState(_return, name);
    }
    ifaces_[i]->getState(_return, name);
    return;
  }

  void remove(const std::string& name, const bool fromFrontEnd) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->remove(name, fromFrontEnd);
    }
    ifaces_[i]->remove(name, fromFrontEnd);
  }

  int32_t numMachines() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->numMachines();
    }
    return ifaces_[i]->numMachines();
  }

  bool hasStateMachine(const std::string& name) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->hasStateMachine(name);
    }
    return ifaces_[i]->hasStateMachine(name);
  }

  void exit() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->exit();
    }
    ifaces_[i]->exit();
  }

};

} // namespace

#endif
