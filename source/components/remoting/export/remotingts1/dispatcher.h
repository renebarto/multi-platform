/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_DISPATCHER_H
#define REMOTINGTS1_DISPATCHER_H

#include "event.h"
#include "functionproxy.h"
#include "remoting1/bulkchannel.h"

#include <cstdint>
#include <exception>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#include <list>
#include <string>

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace std { class mutex; class condition_variable; }

namespace RemotingTs1 {

#ifdef _MSC_VER
# pragma warning (disable : 4191) // Suppress warning from Microsoft headers
#endif

class Dispatcher;

class CommunicationException : public std::exception
{
public:
    CommunicationException(const std::string &text) noexcept;
    ~CommunicationException() noexcept;

    virtual const char* what() const noexcept override final;

private:
    std::string text;
};

template <class _type>
class IMultitonFactory
{
friend class Dispatcher;
public:
    inline IMultitonFactory() : dispatcher(nullptr) { }
    virtual ~IMultitonFactory();

    virtual void create(_type *&) = 0;
    virtual void destroy(_type *) = 0;

private:
    Dispatcher *dispatcher;
};

template <>
class IMultitonFactory<void>
{
friend class Dispatcher;
private:
    inline IMultitonFactory() : dispatcher(nullptr) { }
    inline virtual ~IMultitonFactory() { }

public:
    virtual void create(void *&) = 0;
    virtual void destroy(void *) = 0;

private:
    Dispatcher *dispatcher;
};

#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense
class REMOTING_EXPORT Dispatcher
{
protected:
    typedef void(Dispatcher::* UnregisterFunc)(void *);
    typedef bool(Dispatcher::* CreateFunc)(IMultitonFactory<void> *, const std::string &, void *&);
    typedef void(Dispatcher::* DestructFunc)(IMultitonFactory<void> *, void *);

private:
    struct Factory
    {
        inline Factory() 
            : factory(nullptr), createFunc(nullptr), destructFunc(nullptr) 
        { 
        }

        inline Factory(IMultitonFactory<void> *factory, CreateFunc createFunc, DestructFunc destructFunc)
            : factory(factory), createFunc(createFunc), destructFunc(destructFunc)
        {
        }

        IMultitonFactory<void> *factory;
        CreateFunc createFunc;
        DestructFunc destructFunc;
    };

    struct Instance : Factory
    {
        inline Instance() 
            : Factory(), object(nullptr) 
        { 
        }

        inline Instance(const Factory &creator, const std::string &name, const std::string &hostId, void *object)
            : Factory(creator), object(object)
        {
			hostIds[name].insert(hostId);
        }

		std::map<std::string, std::set<std::string>> hostIds;
        void *object;
    };

	struct Dummy { };

public:
    typedef void(Dummy::* ProxyId)();
	struct ProxyList { std::string instance; std::vector<ProxyId> proxies; };

public:
    Dispatcher(void);
    virtual ~Dispatcher();

    /*! Registers the singleton interface.
     */
    template <class _type>
    inline void registerInterface(const std::string &instance)
    {
        registerInterface(_type::_remoting_name, instance, nullptr, nullptr, nullptr);
    }

    /*! Registers the multiton interface.
     */
    template <class _type>
    inline void registerInterface(const std::string &instance, IMultitonFactory<_type> *multitonFactory)
    {
        multitonFactory->dispatcher = this;

        registerInterface(
            _type::_remoting_name, instance,
            reinterpret_cast<IMultitonFactory<void> *>(multitonFactory), 
            reinterpret_cast<CreateFunc>(&Dispatcher::createObjectFunc<_type>), 
            reinterpret_cast<DestructFunc>(&Dispatcher::destructObjectFunc<_type>));
    }

    /*! Unregisters all events for the specified instance.
     */
    template <class _interface>
    inline void unregisterInterface(const std::string &instance)
    {
        unregisterInterface(instance, _interface::_remoting_name);
    }

    /*! Registers the interface object, it will be notified when this object is
        destroyed.
     */
    template <class _type>
    inline void registerObject(_type *object)
    {
        void(Dispatcher::* const unregisterFunc)(_type *) = &Dispatcher::unregisterObjectFunc<_type>;

        registerObject(object, reinterpret_cast<UnregisterFunc>(unregisterFunc));
    }

