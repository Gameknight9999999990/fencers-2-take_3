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
var RCooldown = 6.7
var SM1Cooldown = 3.5
var FramesMoved = 0
var Style = 0
var MovingHitbox = false
var Moving = false
var Chips = []
var IsAttacking = false
var MoveSpeed = 1
@export var IsDummy =false
var Hitboxes = []
var Blocking = false
var I = 0
var CanAttack = true
var Frames = 0 #how many frames is the player being moved
# "_" is for declaring a value after the function, "," is for doing the next step in the attack and "|" is for the next attack (m1 m2 R sm1)
var AttackArray = ["AX_1.0,MHZ_0.35,AZ_1.3,D_0.4,A_1.1,D_0.4,E_6.9|D_0.5,M_100:40,A_1,MH_1,D_0.8,E_1|D_0.2,AE_B,D_1,M_45:35,AX_1,MHZ_05,AZ_2.5,A_1,D_0.3,E_1|AX_1,D_0.1,M_-100:20,D_0.5,M_100:25,D_0.48,A_1,D_0.4,E_1" ] 
#Use AX for setting the X Scale of the hitbox AZ for the Z scale, MHZ to move the hitbox's Z pos D for delay A(you need to assign a value to "a" because of my bad programming) for attack(setting the animaition and enabeling the hitbox) and E for ending the attack
#M is for moving the player (M_VelocityToApply:NumberOfFramesToMoveByTheVelocityAmount)
func _process(delta: float) -> void:
	Chips = HealthScript.Chips
	if(Input.is_action_pressed("Block")):
		ApplyEffects("Blocking", 0.0001)
	if(Blocking):
		print("Block")
	
func _ready() -> void:
	if(!IsDummy):
		$WeponHitbox.body_entered.connect(_on_hitbox_collided)
		
		

func _physics_process(delta: float) -> void:

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
		if(Input.is_action_pressed("Move_Back")): #Use "Input" instead if "input"  (no idea why you need to do this. i stole this from a random reddit thread so i dont know what exactly this does but i think that the capital "I" makes it so that it checks for a button press every frame)
			DoAttack(3)
			CanAttack = false
		else:
			DoAttack(0)
			CanAttack = false
	if(input.is_action_pressed("M2")):
		DoAttack(1)
		CanAttack = false
	if(input.is_action_pressed("R")):
		DoAttack(2)
		CanAttack = false
func DoAttack(AttackIndex):
	IsAttacking = true
	var Attacks = AttackArray[Style].split("|", false, 3)
	var Steps = Attacks[AttackIndex].split(",")
	Hitbox = $WeponHitbox
	for Step in Steps:
		var Command = Step.split("_")[0] 
		var Value = float(Step.split("_")[1])
		if(Command == "AX"):
			set_hitbox_Scale(Value,false)
		if(Command == "AZ"):
			set_hitbox_Scale(Value, true)
		if(Command == "A"):
			Disable_Hitbox(false)
		if(Command == "D"):
			await get_tree().create_timer(Value).timeout
		if(Command == "E"):
			get_parent_node_3d().linear_velocity = Vector3(0,0,0)
			CanAttack = true
			Moving = false
			Disable_Hitbox(true)
			IsAttacking = false
			FramesMoved = 0
			MovingHitbox = false 
			if(!Hitboxes.is_empty()):
				for Hitbox in Hitboxes:
					Hitbox.queue_free()
			Hitboxes.clear()
		if(Command == "MH"):
			MovingHitbox = true
			ApplyMovingHitbox()
		if(Command == "M"):
			FramesMoved = 0
			var NewValue = Step.split("_")[1].split(":") #0 in this array is the move-speed and 1 is the amount of frames that the player is being moved
			Frames = NewValue[1]
			print(NewValue[1])
			Moving =true
			MoveSpeed = NewValue[0]
	print(MovingHitbox)
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
	if(collided_object == Player):
		return
	var AttackScript = load("res://Attack.gd")
	print("COLLIDED")
	print(collided_object.name)
	print(collided_object.get_child(3).name)
	if(collided_object.get_child(3).get_script() != null):
		if(collided_object.get_child(3).get_script() == AttackScript):
			print("Damaged other player")
			#apply knockback
			var rb =collided_object as RigidBody3D 
			if(!Blocking):
				var forward = PlayerScript.twist_pivot.global_transform.basis.z * -1
				rb.apply_central_force(forward * 550)
				print("Apply Force")
				ApplyEffects("Blocking", 0.2)
			else:
				var forward = PlayerScript.twist_pivot.global_transform.basis.z * -1
				PlayerRb.apply_central_force(forward * -550)
	var HealthScriptEnemy = preload("res://HealthScript.gd")
	if collided_object.get_child(2).get_script() == HealthScriptEnemy:
		print("DAMAGE")
	for child in collided_object.get_children():
		if child.get_script() == HealthScriptEnemy:
			if child.has_method("TakeDamage"):
				child.TakeDamage()
				return


		
