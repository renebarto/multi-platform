#include <iostream>

#include "osal/Network.h"

using namespace std;

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else
int main(int argc, const char * argv[])
#endif
{
    Network::AdapterList adapterList;

    for (auto adapter : adapterList.GetAdapters())
    {
        cout << setw(15) << left << adapter->Name() << endl
             << "  Family             : " << adapter->Family() << endl;
        cout << "  Local address      : " << adapter->LocalAddress()->ToString() << endl;
        if (adapter->NetMask() != nullptr)
            cout << "  Network mask       : " << adapter->NetMask()->ToString() << endl;
        if (adapter->BroadcastAddress() != nullptr)
            cout << "  Broadcase address  : " << adapter->BroadcastAddress()->ToString() << endl;
        if (adapter->DestAddress() != nullptr)
            cout << "  Destination address: " << adapter->DestAddress()->ToString() << endl;
    }

    return EXIT_SUCCESS;
}