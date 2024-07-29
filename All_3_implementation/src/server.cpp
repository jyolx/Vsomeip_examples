#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>
using namespace vsomeip_v3;

#include "address.hpp"

class Server
{
    private:
        shared_ptr <application> app;
        set<eventgroup_t> mygroup;
        shared_ptr<payload> mypayload_1;
        shared_ptr<payload> mypayload_2;
        shared_ptr<payload> mypayload_3;
        vector<Tool> Toolbox;

    public:
        Server(vector<Tool> data) : app(runtime::get()->create_application()),Toolbox(data) {}
        void register_for_req_res();
        void register_for_event();
        void register_for_field();
        void init();
        void response(const shared_ptr<message> &_request);
        void setting_data(const shared_ptr<message> &_request);
        void getting_data(const shared_ptr<message> &_request);
        void notify();
        void notify_field(int tool);
        void start();
};

void Server :: register_for_req_res()
{
    app->register_message_handler(SERVICE_ID_1,
                                INSTANCE_ID_1,
                                METHOD_ID_1,
                                bind(&Server::response,this,placeholders::_1));
    app->offer_service(SERVICE_ID_1,INSTANCE_ID_1);
};

void Server :: register_for_event()
{
    app->offer_service(SERVICE_ID_2,INSTANCE_ID_2);
    app->offer_event(SERVICE_ID_2,INSTANCE_ID_2,EVENT_ID_2,mygroup);
};

void Server :: register_for_field()
{
    app->offer_service(SERVICE_ID_3,INSTANCE_ID_3);
    app->offer_event(SERVICE_ID_3,INSTANCE_ID_3,EVENT_ID_3,mygroup);
    app->register_message_handler(SERVICE_ID_3,
                                INSTANCE_ID_3,
                                SET_METHOD_ID,
                                bind(&Server::setting_data,this,placeholders::_1));
    app->register_message_handler(SERVICE_ID_3,
                                INSTANCE_ID_3,
                                GET_METHOD_ID,
                                bind(&Server::getting_data,this,placeholders::_1));
};

void Server :: init()
{
    app->init();
    mygroup.insert(EVENT_GROUP_ID);
    register_for_req_res();
    register_for_event();
    register_for_field();
};

void Server :: response(const shared_ptr<message> &_request)
{
    cout << "SERVICE: Received Request with Client/Session ["
        << setw(4) << setfill('0') << hex << _request->get_client() << "/"
        << setw(4) << setfill('0') << hex << _request->get_session() << "] "
        <<endl;
    
    shared_ptr<payload> request_payload = _request->get_payload();
    length_t l = request_payload->get_length();
    int data = (int)*request_payload->get_data();

    vector<byte_t> response_data;
    if(l!=1 )
    {
        cout<<"Invalid request"<<endl;
        response_data.push_back(-1);
    }
    else if(!(data==0 || data==1))
    {
        cout<<"Invalid request"<<endl;
        response_data.push_back(-1);
    }
    else
    {
        for(int i=0;i<Toolbox.size();i++)
        {
            if(data==0)
            {
                if(Toolbox[i].taken==0)
                {
                    response_data.push_back(i);
                }
            }
            else
            {
                if(Toolbox[i].taken==1)
                {
                    response_data.push_back(i);
                }
            }
        }
    }
    shared_ptr<message> its_response = runtime::get()->create_response(_request);
    mypayload_1 = runtime::get()->create_payload();
    mypayload_1->set_data(response_data);
    its_response->set_payload(mypayload_1);
    app->send(its_response);
};

void Server :: setting_data(const shared_ptr<message> &_request)
{
    cout << "SERVICE: Received Setter Field with Client/Session ["
        << setw(4) << setfill('0') << hex << _request->get_client() << "/"
        << setw(4) << setfill('0') << hex << _request->get_session() << "] "
        <<endl;

    shared_ptr<payload> request_payload = _request->get_payload();
    length_t l = request_payload->get_length();
    int data = (int)*request_payload->get_data();

    int flag=0;
    vector<byte_t> response_data;
    if(l!=1 )
    {
        cout<<"Invalid request"<<endl;
        response_data.push_back(-1);
    }
    else if(data<0 || data>Toolbox.size()-1)
    {
        cout<<"Invalid request"<<endl;
        response_data.push_back(-1);
    }
    else
    {
        if(Toolbox[data].taken==0)
        {
            Toolbox[data].taken=1;
            response_data.push_back(1);
            flag=1;
        }
        else
        {
            response_data.push_back(0);
        }
    }
    shared_ptr<message> its_response = runtime::get()->create_response(_request);
    mypayload_3 = runtime::get()->create_payload();
    mypayload_3->set_data(response_data);
    its_response->set_payload(mypayload_3);
    app->send(its_response);
    if(flag)
    {
        notify_field(data);
    }
};

void Server :: getting_data(const shared_ptr<message> &_request)
{
    cout << "SERVICE: Received Getting Field with Client/Session ["
        << setw(4) << setfill('0') << hex << _request->get_client() << "/"
        << setw(4) << setfill('0') << hex << _request->get_session() << "] "
        <<endl;

    shared_ptr<payload> request_payload = _request->get_payload();
    length_t l = request_payload->get_length();
    int data = (int)*request_payload->get_data();

    vector<byte_t> response_data;
    if(l!=1 )
    {
        cout<<"Invalid request a"<<endl;
        response_data.push_back(-1);
    }
    else if(data<0 || data>LEN-1)
    {
        cout<<"Invalid request b"<<data<<"  "<<LEN<<endl;
        response_data.push_back(-1);
    }
    else
    {
        cout<<"Sending "<<Toolbox[data].status()<<endl;
        response_data.push_back(Toolbox[data].status());
    }
    shared_ptr<message> its_response = runtime::get()->create_response(_request);
    mypayload_3 = runtime::get()->create_payload();
    mypayload_3->set_data(response_data);
    its_response->set_payload(mypayload_3);
    app->send(its_response);
};

void Server :: notify()
{
    mypayload_2 = runtime::get()->create_payload();
    vector<byte_t> mypayload_data;
    for(int i=0;i<Toolbox.size();i++)
    {
        mypayload_data.push_back(Toolbox[i].taken);
    }
    mypayload_2->set_data(mypayload_data);
    app->notify(SERVICE_ID_2, INSTANCE_ID_2, EVENT_ID_2, mypayload_2);
    cout<<"Daily notification for event has been sent"<<endl;
};

void Server :: notify_field(int tool)
{
    mypayload_3 = runtime::get()->create_payload();
    vector<byte_t> mypayload_data;
    mypayload_data.push_back(tool);
    mypayload_3->set_data(mypayload_data);
    app->notify(SERVICE_ID_3, INSTANCE_ID_3, EVENT_ID_3, mypayload_3);
    cout<<"Notification for tool taken has been sent"<<endl;
};

void Server :: start()
{
    app->start();
};

void notify(Server instance)
{
    this_thread::sleep_for(5s);
    instance.notify();
};

int main()
{
    vector<Tool> Toolbox;
    for(int i=0;i<LEN;i++)
    {
        Toolbox.push_back(Tool(Inventory[i]));
    }
    for(int i=0;i<LEN;i++)
    {
        cout<<Toolbox[i].name<<" : "<<Toolbox[i].status()<<endl;
    }
    Server server(Toolbox);
    server.init();
    thread run(notify,server);
    server.start();
    return 0;
};