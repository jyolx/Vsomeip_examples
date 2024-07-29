#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>
using namespace std;
using namespace vsomeip_v3;

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678
#define SAMPLE_EVENTGROUP_ID 0x4465
#define SAMPLE_EVENT_ID 0x8778

shared_ptr< application > app;
mutex run_mutex;
condition_variable mycondition;

void run() {
    unique_lock<mutex> its_lock(run_mutex);
    mycondition.wait(its_lock);

    set<eventgroup_t> mygroup;
    mygroup.insert(SAMPLE_EVENTGROUP_ID);
    app->request_event(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENT_ID, mygroup);
    app->subscribe(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_EVENTGROUP_ID);
    cout<<"------------SUBSCRIBED---------------"<<endl;
}

void on_message(const shared_ptr<vsomeip::message> &_response) {
    cout<<"-----------GOT MESSAGE---------------"<<endl;
    stringstream its_message;
    its_message << "CLIENT: received a notification for event ["
            << setw(4) << setfill('0') << hex
            << _response->get_service() << "."
            << setw(4) << setfill('0') << hex
            << _response->get_instance() << "."
            << setw(4) << setfill('0') << hex
            << _response->get_method() << "] to Client/Session ["
            << setw(4) << setfill('0') << hex
            << _response->get_client() << "/"
            << setw(4) << setfill('0') << hex
            << _response->get_session()
            << "] = ";
    shared_ptr<payload> its_payload = _response->get_payload();
    its_message << "(" << dec << its_payload->get_length() << ") ";
    for (uint32_t i = 0; i < its_payload->get_length(); ++i)
        its_message << hex << setw(2) << setfill('0')
            << (int) its_payload->get_data()[i] << " ";
    cout << its_message.str() << endl;
}

void on_availability(service_t _service, instance_t _instance, bool _is_available) {
    cout<<"-----------AVAILABLE FUNCTION---------------"<<endl;
    cout << "CLIENT: Service ["
            << setw(4) << setfill('0') << hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << endl;
    if(_is_available)
    {
        mycondition.notify_one();
    }
    
}

int main() {
    cout<<"------------------INITIALIZING CLIENT----------------"<<endl;
    app = runtime::get()->create_application("Subscriber");
    app->init();

    cout<<"------------------REQUESTING SERVICE----------------"<<endl;
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);

    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    
    app->register_message_handler(ANY_SERVICE, ANY_INSTANCE, ANY_METHOD, on_message);
    thread sender(run);
    app->start();
}