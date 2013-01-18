/***************************************************************************************************
 *  Copyright 2012 MaidSafe.net limited                                                            *
 *                                                                                                 *
 *  The following source code is property of MaidSafe.net limited and is not meant for external    *
 *  use.  The use of this code is governed by the licence file licence.txt found in the root of    *
 *  this directory and also on www.maidsafe.net.                                                   *
 *                                                                                                 *
 *  You are not free to copy, amend or otherwise use this source code without the explicit         *
 *  written permission of the board of directors of MaidSafe.net.                                  *
 **************************************************************************************************/

#ifndef MAIDSAFE_NFS_ACCUMULATOR_H_
#define MAIDSAFE_NFS_ACCUMULATOR_H_

#include <deque>
#include <utility>
#include <vector>

#include "maidsafe/routing/routing_api.h"

#include "maidsafe/nfs/data_message.h"
#include "maidsafe/nfs/return_code.h"
#include "maidsafe/nfs/types.h"
#include "maidsafe/nfs/utils.h"


namespace maidsafe {

namespace nfs {

class Accumulator {
 public:
  struct Request {
    Request(DataMessage msg_in, routing::ReplyFunctor reply_functor_in, ReturnCode ret_code_in)
        : msg(msg_in), reply_functor(reply_functor_in), ret_code(ret_code_in) {}
    DataMessage msg;
    routing::ReplyFunctor reply_functor;
    ReturnCode ret_code;
  };
  typedef std::pair<MessageId, PersonaType> RequestIdentity;

  Accumulator();

  bool CheckHandled(const RequestIdentity& request_identity, ReturnCode& ret_code) const;
  std::vector<ReturnCode> PushRequest(const Request& request);
  std::vector<Request> SetHandled(const RequestIdentity& request_identity,
                                  const ReturnCode& ret_code);

 private:
  typedef std::deque<std::pair<RequestIdentity, Request>> Requests;
  typedef std::deque<std::pair<RequestIdentity, ReturnCode>> HandledRequests;

  Requests pending_requests_;
  HandledRequests handled_requests_;
  const size_t kMaxPendingRequestsCount_, kMaxHandledRequestsCount_;
  mutable std::mutex mutex_;
};

}  // namespace nfs

}  // namespace maidsafe

#endif  // MAIDSAFE_NFS_ACCUMULATOR_H_