    /*! Unregisters the interface object.
     */
    void unregisterObject(void *object);

    /*! Unregisters all specified proxies.
     */
    void unregisterProxies(const std::string *hostId, const std::string &instance, const std::vector<ProxyId> &);

    /*! Returns true if the specified type is registred.
     */
    bool providesType(const std::string &name);

    /*! Returns an instance ID for the specified type.
     */
    std::string createInstance(const std::string &name, const std::string &instance);

    /*! Destroys an instance.
     */
    void destroyInstance(const std::string &instance);

	template <class _object, typename _ret>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(), _object *);

	template <class _object, typename _ret, typename _prm1>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(_prm1 &), _object *);

	template <class _object, typename _ret, typename _prm1, typename _prm2>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(_prm1 &, _prm2 &), _object *);

	template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(_prm1 &, _prm2 &, _prm3 &), _object *);

	template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *);

	template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    ProxyId registerProxy(const std::string *, const std::string &, const char *, _ret(_object::*)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *);

    EventBase::Subscription subscribeNotificationDelegate(Event<void()> &, const std::string &, const char *, const char *);

    template <typename _prm1>
    EventBase::Subscription subscribeNotificationDelegate(Event<void(_prm1 &)> &, const std::string &, const char *, const char *);

    template <typename _prm1, typename _prm2>
    EventBase::Subscription subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &)> &, const std::string &, const char *, const char *);

    template <typename _prm1, typename _prm2, typename _prm3>
    EventBase::Subscription subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &)> &, const std::string &, const char *, const char *);

    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    EventBase::Subscription subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &, _prm4 &)> &, const std::string &, const char *, const char *);

    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    EventBase::Subscription subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &)> &, const std::string &, const char *, const char *);

    template <typename _ret>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path);

    template <typename _ret, typename _prm1>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1);

    template <typename _ret, typename _prm1, typename _prm2>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2);

    template <typename _ret, typename _prm1, typename _prm2, typename _prm3>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3);

    template <typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4);

    template <typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    _ret invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4, _prm5 &prm5);

    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path);

    template <typename _prm1>
    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1);

    template <typename _prm1, typename _prm2>
    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2);

    template <typename _prm1, typename _prm2, typename _prm3>
    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3);

    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4);

    template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    void invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5);

	virtual void sendDestroyInstance(const std::string &hostId, const std::string &instance) = 0;

	/*! Executes the specified function for all items. If one of the functions
		enters a blocking wait for a response from an REMOTING client or server it
		will execute the next one during the wait.
	 */
	template <typename _iterator, typename _function>
	static void parallelForEach(_iterator first, _iterator last, const _function &func);

	/*! Executes the specified functions. If one of the functions enters a
        blocking wait for a response from an REMOTING client or server it will
        execute the next one during the wait.
	 */
	static void parallelInvoke(std::function<void()> task1, std::function<void()> task2);

