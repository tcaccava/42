extends Node2D
@onready var taxi = $Taxi
@onready var hud = $Hud
@onready var pause_menu = $Pause
func _ready():
	# Collega i segnali
	taxi.fuel_changed.connect(hud.update_fuel)
	taxi.game_over.connect(_on_game_over)
	taxi.distance_changed.connect(hud.update_distance)
	
func _input(event):
	# Premi ESC o P per mettere in pausa
	if event.is_action_pressed("ui_cancel"):  # ESC di default
		toggle_pause()

func toggle_pause():
	if get_tree().paused:
		pause_menu.hide()
		get_tree().paused = false
		hud.show()
	else:
		pause_menu.show()
		hud.hide()
		get_tree().paused = true

func _on_game_over():
	# Per ora solo un print, poi aggiungeremo la schermata
	print("GAME OVER")
	get_tree().change_scene_to_file("res://scenes/game_over.tscn")
	#get_tree().paused = true
