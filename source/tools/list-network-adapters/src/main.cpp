#include <iomanip>
#include <iostream>

#include "osal/NetworkAdapterList.h"
#include "osal/NetworkAdapter.h"
#include "osal/Unused.h"

using namespace std;

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else
int main(int UNUSED(argc), const char * UNUSED(argv[]))
#endif
{
    OSAL::Network::AdapterList adapterList;

    for (auto adapter : adapterList.GetAdapters())
    {
        cout << setw(15) << left << adapter->Name() << endl
             << "  Family             : " << adapter->Family() << endl;
        cout << "  Local address      : " << *adapter->LocalAddress() << endl;
        if (adapter->NetMask() != nullptr)
            cout << "  Network mask       : " << *adapter->NetMask() << endl;
        if (adapter->BroadcastAddress() != nullptr)
            cout << "  Broadcase address  : " << *adapter->BroadcastAddress() << endl;
        if (adapter->DestAddress() != nullptr)
            cout << "  Destination address: " << *adapter->DestAddress() << endl;
    }

    return EXIT_SUCCESS;
}
