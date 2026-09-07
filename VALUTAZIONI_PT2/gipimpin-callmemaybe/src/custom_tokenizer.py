"""BPE tokenizer compatible with GPT-2 / GPT-NeoX style vocabularies.

This module implements a Byte-Pair Encoding (BPE) tokenizer as a
Pydantic ``BaseModel``.  It supports encoding text to token ids,
decoding token ids back to text, and maintains an internal DFA-like
cache that maps each leading character to the set of token ids whose
decoded form starts with that character.
"""

from pydantic import BaseModel, model_validator, PrivateAttr
import re
import numpy as np
from typing import Any, ClassVar

_DIGITS_AND_SIGNS = frozenset("0123456789.-")
_NUMBER_DELIMITERS = frozenset({",", "}", " ", "\n"})


class CustomTokenizer(BaseModel):
    """BPE tokenizer with GPT-2-style byte-level encoding.

    The tokenizer is initialised from three external resources:

    * **token_id** -- vocabulary mapping (token string -> integer id).
    * **dict_merge** -- ordered BPE merge table
      (pair of strings -> merge rank).
    * **special_token_id** -- special / added tokens
      (token string -> integer id).

    After initialisation the model builds reverse look-up tables and
    a per-first-character index that accelerates constrained decoding.

    Attributes:
        SPLIT_REGEX: Pre-compiled regex used to split raw text into
            BPE-compatible chunks (contractions, words, digits, etc.).
        token_id: Vocabulary mapping from token strings to integer ids.
        dict_merge: Ordered BPE merge table mapping pairs of token
            strings to their merge rank.
        special_token_id: Mapping from special token strings to
            their integer ids.
    """

    SPLIT_REGEX: ClassVar[re.Pattern[str]] = re.compile(
        r"""(?i:'s|'t|'re|'ve|'m|'ll|'d)"""
        r"""|[^\r\n\w]?[^\W\d_]+"""
        r"""|\d"""
        r"""|\ ?[^\s\w]+[\r\n]*"""
        r"""|\s*[\r\n]+"""
        r"""|\s+(?!\S)"""
        r"""|\s+""",
        re.VERBOSE,
    )
    token_id: dict[str, int]
    dict_merge: dict[tuple[str, str], int]
    special_token_id: dict[str, Any]
    _id_token: dict[int, str] = PrivateAttr(default_factory=dict)
    _special_id_token: dict[int, Any] = PrivateAttr(default_factory=dict)
    _byte_encoder: dict[int, str] = PrivateAttr(default_factory=dict)
    _byte_decoder: dict[str, int] = PrivateAttr(
        default_factory=dict
    )
    _special_split_regex: re.Pattern[str] | None = PrivateAttr(default=None)
    _tokens_by_first_char: dict[str, list[int]] = PrivateAttr(
        default_factory=dict
    )
    _decoded_tokens: dict[int, str] = PrivateAttr(default_factory=dict)
    _digit_token_ids: "np.ndarray" = PrivateAttr(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    _delimiter_token_ids: "np.ndarray" = PrivateAttr(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    _quote_token_ids: "np.ndarray" = PrivateAttr(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    _non_quote_token_ids: "np.ndarray" = PrivateAttr(
        default_factory=lambda: np.array([], dtype=np.int64)
    )

    @model_validator(mode="before")
    @classmethod
    def filter_special_token(cls, data: Any) -> Any:
        """Normalise the ``special_token_id`` field before validation.

        When ``special_token_id`` is provided as a list of dicts
        (the raw format from *tokenizer.json*), this validator
        converts it into a flat ``{content: id}`` mapping.

        Args:
            data: Raw input data dictionary.

        Returns:
            The (possibly mutated) data dictionary ready for Pydantic
            field validation.
        """
        if isinstance(data, dict):
            special_token_id = data.get("special_token_id", None)
            if special_token_id:
                special_token_id = {
                    token["content"]: token["id"] for token in special_token_id
                }
                data["special_token_id"] = special_token_id
        return data

    def model_post_init(self, __context: Any) -> None:
        """Finish initialisation after Pydantic field validation.

        Builds the reverse look-up tables (``_id_token``,
        ``_byte_encoder``, ``_byte_decoder``,
        ``_special_id_token``), compiles a regex for splitting on
        special tokens, and populates the DFA cache.

        Args:
            __context: Pydantic validation context (unused).
        """
        self._id_token = {id_: token for token, id_ in self.token_id.items()}
        self._byte_encoder = self._bytes_to_unicode()
        self._byte_decoder = {v: k for k, v in self._byte_encoder.items()}
        self._special_id_token = {
            v: k for k, v in self.special_token_id.items()
        }
        if self.special_token_id:
            pattern = "|".join(re.escape(tok) for tok in self.special_token_id)
            self._special_split_regex = re.compile(f"({pattern})")
        self._init_dfa_cache()

    @property
    def id_token(self) -> dict[int, str]:
        """Return the reverse vocabulary (id -> token string)."""
        return self._id_token

    @property
    def special_id_token(self) -> dict[int, Any]:
        """Return the reverse special-token map (id -> token string)."""
        return self._special_id_token

    @property
    def decoded_tokens(self) -> dict[int, str]:
        """Return the cached decoded-token map (id -> string)."""
        return self._decoded_tokens

    @property
    def tokens_by_first_char(self) -> dict[str, list[int]]:
        """Return the first-character token index."""
        return self._tokens_by_first_char

    @property
    def digit_token_ids(self) -> "np.ndarray":
        """Return precomputed token IDs composed of digits, signs, or dot."""
        return self._digit_token_ids

    @property
    def delimiter_token_ids(self) -> "np.ndarray":
        """Return precomputed token IDs representing numeric delimiters."""
        return self._delimiter_token_ids

    @property
    def quote_token_ids(self) -> "np.ndarray":
        """Return precomputed token IDs containing a quote character."""
        return self._quote_token_ids

    @property
    def non_quote_token_ids(self) -> "np.ndarray":
        """Return precomputed token IDs without a quote character."""
        return self._non_quote_token_ids

    def _init_dfa_cache(self) -> None:
        """Build the per-first-character token index.

        Iterates over every non-special token, decodes it, and
        records the mapping ``first_char -> [token_id, ...]`` in
        ``_tokens_by_first_char``.  Tokens that cannot be decoded or
        that contain the Unicode replacement character are skipped.
        """
        self._tokens_by_first_char = {}
        self._decoded_tokens = {}
        for id_ in self._id_token:
            if id_ in self.special_id_token:
                continue
            try:
                decoded = self.decode(id_)
            except (KeyError, UnicodeDecodeError):
                continue
            if not decoded or "\ufffd" in decoded:
                continue

            self._decoded_tokens[id_] = decoded
            first_char = decoded[0]
            if first_char not in self._tokens_by_first_char:
                self._tokens_by_first_char[first_char] = []
            self._tokens_by_first_char[first_char].append(id_)

        digit_ids, delim_ids, quote_ids, non_quote_ids = [], [], [], []
        for tid, tok_str in self._decoded_tokens.items():
            if tok_str in _NUMBER_DELIMITERS:
                delim_ids.append(tid)
            elif tok_str and all(c in _DIGITS_AND_SIGNS for c in tok_str):
                digit_ids.append(tid)

            if '"' in tok_str:
                quote_ids.append(tid)
            else:
                non_quote_ids.append(tid)

        self._digit_token_ids = np.array(digit_ids, dtype=np.int64)
        self._delimiter_token_ids = np.array(delim_ids, dtype=np.int64)
        self._quote_token_ids = np.array(quote_ids, dtype=np.int64)
        self._non_quote_token_ids = np.array(non_quote_ids, dtype=np.int64)

    def get_tokens_starting_with(self, char: str) -> list[int]:
        """Return token ids whose decoded form starts with *char*.

        Args:
            char: A single character to look up.

        Returns:
            A list of token ids (possibly empty) whose decoded
            representation begins with *char*.
        """
        return self._tokens_by_first_char.get(char, [])

    def get_decoded_token(self, id_: int) -> str:
        """Return the cached decoded string for a token id.

        Args:
            id_: The integer token id.

        Returns:
            The decoded string, or an empty string if *id_* is not
            present in the cache.
        """
        return self._decoded_tokens.get(id_, "")

    def _bytes_to_unicode(self) -> dict[int, str]:
        """Create a byte-to-unicode mapping (GPT-2 style).

        Builds a reversible mapping from every byte value (0-255) to
        a unique Unicode character.  Printable ASCII and Latin-1
        characters map to themselves; remaining bytes are mapped to
        characters starting at code-point 256.

        Returns:
            A dictionary mapping byte values to single Unicode
            characters.
        """
        tokens = (
            list(range(ord("!"), ord("~") + 1))
            + list(range(ord("¡"), ord("¬") + 1))
            + list(range(ord("®"), ord("ÿ") + 1))
        )
        copy = tokens[:]
        n = 0
        for c in range(256):
            if c not in tokens:
                copy.append(c)
                tokens.append(256 + n)
                n += 1
        return dict(zip(copy, [chr(c) for c in tokens]))

    def encode(self, text: str) -> list[int]:
        """Encode a text string into a list of token ids.

        The text is first split on special tokens (if any), then each
        non-special segment is split into BPE-compatible chunks using
        ``SPLIT_REGEX``.  Each chunk is byte-encoded and iteratively
        merged according to ``dict_merge`` until no more merges
        apply.

        Args:
            text: The input string to tokenise.

        Returns:
            An ordered list of integer token ids.
        """
        l_index = []
        pieces = (
            self._special_split_regex.split(text)
            if self._special_split_regex
            else [text]
        )
        for piece in pieces:
            if not piece:
                continue
            if piece in self.special_token_id:
                l_index.append(self.special_token_id[piece])
                continue
            l_text = self.SPLIT_REGEX.findall(piece)
            for word in l_text:
                word_bytes = word.encode("utf-8")
                chars = [self._byte_encoder[b] for b in word_bytes]
                while len(chars) >= 2:
                    best_pair = None
                    best_rank = float("inf")
                    for i in range(len(chars) - 1):
                        pair = (chars[i], chars[i + 1])
                        rank = self.dict_merge.get(pair)
                        if rank is not None and rank < best_rank:
                            best_rank = rank
                            best_pair = pair
                    if best_pair is None:
                        break
                    new_word = []
                    i = 0
                    while i < len(chars):
                        if (
                            i < len(chars) - 1 and
                            (chars[i], chars[i + 1]) == best_pair
                        ):
                            new_word.append(chars[i] + chars[i + 1])
                            i += 2
                        else:
                            new_word.append(chars[i])
                            i += 1
                    chars = new_word
                for c in chars:
                    l_index.append(self.token_id[c])
        return l_index

    def decode(self, id_: int) -> Any:
        """Decode a single token id back to its string representation.

        Special tokens are returned as-is from the special token
        map.  Regular tokens are converted from their byte-level
        Unicode representation back to a UTF-8 string.

        Args:
            id_: The integer token id to decode.

        Returns:
            The decoded string for regular tokens, or the special
            token string for special token ids.
        """
        if id_ in self.special_id_token:
            return self.special_id_token[id_]
        token_str = self._id_token.get(id_, "")
        byte_list = [self._byte_decoder[c] for c in token_str]
        return bytes(byte_list).decode("utf-8", errors="replace")

    def decode_list(self, ids: list[int]) -> str:
        """Decode a list of token ids into a single string.

        Accumulates raw bytes for regular tokens and flushes them
        whenever a special token is encountered, ensuring correct
        multi-byte UTF-8 handling across token boundaries.

        Args:
            ids: An ordered list of integer token ids.

        Returns:
            The fully decoded string.
        """
        res = ""
        accumulate_byte = bytearray()
        for id_ in ids:
            if id_ in self.special_id_token:
                if accumulate_byte:
                    res += accumulate_byte.decode("utf-8", errors="replace")
                    accumulate_byte.clear()
                res += self.special_id_token[id_]
            else:
                token_str = self._id_token.get(id_, "")
                for c in token_str:
                    accumulate_byte.append(self._byte_decoder[c])
        if accumulate_byte:
            res += accumulate_byte.decode("utf-8", errors="replace")
        return str(res)
