def ft_water_reminder():
    dw = int(input("Days since last watering: "))
    if dw > 2:
        print("Water the plants!")
    else:
        print("Plants are fine")
