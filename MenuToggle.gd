extends Node3D

@export var menu: Node2D 

func _ready() -> void:
	visible = false



func _input(event: InputEvent) -> void:
	if event.is_action_pressed("OpenMenu"): 
		if visible:
			close_menu()
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
		else:
			open_menu()
			Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func open_menu() -> void:
	visible = true
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	menu.set_visible(true) 
	
func close_menu() -> void:
	menu.visible = !menu.visible
	visible = false

	
