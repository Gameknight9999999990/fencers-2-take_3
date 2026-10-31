extends Node3D
@export var Hitbox : Area3D
@export var CurrentObject : Node3D
@export var Player : Node3D
@export var PlayerRb : RigidBody3D
@onready var PlayerScript = $".."
@onready var HealthScript =$"../Health"
func get_hitbox() -> Area3D:
	return $WeponHitbox as Area3D
var M1Cooldown = 1.5
var M2Cooldown = 3
var RCooldown = 6.7;
var SM1Cooldown = 3.5
var CanM1 = true;
var CanM2 = true;
var CanSM1 = true
var CanR = true;

var Dice = 0;
var FramesMoved = 0;
var Style = 0
var MovingHitbox = false
var Moving = false
var Chips = []
var IsAttacking = false
var MoveSpeed = 1
@export var IsDummy =false
var DestroyBlock = false;
var Hitboxes = []
var Blocking = false
var I = 0
var CanAttack = true
var Frames = 0 #how many frames is the player being moved
# "_" is for declaring a value after the function, "," is for doing the next step in the attack and "|" is for the next attack (m1 m2 R sm1)
var AttackArray = ["AX_1.0,MHZ_0.35,AZ_1.3,D_0.4,A_1.1,D_0.4,E_6.9|D_0.5,AX_1,AZ_1,M_100:40,A_1,MH_1,D_0.8,E_1|D_0.2,AE_0,D_1,M_45:35,AX_1,MHZ_6,AZ_1.3,A_1,D_0.3,E_1|AX_1.3,AZ_0.7,D_0.1,M_-100:20,D_0.5,M_100:25,D_0.48,A_1,D_0.4,E_1" ] 
#Use AX for setting the X Scale of the hitbox AZ for the Z scale, MHZ to move the hitbox's Z pos D for delay A(you need to assign a value to "a" because of my bad programming) for attack(setting the animaition and enabeling the hitbox) and E for ending the attack
#M is for moving the player (M_VelocityToApply:NumberOfFramesToMoveByTheVelocityAmount)
func _process(delta: float) -> void:
	Dice = HealthScript.Dice
	Chips = HealthScript.Chips
	if(Input.is_action_pressed("Block")):
		ApplyEffects("Blocking", 0.0001)

	
	
func _ready() -> void:
	if(!IsDummy):
		$WeponHitbox.body_entered.connect(_on_hitbox_collided)
		
		

func _physics_process(delta: float) -> void:
	if !is_multiplayer_authority():
		return
	if Moving and int(FramesMoved) <= int( Frames):
		if PlayerRb != null:
			var forward_direction = -PlayerRb.global_transform.basis.z
			PlayerRb.apply_central_force(forward_direction * int(MoveSpeed))
			FramesMoved+=1
			FramesMoved +=1
		if(FramesMoved >= int(Frames)):
			Moving = false

func _input(input):
	if(IsDummy):
		return
	var current_cam = get_viewport().get_camera_3d()
	if current_cam != null:
		global_rotation.y = current_cam.global_rotation.y 
	if(IsAttacking): #make shure that the player cant attack while attacking
		return
	if(!CanAttack):
		return
	

	if(input.is_action_pressed("M1")):
		if(Input.is_action_pressed("Move_Back") and CanSM1): #Use "Input" instead if "input"  (no idea why you need to do this. i stole this from a random reddit thread so i dont know what exactly this does but i think that the capital "I" makes it so that it checks for a button press every frame)
			CanSM1 = false
			DoAttack(3,null)
			CanAttack = false
			StartCD(3)
		else:
			if(CanM1):
				DoAttack(0,null)			
				CanM1 = true
				CanAttack = false
				StartCD(1)
	if(input.is_action_pressed("M2") and CanM2):
		DoAttack(1,null)
		CanAttack = false
		CanM2 = false
		StartCD(2)
	if(input.is_action_pressed("R") and CanR):
		DoAttack(2, null)
		CanAttack = false
		CanR = false
		StartCD(4)
		
		
		
