# ---------------------------------------------------------------------
# Fly-in - drone fleet routing simulator
#
# Dependencies and the virtual environment are managed with uv.
# If uv is not installed, `make install` falls back to python3 -m venv
# and pip, so the project always builds the same .venv either way.
#
# The simulator itself imports the standard library only, so `run`,
# `debug` and `benchmark` deliberately use a plain interpreter and never
# touch the network: they work on a fresh clone, before `make install`,
# and on a machine with no internet access at all.
# Only `lint`, `lint-strict` and `animate` need installed packages.
# ---------------------------------------------------------------------

MAP    ?= maps/easy/01_linear_path.txt
ARGS   ?=
SPEED  ?= 600
VENV   := .venv
VENV_PY := $(VENV)/bin/python

UV := $(shell command -v uv 2> /dev/null)

# Interpreter used to RUN the program: the project venv when it exists,
# otherwise any system Python 3.10+. Never needs to resolve packages.
PY := $(shell test -x $(VENV_PY) && echo $(VENV_PY) || echo python3)

# Interpreter used for the TOOLING (flake8, mypy). Prefers the venv
# built by `make install`; falls back to uv, which syncs on demand.
ifneq ($(wildcard $(VENV_PY)),)
TOOL_PY := $(VENV_PY)
else ifdef UV
TOOL_PY := uv run --group dev python
else
TOOL_PY := python3
endif

# Interpreter used for the optional pygame window.
ifneq ($(wildcard $(VENV_PY)),)
VIZ_PY := $(VENV_PY)
else ifdef UV
VIZ_PY := uv run --extra viz python
else
VIZ_PY := python3
endif

# Exactly the flags the subject makes mandatory for the lint rule.
MYPY_FLAGS := --warn-return-any --warn-unused-ignores \
              --ignore-missing-imports --disallow-untyped-defs \
              --check-untyped-defs

MAPS := $(wildcard maps/easy/*.txt maps/medium/*.txt maps/hard/*.txt \
                   maps/challenger/*.txt maps/custom/*.txt)

# A map path given as a bare argument (`make run maps/x.txt`) would be
# read by make as a target, and the default MAP used instead. Catch the
# slip early rather than silently animating the wrong map.
STRAY := $(filter %.txt,$(MAKECMDGOALS))
ifneq ($(STRAY),)
$(error pass the map with MAP=..., e.g. `make $(firstword $(MAKECMDGOALS)) \
MAP=$(firstword $(STRAY))`)
endif

.DEFAULT_GOAL := help
.PHONY: help install run animate debug lint lint-strict benchmark clean

help:
	@echo "Fly-in - available rules"
	@echo "  make install              create .venv and install the tooling (uv)"
	@echo "  make run     [MAP=<file>] run the simulation on a map"
	@echo "  make animate [MAP=<file>] run it with the pygame window"
	@echo "  make debug   [MAP=<file>] run the simulation under pdb"
	@echo "  make lint                 flake8 . + mypy . (mandatory flags)"
	@echo "  make lint-strict          flake8 . + mypy . --strict"
	@echo "  make benchmark            run every map and print the turn counts"
	@echo "  make clean                remove caches and temporary files"
	@echo ""
	@echo "  run interpreter  : $(PY)"
	@echo "  tool interpreter : $(TOOL_PY)"
	@echo "  MAP              : $(MAP)"

install:
ifdef UV
	uv sync --group dev
	@echo ">>> installing the optional pygame extra (safe to fail)"
	-uv sync --group dev --extra viz
else
	@echo ">>> uv not found: falling back to python3 -m venv + pip"
	python3 -m venv $(VENV)
	$(VENV_PY) -m pip install --upgrade pip
	$(VENV_PY) -m pip install "flake8>=7.0" "mypy>=1.8"
	@echo ">>> installing the optional pygame extra (safe to fail)"
	-$(VENV_PY) -m pip install "pygame>=2.5"
endif
	@echo ">>> done. Tooling is in $(VENV)"

run:
	$(PY) main.py $(MAP) $(ARGS)

animate:
	$(VIZ_PY) main.py $(MAP) --animate --speed $(SPEED) $(ARGS)

debug:
	$(PY) -m pdb main.py $(MAP) $(ARGS)

lint:
	$(TOOL_PY) -m flake8 .
	$(TOOL_PY) -m mypy . $(MYPY_FLAGS)

lint-strict:
	$(TOOL_PY) -m flake8 .
	$(TOOL_PY) -m mypy . --strict

benchmark:
	@printf '%-46s %s\n' "MAP" "TURNS"
	@for map in $(MAPS); do \
		turns=$$($(PY) main.py $$map --quiet --no-color 2>/dev/null | wc -l); \
		printf '%-46s %5s\n' "$$map" "$$turns"; \
	done

clean:
	rm -rf .mypy_cache .pytest_cache .ruff_cache
	find . -type d -name '__pycache__' -prune -exec rm -rf {} +
	find . -type f -name '*.py[co]' -delete
