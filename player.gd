extends RigidBody3D

var mouse_sensitivity := 0.001
var twist_input := 0.0
var pitch_input := 0.0
var move_speed := 2400.0
var dashDir = Vector3()
var DashTime = 0.4
var DashSpeed = 7500
var DashesLeft = 2
var  IsDashing = false
var CanDash = true
@onready var twist_pivot := $TwistPivot
@onready var pitch_pivot := $TwistPivot/PitchPivot

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if(DashesLeft > 0 and !IsDashing):
		CanDash = true
	var input := Vector3.ZERO
	if(!IsDashing):
		input.x = Input.get_axis("straif_left", "straif_right")

		input.z = Input.get_axis("move_forward", "Move_Back")
	var HasGottenSlowed = false #variable to make shure that the movement speed nerf is only applied once when dashing
	#make the player slower while dashing so they dont have as much control
	if(IsDashing):
		
		if(!HasGottenSlowed):
			move_speed = move_speed / 2.5
	else:
		if(HasGottenSlowed):
			move_speed = move_speed * 2.5
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
	
	
func DashCooldown():
	CanDash =false
	DashesLeft -= 1
	IsDashing = true
	await get_tree().create_timer(DashTime).timeout
	IsDashing = false
	await get_tree().create_timer(0.3).timeout
	DashesLeft += 1
func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
			twist_input = - event.relative.x * mouse_sensitivity
			pitch_input = - event.relative.y * mouse_sensitivity
