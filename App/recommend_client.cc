#include <iostream>
#include <memory>
#include <string>
 
#include <grpc++/grpc++.h>
#include <grpc/support/log.h>
 
#include "recommend.grpc.pb.h"
 
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
 
 
class RecommendClient {
 public:
  explicit RecommendClient(std::shared_ptr<Channel> channel)
      : stub_(RecommendService::NewStub(channel)) {}
 
  int Recommend(const int gender, const int age, const int occupation) {
 
    RecommendRequest request;
    request.set_gender(gender);
    request.set_age(age);
    request.set_occupation(occupation);
 
    RecommendResponse reply;
   
    ClientContext context;
 
    CompletionQueue cq;
 
    Status status;
 
    std::unique_ptr<ClientAsyncResponseReader<RecommendResponse> > rpc(
        stub_->AsyncRecommend(&context, request, &cq));
 
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
  
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    GPR_ASSERT(got_tag == (void*)1);
    GPR_ASSERT(ok);
 
    if (status.ok()) {
      return reply.best_ad();
    } else {
      return -1;
    }
  }
 
 private:
 
  std::unique_ptr<RecommendService::Stub> stub_;
};
 
int main(int argc, char** argv) { 
    RecommendClient client(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    int gender = 1;
    int age = 3;
    int occupation = 3;
  
    int reply = client.Recommend(gender, age, occupation);  // The actual RPC call!
    std::cout << "client received: " << reply << std::endl;
 
    return 0;
}

