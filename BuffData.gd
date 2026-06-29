extends Node2D
@export var Hp = Node3D
var SlotSelected = 0
var ChipSelected = "" 
func SelectSlot():
	Hp.ChipLoadout[SlotSelected] = ChipSelected
	
#i kinda fucked up with the naming of the buttons so button3 is the first button button2 is the second button is the 3 and Button4 is the 4th
func _on_button_2_pressed() -> void:
	SlotSelected = 1
	SelectSlot()

func _on_button_3_pressed() -> void:
	SlotSelected = 0
	SelectSlot()
func _on_button_pressed() -> void:
	SlotSelected = 2
	SelectSlot()
	
func _on_button_4_pressed() -> void:
	SlotSelected = 3
	SelectSlot()
