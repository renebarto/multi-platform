#include <iostream>

#include "network/Adapter.h"
#include "network/AdapterList.h"

using namespace std;

int main(int UNUSED(argc), const char * UNUSED(argv[]))
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