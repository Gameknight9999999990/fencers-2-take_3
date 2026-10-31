extends Node2D

var LobbyID = 0
var peer = SteamMultiplayerPeer.new()
@onready var ms = $HostButton/MultiplayerSpawner
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	ms.spawn_function = spawn_level
	Steam.lobby_created.connect(_on_lobby_created)
	Steam.lobby_match_list.connect(on_lobby_match_list)
	open_lobby_list()
	

func spawn_level(data):
	var a = (load(data) as PackedScene).instantiate()
	return a
	
func _on_host_button_pressed() -> void:
	peer.create_host(Steam.LOBBY_TYPE_PUBLIC)
	multiplayer.multiplayer_peer = peer
	ms.spawn("res://level.tscn")	
	$HostButton.hide()
	$LobbyContainer/Lobbies.hide()
	$RefreshButton.hide()
	
	
func joinLobby(id):
	peer.connect_to_lobby(id)
	multiplayer.multiplayer_peer = peer
	LobbyID = id
	$HostButton.hide()
	$LobbyContainer/Lobbies.hide()
	$RefreshButton.hide()
func _on_lobby_created(connect, id):
	print("lobby creat")
	if(connect == 0):
		LobbyID = id
		Steam.setLobbyData(LobbyID, "Name",str(Steam.getPersonaName()+"'s Lobby")) # set the lobby's name to "Username's Lobby"
		Steam.setLobbyJoinable(LobbyID,true) # make the lobby joinable with the id
		print(LobbyID)
		
func open_lobby_list():
	Steam.addRequestLobbyListDistanceFilter(Steam.LOBBY_DISTANCE_FILTER_WORLDWIDE)
	Steam.requestLobbyList()
	
func on_lobby_match_list(lobbies):
	for lobby in lobbies:
		var lobbyName = Steam.getLobbyData(lobby, "name")
		var MemberCount = Steam.getNumLobbyMembers(lobby)
		var but = Button.new()
		but.set_text(str(lobbyName, "|Players: ", MemberCount))
		but.set_size(Vector2(100,5))
		but.connect("pressed", Callable(self, "joinLobby").bind(lobby))
		$LobbyContainer/Lobbies.add_child(but)


func _on_refresh_button_pressed() -> void:
	if $LobbyContainer/Lobbies.get_child_count() > 0:
		for n in $LobbyContainer/Lobbies.get_children():
			n.queue_free()
		
