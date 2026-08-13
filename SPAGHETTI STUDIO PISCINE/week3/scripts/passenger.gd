extends Area2D

@onready var passenger_sprite = $PassengerSprite
@onready var pickup_icon = $Pickup

enum States {WAIT, TAXI, DESTINATION}
var current_state = States.WAIT
var destination : Vector2
var blink_time = 0.5
var blink_timer = 0.0
var pickup_timer = 0.0
var deliver_timer = 0.0
var tic_toc = 3.7
var time_alert_emitted = false

var png = [
	preload("res://assets/pax1.png"),
	preload("res://assets/pax2.png"),
	preload("res://assets/pax3.png"),
	preload("res://assets/pax4.png")
]

signal passenger_pickedup(passenger)
signal passenger_delivered(passenger)
signal pickup_failed(passenger)
signal delivery_failed(passenger)
signal time_passing(passenger)

func _ready():
	body_entered.connect(_on_body_entered)
	randomize_spawn()
	randomize_destination()
	randomize_png()
	passenger_sprite.scale = Vector2(2.65, 2.65)
	set_process(true)
	
func randomize_destination():
	destination =  _get_random_position()


func randomize_spawn():
	position = _get_random_position()

func _get_random_position() -> Vector2:
	var map_width = 960
	var map_height = 950
	var pos = 	Vector2(
		randf_range(100, map_width - 100), 
		randf_range(100, map_height - 100)
	)
	return pos

func _on_body_entered(body):
	if body.name == "Taxi" and current_state == States.WAIT:
		pickup(body)

func pickup(_taxi):
	current_state = States.TAXI
	time_alert_emitted = false
	pickup_timer = 0.0
	call_deferred("hide")
	passenger_pickedup.emit(self)

func deliver():
	current_state = States.DESTINATION
	deliver_timer = 0.0
	passenger_delivered.emit(self)
	time_alert_emitted = false
	queue_free()

func is_waiting() -> bool:
	return current_state == States.WAIT

func in_taxi() -> bool:
	return current_state == States.TAXI

func _process(delta):
	blink_timer += delta
	if blink_timer >= blink_time:
		pickup_icon.visible = not pickup_icon.visible
		blink_timer = 0.0
	if current_state == States.WAIT:
		var ratio = pickup_timer / tic_toc
		var start_color = Color(1, 1, 0) 
		var end_color = Color(1, 0, 0)
		pickup_icon.modulate = start_color.lerp(end_color, ratio)
	
	if current_state == States.WAIT:
		pickup_timer += delta
		if pickup_timer > 2.5 and not time_alert_emitted:
			time_passing.emit(self)
			time_alert_emitted = true
		if pickup_timer >= tic_toc:
			pickup_timer = 0
			time_alert_emitted = false
			pickup_failed.emit(self)
	elif current_state == States.TAXI:
		deliver_timer += delta
		if deliver_timer > 2.5 and not time_alert_emitted:
			time_passing.emit(self)
			time_alert_emitted = true
		if deliver_timer >= tic_toc:
			deliver_timer = 0
			delivery_failed.emit(self)
			time_alert_emitted = false
			
func randomize_png():
	var random_index = randi_range(0,3)
	passenger_sprite.texture = png[random_index]
