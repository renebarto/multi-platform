/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_INTERFACE_BEGIN_H
# error Please #include <remotingts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
# if defined(REMOTINGTS1_DECLARE_DISPATCHER)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
	REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    class NAME ## Dispatcher : public NAME { \
    friend class ::RemotingTs1::Dispatcher; \
    friend class ::RemotingTs1::ClientNode; \
    friend class ::RemotingTs1::ServerNode; \
        NAME ## Dispatcher(const NAME ## Dispatcher &); \
        NAME ## Dispatcher & operator=(const NAME ## Dispatcher &); \
		void _remoting_subscribeEvents(); \
		void _remoting_unsubscribeEvents(); \
        NAME &_remoting_target; \
		::RemotingTs1::IEventLoop &_remoting_targetEventLoop, &_remoting_sourceEventLoop; \
		std::unique_ptr<::RemotingTs1::EventHandler> _remoting_eventHandler; \
    public: \
        NAME ## Dispatcher(::RemotingTs1::IEventLoop &sourceEventLoop, NAME &target, ::RemotingTs1::IEventLoop &targetEventLoop); \
        virtual ~NAME ## Dispatcher();

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
        virtual RETURNTYPE NAME() override;

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1) override;

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) override;

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) override;

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) override;

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
        virtual void NAME() override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
        virtual void NAME(const ARGTYPE1 &arg1) override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) override;

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) override;

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

# elif defined(REMOTINGTS1_DEFINE_DISPATCHER)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
	REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    NAME ## Dispatcher :: NAME ## Dispatcher(::RemotingTs1::IEventLoop &sourceEventLoop, NAME &target, ::RemotingTs1::IEventLoop &targetEventLoop) \
    : _remoting_target(target), _remoting_targetEventLoop(targetEventLoop), _remoting_sourceEventLoop(sourceEventLoop), _remoting_eventHandler() { \
		_remoting_targetEventLoop.send(std::bind(& NAME ## Dispatcher :: _remoting_subscribeEvents, this)); } \
    NAME ## Dispatcher :: ~NAME ## Dispatcher() { \
		_remoting_targetEventLoop.send(std::bind(& NAME ## Dispatcher :: _remoting_unsubscribeEvents, this)); }

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
	RETURNTYPE CLASS ## Dispatcher :: NAME() { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke0<CLASS>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target); }

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1) { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke1<CLASS, ARGTYPE1>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1); }

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke2<CLASS, ARGTYPE1, ARGTYPE2>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2); }

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke3<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3); }

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke4<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3, arg4); }

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) { \
		return ::RemotingTs1::FunctionDispatcher<RETURNTYPE>::invoke5<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3, arg4, arg5); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
    void CLASS ## Dispatcher :: NAME() { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke0<CLASS>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1) { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke1<CLASS, ARGTYPE1>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke2<CLASS, ARGTYPE1, ARGTYPE2>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke3<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke4<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3, arg4); }

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) { \
		return ::RemotingTs1::FunctionDispatcher<void>::asyncInvoke5<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_remoting_targetEventLoop, & CLASS :: NAME, &_remoting_target, arg1, arg2, arg3, arg4, arg5); }

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS) \
    void CLASS ## Dispatcher :: _remoting_unsubscribeEvents() { \
		_remoting_eventHandler = nullptr;	\
	} \
    void CLASS ## Dispatcher :: _remoting_subscribeEvents() { \
		_remoting_eventHandler.reset(new ::RemotingTs1::EventHandler());

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe0(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe1<ARGTYPE1>(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe2<ARGTYPE1, ARGTYPE2>(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe3<ARGTYPE1, ARGTYPE2, ARGTYPE3>(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		::RemotingTs1::FunctionDispatcher<void>::subscribe5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_remoting_sourceEventLoop, *_remoting_eventHandler, _remoting_target.NAME, NAME);

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
# if defined(REMOTINGTS1_DECLARE_DISPATCHER)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define REMOTINGTS1_BEGIN_INTERFACE(NAME) REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) class NAME ## Dispatcher : public NAME { public: explicit NAME ## Dispatcher(::RemotingTs1::IEventLoop &sourceEventLoop, NAME &target, ::RemotingTs1::IEventLoop &targetEventLoop);
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
# elif defined(REMOTINGTS1_DEFINE_DISPATCHER)
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
