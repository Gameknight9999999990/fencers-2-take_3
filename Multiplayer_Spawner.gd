extends MultiplayerSpawner
#if you don't know  how to work with this code watch the "godot 4 steam multiplayer" video from "Gwizz1027" on youtube

@export var PlayerScene :PackedScene
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	spawn_function = SpawnPlayer
	if is_multiplayer_authority():
		spawnHost.call_deferred() #delay the spawning of the player by 1 frame so the game dosnt crash
		multiplayer.peer_connected.connect(spawn)
		multiplayer.peer_disconnected.connect(RemovePlayer)


func spawnHost() -> void:
	spawn(1)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
var PlayerData = {}
	
func SpawnPlayer(data):
	var Player = PlayerScene.instantiate()
	Player.set_multiplayer_authority(data)
	PlayerData[data] = Player
	return Player
	
func RemovePlayer(data):
	PlayerData[data].queue_free()
	PlayerData.erase(data)
	
