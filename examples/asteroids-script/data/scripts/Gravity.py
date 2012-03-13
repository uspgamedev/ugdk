from ugdk.ugdk_math import Vector2D
from BasicEntity import BasicEntity, EntityInterface
from Asteroid import Asteroid
from random import randint, shuffle
from math import pi

# Factor to which multiply the gravity force.
# just a configurational value to fine tune the gravity, in case you want
# it stronger or weaker.
GRAVITY_FACTOR = 50.0


####################################
# this is going to be nice :D
# just like old times.
# back to the roots you know?
# roots and branches...
# Jethro Tull? O_o
class GravityWell (EntityInterface):
    def __init__(self, x, y, planet_radius):
        self.mass = GetMassByRadius(planet_radius)
        r = GetMaxGravForceDist(self.mass)
        EntityInterface.__init__(self, x, y, r)
        self.is_antigrav = False
        self.ignore_ids = []
        
    def AddIDToIgnoreList(self, ID):
        if ID not in self.ignore_ids:
            self.ignore_ids.append(ID)
            
    def RemoveIDFromIgnoreList(self, ID):
        if ID in self.ignore_ids:
            self.ignore_ids.remove(ID)

    def HandleCollision(self, target):
        ignore_types = [self.type, "Planet.Planet", "Shockwave.Shockwave"]
        if target.type in ignore_types or target.id in self.ignore_ids:
            return #we don't affect planets (neither their wells)
        
        grav_vec = self.GetPos() - target.GetPos()
        dist = grav_vec.Length()

        GravForce = GetGravForce(self.mass, dist)

        grav_vec = grav_vec.Normalize()
        if self.is_antigrav:
            grav_vec = grav_vec * -1

        grav_vec = grav_vec * GravForce

        target.ApplyVelocity(grav_vec)
        
        
########
################################################################
#  The Gravity Constant
###
# Constant variable used in various gravity force related calculations
# - Real Physics value - don't change.
################################################################
GRAVITY_CONSTANT = 6.67e-017

################################################################
###    GetGravForce
# Theoretically, plamass is in Kilograms and distance in Kilometers
# then the return is in km/h^2 (wihout the /3.6 -> return em m/s^2)
##
# speaking theoretically since in this game we are not really
# using units other than 'pixels'... 
# So we will just assume that all 'masses' are in Kg and
# pixels <=> kilometers :D
################################################################
def GetGravForce(plamass, distance):
    force = (GRAVITY_CONSTANT * plamass * GRAVITY_FACTOR) / (distance**2)
    return force/3.6
	
######################################################
# GetMassByRadius
#radius --> in Km 
#density --> in g/cm^3, default is 5.515  (Earth's density)
#return ==> mass in Kg
################################################################
def GetMassByRadius(radius, density = 5.515):
    densityConversionConst = 10**12
    nKesph = 4.0/3.0
    vol = nKesph * pi * (radius**3)
    mass = (density*densityConversionConst)*vol
    return mass
    
################################################################
##    GetMaxGravForceDist
#return in kilometers
#the distance from the planet with given mass that the gravity force will be 0.1m/s^2  (0.02km/h^2)
#App.UtopiaModule_ConvertGameUnitsToKilometers()
################################################################
def GetMaxGravForceDist(plamass):
	if plamass > 0:
		dkm = (GRAVITY_CONSTANT * plamass * GRAVITY_FACTOR * 10) ** 0.5 #yes, we elevate to 0.5 to do sqrt() =P
		return dkm
	return 0
