from ugdk.ugdk_math import Vector2D

#################################
# Game Settings
#################################
# Video Config
resolution = Vector2D(800.0, 600.0)
fullscreen = False

# Language Config
language = "en_US"



##################################
# Game Parameters
##################################

# PowerUps
gamesize = Vector2D(1000.0, 1000.0)
repairChance = 30.0    # the <powerUpType>Chance variables should sum up to 100.0
pulseChance = 10.0
passiveChance = 20.0
activeChance = 20.0
instantChance = 20.0
