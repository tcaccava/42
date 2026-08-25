const COMPLETION_COLORS = [
  'bg-primary-container', 'bg-primary-container',
  'bg-primary-fixed-dim', 'bg-primary-fixed-dim',
  'bg-primary-fixed',     'bg-primary-fixed',
  'bg-error',             'bg-error',
  'bg-error-container',   'bg-error-container',
];

/**
 * Calculates the spectrum for a board: 1-indexed height of each column's highest block.
 * @param {Array<Array<string|null>>} board
 * @returns {{ spectrum: number[], maxHeight: number }}
 */
function calcSpectrum(board) {
  const rows = board.length;
  const cols = board[0].length;
  const spectrum = Array(cols).fill(0);
  for (let c = 0; c < cols; c++) {
    for (let r = 0; r < rows; r++) {
      if (board[r][c] !== null) {
        spectrum[c] = rows - r;
        break;
      }
    }
  }
  const maxHeight = Math.max(...spectrum);
  return { spectrum, maxHeight };
}

/**
 * Opponent field spectrum view + completion bar.
 * @param {Object} props
 * @param {Array<Array<string|null>>} props.board
 * @param {boolean} [props.isDead]
 */
function MiniGameBoard({ board, isDead }) {
  const rows = board.length;
  const cols = board[0].length;

  const { spectrum, maxHeight } = calcSpectrum(board);
  const numSegments = COMPLETION_COLORS.length;
  const rowsPerSegment = rows / numSegments;

  return (
    <div className="flex gap-1 items-stretch">
      {/* Spectrum grid */}
      <div
        className={`grid gap-px ${isDead ? 'opacity-50 grayscale' : ''}`}
        style={{
          gridTemplateColumns: `repeat(${cols}, 1fr)`,
          width: '96px',
        }}
      >
        {Array.from({ length: rows * cols }, (_, i) => {
          const row = Math.floor(i / cols);
          const col = i % cols;
          const rowFromBottom = rows - row;
          const isFilled = rowFromBottom <= spectrum[col];
          return (
            <div
              key={i}
              className={`aspect-square ${
                isFilled
                  ? 'bg-primary-container border border-primary-container'
                  : 'bg-white/3 border border-white/5'
              }`}
            />
          );
        })}
      </div>

      {/* Completion bar */}
      <div className="flex flex-col-reverse gap-px w-2">
        {Array.from({ length: numSegments }, (_, i) => (
          <div
            key={i}
            className={`flex-1 rounded-sm transition-colors ${
              maxHeight > i * rowsPerSegment
                ? COMPLETION_COLORS[i]
                : 'bg-white/3 border border-white/5'
            }`}
          />
        ))}
      </div>
    </div>
  );
}

export default MiniGameBoard;
export { calcSpectrum };
