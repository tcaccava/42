# The script inherits from CharacterBody2D node
extends CharacterBody2D

# Export makes the variables editable from the Inspector
@export var max_speed = 800.0 # Max speed
@export var deceleration = 600.0 # How far the spaceship decelerates(higher = stops faster)
@export var acceleration = 700.0 # How fast the spaceship accelerates

# $ Finds the AnimatedSprite2d child node when the scene is ready(onready) 
@onready var sprite = $AnimatedSprite2D

# Centers the spaceship in the window
func _ready():
	position = get_viewport_rect().size / 2

# This special function updates the games 60 times per second for physics calculations
func _physics_process(delta):
	var direction = Input.get_vector("ui_left","ui_right","ui_up","ui_down") # Get input from arrow keys (returns a Vector2)
	direction = direction.normalized()
	var target_velocity = direction * max_speed
	# Inertial system, move_toward smoothly interpolates between 2 values,in this case between current velocity and target velocity
	if direction != Vector2.ZERO:
		velocity = velocity.move_toward(target_velocity,acceleration * delta) # Player is pressing key, so it accelerates toowards target velocityy
	else:
		velocity = velocity.move_toward(Vector2.ZERO,deceleration * delta)	# No input, decelerates toward zero	
	
	animate_ship()
	# Moves spaceship using built-in physics systems
	move_and_slide()

# This func handles spaceship animation and changes sprite frame basing on direction		
func animate_ship():
	# Change sprite based on input for immediate response
	var direction = Input.get_vector("ui_left","ui_right","ui_up","ui_down")
	
	if direction != Vector2.ZERO:  # Only change sprite if player is pressing keys
		# Check which direction has the stronger input
		if abs(direction.x) > abs(direction.y):
			# Horizontal input is stronger
			if direction.x > 0:	
				sprite.frame = 0  # Set to right-facing sprite
			else:		
				sprite.frame = 3  # Set to left-facing sprite
		else:
			# Vertical input is stronger  
			if direction.y < 0:
				sprite.frame = 1  # Set to up-facing sprite
			else:
				sprite.frame = 2  # Set to down-facing sprite

	# Bonus animation: change color based on speed
	var color_stopped = Color.WHITE        # white
	var color_slow = Color(1,1,0)          # yellow
	var color_medium = Color(0.3,1,1)      # cyan
	var color_fast = Color(1,0.5,1)          # magenta

	var speed_ratio = velocity.length() / max_speed

	# Smooth transitions between colors
	if speed_ratio < 0.1:
		sprite.modulate = color_stopped
	elif speed_ratio < 0.4:
		var t = (speed_ratio - 0.1) / (0.4 - 0.1)
		sprite.modulate = color_stopped.lerp(color_slow, t)
	elif speed_ratio < 0.7:
		var t = (speed_ratio - 0.4) / (0.7 - 0.4)
		sprite.modulate = color_slow.lerp(color_medium, t)
	else:
		var t = (speed_ratio - 0.7) / (1.0 - 0.7)
		sprite.modulate = color_medium.lerp(color_fast, t)
