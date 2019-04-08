/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2014, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_FUNCTIONDISPATCHER_H
#define REMOTINGTS1_FUNCTIONDISPATCHER_H
#if !defined(__INTELLISENSE__) || defined(REMOTINGTS1_BUILD) // Hide internals from IntelliSense

#include "ieventloop.h"

namespace RemotingTs1 {

template <typename _ret>
class FunctionDispatcher
{
public:
	template <class _object>
    inline static _ret invoke0(IEventLoop &eventLoop, _ret(_object::* fptr)(), _object *obj)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret]
		{ 
			ret = (obj->*fptr)();
		});

		return ret;
	}

	template <class _object, typename _prm1>
    inline static _ret invoke1(IEventLoop &eventLoop, _ret(_object::* fptr)(_prm1 &), _object *obj, _prm1 &prm1)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret, &prm1]
		{
			ret = (obj->*fptr)(prm1);
		});

		return ret;
	}

	template <class _object, typename _prm1, typename _prm2>
    inline static _ret invoke2(IEventLoop &eventLoop, _ret(_object::* fptr)(_prm1 &, _prm2 &), _object *obj, _prm1 &prm1, _prm2 &prm2)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret, &prm1, &prm2]
		{
			ret = (obj->*fptr)(prm1, prm2);
		});

		return ret;
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3>
    inline static _ret invoke3(IEventLoop &eventLoop, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret, &prm1, &prm2, &prm3]
		{
			ret = (obj->*fptr)(prm1, prm2, prm3);
		});

		return ret;
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static _ret invoke4(IEventLoop &eventLoop, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret, &prm1, &prm2, &prm3, &prm4]
		{
			ret = (obj->*fptr)(prm1, prm2, prm3, prm4);
		});

		return ret;
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static _ret invoke5(IEventLoop &eventLoop, _ret(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4, _prm5 &prm5)
    {
		_ret ret;
		eventLoop.send([fptr, obj, &ret, &prm1, &prm2, &prm3, &prm4, &prm5]
		{
			ret = (obj->*fptr)(prm1, prm2, prm3, prm4, prm5);
		});

		return ret;
	}
};

template <>
class FunctionDispatcher<void>
{
public:
	template <class _object>
    inline static void invoke0(IEventLoop &eventLoop, void(_object::* fptr)(), _object *obj)
    {
		eventLoop.send([fptr, obj]
		{
			(obj->*fptr)();
		});
	}

	template <class _object, typename _prm1>
    inline static void invoke1(IEventLoop &eventLoop, void(_object::* fptr)(_prm1 &), _object *obj, _prm1 &prm1)
    {
		eventLoop.send([fptr, obj, &prm1]
		{
			(obj->*fptr)(prm1);
		});
	}

