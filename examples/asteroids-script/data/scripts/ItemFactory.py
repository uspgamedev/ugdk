import Items
import Config
import random

####################
# Repairs
####################
def CreateRepairPack(x, y):
    powerUps = {
        "Life Boost": CreateLifeBoost,
        "Life Regen": CreateLifeRegen,
        "Life Max Increase": CreateLifeMaxBoost,
        "Energy Boost": CreateEnergyBoost,
        "Energy Regen": CreateEnergyRegen,
        "Energy Max Increase": CreateEnergyMaxBoost,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/repairPowerUp.png", 30.0, e, choice)
    return p

def CreateLifeBoost():
    return Items.AbsoluteLifeEffect(0, 25.0)
def CreateLifeRegen():
    return Items.AbsoluteLifeEffect(10.0, 5.0, True, True)
def CreateLifeMaxBoost():
    pass

def CreateEnergyBoost():
    return Items.AbsoluteEnergyEffect(0, 25.0)
def CreateEnergyRegen():
    return Items.AbsoluteEnergyEffect(10.0, 15.0, True)
def CreateEnergyMaxBoost():
    pass

####################
# Pulse
####################
def CreatePulsePack(x, y):
    powerUps = {
        "Power Boost": None,
        "Multiplicity Boost": None,
        "Homing Boost": None,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/pulsePowerUp.png", 20.0, e, choice)
    return p

####################
# Passive
####################
def CreatePassivePack(x, y):
    powerUps = {
        "Orbit Armor": None,
        "Force Shield": None,
        "Item Attractor": None,
        "Matter Absorption": None,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/passivePowerUp.png", 20.0, e, choice)
    return p

####################
# Active
####################
def CreateActivePack(x, y):
    powerUps = {
        "Anti Grav Shield": None,
        "Laser": None,
        "Shock Bomb": None,
        "Blackhole": None,
        "Hyperspace": None,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/activePowerUp.png", 20.0, e, choice)
    return p

####################
# Instant
####################
def CreateInstantPack(x, y):
    powerUps = {
        "Explosion": None,
        "Freeze": None,
        "Slowdown": None,
        "Fractal Shot": None,
        "Fracture": None,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/instantPowerUp.png", 15.0, e, choice)
    return p


###############################################
# Main Factory Function
###############################################
# we have 5 kinds of power ups: repair, pulse, passive, active and instant
# probability distribution dictates which type will be chosen, 
# and in the given type, the actual power up is selected at random uniformly
def CreatePowerUp(x, y):
    select = random.random() * 100
    types = {
        "Repair": (Config.repairChance, CreateRepairPack),
        "Pulse": (Config.pulseChance, CreatePulsePack),
        "Passive": (Config.passiveChance, CreatePassivePack),
        "Active": (Config.activeChance, CreateActivePack),
        "Instant": (Config.instantChance, CreateInstantPack),
    }
    chanceTotal = 0.0
    for chance, function in types.values():
        chanceTotal += chance
        if select <= chanceTotal:
            return function(x,y)
    return None
