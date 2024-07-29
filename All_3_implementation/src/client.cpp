#include <iomanip>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>
using namespace vsomeip_v3;

#include "address.hpp"

mutex lock;
condition_variable all;

class Client 
{
    private:
        shared_ptr <application> app;
        set<eventgroup_t> mygroup;
        condition_variable available_rr;
        condition_variable available_e;
        condition_variable available_f;
        shared_ptr<payload> payload_1; 
        shared_ptr<payload> payload_2;
        shared_ptr<payload> payload_3;
        mutex waiting_for_all;
        mutex waiting_event;
        mutex waiting_field;
    public:
        Client() : app(runtime::get()->create_application()) {};
        void available_res_req(service_t s, instance_t i, bool flag);
        void available_event(service_t s, instance_t i, bool flag);
        void available_field(service_t s, instance_t i, bool flag);
        void register_res_req();
        void register_event();
        void register_field();
        void sub_event();
        void sub_field();
        void init();
        void requesting();
        void getting_response(const shared_ptr<message> &_response);
        void getting_event(const shared_ptr<message> &_response);
        void getter();
        void setter();
        void getter_field_response(const shared_ptr<message> &_response);
        void setter_field_response(const shared_ptr<message> &_response);
        void getting_field(const shared_ptr<message> &_response);
        void start();
        void waiting();
        
};

void Client :: register_res_req()
{   
    app->register_availability_handler(SERVICE_ID_1, 
                                        INSTANCE_ID_1, 
    bind(&Client::available_res_req,this,placeholders::_1,placeholders::_2,placeholders::_3));

    app->request_service(SERVICE_ID_1, INSTANCE_ID_1);

    app->register_message_handler(SERVICE_ID_1,
                                 INSTANCE_ID_1,
                                  METHOD_ID_1,
    bind(&Client::getting_response,this,placeholders::_1));
};

void Client :: register_event()
{
    app->register_availability_handler(SERVICE_ID_2, 
                                        INSTANCE_ID_2, 
    bind(&Client::available_event,this,placeholders::_1,placeholders::_2,placeholders::_3));

    app->request_service(SERVICE_ID_2, INSTANCE_ID_2);

    app->register_message_handler(SERVICE_ID_2,
                                 INSTANCE_ID_2,
                                  ANY_METHOD,
    bind(&Client::getting_event,this,placeholders::_1));
};

void Client :: register_field()
{
    app->register_availability_handler(SERVICE_ID_3, 
                                        INSTANCE_ID_3, 
    bind(&Client::available_field,this,placeholders::_1,placeholders::_2,placeholders::_3));

    app->request_service(SERVICE_ID_3, INSTANCE_ID_3);

    app->register_message_handler(SERVICE_ID_3,
                                 INSTANCE_ID_3,
                                  GET_METHOD_ID,
    bind(&Client::getter_field_response,this,placeholders::_1));

    app->register_message_handler(SERVICE_ID_3,
                                 INSTANCE_ID_3,
                                  SET_METHOD_ID,
    bind(&Client::setter_field_response,this,placeholders::_1));

    app->register_message_handler(SERVICE_ID_3,
                                 INSTANCE_ID_3,
                                  ANY_METHOD,
    bind(&Client::getting_field,this,placeholders::_1));
};

void Client :: available_res_req(service_t s, instance_t i, bool flag)
{
    cout << "CLIENT: Service ["
        << setw(4) << setfill('0') << hex << s << "." << i
            << "] is "
            << (flag ? "available." : "NOT available.")
            << endl;
    available_rr.notify_one();
};

void Client :: available_event(service_t s, instance_t i, bool flag)
{
    cout << "CLIENT: Service ["
        << setw(4) << setfill('0') << hex << s << "." << i
            << "] is "
            << (flag ? "available." : "NOT available.")
            << endl;
    available_e.notify_one();
};

void Client :: available_field(service_t s, instance_t i, bool flag)
{
    cout << "CLIENT: Service ["
        << setw(4) << setfill('0') << hex << s << "." << i
            << "] is "
            << (flag ? "available." : "NOT available.")
            << endl;
    available_f.notify_one();
};

