import time

def retry(max_attempts=3, base_delay=1, max_delay=30):
	def decorator(func):
		async def wrapper(*args, **kwargs):
			delay = base_delay
			for attempt in range(1, max_attempts + 1):
				try:
					return await func(*args, **kwargs)
				except Exception as e:
					if attempt < max_attempts:
						time.sleep(delay)
						delay = min(max_delay, delay * 2)
					else:
						raise e
		return wrapper
	return decorator