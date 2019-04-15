/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_INTERFACE_BEGIN_H
# error Please #include <remotingts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
# if defined(REMOTINGTS1_DECLARE_INTERFACE)

#define in(X) X
#define inout(X) X%
#define reftype(X) X^

#define std
#define string System::String
#define wstring System::String
#define list cli::array
#define set cli::array
#define vector cli::array

#define REMOTINGTS1_BEGIN_INTERFACE(NAME) \
	[::RemotingTs1DotNet::CppInterface(#NAME)] \
    public interface class NAME {

#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
	[::RemotingTs1DotNet::CppInterface(#NAME, VERSION)] \
    public interface class NAME {

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
		[::RemotingTs1DotNet::CppMethod(#NAME)] \
        RETURNTYPE NAME();

#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
		[::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1)] \
        RETURNTYPE NAME(ARGTYPE1 arg1);

#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);

#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);

#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);

#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4, #ARGTYPE5)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME)] \
        void NAME();

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1)] \
        void NAME(ARGTYPE1 arg1);

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::RemotingTs1DotNet::Async, ::RemotingTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4, #ARGTYPE5)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS)

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
		event System::Action ^NAME;

#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1> ^NAME;

#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2> ^NAME;

#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3> ^NAME;

#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> ^NAME;

#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::RemotingTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> ^NAME;

#define REMOTINGTS1_END_EVENTS

#define REMOTINGTS1_END_INTERFACE \
    };


#define REMOTINGTS1_BEGIN_STRUCT(NAME) \
	namespace _remoting { ref class NAME ## _serializer; } \
	[::RemotingTs1DotNet::CppSerializer(_remoting:: NAME ## _serializer::typeid)] \
    public ref class NAME { \
    public: \
		NAME() { ::RemotingTs1DotNet::AutoSerializer::Initialize(this); }

#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
	namespace _remoting { ref class NAME ## _serializer; } \
	[::RemotingTs1DotNet::CppSerializer(_remoting:: NAME ## _serializer::typeid)] \
    public ref class NAME : public FROM { \
    public: \
		NAME() { ::RemotingTs1DotNet::AutoSerializer::Initialize(this); } \
		void Assign(FROM ^from) { return ::RemotingTs1DotNet::AutoSerializer::Copy(this, from); }

#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) \
		TYPE NAME;

#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
		array<TYPE> ^ NAME;

#define REMOTINGTS1_END_STRUCT \
    };

# elif defined(REMOTINGTS1_DEFINE_INTERFACE)

#define in(X) X
#define inout(X) X%
#define reftype(X) X^

#define std
#define string System::String
#define wstring System::String
#define list cli::array
#define set cli::array
#define vector cli::array

#define REMOTINGTS1_BEGIN_INTERFACE(NAME)
#define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION)

#define REMOTINGTS1_BEGIN_METHODS(CLASS)

#define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME)
#define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1)
#define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME)
#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1)
#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define REMOTINGTS1_END_METHODS

#define REMOTINGTS1_BEGIN_EVENTS(CLASS)

#define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME)
#define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define REMOTINGTS1_END_EVENTS

#define REMOTINGTS1_END_INTERFACE

#define REMOTINGTS1_BEGIN_STRUCT(NAME) \
	namespace _remoting { \
		public ref class NAME ## _serializer : public ::RemotingTs1DotNet::AutoSerializer { \
		public: \
			NAME ## _serializer() : AutoSerializer(NAME::typeid) {

#define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
	namespace _remoting { \
		public ref class NAME ## _serializer : public ::RemotingTs1DotNet::AutoSerializer { \
		public: \
			NAME ## _serializer() : AutoSerializer(NAME::typeid) {

#define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) \
				AddField(#NAME);

#define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
				AddField(#NAME, SIZE);

#define REMOTINGTS1_END_STRUCT \
			} \
		}; \
	}

# elif defined(REMOTINGTS1_DECLARE_NATIVE_STRUCTS)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define REMOTINGTS1_BEGIN_INTERFACE

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

# elif defined(REMOTINGTS1_DEFINE_NATIVE_STRUCTS)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

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
#  define Remoting1 Remoting1DotNet
#  define RemotingTs1 RemotingTs1DotNet
#  define in(X) X
#  define inout(X) X%
#  define reftype(X) X^
#  define std
#  define string System::String
#  define wstring System::String
#  define list cli::array
#  define set cli::array
#  define vector cli::array
#  define REMOTINGTS1_BEGIN_INTERFACE(NAME) REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define REMOTINGTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) public interface class NAME {
#  define REMOTINGTS1_BEGIN_METHODS(CLASS)
#  define REMOTINGTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) RETURNTYPE NAME();
#  define REMOTINGTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) RETURNTYPE NAME(ARGTYPE1 arg1);
#  define REMOTINGTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);
#  define REMOTINGTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);
#  define REMOTINGTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);
#  define REMOTINGTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) void NAME();
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) void NAME(ARGTYPE1 arg1);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);
#  define REMOTINGTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);
#  define REMOTINGTS1_END_METHODS
#  define REMOTINGTS1_BEGIN_EVENTS(CLASS)
#   define REMOTINGTS1_INTERFACE_EVENT_0(CLASS, NAME) event System::Action ^NAME;
#   define REMOTINGTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) event System::Action<ARGTYPE1> ^NAME;
#   define REMOTINGTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) event System::Action<ARGTYPE1, ARGTYPE2> ^NAME;
#   define REMOTINGTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3> ^NAME;
#   define REMOTINGTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> ^NAME;
#   define REMOTINGTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> ^NAME;
#  define REMOTINGTS1_END_EVENTS
#  define REMOTINGTS1_END_INTERFACE };
#  define REMOTINGTS1_BEGIN_STRUCT(NAME) public ref class NAME { public: NAME();
#  define REMOTINGTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) public ref class NAME : public FROM { public: NAME(); void Assign(FROM ^from);
#  define REMOTINGTS1_STRUCT_MEMBER(TYPE, NAME) TYPE NAME;
#  define REMOTINGTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) array<TYPE> ^ NAME;
#  define REMOTINGTS1_END_STRUCT };
# elif defined(REMOTINGTS1_DEFINE_INTERFACE)
#  define Remoting1 Remoting1DotNet
#  define RemotingTs1 RemotingTs1DotNet
#  define in(X) X
#  define inout(X) X%
#  define reftype(X) X^
#  define std
#  define string System::String
#  define wstring System::String
#  define list cli::array
#  define set cli::array
#  define vector cli::array
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