void Client :: sub_event()
{
    unique_lock<mutex> its_lock(waiting_event);
    available_e.wait(its_lock);
    app->request_event(SERVICE_ID_2, INSTANCE_ID_2, EVENT_ID_2, mygroup);
    app->subscribe(SERVICE_ID_2, INSTANCE_ID_2, EVENT_GROUP_ID);
};

void Client :: sub_field()
{
    unique_lock<mutex> its_lock(waiting_field);
    available_f.wait(its_lock);
    app->request_event(SERVICE_ID_3, INSTANCE_ID_3, EVENT_ID_3, mygroup);
    app->subscribe(SERVICE_ID_3, INSTANCE_ID_3, EVENT_GROUP_ID);
};

void Client :: init()
{
    app->init();
    mygroup.insert(EVENT_GROUP_ID);
    register_res_req();
    register_event();
    register_field();
};

void Client :: requesting()
{
    shared_ptr< vsomeip::message > request;
    request = runtime::get()->create_request();
    request->set_service(SERVICE_ID_1);
    request->set_instance(INSTANCE_ID_1);
    request->set_method(METHOD_ID_1);

    int request_data = 2;
    cout<<"Menu :"<<endl<<"List of taken tools : 1"<<endl<<"List of not taken tools : 0"<<endl;
    while(request_data!=1 && request_data!=0)
    {
        cout<<"Enter valid choice : ";
        cin>>request_data;
    }

    shared_ptr< payload > request_payload = runtime::get()->create_payload();
    vector< byte_t > request_payload_data;
    request_payload_data.push_back(request_data);
    request_payload->set_data(request_payload_data);
    request->set_payload(request_payload);
    app->send(request);
    cout<<"Request sent successfully"<<endl;
};

void Client :: getting_response(const shared_ptr<message> &_response)
{
    cout << "CLIENT: Received message with Client/Session ["
      << setw(4) << setfill('0') << hex << _response->get_client() << "/"
      << setw(4) << setfill('0') << hex << _response->get_session() << "] "
      << endl;
    payload_1=_response->get_payload();
    byte_t* mydata=payload_1->get_data();
    length_t l = payload_1->get_length();
    if(mydata[0]==-1)
    {
        cout<<"Invalid request"<<endl;
    }
    else
    {
        cout<<"Tools : "<<endl;
        for(length_t i=0;i<l;i++)
        {
            cout<<Inventory[(int)*(mydata+i)]<<endl;
        }
    }
};

void Client :: getting_event(const shared_ptr<message> &_response)
{
    cout<<"Received Daily status notification"<<endl;
    cout << " Event ["
                << setfill('0') << hex
                << setw(4) << _response->get_service() << "."
                << setw(4) << _response->get_instance() << "."
                << setw(4) << _response->get_method() << "] to Client/Session ["
                << setw(4) << _response->get_client() << "/"
                << setw(4) << _response->get_session()
                << "] = ";
    payload_2=_response->get_payload();
    length_t len = payload_2->get_length();
    byte_t* mydata = payload_2->get_data();
    cout<<"Inventory : "<<endl;
    for(length_t i=0;i<len;i++)
    {
        cout<<Inventory[i]<<" : ";
        if((int)*(mydata+i))
        {
            cout<<"Taken";
        }
        else
        {
            cout<<"Not Taken";
        }
        cout<<endl;
    }
};

void Client :: getter()
{
    shared_ptr< vsomeip::message > request;
    request = runtime::get()->create_request();
    request->set_service(SERVICE_ID_3);
    request->set_instance(INSTANCE_ID_3);
    request->set_method(GET_METHOD_ID);

    int index;
    cout<<"Tools Menu : "<<endl;
    for(int i=0;i<LEN;i++)
    {
        cout<<i<<"."<<Inventory[i]<<endl;
    }
    cout<<"Enter Tool Number : ";
    cin>>index;

    shared_ptr< payload > request_payload = runtime::get()->create_payload();
    vector< byte_t > request_payload_data;
    request_payload_data.push_back(index);
    request_payload->set_data(request_payload_data);
    request->set_payload(request_payload);
    app->send(request);
    cout<<"Getter sent successfully"<<endl; 
};

