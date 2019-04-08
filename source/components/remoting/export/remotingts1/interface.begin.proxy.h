/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_INTERFACE_BEGIN_H
# error Please #include <remotingts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
# if defined(REMOTINGTS1_DECLARE_PROXY)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
	REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    class NAME ## Proxy final : public NAME { \
    friend class ::RemotingTs1::Dispatcher; \
    friend class ::RemotingTs1::ClientNode; \
    friend class ::RemotingTs1::ServerNode; \
        NAME ## Proxy(const NAME ## Proxy &); \
        NAME ## Proxy & operator=(const NAME ## Proxy &); \
        void _remoting_init(::RemotingTs1::Dispatcher &); \
        ::RemotingTs1::Dispatcher * _remoting_dispatcher; \
        const ::std::string _remoting_clientHostId; \
        const ::std::string _remoting_instance; \
        std::vector< ::RemotingTs1::Dispatcher::ProxyId > _remoting_proxies; \
    public: \
        explicit NAME ## Proxy(::RemotingTs1::ClientNode &client); \
        NAME ## Proxy(::RemotingTs1::ClientNode &client, const std::string &instance); \
        NAME ## Proxy(::RemotingTs1::ServerNode &server, const std::string &clientHostId); \
        NAME ## Proxy(::RemotingTs1::ServerNode &server, const std::string &clientHostId, const std::string &instance); \
        virtual ~NAME ## Proxy();

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
        virtual RETURNTYPE NAME() override final;

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1) override final;

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) override final;

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) override final;

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) override final;

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
        virtual void NAME() override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
        virtual void NAME(const ARGTYPE1 &arg1) override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) override final;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) override final;

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS)

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME)

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define REMOTINGTS1_END_EVENTS

#define REMOTINGTS1_END_INTERFACE \
    };


#define REMOTINGTS1_BEGIN_STRUCT(NAME)
#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME)
#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#define REMOTINGTS1_END_STRUCT

# elif defined(REMOTINGTS1_DEFINE_PROXY)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
	REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    NAME ## Proxy :: NAME ## Proxy(::RemotingTs1::ClientNode &client) \
    : _remoting_dispatcher(nullptr), _remoting_clientHostId(), _remoting_instance(client.createInstance<NAME>()) { client.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::RemotingTs1::ClientNode &client, const std::string &instance) \
    : _remoting_dispatcher(nullptr), _remoting_clientHostId(), _remoting_instance(client.createInstance<NAME>(instance)) { client.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::RemotingTs1::ServerNode &server, const std::string &clientHostId) \
    : _remoting_dispatcher(nullptr), _remoting_clientHostId(clientHostId), _remoting_instance(server.createInstance<NAME>(clientHostId)) { server.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::RemotingTs1::ServerNode &server, const std::string &clientHostId, const std::string &instance) \
    : _remoting_dispatcher(nullptr), _remoting_clientHostId(clientHostId), _remoting_instance(server.createInstance<NAME>(clientHostId, instance)) { server.initProxy(this); } \
    NAME ## Proxy :: ~NAME ## Proxy() { \
        if (_remoting_dispatcher) { \
			_remoting_dispatcher->unregisterProxies(&_remoting_clientHostId, _remoting_instance, _remoting_proxies); \
			_remoting_dispatcher->sendDestroyInstance(_remoting_clientHostId, _remoting_instance); } }

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
    RETURNTYPE CLASS ## Proxy :: NAME() { \
        return _remoting_dispatcher->invoke<RETURNTYPE>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME); }

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1) { \
        return _remoting_dispatcher->invoke<RETURNTYPE, ARGTYPE1>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, arg1); }

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) { \
        return _remoting_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, arg1, arg2); }

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) { \
        return _remoting_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, arg1, arg2, arg3); }

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) { \
        return _remoting_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, arg1, arg2, arg3, arg4); }

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) { \
        return _remoting_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, arg1, arg2, arg3, arg4, arg5); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
    void CLASS ## Proxy :: NAME() { \
        return _remoting_dispatcher->invokeAsync(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1) { \
        return _remoting_dispatcher->invokeAsync<ARGTYPE1>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, arg1); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) { \
        return _remoting_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, arg1, arg2); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) { \
        return _remoting_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, arg1, arg2, arg3); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) { \
        return _remoting_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, arg1, arg2, arg3, arg4); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) { \
        return _remoting_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, arg1, arg2, arg3, arg4, arg5); }

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS) \
    void CLASS ## Proxy :: _remoting_init(::RemotingTs1::Dispatcher &dispatcher) { \
		_remoting_dispatcher = &dispatcher;

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event0::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event1<ARGTYPE1>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event2<ARGTYPE1, ARGTYPE2>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event3<ARGTYPE1, ARGTYPE2, ARGTYPE3>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        _remoting_proxies.push_back(dispatcher.registerProxy( \
			&_remoting_clientHostId, _remoting_instance, "/" #CLASS "/" #NAME, \
			&::RemotingTs1::Event5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define REMOTINGTS1_END_EVENTS \
    }

#define REMOTINGTS1_END_INTERFACE


#define REMOTINGTS1_BEGIN_STRUCT(NAME)
#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME)
#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#define REMOTINGTS1_END_STRUCT

# endif
#else // __INTELLISENSE__
///////////////////////////////////////////////////////////////////////////////
// These definitions are here to make intellisense work properly.            //
///////////////////////////////////////////////////////////////////////////////
# if defined(REMOTINGTS1_DECLARE_PROXY)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define REMOTINGTS1_BEGIN_INTERFACE(NAME) REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) class NAME ## Proxy : public NAME { public: explicit NAME ## Proxy(::RemotingTs1::ClientNode &); NAME ## Proxy(::RemotingTs1::ClientNode &, const std::string &instance); NAME ## Proxy(::RemotingTs1::ServerNode &, const std::string &clientHostId); NAME ## Proxy(::RemotingTs1::ServerNode &, const std::string &clientHostId, const std::string &instance);
#  define REMOTINGTS1_BEGIN_METHODS(CLASS)
#  define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) virtual RETURNTYPE NAME();
#  define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) virtual RETURNTYPE NAME(ARGTYPE1 &arg1);
#  define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2);
#  define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3);
#  define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4);
#  define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) virtual void NAME();
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) virtual void NAME(const ARGTYPE1 &arg1);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5);
#  define REMOTINGTS1_END_METHODS
#  define REMOTINGTS1_BEGIN_EVENTS(CLASS)
#  define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME)
#  define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#  define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define REMOTINGTS1_END_EVENTS
#  define REMOTINGTS1_END_INTERFACE };
#  define REMOTINGTS1_BEGIN_STRUCT(NAME)
#  define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#  define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME)
#  define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#  define REMOTINGTS1_END_STRUCT
# elif defined(REMOTINGTS1_DEFINE_PROXY)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define REMOTINGTS1_BEGIN_INTERFACE(NAME)
#  define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION)
#  define REMOTINGTS1_BEGIN_METHODS(CLASS)
#  define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME)
#  define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1)
#  define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define REMOTINGTS1_END_METHODS
#  define REMOTINGTS1_BEGIN_EVENTS(CLASS)
#  define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME)
#  define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#  define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define REMOTINGTS1_END_EVENTS
#  define REMOTINGTS1_END_INTERFACE
#  define REMOTINGTS1_BEGIN_STRUCT(NAME)
#  define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#  define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME)
#  define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#  define REMOTINGTS1_END_STRUCT
# endif
#endif