protected:
	virtual Remoting1::BulkChannel &bulkChannel() = 0;
    virtual std::string clientHostId() = 0;

	std::vector<std::string> hostIdsForInstance(const std::string &, const char *name);

	void handleDisconnect(const std::string &hostId);

	void destroyInstances(const std::string &hostId);
	void destroyInstances();

    void handleMessage(const Message &in, Message &out);
	virtual void sendRequest(const std::string &hostId, const std::string &instance, const char *path, Message &in, Message &out) = 0;
    virtual void sendRequestAsync(const std::string &hostId, const std::string &instance, const char *path, Message &in) = 0;
    virtual void sendNotification(const std::vector<std::string> &hostIds, const std::vector<Remoting1::BulkChannel::BulkId> &, const std::string &instance, const char *path, Message &in) = 0;

    virtual void registerInterface(const char *name, const std::string &instance, IMultitonFactory<void> *, CreateFunc, DestructFunc);
    virtual void unregisterInterface(const std::string &instance, const char *name);

    void registerObject(void *, UnregisterFunc);

    typedef void(Dummy::* ProxyFuncPtr)();
    typedef void(* ProxyFunc)(Remoting1::BulkChannel &, const Message &, Message &, ProxyFuncPtr, void *);
    ProxyId registerProxy(const std::string *, const std::string &, const char *, ProxyFunc, ProxyFuncPtr, void *);

    template <class _type>
    inline void unregisterObjectFunc(_type *object)
    {
        object->_remoting_register(*this, empty);
    }

    template <class _type>
    inline bool createObjectFunc(IMultitonFactory<_type> *factory, const std::string &instance, _type *&object)
    {
        factory->create(object);
        if (object)
        {
            object->_remoting_register(*this, instance);
            return true;
        }

        return false;
    }

    template <class _type>
    inline void destructObjectFunc(IMultitonFactory<_type> *factory, _type *object)
    {
        factory->destroy(object);
    }

    template <typename _prm>
    inline static void parameter(_prm &prm, Formatter::Serializer &serializer)
    {
		serializer << prm;
    }

    template <typename _prm>
	inline static void parameter(const _prm &prm, Formatter::Serializer &serializer)
    {
		serializer << prm;
    }
        
    template <typename _prm>
	inline static void parameter(_prm &prm, Formatter::Deserializer &deserializer)
    {
		deserializer >> prm;
    }
            
    template <typename _prm>
    inline static void parameter(const _prm &, Formatter::Deserializer &)
    {
        // Const parameters do not have to be deserialized again.
    }

    template <typename _ret>
    inline static _ret returnValue(Formatter::Deserializer &deserializer)
    {
		_ret ret;
		deserializer >> ret;
		return ret;
    }
        
	static void execAsync(std::list<std::function<void()>> &);
    static void (* volatile waitForResponse)(bool &, std::mutex &, std::condition_variable &);

private:
    Dispatcher(const Dispatcher &);
    Dispatcher & operator=(const Dispatcher &);

public:
	static const std::string empty;
	static const std::string singleton_instance;

private:
    struct Private;
    Private * const p;
};

template <class _object, typename _ret>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy0<_object>),
        ProxyFuncPtr(fptr), obj);
}

template <class _object, typename _ret, typename _prm1>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(_prm1 &), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy1<_object, _prm1>),
		ProxyFuncPtr(fptr), obj);
}

template <class _object, typename _ret, typename _prm1, typename _prm2>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(_prm1 &, _prm2 &), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy2<_object, _prm1, _prm2>),
		ProxyFuncPtr(fptr), obj);
}

template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy3<_object, _prm1, _prm2, _prm3>),
		ProxyFuncPtr(fptr), obj);
}

template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy4<_object, _prm1, _prm2, _prm3, _prm4>),
		ProxyFuncPtr(fptr), obj);
}

template <class _object, typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
inline Dispatcher::ProxyId Dispatcher::registerProxy(
	const std::string *clientHostId, const std::string &instance, const char *path, 
	_ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *obj)
{
	return registerProxy(
		clientHostId, instance, path,
        ProxyFunc(&::RemotingTs1::FunctionProxy<_ret>::template proxy5<_object, _prm1, _prm2, _prm3, _prm4, _prm5>),
		ProxyFuncPtr(fptr), obj);
}

template <typename _prm1>
EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void(_prm1 &)> &event, const std::string &instance, const char *name, const char *path)
{
	return event.subscribe([this, instance, name, path](_prm1 &prm1)
	{
		Message out;
		Formatter::Serializer serializer(out.content(), &bulkChannel());
		serializer << prm1;

		sendNotification(hostIdsForInstance(instance, name), serializer.bulkIds(), instance, path, out);
	});
}

template <typename _prm1, typename _prm2>
EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &)> &event, const std::string &instance, const char *name, const char *path)
{
	return event.subscribe([this, instance, name, path](_prm1 &prm1, _prm2 &prm2)
	{
		Message out;
		Formatter::Serializer serializer(out.content(), &bulkChannel());
		serializer << prm1 << prm2;

		sendNotification(hostIdsForInstance(instance, name), serializer.bulkIds(), instance, path, out);
	});
}