void Client :: setter()
{
    shared_ptr< vsomeip::message > request;
    request = runtime::get()->create_request();
    request->set_service(SERVICE_ID_3);
    request->set_instance(INSTANCE_ID_3);
    request->set_method(SET_METHOD_ID);

    int index;
    cout<<"Tools Menu : "<<endl;
    for(int i=0;i<LEN;i++)
    {
        cout<<i<<"."<<Inventory[i]<<endl;
    }
    cout<<"Enter Tool Number : ";
    cin>>index;

    shared_ptr< payload > request_payload = runtime::get()->create_payload();
    vector< byte_t > request_payload_data;
    request_payload_data.push_back(index);
    request_payload->set_data(request_payload_data);
    request->set_payload(request_payload);
    app->send(request);
    cout<<"Setter sent successfully"<<endl;
};

void Client :: getter_field_response(const shared_ptr<message> &_response)
{
    cout << "CLIENT: Received message with Client/Session ["
      << setw(4) << setfill('0') << hex << _response->get_client() << "/"
      << setw(4) << setfill('0') << hex << _response->get_session() << "] "
      << endl;
    payload_3=_response->get_payload();
    int data=(int)*payload_3->get_data();
    length_t l = payload_3->get_length();
    
    if(data==-1)
    {
        cout<<"Invalid request"<<endl;
    }
    else 
    {
        if(data==0)
        {
            cout<<"Tool not taken"<<endl;
        }
        else if(data==1)
        {
            cout<<"Tool taken"<<endl;
        }
        else
        {
            cout<<"Received a wrong response"<<endl;
        }
    }
};

void Client :: setter_field_response(const shared_ptr<message> &_response)
{
    cout << "CLIENT: Received message with Client/Session ["
      << setw(4) << setfill('0') << hex << _response->get_client() << "/"
      << setw(4) << setfill('0') << hex << _response->get_session() << "] "
      << endl;
    payload_3=_response->get_payload();
    int mydata=(int)*payload_3->get_data();
    length_t l = payload_3->get_length();
    if(mydata==-1)
    {
        cout<<"Invalid request"<<endl;
    }
    else 
    {
        if(mydata==0)
        {
            cout<<"Tool is not available....setter failed"<<endl;
        }
        else if(mydata==1)
        {
            cout<<"Tool available......setter successful"<<endl;
        }
        else
        {
            cout<<"Received a wrong response"<<endl;
        }
    }
}

void Client :: getting_field(const shared_ptr<message> &_response)
{
    cout<<"Notification : Tool Taken"<<endl;
    cout << " Event ["
                << setfill('0') << hex
                << setw(4) << _response->get_service() << "."
                << setw(4) << _response->get_instance() << "."
                << setw(4) << _response->get_method() << "] to Client/Session ["
                << setw(4) << _response->get_client() << "/"
                << setw(4) << _response->get_session()
                << "] = ";
    payload_3=_response->get_payload();
    length_t len = payload_3->get_length();
    int mydata =(int)*payload_3->get_data();
    cout<<"Tool : "<<Inventory[mydata]<<endl;
}

void Client :: start()
{
    thread sub_e(bind(&Client::sub_event,this));
    thread sub_f(bind(&Client::sub_field,this));
    thread to_wait(bind(&Client::waiting,this));
    app->start();
};

void Client :: waiting()
{
    unique_lock<mutex> its_lock(waiting_for_all);
    available_rr.wait(its_lock);
    available_e.wait(its_lock);
    available_f.wait(its_lock);
    cout<<"----------all waiting done---------"<<endl;
    all.notify_one();
};

void menu(Client& client)
{
    unique_lock<mutex> its_lock(lock);
    all.wait(its_lock);   
    this_thread::sleep_for(5s);
    cout<<"-------------------------------"<<endl;
    int choice=-1;
    while(1)
    {
        
        cout<<"Menu :"<<endl<<"1.Request for listing tools(Request)"<<endl
            <<"2.Check if a tool is taken or not(Get)"<<endl
            <<"3.Take a tool(Set)"<<endl<<"Enter choice : ";
        cin>>choice;
        switch(choice)
        {
            case 1:
            {
                client.requesting();
                break;
            }
            case 2:
            {
                client.getter();
                break;
            }
            case 3:
            {
                client.setter();
                break;
            }
            default:
            {
                cout<<"Invalid Choice"<<endl;
            }
        }
        this_thread::sleep_for(15s);
        
    }

};

int main()
{
    Client client;
    client.init();
    thread run(menu,ref(client));
    client.start();
    return 0;
};