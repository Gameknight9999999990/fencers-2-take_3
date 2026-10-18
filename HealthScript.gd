extends Node
@export var Dice = 0;
var health = 5;
var HalfLife = false;
@export var IsDummy = false
@onready var Attack  = $"../Node3D"
var ChipLoadout = ["Chip:Agility","Chip:percision","Action:Pride:3","Chip:Haste"]
@export var Chips = ["Normal","Normal","Normal" ,"Normal" ,"Normal"]
@onready var Movement = $".." #Get playercontroller to adjust dashes when using agility
var Invounerable = false



			  
func _input(event):
	if(event.is_action_released("DamagePlayer")):
		TakeDamage()
	elif(event.is_action_released("PrintHealth")):
		print("player Health is ", health, "and the chips are ", Chips, " the player has reached half health = ", HalfLife)
	elif(event.is_action_pressed("Wager1")):
		Wager(ChipLoadout[0])
	elif(event.is_action_pressed("Wager2")):
		Wager(ChipLoadout[1])
	elif(event.is_action_pressed("Wager3")):
		Wager(ChipLoadout[2])
	elif(event.is_action_pressed("Wager4")):
		Wager(ChipLoadout[3])
func _ready():
	print("the game has started")

func _process(delta: float) -> void:
		
			
	if (health <= 0):
			Remove1Stock()



func Remove1Stock():
	print("halfLife")
	if(!HalfLife):
		HalfLife = true;
		health = 5;
		Chips = ["Normal", "Normal", "Normal", "Normal","Normal"]
	else:
		#die
		print("the player is dead")
		
	
		
#damage the player 
func TakeDamage():
	Dice = 0;
	if(Invounerable):
		return
	print("Ow")
	var I = 0;
	
	for Chip in Chips:
		Chip = Chips[I]
		if(Chip == "Empty"):
			I+=1;
			continue
			
		else:
			if(Chip == "Normal"):
				
				Chips[I] = "Empty"
				health-= 1;
				break
			else:
				Chips[I] = "Empty"
				health-= 1;
				I+1;
	if(!IsDummy):
		Movement.UpdateDash()
		Attack.UpdateChips()
func Wager(WageredChip):
	var ChipCount = 0
	for Chip in Chips:
		if(Chip != "Normal"):
			ChipCount = ChipCount +1
	if(ChipCount >= 5 and WageredChip.split(":")[0] == "Chip"):
		return
	
	if(IsDummy):
		return
	print(Dice)
	Dice = Dice +1
	print(Dice)
		
	var ChipNum = 0
	var Action = WageredChip.split(":", false, 2)
	#wagers one of the given chip
	if(Action[0] == "Chip"):
		for Chip in Chips:
			if(Chip != "Normal"):
				ChipNum+= 1
				continue
			if(Chip == "Normal"):
				Chips[ChipNum] = WageredChip 
				ChipNum += 1; 
				break
	else: #do actions like pride 
		if(Dice >=int(Action[1])):
			Attack.DoAttack(0, Action[1])
	Movement.UpdateDash()
	Attack.UpdateChips()
			
