print "starting wassup.py"

import sys

#try:
from ugdk_math import Vector2D
#except:
#    exc = sys.exc_info()
#    import traceback
#    tb = traceback.format_exception(exc[0], exc[1], exc[2])
#    for s in tb:    print s

print "[Python] imported Vector2D"

vecx = Vector2D(4.2, 7.2)

print "[Python] Created 'vecx' instance"

def supimpa(p):
    print "supimpa"
    print "[Python] Supimpa = (%s, %s)[%s]" % (p.get_x(), p.get_y(), p)

print "Finished wassup - defined supimpa"
