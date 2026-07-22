.PHONY: install run debug lint lint-strict clean

install:
	uv sync

run:
	uv run python -m src

debug:
	uv run python -m pdb -m src
	
lint:
	uv run flake8 src
	uv run mypy src --warn-return-any --warn-unused-ignores --ignore-missing-imports \
		--disallow-untyped-defs --check-untyped-defs

lint-strict:
	uv run flake8 src
	uv run mypy src --strict

clean:
	rm -rf .mypy_cache .pytest_cache .ruff_cache .coverage htmlcov .tox dist build
	find . -type d -name "__pycache__" -exec rm -rf {} +
	find . -type d -name "*.egg-info" -exec rm -rf {} +
	find . -type d -name ".mypy_cache" -exec rm -rf {} +
	find . -type d -name ".venv" -exec rm -rf {} +
	find . -type d -name ".uv-cache" -exec rm -rf {} +
	find . -type f -name "*.egg-info" -delete
	find . -type f -name "*.pyc" -delete
	rm -rf data/output/*