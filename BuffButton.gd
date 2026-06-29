extends Button
var ButtonText = text
@onready var BuffData = $".."
func _pressed() -> void:
	if(ButtonText == "Pride" or ButtonText == "Courage"):
		if(!ButtonText.begins_with("Action:")):
			ButtonText = "Action:" + ButtonText
	else:
		if(!ButtonText.begins_with("Chip:")): #Make shure that the Chip: dosent stack (example: Chip:Chip:Chip:Chip:Chip:Agility)
			ButtonText = "Chip:" + ButtonText
	BuffData.ChipSelected = ButtonText
