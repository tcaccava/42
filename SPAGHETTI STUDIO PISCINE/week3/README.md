## Mandatory Requirements ✅

### Passenger Management System
- ☑️ **Random passenger spawn**: Passengers appear at random positions on the map
- ☑️ **Random sprite selection**: Each passenger has a randomly selected sprite from 4 available spaghetti guys
- ☑️ **Random destinations**: Each passenger has a unique random destination point
- ☑️ **Blinking pickup indicator**: Arrow icon blinks above waiting passengers
- ☑️ **Scoring system**: +10 points for each successful delivery

## Bonus Features ⭐

### Failure Penalties & Game Over
- ☑️ **Pickup failure handling**: If timer expires before pickup, passenger respawns without penalty
- ☑️ **Delivery failure penalty**: -10 points if delivery timer expires
- ☑️ **Score-based game over**: Game ends when score drops to 0 or below

### Timer System
- ☑️ **Pickup timer**: 4 seconds to reach and pick up a passenger
- ☑️ **Delivery timer**: 4 seconds to deliver passenger to destination

### Strategic Gameplay Balance
- ☑️ **Fuel vs delivery balance**: Core gameplay revolves around balancing deliveries with refueling needs
- ☑️ **Strategic decision making**: Players must evaluate whether to skip distant passengers based on fuel level. DON'T FORGET TO REFUEL.
- ☑️ **Time maximization objective**: Goal is to survive as long as possible by managing fuel and deliveries. The game is designed to be difficult, but with a little strategy, you can do it.

### Audio Feedback
- ☑️ **Low fuel warning sound**: Double audio alert plays when fuel drops below 30% and 15%
- ☑️ **Low time warning sound**: Audio alert plays when there are less than 1.3 seconds left until the end of the pickup and delivery time.
- ☑️ **Delivery completion sound**: Audio alert plays when the png is delivered to the destination
- ☑️ **VROOM sound when taxi moves**: Sound plays when arrow keys are pressed
- ☑️ **Pickup sound**: Sound plays when the passenger gets into the taxi
- ☑️ **New delivery sound**: Sound plays when taxi driver gets a new call
- ☑️ **GLUGLU sound**: Audio plays when refueling
- ☑️ **Pause menu and Gameover sounds**: Sounds play when you go in pause and when you get game over


### Persistent Score System
- ☑️ **Global timer variable**: Elapsed time tracked as main score (higher is better)
- ☑️ **Best time tracking**: Personal best time saved across game sessions
- ☑️ **Game over display**: Shows current survival time and best time
- ☑️ **New record notification**: Blinking "NEW RECORD" label appears when beating personal best

