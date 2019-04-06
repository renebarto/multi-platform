/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_INTERFACE_BEGIN_H
# error Please #include <sscfts1/interface.begin.h>
#endif

#if !defined(__INTELLISENSE__) || defined(SSCFTS1_BUILD) // Hide internals from IntelliSense
# if defined(SSCFTS1_DECLARE_INTERFACE)

#define in(X) X
#define inout(X) X%
#define reftype(X) X^

#define std
#define string System::String
#define wstring System::String
#define list cli::array
#define set cli::array
#define vector cli::array

#define SSCFTS1_BEGIN_INTERFACE(NAME) \
	[::SscfTs1DotNet::CppInterface(#NAME)] \
    public interface class NAME {

#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) \
	[::SscfTs1DotNet::CppInterface(#NAME, VERSION)] \
    public interface class NAME {

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) \
		[::SscfTs1DotNet::CppMethod(#NAME)] \
        RETURNTYPE NAME();

#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) \
		[::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1)] \
        RETURNTYPE NAME(ARGTYPE1 arg1);

#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);

#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);

#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);

#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4, #ARGTYPE5)] \
        RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME)] \
        void NAME();

#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1)] \
        void NAME(ARGTYPE1 arg1);

#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);

#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);

#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);

#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::SscfTs1DotNet::Async, ::SscfTs1DotNet::CppMethod(#NAME, #ARGTYPE1, #ARGTYPE2, #ARGTYPE3, #ARGTYPE4, #ARGTYPE5)] \
        void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);

#define SSCFTS1_END_METHODS

#define SSCFTS1_BEGIN_EVENTS(CLASS)

#define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
		event System::Action ^NAME;

#define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1> ^NAME;

#define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2> ^NAME;

#define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3> ^NAME;

#define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> ^NAME;

#define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) \
		[::SscfTs1DotNet::CppEvent(#NAME)] \
        event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> ^NAME;

#define SSCFTS1_END_EVENTS

#define SSCFTS1_END_INTERFACE \
    };


#define SSCFTS1_BEGIN_STRUCT(NAME) \
	namespace _sscf { ref class NAME ## _serializer; } \
	[::SscfTs1DotNet::CppSerializer(_sscf:: NAME ## _serializer::typeid)] \
    public ref class NAME { \
    public: \
		NAME() { ::SscfTs1DotNet::AutoSerializer::Initialize(this); }

#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
	namespace _sscf { ref class NAME ## _serializer; } \
	[::SscfTs1DotNet::CppSerializer(_sscf:: NAME ## _serializer::typeid)] \
    public ref class NAME : public FROM { \
    public: \
		NAME() { ::SscfTs1DotNet::AutoSerializer::Initialize(this); } \
		void Assign(FROM ^from) { return ::SscfTs1DotNet::AutoSerializer::Copy(this, from); }

#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME) \
		TYPE NAME;

#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
		array<TYPE> ^ NAME;

#define SSCFTS1_END_STRUCT \
    };

# elif defined(SSCFTS1_DEFINE_INTERFACE)

#define in(X) X
#define inout(X) X%
#define reftype(X) X^

#define std
#define string System::String
#define wstring System::String
#define list cli::array
#define set cli::array
#define vector cli::array

#define SSCFTS1_BEGIN_INTERFACE(NAME)
#define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION)

#define SSCFTS1_BEGIN_METHODS(CLASS)

#define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME)
#define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1)
#define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) 

#define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME)
#define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1)
#define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) 

#define SSCFTS1_END_METHODS

#define SSCFTS1_BEGIN_EVENTS(CLASS)

#define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME)
#define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1)
#define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2)
#define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3)
#define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4)
#define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5)

#define SSCFTS1_END_EVENTS

#define SSCFTS1_END_INTERFACE

#define SSCFTS1_BEGIN_STRUCT(NAME) \
	namespace _sscf { \
		public ref class NAME ## _serializer : public ::SscfTs1DotNet::AutoSerializer { \
		public: \
			NAME ## _serializer() : AutoSerializer(NAME::typeid) {

#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
	namespace _sscf { \
		public ref class NAME ## _serializer : public ::SscfTs1DotNet::AutoSerializer { \
		public: \
			NAME ## _serializer() : AutoSerializer(NAME::typeid) {

#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME) \
				AddField(#NAME);

#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
				AddField(#NAME, SIZE);

#define SSCFTS1_END_STRUCT \
			} \
		}; \
	}

# elif defined(SSCFTS1_DECLARE_NATIVE_STRUCTS)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_INTERFACE

#define SSCFTS1_BEGIN_STRUCT(NAME) \
struct NAME : private ::SscfTs1::Formatter::Struct { \
private: \
    void _sscf_serializer(::SscfTs1::Formatter::Serializer *s, ::SscfTs1::Formatter::Deserializer *d); \
protected: \
    explicit NAME(size_t); \
public: \
    friend ::SscfTs1::Formatter::Serializer & operator<<(::SscfTs1::Formatter::Serializer &s, const NAME &v) { const_cast<NAME &>(v)._sscf_serializer(&s, 0); return s; } \
    friend ::SscfTs1::Formatter::Deserializer & operator>>(::SscfTs1::Formatter::Deserializer &d, NAME &v) { v._sscf_serializer(0, &d); return d; } \
    NAME();

#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
struct NAME : public FROM { \
private: \
    void _sscf_serializer(::SscfTs1::Formatter::Serializer *s, ::SscfTs1::Formatter::Deserializer *d); \
protected: \
    explicit NAME(size_t); \
public: \
    friend ::SscfTs1::Formatter::Serializer & operator<<(::SscfTs1::Formatter::Serializer &s, const NAME &v) { const_cast<NAME &>(v)._sscf_serializer(&s, 0); return s; } \
    friend ::SscfTs1::Formatter::Deserializer & operator>>(::SscfTs1::Formatter::Deserializer &d, NAME &v) { v._sscf_serializer(0, &d); return d; } \
    NAME();

#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME) \
    TYPE NAME;

#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
    TYPE NAME[SIZE];

#define SSCFTS1_END_STRUCT \
};

# elif defined(SSCFTS1_DEFINE_NATIVE_STRUCTS)

#define in(X) const X
#define inout(X) X
#define reftype(X) X

#define SSCFTS1_BEGIN_STRUCT(NAME) \
    NAME::NAME() : ::SscfTs1::Formatter::Struct(sizeof(*this)) { } \
    NAME::NAME(size_t size) : ::SscfTs1::Formatter::Struct(size) { } \
    void NAME::_sscf_serializer(::SscfTs1::Formatter::Serializer *s, ::SscfTs1::Formatter::Deserializer *d) {

#define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) \
    NAME::NAME() : FROM(sizeof(*this)) { } \
    NAME::NAME(size_t size) : FROM(size) { } \
    void NAME::_sscf_serializer(::SscfTs1::Formatter::Serializer *s, ::SscfTs1::Formatter::Deserializer *d) { \
    if (s) *s << static_cast<const FROM &>(*this); if (d) *d >> static_cast<FROM &>(*this); \

#define SSCFTS1_STRUCT_MEMBER(TYPE, NAME) \
    if (s) *s << NAME; if (d) *d >> NAME;

#define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) \
    for (unsigned i=0; i<SIZE; i++) { \
    if (s) *s << NAME[i]; if (d) *d >> NAME[i]; \
    }

#define SSCFTS1_END_STRUCT \
    }

# endif
#else // __INTELLISENSE__
///////////////////////////////////////////////////////////////////////////////
// These definitions are here to make intellisense work properly.            //
///////////////////////////////////////////////////////////////////////////////
# if defined(SSCFTS1_DECLARE_INTERFACE)
#  define Sscf1 Sscf1DotNet
#  define SscfTs1 SscfTs1DotNet
#  define in(X) X
#  define inout(X) X%
#  define reftype(X) X^
#  define std
#  define string System::String
#  define wstring System::String
#  define list cli::array
#  define set cli::array
#  define vector cli::array
#  define SSCFTS1_BEGIN_INTERFACE(NAME) SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, nullptr)
#  define SSCFTS1_BEGIN_INTERFACE_VERSION(NAME, VERSION) public interface class NAME {
#  define SSCFTS1_BEGIN_METHODS(CLASS)
#  define SSCFTS1_INTERFACE_METHOD_0(RETURNTYPE, CLASS, NAME) RETURNTYPE NAME();
#  define SSCFTS1_INTERFACE_METHOD_1(RETURNTYPE, CLASS, NAME, ARGTYPE1) RETURNTYPE NAME(ARGTYPE1 arg1);
#  define SSCFTS1_INTERFACE_METHOD_2(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);
#  define SSCFTS1_INTERFACE_METHOD_3(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);
#  define SSCFTS1_INTERFACE_METHOD_4(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);
#  define SSCFTS1_INTERFACE_METHOD_5(RETURNTYPE, CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) RETURNTYPE NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_0(CLASS, NAME) void NAME();
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_1(CLASS, NAME, ARGTYPE1) void NAME(ARGTYPE1 arg1);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4);
#  define SSCFTS1_INTERFACE_METHOD_ASYNC_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) void NAME(ARGTYPE1 arg1, ARGTYPE2 arg2, ARGTYPE3 arg3, ARGTYPE4 arg4, ARGTYPE5 arg5);
#  define SSCFTS1_END_METHODS
#  define SSCFTS1_BEGIN_EVENTS(CLASS)
#   define SSCFTS1_INTERFACE_EVENT_0(CLASS, NAME) event System::Action ^NAME;
#   define SSCFTS1_INTERFACE_EVENT_1(CLASS, NAME, ARGTYPE1) event System::Action<ARGTYPE1> ^NAME;
#   define SSCFTS1_INTERFACE_EVENT_2(CLASS, NAME, ARGTYPE1, ARGTYPE2) event System::Action<ARGTYPE1, ARGTYPE2> ^NAME;
#   define SSCFTS1_INTERFACE_EVENT_3(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3> ^NAME;
#   define SSCFTS1_INTERFACE_EVENT_4(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4> ^NAME;
#   define SSCFTS1_INTERFACE_EVENT_5(CLASS, NAME, ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5) event System::Action<ARGTYPE1, ARGTYPE2, ARGTYPE3, ARGTYPE4, ARGTYPE5> ^NAME;
#  define SSCFTS1_END_EVENTS
#  define SSCFTS1_END_INTERFACE };
#  define SSCFTS1_BEGIN_STRUCT(NAME) public ref class NAME { public: NAME();
#  define SSCFTS1_BEGIN_EXTEND_STRUCT(NAME, FROM) public ref class NAME : public FROM { public: NAME(); void Assign(FROM ^from);
#  define SSCFTS1_STRUCT_MEMBER(TYPE, NAME) TYPE NAME;
#  define SSCFTS1_STRUCT_MEMBER_ARRAY(TYPE, NAME, SIZE) array<TYPE> ^ NAME;
#  define SSCFTS1_END_STRUCT };
# elif defined(SSCFTS1_DEFINE_INTERFACE)
#  define Sscf1 Sscf1DotNet
#  define SscfTs1 SscfTs1DotNet
#  define in(X) X
#  define inout(X) X%
#  define reftype(X) X^
#  define std
#  define string System::String
#  define wstring System::String
#  define list cli::array
#  define set cli::array
#  define vector cli::array
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
