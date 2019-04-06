/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_INTERFACE_BEGIN_H
# error Please #include <sscfts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
# if defined(SSCFTS1_DECLARE_PROXY)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_INTERFACE(NAME) \
	SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    class NAME ## Proxy final : public NAME { \
    friend class ::SscfTs1::Dispatcher; \
    friend class ::SscfTs1::ClientNode; \
    friend class ::SscfTs1::ServerNode; \
        NAME ## Proxy(const NAME ## Proxy &); \
        NAME ## Proxy & operator=(const NAME ## Proxy &); \
        void _sscf_init(::SscfTs1::Dispatcher &); \
        ::SscfTs1::Dispatcher * _sscf_dispatcher; \
        const ::std::string _sscf_clientHostId; \
        const ::std::string _sscf_instance; \
        std::vector< ::SscfTs1::Dispatcher::ProxyId > _sscf_proxies; \
    public: \
        explicit NAME ## Proxy(::SscfTs1::ClientNode &client); \
        NAME ## Proxy(::SscfTs1::ClientNode &client, const std::string &instance); \
        NAME ## Proxy(::SscfTs1::ServerNode &server, const std::string &clientHostId); \
        NAME ## Proxy(::SscfTs1::ServerNode &server, const std::string &clientHostId, const std::string &instance); \
        virtual ~NAME ## Proxy();

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
        virtual RETURNTYPE NAME() override final;

#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1) override final;

#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) override final;

#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) override final;

#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) override final;

#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
        virtual void NAME() override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
        virtual void NAME(const ARGTYPE1 &arg1) override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) override final;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) override final;

#define SSCFTS1_END_METHODS

#define SSCFTS1_BEGIN_EVENTS(CLASS)

#define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME)

#define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)

#define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)

#define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)

#define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)

#define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define SSCFTS1_END_EVENTS

#define SSCFTS1_END_INTERFACE \
    };


#define SSCFTS1_BEGIN_STRUCT(NAME)
#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME)
#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#define SSCFTS1_END_STRUCT

# elif defined(SSCFTS1_DEFINE_PROXY)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_INTERFACE(NAME) \
	SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    NAME ## Proxy :: NAME ## Proxy(::SscfTs1::ClientNode &client) \
    : _sscf_dispatcher(nullptr), _sscf_clientHostId(), _sscf_instance(client.createInstance<NAME>()) { client.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::SscfTs1::ClientNode &client, const std::string &instance) \
    : _sscf_dispatcher(nullptr), _sscf_clientHostId(), _sscf_instance(client.createInstance<NAME>(instance)) { client.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::SscfTs1::ServerNode &server, const std::string &clientHostId) \
    : _sscf_dispatcher(nullptr), _sscf_clientHostId(clientHostId), _sscf_instance(server.createInstance<NAME>(clientHostId)) { server.initProxy(this); } \
    NAME ## Proxy :: NAME ## Proxy(::SscfTs1::ServerNode &server, const std::string &clientHostId, const std::string &instance) \
    : _sscf_dispatcher(nullptr), _sscf_clientHostId(clientHostId), _sscf_instance(server.createInstance<NAME>(clientHostId, instance)) { server.initProxy(this); } \
    NAME ## Proxy :: ~NAME ## Proxy() { \
        if (_sscf_dispatcher) { \
			_sscf_dispatcher->unregisterProxies(&_sscf_clientHostId, _sscf_instance, _sscf_proxies); \
			_sscf_dispatcher->sendDestroyInstance(_sscf_clientHostId, _sscf_instance); } }

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
    RETURNTYPE CLASS ## Proxy :: NAME() { \
        return _sscf_dispatcher->invoke<RETURNTYPE>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME); }

#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1) { \
        return _sscf_dispatcher->invoke<RETURNTYPE, ARGTYPE1>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, arg1); }

#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) { \
        return _sscf_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, arg1, arg2); }

#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) { \
        return _sscf_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, arg1, arg2, arg3); }

#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) { \
        return _sscf_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, arg1, arg2, arg3, arg4); }

