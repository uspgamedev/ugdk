# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pyramidworks_collision', [dirname(__file__)])
        except ImportError:
            import _pyramidworks_collision
            return _pyramidworks_collision
        if fp is not None:
            try:
                _mod = imp.load_module('_pyramidworks_collision', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _pyramidworks_collision = swig_import_helper()
    del swig_import_helper
else:
    import _pyramidworks_collision
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class SwigPyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SwigPyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SwigPyIterator, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyramidworks_collision.delete_SwigPyIterator
    __del__ = lambda self : None;
    def value(self): return _pyramidworks_collision.SwigPyIterator_value(self)
    def incr(self, n = 1): return _pyramidworks_collision.SwigPyIterator_incr(self, n)
    def decr(self, n = 1): return _pyramidworks_collision.SwigPyIterator_decr(self, n)
    def distance(self, *args): return _pyramidworks_collision.SwigPyIterator_distance(self, *args)
    def equal(self, *args): return _pyramidworks_collision.SwigPyIterator_equal(self, *args)
    def copy(self): return _pyramidworks_collision.SwigPyIterator_copy(self)
    def next(self): return _pyramidworks_collision.SwigPyIterator_next(self)
    def __next__(self): return _pyramidworks_collision.SwigPyIterator___next__(self)
    def previous(self): return _pyramidworks_collision.SwigPyIterator_previous(self)
    def advance(self, *args): return _pyramidworks_collision.SwigPyIterator_advance(self, *args)
    def __eq__(self, *args): return _pyramidworks_collision.SwigPyIterator___eq__(self, *args)
    def __ne__(self, *args): return _pyramidworks_collision.SwigPyIterator___ne__(self, *args)
    def __iadd__(self, *args): return _pyramidworks_collision.SwigPyIterator___iadd__(self, *args)
    def __isub__(self, *args): return _pyramidworks_collision.SwigPyIterator___isub__(self, *args)
    def __add__(self, *args): return _pyramidworks_collision.SwigPyIterator___add__(self, *args)
    def __sub__(self, *args): return _pyramidworks_collision.SwigPyIterator___sub__(self, *args)
    def __iter__(self): return self
SwigPyIterator_swigregister = _pyramidworks_collision.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)
cvar = _pyramidworks_collision.cvar
LuaLang = cvar.LuaLang
PythonLang = cvar.PythonLang

