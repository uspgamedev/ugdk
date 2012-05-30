import Config
import Items
import Weapons
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
    return Items.MaxValueIncreaseEffect(Items.MaxValueIncreaseEffect.LIFE, 25.0)

def CreateEnergyBoost():
    return Items.AbsoluteEnergyEffect(0, 25.0)
def CreateEnergyRegen():
    return Items.AbsoluteEnergyEffect(10.0, 15.0, True)
def CreateEnergyMaxBoost():
    return Items.MaxValueIncreaseEffect(Items.MaxValueIncreaseEffect.ENERGY, 25.0)

####################
# Pulse
####################
def CreatePulsePack(x, y):
    powerUps = {
        "Power Boost": CreatePulseDamagePack,
        "Multiplicity Boost": CreatePulseDamagePack,
        "Homing Boost": CreatePulseDamagePack,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/pulsePowerUp.png", 20.0, e, choice)
    return p

def CreatePulseDamagePack():
    return Items.PulseDamageIncreaseEffect(5.0)

####################
# Passive
####################
def CreatePassivePack(x, y):
    powerUps = {
        "Satellites": CreateSatellitePack,
        "Force Shield": CreateShieldPack,
        "Item Attractor": CreateItemAttractorPack,
        "Matter Absorption": CreateSatellitePack,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/passivePowerUp.png", 20.0, e, choice)
    return p

def CreateSatellitePack():
    return Items.SatelliteEffect()

def CreateShieldPack():
    return Items.ShieldEffect(300.0)

def CreateItemAttractorPack():
    return Items.ItemAttractorEffect(60.0, 320.0, 15.0)

####################
# Active
####################
def CreateActivePack(x, y):
    powerUps = {
        "Anti Grav Shield": CreateAntiGravShieldPack,
        "Laser": CreateShockBombPack,
        "Shock Bomb": CreateShockBombPack,
        "Blackhole": CreateShockBombPack,
        "Hyperspace": CreateHyperspacePack,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/activePowerUp.png", 20.0, e, choice)
    return p

def CreateAntiGravShieldPack():
    return Items.WeaponPickupEffect( Weapons.AntiGravShield(35.0) )

def CreateShockBombPack():
    return Items.WeaponPickupEffect( Weapons.ShockBomb() )

def CreateHyperspacePack():
    return Items.WeaponPickupEffect( Weapons.Hyperspace() )

####################
# Instant
####################
def CreateInstantPack(x, y):
    powerUps = {
        "Explosion": CreateExplosionPack,
        "Freeze": CreateExplosionPack,
        "Slowdown": CreateExplosionPack,
        "Fractal Shot": CreateFracturePack,
        "Fracture": CreateFracturePack,
    }
    choice = random.choice(powerUps.keys())
    e = powerUps[choice]()
    p = Items.PowerUp(x, y, "images/instantPowerUp.png", 15.0, e, choice)
    return p

def CreateExplosionPack():
    return Items.ShockwaveEffect(2.0, [5.0, 300.0], 100.0, 1.0)

def CreateFracturePack():
    return Items.FractureEffect()

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
