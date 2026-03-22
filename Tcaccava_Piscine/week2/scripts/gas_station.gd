extends StaticBody2D

@export var refuel_amount = 50.0
@export var refuel_rate = 5

var taxi_in_range = false
var taxi_node = null

func _ready():
	$Area2D.body_entered.connect(_on_body_entered)
	$Area2D.body_exited.connect(_on_body_exited)
	
func _on_body_entered(body):
	if body.name == "Taxi":
		taxi_in_range = true
		taxi_node = body
		print("Press SPACEBAR to refuel")

func _on_body_exited(body):
	if body.name == "Taxi":
		taxi_in_range = false
		taxi_node = null

func _process(delta):
	if taxi_in_range and taxi_node != null:
		if Input.is_action_pressed("ui_accept"):  # SPAZIO
			refuel(delta)

func refuel(delta):
	if taxi_node.current_fuel < taxi_node.max_fuel:
		taxi_node.current_fuel += refuel_rate * delta
		taxi_node.current_fuel = min(taxi_node.current_fuel, taxi_node.max_fuel)
		taxi_node.fuel_changed.emit(taxi_node.current_fuel, taxi_node.max_fuel)
		
