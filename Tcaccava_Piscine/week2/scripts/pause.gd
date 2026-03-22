extends CanvasLayer

@onready var resume_button = $CenterContainer/VBoxContainer/Resume
@onready var restart_button = $CenterContainer/VBoxContainer/Restart
@onready var menu_button = $CenterContainer/VBoxContainer/Menu
@onready var background = $Background
@onready var center_container = $CenterContainer

func _ready():
	background.position = Vector2.ZERO
	# Collega pulsanti
	resume_button.pressed.connect(_on_resume_pressed)
	restart_button.pressed.connect(_on_restart_pressed)
	menu_button.pressed.connect(_on_menu_pressed)
	# Nascondi il menu all'inizio
	hide()

func _on_resume_pressed():
	hide()
	get_tree().paused = false
	get_parent().get_node("Hud").show()
func _on_restart_pressed():
	hide()
	get_tree().paused = false
	get_tree().reload_current_scene()

func _on_menu_pressed():
	hide()
	get_tree().paused = false
	get_tree().change_scene_to_file("res://scenes/start.tscn")
