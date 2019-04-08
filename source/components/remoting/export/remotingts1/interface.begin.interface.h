/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_INTERFACE_BEGIN_H
# error Please #include <remotingts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
# if defined(REMOTINGTS1_DECLARE_INTERFACE)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
    REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    class NAME { \
    friend class ::RemotingTs1::Dispatcher; \
    friend class ::RemotingTs1::ClientNode; \
    friend class ::RemotingTs1::ServerNode; \
		NAME(const NAME &); \
		NAME & operator=(const NAME &); \
        void _remoting_register(::RemotingTs1::Dispatcher &, const std::string &); \
        std::map<::RemotingTs1::Dispatcher *, ::RemotingTs1::Dispatcher::ProxyList> _remoting_dispatchers; \
        static const char _remoting_name[]; \
    public: \
        NAME(); \
        virtual ~NAME();

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
        virtual RETURNTYPE NAME() = 0;

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
        virtual RETURNTYPE NAME(ARGTYPE1 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual RETURNTYPE NAME(ARGTYPE1 &, ARGTYPE2 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual RETURNTYPE NAME(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual RETURNTYPE NAME(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &, ARGTYPE4 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual RETURNTYPE NAME(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &, ARGTYPE4 &, ARGTYPE5 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
        virtual void NAME() = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
        virtual void NAME(const ARGTYPE1 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual void NAME(const ARGTYPE1 &, const ARGTYPE2 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual void NAME(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual void NAME(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &, const ARGTYPE4 &) = 0;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual void NAME(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &, const ARGTYPE4 &, const ARGTYPE5 &) = 0;

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS)

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) \
        ::RemotingTs1::Event0 NAME;

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
        ::RemotingTs1::Event1<ARGTYPE1> NAME;

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        ::RemotingTs1::Event2<ARGTYPE1, ARGTYPE2> NAME;

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        ::RemotingTs1::Event3<ARGTYPE1, ARGTYPE2, ARGTYPE3> NAME;

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        ::RemotingTs1::Event4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> NAME;

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        ::RemotingTs1::Event5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> NAME;

#define REMOTINGTS1_END_EVENTS

#define REMOTINGTS1_END_INTERFACE \
    };


#define REMOTINGTS1_BEGIN_STRUCT(NAME) \
    struct NAME : private ::RemotingTs1::Formatter::Struct { \
    private: \
        void _remoting_serializer(::RemotingTs1::Formatter::Serializer *s, ::RemotingTs1::Formatter::Deserializer *d); \
    protected: \
        explicit NAME(size_t); \
    public: \
		friend ::RemotingTs1::Formatter::Serializer & operator<<(::RemotingTs1::Formatter::Serializer &s, const NAME &v) { const_cast<NAME &>(v)._remoting_serializer(&s, 0); return s; } \
		friend ::RemotingTs1::Formatter::Deserializer & operator>>(::RemotingTs1::Formatter::Deserializer &d, NAME &v) { v._remoting_serializer(0, &d); return d; } \
        NAME();

#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
    struct NAME : public FROM { \
    private: \
        void _remoting_serializer(::RemotingTs1::Formatter::Serializer *s, ::RemotingTs1::Formatter::Deserializer *d); \
    protected: \
        explicit NAME(size_t); \
    public: \
		friend ::RemotingTs1::Formatter::Serializer & operator<<(::RemotingTs1::Formatter::Serializer &s, const NAME &v) { const_cast<NAME &>(v)._remoting_serializer(&s, 0); return s; } \
		friend ::RemotingTs1::Formatter::Deserializer & operator>>(::RemotingTs1::Formatter::Deserializer &d, NAME &v) { v._remoting_serializer(0, &d); return d; } \
        NAME();

#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) \
        TYPE NAME;

#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
        TYPE NAME[SIZE];

#define REMOTINGTS1_END_STRUCT \
    };

# elif defined(REMOTINGTS1_DEFINE_INTERFACE)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
    const char NAME :: _remoting_name[] = #NAME; \
    NAME :: NAME() { } \
    NAME :: ~NAME() { \
		const auto copy = _remoting_dispatchers; \
        for (const auto &i : copy) { \
		    i.first->unregisterProxies(nullptr, i.second.instance, i.second.proxies); \
            i.first->unregisterInterface<NAME>(i.second.instance); \
            i.first->unregisterObject(this); } } \
    void NAME :: _remoting_register(::RemotingTs1::Dispatcher &dispatcher, const std::string &instance) { \
		if (instance.empty()) \
			_remoting_dispatchers.erase(&dispatcher); \
		else { \
			auto &proxyList = _remoting_dispatchers[&dispatcher]; \
			proxyList.instance = instance; \
			dispatcher.registerObject(this);

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
	const char NAME :: _remoting_name[] = #NAME "/" VERSION; \
    NAME :: NAME() { } \
    NAME :: ~NAME() { \
		const auto copy = _remoting_dispatchers; \
        for (const auto &i : copy) { \
		    i.first->unregisterProxies(nullptr, i.second.instance, i.second.proxies); \
            i.first->unregisterInterface<NAME>(i.second.instance); \
            i.first->unregisterObject(this); } } \
    void NAME :: _remoting_register(::RemotingTs1::Dispatcher &dispatcher, const std::string &instance) { \
		if (instance.empty()) \
			_remoting_dispatchers.erase(&dispatcher); \
		else { \
			auto &proxyList = _remoting_dispatchers[&dispatcher]; \
			proxyList.instance = instance; \
			dispatcher.registerObject(this);

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME, \
					static_cast<RETURNTYPE(CLASS::*)()>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, \
					static_cast<RETURNTYPE(CLASS::*)(ARGTYPE1 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, \
					static_cast<RETURNTYPE(CLASS::*)(ARGTYPE1 &, ARGTYPE2 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, \
					static_cast<RETURNTYPE(CLASS::*)(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, \
					static_cast<RETURNTYPE(CLASS::*)(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &, ARGTYPE4 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, \
					static_cast<RETURNTYPE(CLASS::*)(ARGTYPE1 &, ARGTYPE2 &, ARGTYPE3 &, ARGTYPE4 &, ARGTYPE5 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME, \
					static_cast<void(CLASS::*)()>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1, \
					static_cast<void(CLASS::*)(const ARGTYPE1 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2, \
					static_cast<void(CLASS::*)(const ARGTYPE1 &, const ARGTYPE2 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3, \
					static_cast<void(CLASS::*)(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4, \
					static_cast<void(CLASS::*)(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &, const ARGTYPE4 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
            proxyList.proxies.push_back(dispatcher.registerProxy( \
				nullptr, instance, "/" #CLASS "/" #NAME "@" #ARGTYPE1 "@" #ARGTYPE2 "@" #ARGTYPE3 "@" #ARGTYPE4 "@" #ARGTYPE5, \
					static_cast<void(CLASS::*)(const ARGTYPE1 &, const ARGTYPE2 &, const ARGTYPE3 &, const ARGTYPE4 &, const ARGTYPE5 &)>(& CLASS :: NAME), static_cast<CLASS *>(this)));

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS)

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
            dispatcher.subscribeNotificationDelegate(NAME, instance, _remoting_name, "/" #CLASS "/" #NAME);

#define REMOTINGTS1_END_EVENTS

#define REMOTINGTS1_END_INTERFACE \
    } }

#define REMOTINGTS1_BEGIN_STRUCT(NAME) \
    NAME::NAME() : ::RemotingTs1::Formatter::Struct(sizeof(*this)) { } \
    NAME::NAME(size_t size) : ::RemotingTs1::Formatter::Struct(size) { } \
	void NAME::_remoting_serializer(::RemotingTs1::Formatter::Serializer *s, ::RemotingTs1::Formatter::Deserializer *d) {

#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
    NAME::NAME() : FROM(sizeof(*this)) { } \
    NAME::NAME(size_t size) : FROM(size) { } \
	void NAME::_remoting_serializer(::RemotingTs1::Formatter::Serializer *s, ::RemotingTs1::Formatter::Deserializer *d) { \
		if (s) *s << static_cast<const FROM &>(*this); if (d) *d >> static_cast<FROM &>(*this); \

#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) \
		if (s) *s << NAME; if (d) *d >> NAME;

#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
        for (unsigned i=0; i<SIZE; i++) { \
			if (s) *s << NAME[i]; if (d) *d >> NAME[i]; \
		}

#define REMOTINGTS1_END_STRUCT \
    }

# endif
#else // __INTELLISENSE__
///////////////////////////////////////////////////////////////////////////////
// These definitions are here to make intellisense work properly.            //
///////////////////////////////////////////////////////////////////////////////
# if defined(REMOTINGTS1_DECLARE_INTERFACE)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define REMOTINGTS1_BEGIN_INTERFACE(NAME) REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) class NAME { public: NAME(); virtual ~NAME();
#  define REMOTINGTS1_BEGIN_METHODS(CLASS)
#  define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) virtual RETURNTYPE NAME() = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) virtual RETURNTYPE NAME(ARGTYPE1 &arg1) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) virtual void NAME() = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) virtual void NAME(const ARGTYPE1 &arg1) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) = 0;
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) = 0;
#  define REMOTINGTS1_END_METHODS
#  define REMOTINGTS1_BEGIN_EVENTS(CLASS)
#  define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) ::RemotingTs1::Event0 NAME;
#  define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) ::RemotingTs1::Event1<ARGTYPE1> NAME;
#  define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) ::RemotingTs1::Event2<ARGTYPE1, ARGTYPE2> NAME;
#  define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) ::RemotingTs1::Event3<ARGTYPE1, ARGTYPE2, ARGTYPE3> NAME;
#  define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) ::RemotingTs1::Event4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> NAME;
#  define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) ::RemotingTs1::Event5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> NAME;
#  define REMOTINGTS1_END_EVENTS
#  define REMOTINGTS1_END_INTERFACE };
#  define REMOTINGTS1_BEGIN_STRUCT(NAME) struct NAME {
#  define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) struct NAME : public FROM {
#  define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) TYPE NAME;
#  define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) TYPE NAME[SIZE];
#  define REMOTINGTS1_END_STRUCT };
# elif defined(REMOTINGTS1_DEFINE_INTERFACE)
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
