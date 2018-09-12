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
        "0.0.0.0:50051", grpc::InsecureChannelCredentials()));
    //"71.202.180.104:50051", grpc::InsecureChannelCredentials()));
    
    int gender = 30;
    int age = 89;
    int occupation = 201;



    if (argc < 4) {
        std::cout << "insufficient number of input integers. Using the default values..." << std::endl;
    } else {
        gender = std::stoi(argv[1]), age = std::stoi(argv[2]), occupation = std::stoi(argv[3]);
        std::cout << "Your input values:" 
                  << gender << ' '
                  << age << ' '
                  << occupation << std::endl;
    }
 
    int reply = client.Recommend(gender, age, occupation);  // The actual RPC call!
    std::cout << "client received: " << reply << std::endl;
 
    return 0;
}

