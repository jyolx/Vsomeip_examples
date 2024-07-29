#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include<chrono>
#include <vsomeip/vsomeip.hpp>

using namespace std;
using namespace vsomeip_v3;
using namespace std::chrono_literals;

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
// #define SAMPLE_METHOD_ID 0x0421
#define SAMPLE_EVENTGROUP_ID 0x4465
#define SAMPLE_EVENT_ID 0x8778


shared_ptr<payload> mypayload;
shared_ptr<application> app;

void send_message()
{
    
    this_thread::sleep_for(500ms);
    mypayload = runtime::get()->create_payload();
    vector<byte_t> mypayload_data;
    for(int i=0;i<10;i++)
    {
        mypayload_data.push_back(i);
    }
    mypayload->set_data(mypayload_data);
    app->notify(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, mypayload);
    cout<<"----------------MESSAGE SENT------------------"<<endl;
}

int main() {
    cout<<"------------INITIALIZING PUBLISHER------------"<<endl;
    app = runtime::get()->create_application("Publisher");
    app->init();

    cout<<"-----------OFFERING SERVICE----------------"<<endl;
    app->offer_service(SAMPLE_SERVICE_ID,SAMPLE_INSTANCE_ID);

    cout<<"-----------OFFERING EVENT----------------"<<endl;
    set<eventgroup_t> mygroup;
    mygroup.insert(SAMPLE_EVENTGROUP_ID);
    app->offer_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, mygroup);

    thread messaging(send_message);

    app->start();   
   
}