import ugdk_math
import pyramidworks_geometry
import ugdk_util
import ugdk_action
import ugdk_gdd
import ugdk_graphic
class CollisionInstanceList(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionInstanceList, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionInstanceList, name)
    __repr__ = _swig_repr
    def iterator(self): return _pyramidworks_collision.CollisionInstanceList_iterator(self)
    def __iter__(self): return self.iterator()
    def __nonzero__(self): return _pyramidworks_collision.CollisionInstanceList___nonzero__(self)
    def __bool__(self): return _pyramidworks_collision.CollisionInstanceList___bool__(self)
    def __len__(self): return _pyramidworks_collision.CollisionInstanceList___len__(self)
    def pop(self): return _pyramidworks_collision.CollisionInstanceList_pop(self)
    def __getslice__(self, *args): return _pyramidworks_collision.CollisionInstanceList___getslice__(self, *args)
    def __setslice__(self, *args): return _pyramidworks_collision.CollisionInstanceList___setslice__(self, *args)
    def __delslice__(self, *args): return _pyramidworks_collision.CollisionInstanceList___delslice__(self, *args)
    def __delitem__(self, *args): return _pyramidworks_collision.CollisionInstanceList___delitem__(self, *args)
    def __getitem__(self, *args): return _pyramidworks_collision.CollisionInstanceList___getitem__(self, *args)
    def __setitem__(self, *args): return _pyramidworks_collision.CollisionInstanceList___setitem__(self, *args)
    def append(self, *args): return _pyramidworks_collision.CollisionInstanceList_append(self, *args)
    def empty(self): return _pyramidworks_collision.CollisionInstanceList_empty(self)
    def size(self): return _pyramidworks_collision.CollisionInstanceList_size(self)
    def clear(self): return _pyramidworks_collision.CollisionInstanceList_clear(self)
    def swap(self, *args): return _pyramidworks_collision.CollisionInstanceList_swap(self, *args)
    def get_allocator(self): return _pyramidworks_collision.CollisionInstanceList_get_allocator(self)
    def begin(self): return _pyramidworks_collision.CollisionInstanceList_begin(self)
    def end(self): return _pyramidworks_collision.CollisionInstanceList_end(self)
    def rbegin(self): return _pyramidworks_collision.CollisionInstanceList_rbegin(self)
    def rend(self): return _pyramidworks_collision.CollisionInstanceList_rend(self)
    def pop_back(self): return _pyramidworks_collision.CollisionInstanceList_pop_back(self)
    def erase(self, *args): return _pyramidworks_collision.CollisionInstanceList_erase(self, *args)
    def __init__(self, *args): 
        this = _pyramidworks_collision.new_CollisionInstanceList(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(self, *args): return _pyramidworks_collision.CollisionInstanceList_push_back(self, *args)
    def front(self): return _pyramidworks_collision.CollisionInstanceList_front(self)
    def back(self): return _pyramidworks_collision.CollisionInstanceList_back(self)
    def assign(self, *args): return _pyramidworks_collision.CollisionInstanceList_assign(self, *args)
    def resize(self, *args): return _pyramidworks_collision.CollisionInstanceList_resize(self, *args)
    def insert(self, *args): return _pyramidworks_collision.CollisionInstanceList_insert(self, *args)
    def reserve(self, *args): return _pyramidworks_collision.CollisionInstanceList_reserve(self, *args)
    def capacity(self): return _pyramidworks_collision.CollisionInstanceList_capacity(self)
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionInstanceList
    __del__ = lambda self : None;
CollisionInstanceList_swigregister = _pyramidworks_collision.CollisionInstanceList_swigregister
CollisionInstanceList_swigregister(CollisionInstanceList)

class CollisionInstance(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionInstance, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionInstance, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _pyramidworks_collision.new_CollisionInstance(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["first"] = _pyramidworks_collision.CollisionInstance_first_set
    __swig_getmethods__["first"] = _pyramidworks_collision.CollisionInstance_first_get
    if _newclass:first = _swig_property(_pyramidworks_collision.CollisionInstance_first_get, _pyramidworks_collision.CollisionInstance_first_set)
    __swig_setmethods__["second"] = _pyramidworks_collision.CollisionInstance_second_set
    __swig_getmethods__["second"] = _pyramidworks_collision.CollisionInstance_second_get
    if _newclass:second = _swig_property(_pyramidworks_collision.CollisionInstance_second_get, _pyramidworks_collision.CollisionInstance_second_set)
    def __len__(self): return 2
    def __repr__(self): return str((self.first, self.second))
    def __getitem__(self, index): 
      if not (index % 2): 
        return self.first
      else:
        return self.second
    def __setitem__(self, index, val):
      if not (index % 2): 
        self.first = val
      else:
        self.second = val
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionInstance
    __del__ = lambda self : None;
CollisionInstance_swigregister = _pyramidworks_collision.CollisionInstance_swigregister
CollisionInstance_swigregister(CollisionInstance)

class CollisionLogic(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionLogic, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionLogic, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionLogic
    __del__ = lambda self : None;
    def Handle(self, *args): return _pyramidworks_collision.CollisionLogic_Handle(self, *args)
CollisionLogic_swigregister = _pyramidworks_collision.CollisionLogic_swigregister
CollisionLogic_swigregister(CollisionLogic)

class CollisionClass(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionClass, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionClass, name)
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionClass
    __del__ = lambda self : None;
    def parent(self): return _pyramidworks_collision.CollisionClass_parent(self)
    def set_parent(self, *args): return _pyramidworks_collision.CollisionClass_set_parent(self, *args)
    def FindCollidingObjects(self, *args): return _pyramidworks_collision.CollisionClass_FindCollidingObjects(self, *args)
    def AddObject(self, *args): return _pyramidworks_collision.CollisionClass_AddObject(self, *args)
    def RemoveObject(self, *args): return _pyramidworks_collision.CollisionClass_RemoveObject(self, *args)
    def RefreshObject(self, *args): return _pyramidworks_collision.CollisionClass_RefreshObject(self, *args)
CollisionClass_swigregister = _pyramidworks_collision.CollisionClass_swigregister
CollisionClass_swigregister(CollisionClass)

class CollisionManager(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionManager, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _pyramidworks_collision.new_CollisionManager(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionManager
    __del__ = lambda self : None;
    def Generate(self, *args): return _pyramidworks_collision.CollisionManager_Generate(self, *args)
    def Get(self, *args): return _pyramidworks_collision.CollisionManager_Get(self, *args)
CollisionManager_swigregister = _pyramidworks_collision.CollisionManager_swigregister
CollisionManager_swigregister(CollisionManager)

class CollisionObject(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CollisionObject, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CollisionObject, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _pyramidworks_collision.new_CollisionObject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _pyramidworks_collision.delete_CollisionObject
    __del__ = lambda self : None;
    def SearchCollisions(self, *args): return _pyramidworks_collision.CollisionObject_SearchCollisions(self, *args)
    def IsColliding(self, *args): return _pyramidworks_collision.CollisionObject_IsColliding(self, *args)
    def AddCollisionLogic(self, *args): return _pyramidworks_collision.CollisionObject_AddCollisionLogic(self, *args)
    def InitializeCollisionClass(self, *args): return _pyramidworks_collision.CollisionObject_InitializeCollisionClass(self, *args)
    def StartColliding(self): return _pyramidworks_collision.CollisionObject_StartColliding(self)
    def StopColliding(self): return _pyramidworks_collision.CollisionObject_StopColliding(self)
    def shape(self): return _pyramidworks_collision.CollisionObject_shape(self)
    def set_shape(self, *args): return _pyramidworks_collision.CollisionObject_set_shape(self, *args)
    def absolute_position(self): return _pyramidworks_collision.CollisionObject_absolute_position(self)
    def MoveTo(self, *args): return _pyramidworks_collision.CollisionObject_MoveTo(self, *args)
    def GetBoundingBox(self): return _pyramidworks_collision.CollisionObject_GetBoundingBox(self)
CollisionObject_swigregister = _pyramidworks_collision.CollisionObject_swigregister
CollisionObject_swigregister(CollisionObject)


def RegisterType_CollisionLogic(*args):
  return _pyramidworks_collision.RegisterType_CollisionLogic(*args)
RegisterType_CollisionLogic = _pyramidworks_collision.RegisterType_CollisionLogic

def GetNull_CollisionLogic():
  return _pyramidworks_collision.GetNull_CollisionLogic()
GetNull_CollisionLogic = _pyramidworks_collision.GetNull_CollisionLogic

def RegisterType_CollisionClass(*args):
  return _pyramidworks_collision.RegisterType_CollisionClass(*args)
RegisterType_CollisionClass = _pyramidworks_collision.RegisterType_CollisionClass

def GetNull_CollisionClass():
  return _pyramidworks_collision.GetNull_CollisionClass()
GetNull_CollisionClass = _pyramidworks_collision.GetNull_CollisionClass

def RegisterType_CollisionManager(*args):
  return _pyramidworks_collision.RegisterType_CollisionManager(*args)
RegisterType_CollisionManager = _pyramidworks_collision.RegisterType_CollisionManager

def GetNull_CollisionManager():
  return _pyramidworks_collision.GetNull_CollisionManager()
GetNull_CollisionManager = _pyramidworks_collision.GetNull_CollisionManager

def RegisterType_CollisionObject(*args):
  return _pyramidworks_collision.RegisterType_CollisionObject(*args)
RegisterType_CollisionObject = _pyramidworks_collision.RegisterType_CollisionObject

def GetNull_CollisionObject():
  return _pyramidworks_collision.GetNull_CollisionObject()
GetNull_CollisionObject = _pyramidworks_collision.GetNull_CollisionObject
print "Python pyramidworks_collision confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)

# This file is compatible with both classic and new-style classes.


