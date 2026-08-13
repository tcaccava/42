extends CanvasLayer

@onready var tachometer = $Tachometer
@onready var fuel_label = $Fuel_level
@onready var needle = $Tachometer/Needle
@onready var distance_label = $Distance
@onready var score_label = $Score
@export var min_rotation = -270.0
@export var max_rotation = 0.0

func _ready():
	tachometer.position = Vector2(850, 783)
	fuel_label.position = Vector2(848, 845)
	distance_label.position = Vector2(850,865)
	score_label.position = Vector2(10,30)
	
func update_fuel(current_fuel: float, max_fuel: float):
	var fuel_percentage = current_fuel / max_fuel
	fuel_label.text = "FUEL: %.0f%%" % (fuel_percentage * 100)
	needle.rotation_degrees = lerp(min_rotation, max_rotation, fuel_percentage)
	
func update_distance(distance:float):
	distance_label.text = "%.0f m" % distance

func update_score(new_score:int):
	score_label.text = "SCORE: %d" % new_score
