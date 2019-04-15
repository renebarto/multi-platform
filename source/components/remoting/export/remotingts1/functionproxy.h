/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_FUNCTIONPROXY_H
#define REMOTINGTS1_FUNCTIONPROXY_H
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense

#include "remoting1/message.h"
#include "formatter.h"

namespace RemotingTs1 {
    
using ::Remoting1::Message;

class FunctionProxySerializer
{
protected:
    template <typename _prm>
    inline static void parameter(_prm &prm, Formatter::Serializer &serializer)
    {
		serializer << prm;
    }

    template <typename _prm>
	inline static void parameter(const _prm &, Formatter::Serializer &)
    {
        // Const parameters do not have to be serialized again.
    }
        
    template <typename _prm>
	inline static _prm parameter(Formatter::Deserializer &deserializer, _prm *)
    {
        _prm prm;
		deserializer >> prm;
        return prm;
    }
            
    template <typename _prm>
    inline static _prm parameter(Formatter::Deserializer &deserializer, const _prm *)
    {
        _prm prm;
		deserializer >> prm;
        return prm;
    }

	template <typename _ret>
    inline static void returnValue(_ret &ret, Formatter::Serializer &serializer)
    {
		serializer << ret;
    }
};

template <typename _ret>
class FunctionProxy : private FunctionProxySerializer
{
public:
	template <class _object>
    inline static void proxy0(Remoting1::BulkChannel &bulkChannel, const Message &, Message &out, _ret(_object::* fptr)(), _object *obj)
    {
        _ret ret = (obj->*fptr)();

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
	}

    template <class _object, typename _prm1>
    inline static void proxy1(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, _ret(_object::* fptr)(_prm1 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
            
        _ret ret = (obj->*fptr)(prm1);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }

    template <class _object, typename _prm1, typename _prm2>
    inline static void proxy2(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, _ret(_object::* fptr)(_prm1 &, _prm2 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
           
        _ret ret = (obj->*fptr)(prm1, prm2);
            
		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }

    template <class _object, typename _prm1, typename _prm2, typename _prm3>
    inline static void proxy3(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));
           
        _ret ret = (obj->*fptr)(prm1, prm2, prm3);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }

    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static void proxy4(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));
        _prm4 prm4 = parameter(deserializer, reinterpret_cast<_prm4 *>(NULL));
            
        _ret ret = (obj->*fptr)(prm1, prm2, prm3, prm4);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);
		parameter(prm4, serializer);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }

    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static void proxy5(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));
        _prm4 prm4 = parameter(deserializer, reinterpret_cast<_prm4 *>(NULL));
        _prm5 prm5 = parameter(deserializer, reinterpret_cast<_prm5 *>(NULL));
            
        _ret ret = (obj->*fptr)(prm1, prm2, prm3, prm4, prm5);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);
		parameter(prm4, serializer);
		parameter(prm5, serializer);
		returnValue(ret, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }
};

template <>
class FunctionProxy<void> : private FunctionProxySerializer
{
public:
	template <class _object>
    inline static void proxy0(Remoting1::BulkChannel &, const Message &, Message &, void(_object::* fptr)(), _object *obj)
    {
        (obj->*fptr)();
    }

    template <class _object, typename _prm1>
    inline static void proxy1(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, void(_object::* fptr)(_prm1 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));

        (obj->*fptr)(prm1);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
    }

    template <class _object, typename _prm1, typename _prm2>
    inline static void proxy2(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, void(_object::* fptr)(_prm1 &, _prm2 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));

        (obj->*fptr)(prm1, prm2);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
	}

    template <class _object, typename _prm1, typename _prm2, typename _prm3>
    inline static void proxy3(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));

        (obj->*fptr)(prm1, prm2, prm3);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
	}

    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static void proxy4(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));
        _prm4 prm4 = parameter(deserializer, reinterpret_cast<_prm4 *>(NULL));

        (obj->*fptr)(prm1, prm2, prm3, prm4);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);
		parameter(prm4, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
	}

    template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static void proxy5(Remoting1::BulkChannel &bulkChannel, const Message &in, Message &out, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *obj)
    {
		Formatter::Deserializer deserializer(in.content(), &bulkChannel);
        _prm1 prm1 = parameter(deserializer, reinterpret_cast<_prm1 *>(NULL));
        _prm2 prm2 = parameter(deserializer, reinterpret_cast<_prm2 *>(NULL));
        _prm3 prm3 = parameter(deserializer, reinterpret_cast<_prm3 *>(NULL));
        _prm4 prm4 = parameter(deserializer, reinterpret_cast<_prm4 *>(NULL));
        _prm5 prm5 = parameter(deserializer, reinterpret_cast<_prm5 *>(NULL));

        (obj->*fptr)(prm1, prm2, prm3, prm4, prm5);

		Formatter::Serializer serializer(out.content(), &bulkChannel);
		parameter(prm1, serializer);
		parameter(prm2, serializer);
		parameter(prm3, serializer);
		parameter(prm4, serializer);
		parameter(prm5, serializer);

		const std::vector<Remoting1::BulkChannel::BulkId> &bulkIds = serializer.bulkIds();
		bulkChannel.addRecipients(!bulkIds.empty() ? &bulkIds[0] : nullptr, bulkIds.size());
	}
};

} // End of namespaces

#endif // __INTELLISENSE__
#endif
