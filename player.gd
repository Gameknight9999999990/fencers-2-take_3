extends RigidBody3D



var mouse_sensitivity := 0.001
var twist_input := 0.0
var pitch_input := 0.0
var move_speed := 2400.0
var dashDir = Vector3()
var DashTime = 0.35
var DashSpeed = 4550
var DashesLeft = 2
var  IsDashing = false
var HasGottenSlowed = false #variable to make shure that the movement speed nerf is only applied once when dashing
var CanDash = true
var MaxDashes = 2
var DashRegen = 5

#variables to make shure that Dash Variables get reset when agility chips are removed
var DefaultDashSpeed = DashSpeed
var DefaultMaxDashes = MaxDashes
var DefaultDashRegen = 5

@onready var HealthScript = $Health
@onready var Chips = HealthScript.Chips

@onready var twist_pivot := $TwistPivot
@onready var pitch_pivot := $TwistPivot/PitchPivot

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if(DashesLeft > MaxDashes):
		DashesLeft = MaxDashes
	if(DashesLeft > 0 and !IsDashing):
		CanDash = true
	var input := Vector3.ZERO
	if(!IsDashing):
		input.x = Input.get_axis("straif_left", "straif_right")

		input.z = Input.get_axis("move_forward", "Move_Back")
	
	#make the player slower while dashing so they dont have as much control
	if(IsDashing):
		if(!HasGottenSlowed):
			HasGottenSlowed = true
			move_speed = move_speed / 5
	else:
		if(HasGottenSlowed):
			HasGottenSlowed = false
			await get_tree().create_timer(0.2).timeout #keep the player slowed for a short while so the dash needs to be used more strategicly
			move_speed = move_speed * 5
	var forward = -twist_pivot.global_transform.basis.z
	var right = twist_pivot.global_transform.basis.x
	var move_dir : Vector3 = (forward * -input.z + right * input.x).normalized()

	if Input.is_action_just_pressed("Dash") and CanDash:
		IsDashing = true
		DashCooldown()	
		dashDir = input
	var move_dirDash : Vector3 = (forward * -dashDir.z + right * dashDir.x).normalized()
	if(IsDashing):
		apply_central_force(move_dirDash * DashSpeed * delta)
	
	
	if move_dir != Vector3.ZERO:
		apply_central_force(move_dir * move_speed * delta)
	
	if Input.is_action_just_pressed("ui_cancel"):
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
		
	twist_pivot.rotate_y(twist_input)
	pitch_pivot.rotate_x(pitch_input)
	pitch_pivot.rotation.x = clamp(pitch_pivot.rotation.x, -1.5, 1.5)
	twist_input = 0.0
	pitch_input = 0.0
	
func UpdateDash():
	MaxDashes = DefaultMaxDashes
	DashSpeed = DefaultDashSpeed
	DashRegen = DefaultDashRegen
	Chips = HealthScript.Chips
	var AgilityCount = 1
	for chip in Chips:
		if(chip == "Agility"):
			AgilityCount += 1
	if(AgilityCount == 0):
		DashSpeed = DefaultDashSpeed
	for AgilityChips in AgilityCount:
		DashSpeed+= 200
	for i in range(0, AgilityCount, 2): #+1 dash per pair
		MaxDashes =+ 1
		DashRegen =- 1
	
			
func DashCooldown():
	var current_velocity = linear_velocity
	CanDash =false
	DashesLeft -= 1
	IsDashing = true
	await get_tree().create_timer(DashTime).timeout
	current_velocity.x = 0 #stop sliding after dash
	current_velocity.z = 0
	IsDashing = false
	await get_tree().create_timer(DashRegen).timeout
	DashesLeft += 1
func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
			twist_input = - event.relative.x * mouse_sensitivity
			pitch_input = - event.relative.y * mouse_sensitivity
