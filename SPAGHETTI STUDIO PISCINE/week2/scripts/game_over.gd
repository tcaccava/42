extends Control

@onready var restart_button = $VBoxContainer/Restart
@onready var menu_button = $VBoxContainer/Menu

func _ready():
	restart_button.pressed.connect(_on_restart_pressed)
	menu_button.pressed.connect(_on_menu_pressed)

func _on_restart_pressed():
	# Ricarica il gioco
	get_tree().paused = false  # Toglie la pausa
	get_tree().change_scene_to_file("res://scenes/node_2d.tscn")

func _on_menu_pressed():
	# Torna al menu principale
	get_tree().paused = false
	get_tree().change_scene_to_file("res://scenes/start.tscn")