#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    RETURNTYPE CLASS ## Proxy :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) { \
        return _sscf_dispatcher->invoke<RETURNTYPE, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, arg1, arg2, arg3, arg4, arg5); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
    void CLASS ## Proxy :: NAME() { \
        return _sscf_dispatcher->invokeAsync(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1) { \
        return _sscf_dispatcher->invokeAsync<ARGTYPE1>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, arg1); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) { \
        return _sscf_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, arg1, arg2); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) { \
        return _sscf_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, arg1, arg2, arg3); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) { \
        return _sscf_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, arg1, arg2, arg3, arg4); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    void CLASS ## Proxy :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) { \
        return _sscf_dispatcher->invokeAsync<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, arg1, arg2, arg3, arg4, arg5); }

#define SSCFTS1_END_METHODS

#define SSCFTS1_BEGIN_EVENTS(CLASS) \
    void CLASS ## Proxy :: _sscf_init(::SscfTs1::Dispatcher &dispatcher) { \
		_sscf_dispatcher = &dispatcher;

#define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event0::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event1<ARGTYPE1>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event2<ARGTYPE1, ARGTYPE2>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event3<ARGTYPE1, ARGTYPE2, ARGTYPE3>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        _sscf_proxies.push_back(dispatcher.registerProxy( \
			&_sscf_clientHostId, _sscf_instance, "/" #CLASS "/" #NAME, \
			&::SscfTs1::Event5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>::operator(), &(static_cast<CLASS *>(this)->NAME)));

#define SSCFTS1_END_EVENTS \
    }

#define SSCFTS1_END_INTERFACE


#define SSCFTS1_BEGIN_STRUCT(NAME)
#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME)
#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#define SSCFTS1_END_STRUCT

# endif
#else // __INTELLISENSE__
///////////////////////////////////////////////////////////////////////////////
// These definitions are here to make intellisense work properly.            //
///////////////////////////////////////////////////////////////////////////////
# if defined(SSCFTS1_DECLARE_PROXY)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define SSCFTS1_BEGIN_INTERFACE(NAME) SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) class NAME ## Proxy : public NAME { public: explicit NAME ## Proxy(::SscfTs1::ClientNode &); NAME ## Proxy(::SscfTs1::ClientNode &, const std::string &instance); NAME ## Proxy(::SscfTs1::ServerNode &, const std::string &clientHostId); NAME ## Proxy(::SscfTs1::ServerNode &, const std::string &clientHostId, const std::string &instance);
#  define SSCFTS1_BEGIN_METHODS(CLASS)
#  define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) virtual RETURNTYPE NAME();
#  define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) virtual RETURNTYPE NAME(ARGTYPE1 &arg1);
#  define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2);
#  define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3);
#  define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4);
#  define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) virtual void NAME();
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) virtual void NAME(const ARGTYPE1 &arg1);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5);
#  define SSCFTS1_END_METHODS
#  define SSCFTS1_BEGIN_EVENTS(CLASS)
#  define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME)
#  define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#  define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define SSCFTS1_END_EVENTS
#  define SSCFTS1_END_INTERFACE };
#  define SSCFTS1_BEGIN_STRUCT(NAME)
#  define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#  define SSCFTS1_STRUCT_MEMBER(TYPE, NAME)
#  define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#  define SSCFTS1_END_STRUCT
# elif defined(SSCFTS1_DEFINE_PROXY)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define SSCFTS1_BEGIN_INTERFACE(NAME)
#  define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION)
#  define SSCFTS1_BEGIN_METHODS(CLASS)
#  define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME)
#  define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1)
#  define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define SSCFTS1_END_METHODS
#  define SSCFTS1_BEGIN_EVENTS(CLASS)
#  define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME)
#  define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#  define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#  define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#  define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#  define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)
#  define SSCFTS1_END_EVENTS
#  define SSCFTS1_END_INTERFACE
#  define SSCFTS1_BEGIN_STRUCT(NAME)
#  define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM)
#  define SSCFTS1_STRUCT_MEMBER(TYPE, NAME)
#  define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE)
#  define SSCFTS1_END_STRUCT
# endif
#endif
