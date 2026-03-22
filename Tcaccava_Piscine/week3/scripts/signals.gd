extends Node2D

@onready var taxi = $Taxi
@onready var hud = $Hud
@onready var pause_menu = $Pause
@onready var low_fuel_sound = $LowFuel
@onready var out_of_time = $OutOfTime
@onready var delivered_sound = $Delivered
@onready var vroom_sound = $VROOM
@onready var refueling_sound = $Refueling
@onready var gas_station = $GasStation
@onready var pickup_sound = $Pickup
@onready var new_call = $NewPassenger
@onready var pause_sound = $Pause/PauseSound

var passenger_scene = preload("res://scenes/passenger.tscn")
var destination_scene = preload("res://scenes/destination.tscn")
var current_passenger = null
var current_destination_marker = null
var score = 10
var low_fuel_alerted = false
var elapsed_time = 0.0
var is_game_over = false
var low_time_alerted = false
var very_low_fuel_alerted = false

func _ready():
	taxi.fuel_changed.connect(hud.update_fuel)
	taxi.game_over.connect(_on_game_over)
	taxi.distance_changed.connect(hud.update_distance)
	taxi.vroom.connect(_on_vroom)
	gas_station.refueling.connect(_on_refuel)
	spawn_passenger()

func _input(event):
	if event.is_action_pressed("ui_cancel"):
		pause_sound.play()
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
	print("GAME OVER")
	if is_game_over:
		return
	is_game_over = true
	GlobalTimer.elapsed_time = elapsed_time
	get_tree().change_scene_to_file("res://scenes/game_over.tscn")

func spawn_passenger():
	current_passenger = passenger_scene.instantiate()
	current_passenger.passenger_pickedup.connect(_on_passenger_picked_up)
	current_passenger.passenger_delivered.connect(_on_passenger_delivered)
	current_passenger.delivery_failed.connect(_on_delivery_failed)
	current_passenger.pickup_failed.connect(_on_pickup_failed)
	current_passenger.time_passing.connect(_on_little_time)
	add_child(current_passenger)
	new_call.play()
	
func _on_passenger_picked_up(passenger):
	low_time_alerted = false
	current_destination_marker = destination_scene.instantiate()
	current_destination_marker.position = passenger.destination
	call_deferred("add_child", current_destination_marker)
	pickup_sound.play()
	await get_tree().process_frame
	current_destination_marker.reached.connect(_on_destination_reached)

func _on_destination_reached():
	score += 10
	hud.update_score(score)
	delivered_sound.play()
	if current_passenger:
		current_passenger.deliver()
	if current_destination_marker:
		current_destination_marker.queue_free()
		current_destination_marker = null
	await get_tree().create_timer(1.0).timeout
	spawn_passenger()

func _on_passenger_delivered(_passenger):
	current_passenger = null
	low_time_alerted = false

func _on_pickup_failed(_passenger):
	low_time_alerted = false
	score -= 10
	if score < 0:
		_on_game_over()
	hud.update_score(score)
	current_passenger.randomize_spawn()

func _on_delivery_failed(_passenger):
	low_time_alerted = false
	score -= 10
	hud.update_score(score)
	if score < 0:
		_on_game_over()	
	if current_destination_marker:
		current_destination_marker.queue_free()
		current_destination_marker = null
	if current_passenger:
		current_passenger.queue_free()
	spawn_passenger()
	
func _on_low_fuel():
	if not low_fuel_alerted:
		low_fuel_alerted = true
		low_fuel_sound.play()
		
func _on_very_low_fuel():
	if not very_low_fuel_alerted:
		very_low_fuel_alerted = true
		low_fuel_sound.play()
		await get_tree().create_timer(0.5).timeout
		low_fuel_sound.play()
		
func _on_little_time(_passenger):
	if not low_time_alerted:
		out_of_time.play()
		low_time_alerted = true
		
func _on_vroom():
	if not vroom_sound.playing:
		vroom_sound.play()
		

func _on_refuel():
	if not refueling_sound.playing:
		refueling_sound.play()

func _process(delta):
	if taxi.current_fuel / taxi.max_fuel > 0.15:
		very_low_fuel_alerted = false
	if taxi.current_fuel / taxi.max_fuel > 0.3:
		low_fuel_alerted = false
		very_low_fuel_alerted = false
	if taxi.current_fuel / taxi.max_fuel <= 0.3:
		_on_low_fuel()
	if taxi.current_fuel / taxi.max_fuel <= 0.15:
		_on_very_low_fuel()
	if not get_tree().paused and not is_game_over:
		elapsed_time += delta
