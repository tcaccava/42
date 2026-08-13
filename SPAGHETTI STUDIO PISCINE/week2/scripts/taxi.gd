extends CharacterBody2D

@export var speed = 300.0
@export var slowed_speed = 150.0 
@export var max_fuel = 100.0
@export var fuel_consumption_rate = 5.0
var current_speed = speed
var obstacles_touching = 0  # flag for touching obstacles
var current_fuel = max_fuel
var distance_traveled = 0.0
# Segnale per comunicare con l'HUD
signal fuel_changed(new_fuel, max_fuel)
signal game_over
signal distance_changed(distance)
@onready var animated_sprite = $AnimatedSprite2D
@onready var particles = $GPUParticles2D
var current_direction = "up"

func _ready():
	animated_sprite.play("up")
	current_speed = speed
	particles.emitting = false
	current_fuel = max_fuel
	fuel_changed.emit(current_fuel, max_fuel)

func consume_fuel(delta):
	if current_fuel > 0:
		current_fuel -= fuel_consumption_rate * delta
		current_fuel = max(0, current_fuel)  # Non va sotto zero
		
		fuel_changed.emit(current_fuel, max_fuel)
		
		if current_fuel <= 0:
			game_over.emit()


func _physics_process(delta):
	var direction = Vector2.ZERO
	
	if Input.is_action_pressed("ui_right"):
		direction.x = 1
		current_direction = "right"
	if Input.is_action_pressed("ui_left"):
		direction.x = -1
		current_direction = "left"
	if Input.is_action_pressed("ui_down"):
		direction.y = 1
		current_direction = "down"
	if Input.is_action_pressed("ui_up"):
		direction.y = -1
		current_direction = "up"
	
	# normalize the direction to have regular diagonal speed
	if direction != Vector2.ZERO:
		direction = direction.normalized()
		animated_sprite.play(current_direction)
	
	velocity = direction * current_speed
	if velocity.length() > 0:
		consume_fuel(delta)
	if current_fuel <= 0:
		velocity = Vector2.ZERO  # Ferma il taxi
		return
	var move_distance = velocity.length() * delta * 0.05
	distance_traveled += move_distance
	distance_changed.emit(distance_traveled)
	move_and_slide()
	
# Reduce speed when is on traffic cones or road bars,end emits yellow particles to simulate the friction
func _on_cono_1_body_entered(body):
	if body == self: 
		obstacles_touching += 1
		current_speed = slowed_speed
		particles.emitting = true
		print("Slowed down!", obstacles_touching)

func _on_cono_1_body_exited(body):
	if body == self:
		obstacles_touching -= 1
		if obstacles_touching == 0:
			current_speed = speed
			print("Normal speed!")
		particles.emitting = false


func _on_barre_body_entered(body: Node2D) -> void:
	if body == self:
		obstacles_touching += 1
		current_speed = slowed_speed 
		particles.emitting = true 
		print("Slowed down!")

func _on_barre_body_exited(body: Node2D) -> void:
	if body == self:
		obstacles_touching -= 1
		if obstacles_touching == 0:  
			current_speed = speed
			print("Normal speed!")
		particles.emitting = false

# Turbo when is on yellow lines
func _on_turbo_body_entered(body: Node2D) -> void:
	if body == self:
		current_speed = speed * 3  
		print("TURBO!")


func _on_turbo_body_exited(body: Node2D) -> void:
	if body == self:
		if obstacles_touching == 0: 
			current_speed = speed
			print("Normal speed")
