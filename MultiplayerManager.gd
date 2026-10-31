extends Node


	
func _ready() -> void:
	OS.set_environment("SteamAppID","480")
	OS.set_environment("SteamGameID","480")
	Steam.steamInitEx()

func _process(delta: float) -> void:
	Steam.run_callbacks()
	