	template <class _object, typename _prm1, typename _prm2>
    inline static void invoke2(IEventLoop &eventLoop, void(_object::* fptr)(_prm1 &, _prm2 &), _object *obj, _prm1 &prm1, _prm2 &prm2)
    {
		eventLoop.send([fptr, obj, &prm1, &prm2]
		{
			(obj->*fptr)(prm1, prm2);
		});
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3>
    inline static void invoke3(IEventLoop &eventLoop, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3)
    {
		eventLoop.send([fptr, obj, &prm1, &prm2, &prm3]
		{
			(obj->*fptr)(prm1, prm2, prm3);
		});
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static void invoke4(IEventLoop &eventLoop, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4)
    {
		eventLoop.send([fptr, obj, &prm1, &prm2, &prm3, &prm4]
		{
			(obj->*fptr)(prm1, prm2, prm3, prm4);
		});
	}

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static void invoke5(IEventLoop &eventLoop, void(_object::* fptr)(_prm1 &, _prm2 &, _prm3 &, _prm4 &, _prm5 &), _object *obj, _prm1 &prm1, _prm2 &prm2, _prm3 &prm3, _prm4 &prm4, _prm5 &prm5)
    {
		eventLoop.send([fptr, obj, &prm1, &prm2, &prm3, &prm4, &prm5]
		{
			(obj->*fptr)(prm1, prm2, prm3, prm4, prm5);
		});
	}

	template <class _object>
    inline static void asyncInvoke0(IEventLoop &eventLoop, void(_object::* fptr)(), _object *obj)
    {
        if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj]
            {
                (obj->*fptr)();
            });
        }
        else
        {
            eventLoop.post([fptr, obj]
            {
                (obj->*fptr)();
            });
        }
    }

	template <class _object, typename _prm1>
    inline static void asyncInvoke1(IEventLoop &eventLoop, void(_object::* fptr)(const _prm1 &), _object *obj, const _prm1 &prm1)
    {
        if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj, &prm1]
            {
                (obj->*fptr)(prm1);
            });
        }
        else
        {
            eventLoop.post([fptr, obj, prm1]
            {
                (obj->*fptr)(prm1);
            });
        }
    }

	template <class _object, typename _prm1, typename _prm2>
    inline static void asyncInvoke2(IEventLoop &eventLoop, void(_object::* fptr)(const _prm1 &, const _prm2 &), _object *obj, const _prm1 &prm1, const _prm2 &prm2)
    {
        if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj, &prm1, &prm2]
            {
                (obj->*fptr)(prm1, prm2);
            });
        }
        else
        {
            eventLoop.post([fptr, obj, prm1, prm2]
            {
                (obj->*fptr)(prm1, prm2);
            });
        }
    }

	template <class _object, typename _prm1, typename _prm2, typename _prm3>
    inline static void asyncInvoke3(IEventLoop &eventLoop, void(_object::* fptr)(const _prm1 &, const _prm2 &, const _prm3 &), _object *obj, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3)
    {
        if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj, &prm1, &prm2, &prm3]
            {
                (obj->*fptr)(prm1, prm2, prm3);
            });
        }
        else
        {
            eventLoop.post([fptr, obj, prm1, prm2, prm3]
            {
                (obj->*fptr)(prm1, prm2, prm3);
            });
        }
    }

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static void asyncInvoke4(IEventLoop &eventLoop, void(_object::* fptr)(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &), _object *obj, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4)
    {
        if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj, &prm1, &prm2, &prm3, &prm4]
            {
                (obj->*fptr)(prm1, prm2, prm3, prm4);
            });
        }
        else
        {
            eventLoop.post([fptr, obj, prm1, prm2, prm3, prm4]
            {
                (obj->*fptr)(prm1, prm2, prm3, prm4);
            });
        }
    }

	template <class _object, typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static void asyncInvoke5(IEventLoop &eventLoop, void(_object::* fptr)(const _prm1 &, const _prm2 &, const _prm3 &, const _prm4 &, const _prm5 &), _object *obj, const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5)
    {
		if (eventLoop.checkThread())
        {
            eventLoop.send([fptr, obj, &prm1, &prm2, &prm3, &prm4, &prm5]
            {
                (obj->*fptr)(prm1, prm2, prm3, prm4, prm5);
            });
        }
		else
        {
            eventLoop.post([fptr, obj, prm1, prm2, prm3, prm4, prm5]
            {
                (obj->*fptr)(prm1, prm2, prm3, prm4, prm5);
            });
        }
	}

    inline static void subscribe0(IEventLoop &eventLoop, EventHandler &eventHandler, Event0 &source, Event0 &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest]
		{
			eventLoop.post([&dest]()
			{ 
				dest(); 
			});
		});
	}

	template <typename _prm1>
    inline static void subscribe1(IEventLoop &eventLoop, EventHandler &eventHandler, Event1<_prm1> &source, Event1<_prm1> &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest](const _prm1 &prm1)
		{
			eventLoop.post([&dest, prm1]
			{
				dest(prm1);
			});
		});
	}

	template <typename _prm1, typename _prm2>
    inline static void subscribe2(IEventLoop &eventLoop, EventHandler &eventHandler, Event2<_prm1, _prm2> &source, Event2<_prm1, _prm2> &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest](const _prm1 &prm1, const _prm2 &prm2)
		{
			eventLoop.post([&dest, prm1, prm2]
			{
				dest(prm1, prm2);
			});
		});
	}

	template <typename _prm1, typename _prm2, typename _prm3>
    inline static void subscribe3(IEventLoop &eventLoop, EventHandler &eventHandler, Event3<_prm1, _prm2, _prm3> &source, Event3<_prm1, _prm2, _prm3> &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest](const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3)
		{
			eventLoop.post([&dest, prm1, prm2, prm3]
			{
				dest(prm1, prm2, prm3);
			});
		});
	}

	template <typename _prm1, typename _prm2, typename _prm3, typename _prm4>
    inline static void subscribe4(IEventLoop &eventLoop, EventHandler &eventHandler, Event4<_prm1, _prm2, _prm3, _prm4> &source, Event4<_prm1, _prm2, _prm3, _prm4> &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest](const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4)
		{
			eventLoop.post([&dest, prm1, prm2, prm3, prm4]
			{
				dest(prm1, prm2, prm3, prm4);
			});
		});
	}

	template <typename _prm1, typename _prm2, typename _prm3, typename _prm4, typename _prm5>
    inline static void subscribe5(IEventLoop &eventLoop, EventHandler &eventHandler, Event5<_prm1, _prm2, _prm3, _prm4, _prm5> &source, Event5<_prm1, _prm2, _prm3, _prm4, _prm5> &dest)
    {
		eventHandler.subscribe(source, [&eventLoop, &dest](const _prm1 &prm1, const _prm2 &prm2, const _prm3 &prm3, const _prm4 &prm4, const _prm5 &prm5)
		{
			eventLoop.post([&dest, prm1, prm2, prm3, prm4, prm5]
			{
				dest(prm1, prm2, prm3, prm4, prm5);
			});
		});
	}
};

} // End of namespaces

#endif // __INTELLISENSE__
#endif
