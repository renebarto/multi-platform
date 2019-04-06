/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_INTERFACE_BEGIN_H
# error Please #include <sscfts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
# if defined(SSCFTS1_DECLARE_DISPATCHER)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_INTERFACE(NAME) \
	SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    class NAME ## Dispatcher : public NAME { \
    friend class ::SscfTs1::Dispatcher; \
    friend class ::SscfTs1::ClientNode; \
    friend class ::SscfTs1::ServerNode; \
        NAME ## Dispatcher(const NAME ## Dispatcher &); \
        NAME ## Dispatcher & operator=(const NAME ## Dispatcher &); \
		void _sscf_subscribeEvents(); \
		void _sscf_unsubscribeEvents(); \
        NAME &_sscf_target; \
		::SscfTs1::IEventLoop &_sscf_targetEventLoop, &_sscf_sourceEventLoop; \
		std::unique_ptr<::SscfTs1::EventHandler> _sscf_eventHandler; \
    public: \
        NAME ## Dispatcher(::SscfTs1::IEventLoop &sourceEventLoop, NAME &target, ::SscfTs1::IEventLoop &targetEventLoop); \
        virtual ~NAME ## Dispatcher();

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
        virtual RETURNTYPE NAME() override;

#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1) override;

#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) override;

#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) override;

#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) override;

#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual RETURNTYPE NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
        virtual void NAME() override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
        virtual void NAME(const ARGTYPE1 &arg1) override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) override;

#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
        virtual void NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) override;

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

# elif defined(SSCFTS1_DEFINE_DISPATCHER)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_INTERFACE(NAME) \
	SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)

#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
    NAME ## Dispatcher :: NAME ## Dispatcher(::SscfTs1::IEventLoop &sourceEventLoop, NAME &target, ::SscfTs1::IEventLoop &targetEventLoop) \
    : _sscf_target(target), _sscf_targetEventLoop(targetEventLoop), _sscf_sourceEventLoop(sourceEventLoop), _sscf_eventHandler() { \
		_sscf_targetEventLoop.send(std::bind(& NAME ## Dispatcher :: _sscf_subscribeEvents, this)); } \
    NAME ## Dispatcher :: ~NAME ## Dispatcher() { \
		_sscf_targetEventLoop.send(std::bind(& NAME ## Dispatcher :: _sscf_unsubscribeEvents, this)); }

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
	RETURNTYPE CLASS ## Dispatcher :: NAME() { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke0<CLASS>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target); }

#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1) { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke1<CLASS, ARGTYPE1>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1); }

#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2) { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke2<CLASS, ARGTYPE1, ARGTYPE2>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2); }

#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3) { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke3<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3); }

#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4) { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke4<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3, arg4); }

#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    RETURNTYPE CLASS ## Dispatcher :: NAME(ARGTYPE1 &arg1, ARGTYPE2 &arg2, ARGTYPE3 &arg3, ARGTYPE4 &arg4, ARGTYPE5 &arg5) { \
		return ::SscfTs1::FunctionDispatcher<RETURNTYPE>::invoke5<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3, arg4, arg5); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
    void CLASS ## Dispatcher :: NAME() { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke0<CLASS>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1) { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke1<CLASS, ARGTYPE1>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2) { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke2<CLASS, ARGTYPE1, ARGTYPE2>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3) { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke3<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4) { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke4<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3, arg4); }

#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
    void CLASS ## Dispatcher :: NAME(const ARGTYPE1 &arg1, const ARGTYPE2 &arg2, const ARGTYPE3 &arg3, const ARGTYPE4 &arg4, const ARGTYPE5 &arg5) { \
		return ::SscfTs1::FunctionDispatcher<void>::asyncInvoke5<CLASS, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_sscf_targetEventLoop, & CLASS :: NAME, &_sscf_target, arg1, arg2, arg3, arg4, arg5); }

#define SSCFTS1_END_METHODS

#define SSCFTS1_BEGIN_EVENTS(CLASS) \
    void CLASS ## Dispatcher :: _sscf_unsubscribeEvents() { \
		_sscf_eventHandler = nullptr;	\
	} \
    void CLASS ## Dispatcher :: _sscf_subscribeEvents() { \
		_sscf_eventHandler.reset(new ::SscfTs1::EventHandler());

#define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME) \
		::SscfTs1::FunctionDispatcher<void>::subscribe0(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

#define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
		::SscfTs1::FunctionDispatcher<void>::subscribe1<ARGTYPE1>(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

#define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		::SscfTs1::FunctionDispatcher<void>::subscribe2<ARGTYPE1, ARGTYPE2>(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

#define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		::SscfTs1::FunctionDispatcher<void>::subscribe3<ARGTYPE1, ARGTYPE2, ARGTYPE3>(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

#define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		::SscfTs1::FunctionDispatcher<void>::subscribe4<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4>(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

#define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		::SscfTs1::FunctionDispatcher<void>::subscribe5<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5>(_sscf_sourceEventLoop, *_sscf_eventHandler, _sscf_target.NAME, NAME);

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
# if defined(SSCFTS1_DECLARE_DISPATCHER)
#  define in(X) const X
#  define inout(X) X
#  define reftype(X) X
#  define SSCFTS1_BEGIN_INTERFACE(NAME) SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) class NAME ## Dispatcher : public NAME { public: explicit NAME ## Dispatcher(::SscfTs1::IEventLoop &sourceEventLoop, NAME &target, ::SscfTs1::IEventLoop &targetEventLoop);
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
# elif defined(SSCFTS1_DEFINE_DISPATCHER)
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
