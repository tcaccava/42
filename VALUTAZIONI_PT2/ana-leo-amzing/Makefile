# STUB — targets are in place, but the program itself is not written yet.
# The rules below are the ones the subject requires, plus a test target.

PYTHON  := python3
MAIN    := a_maze_ing.py
CONFIG  := config.txt

# Everything runs inside a local virtualenv, so the machine's own
# Python is never touched and a checkout only needs "make".
VENV    := .venv
VPYTHON := $(VENV)/bin/python
# Written once the dependencies are in: the other targets depend on it,
# so they install what they need the first time and skip it afterwards.
STAMP   := $(VENV)/.installed

# Colours for the help screen. ESC is built with printf so the escape
# character is stored literally and any echo prints it as it is.
ESC     := $(shell printf '\033')
BOLD    := $(ESC)[1m
DIM     := $(ESC)[2m
CYAN    := $(ESC)[36m
GREEN   := $(ESC)[32m
YELLOW  := $(ESC)[33m
RED     := $(ESC)[31m
RESET   := $(ESC)[0m

# What a submitted repository has to carry, and nothing else. The
# subject asks for README.md, LICENSE.md, a_maze_ing.py, a mazegen-*
# package file, a configuration file, and everything needed to build
# that package again; the rest of this list is what those need to run
# and to pass the checks — app/ and src/ are the program itself,
# pyproject.toml and setup.cfg build and lint it, the Makefile is
# required in its own right, and .gitignore is asked for by the
# common instructions.
#
# Left out on purpose: tests/ (the subject says they are not
# submitted), docs/, subject/, the virtualenv and every cache.
BUNDLE      := dist
BUNDLE_FILES := README.md LICENSE.md Makefile pyproject.toml setup.cfg \
                .gitignore $(MAIN) $(CONFIG)
BUNDLE_DIRS  := app src/mazegen src/rainbow

.PHONY: install run debug build bundle clean fclean lint help

# First target of the file, so a bare "make" lands here.
help:
	@printf '$(CYAN)$(BOLD)'
	@printf '%s\n' \
	'    ___         __  ___                       _' \
	'   /   |       /  |/  /___ _____  ___        (_)___  ____ _' \
	'  / /| |______/ /|_/ / __ `/_  / / _ \______/ / __ \/ __ `/' \
	' / ___ /_____/ /  / / /_/ / / /_/  __/_____/ / / / / /_/ /' \
	'/_/  |_|    /_/  /_/\__,_/ /___/\___/     /_/_/ /_/\__, /' \
	'                                                  /____/'
	@printf '$(RESET)$(DIM)%s$(RESET)\n\n' 'developed by anade-mo and ltamiano'
	@printf '$(GREEN)$(BOLD)%s$(RESET)\n' 'Run'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'install' 'create $(VENV) and install the dependencies' \
	'run'     'draw the maze $(CONFIG) asks for' \
	'debug'   'run it under pdb'
	@printf '\n$(YELLOW)$(BOLD)%s$(RESET)\n' 'Test'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'lint' 'flake8 + mypy --strict'
	@printf '\n$(RED)$(BOLD)%s$(RESET)\n' 'Clean'
	@printf '  $(BOLD)%-12s$(RESET) %s\n' \
	'clean'  'remove caches and build artefacts' \
	'fclean' 'clean, and remove $(VENV) as well'
	@printf '\n'

install: $(STAMP)

# Remade when pyproject.toml changes, since it is the only place the
# dependencies are declared. "-e ." installs the package itself in
# editable mode, "[dev]" adds the tools the checks need.
#
# Both packages under src/ go in: mazegen, the reusable module, and
# rainbow, the colours the renderer draws with. The import check is
# what makes that a promise rather than a side effect of how the
# distribution happens to be laid out — a_maze_ing.py needs both, so a
# missing one is caught here rather than as a traceback on first run.
$(STAMP): pyproject.toml
	$(PYTHON) -m venv $(VENV)
	$(VPYTHON) -m pip install --upgrade pip
	$(VPYTHON) -m pip install -e .[dev]
	$(VPYTHON) -c 'import mazegen, rainbow' \
	  || { echo 'install failed: mazegen and rainbow are not both importable'; exit 1; }
	touch $(STAMP)

# The terminal is the renderer, so "run" is one rule again: it draws
# the maze from $(VENV), which any target here can build.
run: $(STAMP)
	@$(VPYTHON) $(MAIN) $(CONFIG) || true

debug: $(STAMP)
	$(VPYTHON) -m pdb $(MAIN) $(CONFIG)

# --strict is a superset of the five flags the subject requires of
# this rule; setup.cfg still names them one by one, so the connection
# to the subject stays visible where the flags actually live.
lint: $(STAMP)
	$(VPYTHON) -m flake8 .
	$(VPYTHON) -m mypy . --strict

clean:
	rm -rf .mypy_cache .pytest_cache build dist
	rm -f mazegen-*.whl mazegen-*.tar.gz
	@# setuptools writes the .egg-info beside the package, so under
	@# src/ rather than here; a bare *.egg-info at the root misses it.
	find . -path ./$(VENV) -prune -o \
	  -type d -name '*.egg-info' -exec rm -rf {} + 2>/dev/null || true
	find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true

# The virtualenv is rebuilt by any target that needs it, so removing it
# is safe; kept apart from clean because rebuilding costs a download.
fclean: clean
	rm -rf $(VENV)