func DoAttack(AttackIndex, DoAction): #Use doAction to see if the player is usin an action like pride, if not then set to null
	
	IsAttacking = true
	var DiceCost =0;
	var Attacks = AttackArray[Style].split("|", false, 3)
	var Steps = Attacks[AttackIndex].split(",")
	var IsValidAction = false;
	Hitbox = $WeponHitbox
	var ChipEffects = [""]
	var ChipEffectNumber = [0]
	if(DoAction != null):
		print(DoAction  + "sdyres");
		Steps = ["AX_1","AZ_1","A_0","E_1234"] #if dice are too low the player will default to an m1 attack, assign this to remove the attack hitbox instantly
		#Array index: 0=pride, 1=Courage
		var ActionArray = ["M_-90:10,D_0.3,M_90:30,D_0.4,AX_2,MHZ_0.45,AZ_1.2,ABD_0,A_0,D_0.5,E_0","AE_0,M_90:30,D_0.3,E_0"]; #ABD is for attack block destroy
		if(DoAction == "Pride" and Dice >= 3):
			Steps = ActionArray[0].split(",");
			DiceCost = 3
			IsValidAction = true
		if(DoAction == "Courage" and Dice >=2):
			Steps = ActionArray[1].split(",");
			DiceCost = 2
			IsValidAction= true
		if(Dice < DiceCost):
			IsAttacking = false
			CanAttack = true
			return
		else:
			HealthScript.Dice = Dice-DiceCost
		
		
	

	for Step in Steps:
		print("AEAEAEAEAEAEQAEAQEw")
		var Command = Step.split("_")[0] 
		var Value = float(Step.split("_")[1])
		if(Command == "AX"):
			set_hitbox_Scale(Value,false)
		if(Command == "AE"):
			if(Value == 0):
				ApplyEffects("Blocking", 2);
		if(Command == "AZ"):
			set_hitbox_Scale(Value, true)
		if(Command == "A"):
			   
			Disable_Hitbox(false)
		if(Command == "D"):
	
			await get_tree().create_timer(Value).timeout
		if(Command == "ABD"):
			DestroyBlock =true;
		if(Command == "E"):
			
			if(Value != 1234):
				get_parent_node_3d().linear_velocity = Vector3(0,0,0)
			CanAttack = true
			DestroyBlock = false;
			Moving = false
			Disable_Hitbox(true)
			IsAttacking = false
			FramesMoved = 0
			MovingHitbox = false 
			print("ENd")
			if(!Hitboxes.is_empty()):
				for Hitboxx in Hitboxes:
					Hitboxx.queue_free()
			Hitboxes.clear()
		if(Command == "MH"):
			MovingHitbox = true
			ApplyMovingHitbox()
		if(Command == "M"):
			FramesMoved = 0
			var NewValue = Step.split("_")[1].split(":") #0 in this array is the move-speed and 1 is the amount of frames that the player is being moved
			Frames = NewValue[1]
		
			Moving =true
			MoveSpeed = NewValue[0]

func ApplyEffects(Effect,timer): #apply effects like blocking or slowed
	set(Effect, true)
	await get_tree().create_timer(timer).timeout 
	set(Effect, false)
	
	
	
	
func set_hitbox_Scale(value: float, ChangeZ :bool) -> void:
	# Use get_node() explicitly to find the child node on demand
	var hitbox_node = get_node_or_null("WeponHitbox")
	
	if hitbox_node != null and ChangeZ == false:
		hitbox_node.scale.x = value
	elif ChangeZ and hitbox_node != null:
		hitbox_node.scale.z = value
	else:
		print("hitbox is null")
		
func Disable_Hitbox(Disable : bool):
	# Look for the CollisionShape3D inside the WeponHitbox
	var collision_shape = get_node_or_null("WeponHitbox/CollisionShape3D")
	
	if collision_shape != null:
		collision_shape.disabled = Disable
	else:
		print("Hitbox is nullllll")

func ApplyMovingHitbox():
	if(Hitbox == null):
		print("Null")
	print(get_parent_node_3d().name)
	var NewHitbox = Hitbox.duplicate()
	NewHitbox.top_level = true #make shure that the hitbox stays in place and dosn't move with the playerer
	add_child(NewHitbox)
	var AreaArray = [NewHitbox]
	Hitboxes = Hitboxes +  AreaArray
	NewHitbox.global_position = global_position
	I+=1
	await get_tree().create_timer(0.0001).timeout

	if(MovingHitbox):
		ApplyMovingHitbox()
	else:
		print("ze hitbox iz not moving")
		
func _on_hitbox_collided(collided_object: Node3D) -> void:
	if(collided_object == get_parent_node_3d()):
		return
	var AttackScript = load("res://Attack.gd")
	if collided_object.get_child_count() > 3:
		if(collided_object.get_child(3).get_script() != null):
			if(collided_object.get_child(3).get_script() == AttackScript):
				print("Damaged other player")
				#apply knockback
				var EnemyIsBlocking = collided_object.get_child(3).get("Blocking");
			
			
			
				var rb =collided_object as RigidBody3D 
				if(!EnemyIsBlocking or DestroyBlock):
					var forward = PlayerScript.twist_pivot.global_transform.basis.z * -1
					rb.apply_central_force(forward * 550)
					print("Apply Force")
					ApplyEffects("Blocking", 0.2)
				else:
					var forward = PlayerScript.twist_pivot.global_transform.basis.z * -1
					PlayerRb.apply_central_force(forward * -550)
	var HealthScriptEnemy = preload("res://HealthScript.gd")
	if collided_object.get_child_count() >= 3:
		if collided_object.get_child(2).get_script() == HealthScriptEnemy:
			print("DAMAGE")
		for child in collided_object.get_children():
			if child.get_script() == HealthScriptEnemy:
				if child.has_method("TakeDamage"):
					child.TakeDamage()
					return
				
				
func UpdateChips():
	Chips = HealthScript.Chips
	
func StartCD(Attack): # Attack index: 1=m1 2=m2 3=SM1 4=R
	if(Attack == 1):
		await get_tree().create_timer(M1Cooldown).timeout
		CanM1 = true
	if(Attack == 2):
		print("2")
		await get_tree().create_timer(M2Cooldown).timeout
		CanM2 = true
	if(Attack == 3):
		await get_tree().create_timer(SM1Cooldown).timeout
		CanSM1 = true
	if(Attack == 4):
		await get_tree().create_timer(RCooldown).timeout
		CanR = true
	
	
