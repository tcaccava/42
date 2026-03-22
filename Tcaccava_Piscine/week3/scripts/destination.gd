extends Area2D

var blink_timer = 0.0
var blink_time = 0.3
@onready var flag = $Flag
signal reached

func _ready():
	body_entered.connect(_on_body_entered)

func _on_body_entered(body):
	if body.name == "Taxi":
		reached.emit()

func _process(delta):
	blink_timer += delta
	if blink_timer > blink_time:
		flag.visible = not flag.visible
		blink_timer = 0.0