template <typename _prm1, typename _prm2, typename _prm3>
EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &)> &event, const std::string &instance, const char *name, const char *path)
{
	return event.subscribe([this, instance, name, path](_prm1 &prm1, _prm2 &prm2, _prm3 &prm3)
	{
		Message out;
		Formatter::Serializer serializer(out.content(), &bulkChannel());
		serializer << prm1 << prm2 << prm3;

		sendNotification(hostIdsForInstance(instance, name), serializer.bulkIds(), instance, path, out);
	});
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &, _prm4 &)> &event, const std::string &instance, const char *name, const char *path)
{
	return event.subscribe([this, instance, name, path](_prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4)
	{
		Message out;
		Formatter::Serializer serializer(out.content(), &bulkChannel());
		serializer << prm1 << prm2 << prm3 << prm4;

		sendNotification(hostIdsForInstance(instance, name), serializer.bulkIds(), instance, path, out);
	});
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
EventBase::Subscription Dispatcher::subscribeNotificationDelegate(Event<void(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &)> &event, const std::string &instance, const char *name, const char *path)
{
	return event.subscribe([this, instance, name, path](_prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4, _prm5 &prm5)
	{
		Message out;
		Formatter::Serializer serializer(out.content(), &bulkChannel());
		serializer << prm1 << prm2 << prm3 << prm4 << prm5;

		sendNotification(hostIdsForInstance(instance, name), serializer.bulkIds(), instance, path, out);
	});
}

template <typename _ret>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	return returnValue<_ret>(deserializer);
}

template <typename _ret, typename _prm1>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	parameter(prm1, deserializer);
	return returnValue<_ret>(deserializer);
}

template <typename _ret, typename _prm1, typename _prm2>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	parameter(prm1, deserializer);
	parameter(prm2, deserializer);
	return returnValue<_ret>(deserializer);
}

template <typename _ret, typename _prm1, typename _prm2, typename _prm3>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	parameter(prm1, deserializer);
	parameter(prm2, deserializer);
	parameter(prm3, deserializer);
	return returnValue<_ret>(deserializer);
}

template <typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);
	parameter(prm4, serializer);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	parameter(prm1, deserializer);
	parameter(prm2, deserializer);
	parameter(prm3, deserializer);
	parameter(prm4, deserializer);
	return returnValue<_ret>(deserializer);
}

template <typename _ret, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
_ret Dispatcher::invoke(const std::string &hostId, const std::string &instance, const char *path, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4, _prm5 &prm5)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in, out;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);
	parameter(prm4, serializer);
	parameter(prm5, serializer);

	sendRequest(hostId, instance, path, in, out);

	Formatter::Deserializer deserializer(out.content(), &bulkChannel);
	parameter(prm1, deserializer);
	parameter(prm2, deserializer);
	parameter(prm3, deserializer);
	parameter(prm4, deserializer);
	parameter(prm5, deserializer);
	return returnValue<_ret>(deserializer);
}

template <typename _prm1>
void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);

	sendRequestAsync(hostId, instance, path, in);
}

template <typename _prm1, typename _prm2>
void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);

	sendRequestAsync(hostId, instance, path, in);
}

template <typename _prm1, typename _prm2, typename _prm3>
void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);

	sendRequestAsync(hostId, instance, path, in);
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);
	parameter(prm4, serializer);

	sendRequestAsync(hostId, instance, path, in);
}

template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
void Dispatcher::invokeAsync(const std::string &hostId, const std::string &instance, const char *path, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5)
{
	Remoting1::BulkChannel &bulkChannel = this->bulkChannel();

	Message in;
	Formatter::Serializer serializer(in.content(), &bulkChannel);
	parameter(prm1, serializer);
	parameter(prm2, serializer);
	parameter(prm3, serializer);
	parameter(prm4, serializer);
	parameter(prm5, serializer);

	sendRequestAsync(hostId, instance, path, in);
}

template <typename _iterator, typename _function>
inline void Dispatcher::parallelForEach(_iterator first, _iterator last, const _function &func)
{
	std::list<std::function<void()>> tasks;
	for (_iterator i=first; i!=last; i++)
    {
        tasks.emplace_back([func, i]
		{
			func(*i);
		});
    }

	execAsync(tasks);
}

template <>
inline void Dispatcher::returnValue<void>(Formatter::Deserializer &)
{
}

template <class _type>
IMultitonFactory<_type>::~IMultitonFactory()
{
    if (dispatcher)
        dispatcher->unregisterInterface<_type>(".");
}

#else // __INTELLISENSE__
class Dispatcher { virtual Remoting1::BulkChannel &bulkChannel() = 0; };
#endif // __INTELLISENSE__

} // End of namespaces

#if defined(_MSC_VER)
# undef noexcept
#endif
#ifdef _MSC_VER
# pragma warning (default : 4191)
#endif

#endif
