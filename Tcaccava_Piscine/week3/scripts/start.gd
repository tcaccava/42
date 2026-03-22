extends Control

@onready var start_button = $Center/Start_button

func _ready():
	start_button.pressed.connect(_on_start_pressed)

func _on_start_pressed():
	get_tree().change_scene_to_file("res://scenes/node_2d.tscn")
