"""Schema-aware validation and trie-based constrained decoding.

This module provides:

* :func:`validate_functions_definition` -- structural validation of
  the JSON function-definition list.
* :class:`SchemaAwareValidator` -- a Pydantic model that indexes
  function definitions in a character-level trie so that the
  constrained decoder can efficiently determine which characters
  (and therefore which tokens) are valid continuations at each
  decoding step.
"""

from typing import Any
from pydantic import BaseModel, PrivateAttr


def validate_functions_definition(
    json_functions: list[dict[str, Any]]
) -> None:
    """Validate the structure of a function-definition list.

    Each element must be a dictionary containing at least the keys
    ``name``, ``description``, and ``parameters`` (itself a dict).

    Args:
        json_functions: A list of function-definition dictionaries
            to validate.

    Raises:
        ValueError: If *json_functions* is not a list, or if any
            element is missing a required key or has an invalid
            ``parameters`` type.
    """
    if not isinstance(json_functions, list):
        raise ValueError("The functions JSON must be a list of objects.")

    for i, func in enumerate(json_functions):
        if "name" not in func:
            raise ValueError(
                f"Validation error: the function at index {i} "
                "is missing the 'name' field."
            )

        func_name = func["name"]

        if "description" not in func:
            raise ValueError(
                f"Validation error: the function '{func_name}'"
                " is missing the 'description' field."
            )

        if "parameters" not in func:
            raise ValueError(
                f"Validation error: the function '{func_name}'"
                " is missing the 'parameters' field."
            )

        parameters = func["parameters"]
        if not isinstance(parameters, dict):
            raise ValueError(
                f"Validation error: the 'parameters' of '{func_name}'"
                " must be a dictionary (object)."
            )


class SchemaAwareValidator(BaseModel):
    """Index of function definitions with trie-based name look-up.

    On initialisation, the validator checks that every function
    definition is well-formed and builds a character-level trie
    over all function names.  The trie supports:

    * Enumerating the valid next characters given a partial name.
    * Checking whether a partial name is a complete function name.
    * Auto-completing a partial name when a single unambiguous
      completion exists.

    Attributes:
        json_functions: The raw list of function-definition dicts.
    """

    json_functions: list[dict[str, Any]]
    _function_by_name: dict[str, dict[str, Any]] = PrivateAttr(
        default_factory=dict
    )
    _name_trie: dict[str | None, Any] = PrivateAttr(default_factory=dict)

    def model_post_init(self, __context: Any) -> None:
        """Finish initialisation after Pydantic validation.

        Validates the function definitions, builds the
        ``_function_by_name`` index, and constructs the name trie.

        Args:
            __context: Pydantic validation context (unused).
        """
        validate_functions_definition(self.json_functions)
        self._function_by_name = {f["name"]: f for f in self.json_functions}
        self._name_trie = self._init_trie(
            [f["name"] for f in self.json_functions]
        )

    @property
    def name_trie(self) -> dict[str | None, Any]:
        """Return the character-level trie over function names."""
        return self._name_trie

    @property
    def function_by_name(self) -> dict[str, dict[str, Any]]:
        """Return the function-definition index keyed by name."""
        return self._function_by_name

    def _init_trie(self, words: list[str]) -> dict[str | None, Any]:
        """Build a character-level trie from a list of words.

        Each leaf stores the sentinel key ``None`` mapped to the
        complete word.

        Args:
            words: The strings to insert into the trie.

        Returns:
            A nested dictionary representing the trie.
        """
        trie: dict[str | None, Any] = {}
        for word in words:
            node = trie
            for char in word:
                node = node.setdefault(char, {})
            node[None] = word
        return trie

    @staticmethod
    def _walk_trie(trie: dict[str | None, Any], partial: str) -> set[str]:
        """Return the set of valid next characters for *partial*.

        Traverses *trie* according to the characters in *partial*
        and returns the non-``None`` children of the landing node.

        Args:
            trie: The character-level trie to walk.
            partial: The prefix string consumed so far.

        Returns:
            A set of single characters that may validly follow
            *partial*, or an empty set if the prefix is invalid.
        """
        node = trie
        for char in partial:
            if char not in node:
                return set()
            node = node[char]
        return {c for c in node if c is not None}

    @staticmethod
    def _is_complete_in_trie(
        trie: dict[str | None, Any], partial: str
    ) -> bool:
        """Check whether *partial* is a complete word in the trie.

        Args:
            trie: The character-level trie to check.
            partial: The candidate string.

        Returns:
            ``True`` if *partial* corresponds to a complete entry
            (i.e. its node contains the ``None`` sentinel), ``False``
            otherwise.
        """
        node = trie
        for char in partial:
            if char not in node:
                return False
            node = node[char]
        return None in node

    @staticmethod
    def _unique_completion(
        trie: dict[str | None, Any], partial: str
    ) -> str | None:
        """Return the unique completion suffix if one exists.

        Given the prefix *partial*, if there is exactly one way to
        complete the word in the trie, return the remaining suffix
        (without the prefix itself).  Otherwise return ``None``
        (ambiguous prefix, dead-end, or already complete with
        possible continuations).

        This allows auto-completing function / parameter names
        without querying the model character by character when the
        rest is forced by the schema.

        Args:
            trie: The character-level trie to search.
            partial: The prefix string consumed so far.

        Returns:
            The unambiguous remaining suffix, or ``None`` if the
            completion is not unique.
        """
        node = trie
        for char in partial:
            if char not in node:
                return None
            node = node[char]

        suffix = ""
        while True:
            children = [c for c in node if c is not None]
            has_end = None in node
            if has_end and not children:
                return suffix
            if has_end and children:
                return None
            if len(children) == 1:
                char = children[0]
                suffix += char
                node = node[char]
                continue
            return None

    def next_char_for_name(self, partial: str) -> set[str]:
        """Return the valid next characters for a partial function name.

        Args:
            partial: The function-name prefix typed so far.

        Returns:
            A set of characters that may validly follow *partial*.
        """
        return self._walk_trie(self.name_trie, partial)

    def is_complete(self, partial: str) -> bool:
        """Check whether *partial* is a complete function name.

        Args:
            partial: The candidate function name.

        Returns:
            ``True`` if *partial* matches a registered function
            name, ``False`` otherwise.
        """
        return self._is_complete_in_trie(self.name_trie, partial)

    def get_unique_completion_for_name(self, partial: str) -> str | None:
        """Auto-complete a partial function name if unambiguous.

        Args:
            partial: The function-name prefix typed so far.

        Returns:
            The remaining suffix to append, or ``None`` if the
            completion is ambiguous or the prefix is invalid.
        """
        return self._unique_completion(self.name_trie, partial)

    def get_param_type(self, function_name: str, param_name: str) -> Any:
        """Return the declared type of a function parameter.

        Args:
            function_name: Name of the function to look up.
            param_name: Name of the parameter within that function.

        Returns:
            The ``"type"`` string from the parameter schema, or
            ``"string"`` as a default if not specified.
        """
        func = self._function_by_name.get(function_name, {})
        params = func.get("parameters", {})
        return params.get(param_name, {}).get("type", "string")

    def get_required_params(self, function_name: str) -> list[str]:
        """Return the list of parameter names for a function.

        Args:
            function_name: Name of the function to look up.

        Returns:
            An ordered list of parameter name strings.
        """
        func = self._function_by_name.get(function_name, {})
        return list(func.get("parameters", {}).keys())
