#include <zmqnotificationinterface.h>
static CZMQNotificationInterface* pzmqNotificationInterface = nullptr;

void UnRegister()
{
    if (pzmqNotificationInterface) 
    {
        UnregisterValidationInterface(pzmqNotificationInterface);
        delete pzmqNotificationInterface;
        pzmqNotificationInterface = nullptr;
    }
}

void Register()
{

    pzmqNotificationInterface = CZMQNotificationInterface::Create();

    if (pzmqNotificationInterface) 
    {
        RegisterValidationInterface(pzmqNotificationInterface);
    }
}


int main(int argc,char*argv[])
{
    Register();

    return 0;
}

