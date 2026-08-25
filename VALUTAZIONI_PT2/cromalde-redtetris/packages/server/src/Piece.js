import { getCells, nextRotation } from '@red-tetris/shared';

class Piece {
  constructor(name, rotation = 0, row = 0, col = 3) {
    this.name = name;
    this.rotation = rotation;
    this.row = row;
    this.col = col;
  }

  /**
   * Returns absolute [row, col] positions on the board.
   * @returns {Array<[number, number]>}
   */
  getCells() {
    const offsets = getCells(this.name, this.rotation);
    return offsets.map(([r, c]) => [this.row + r, this.col + c]);
  }

  rotateCW() {
    this.rotation = nextRotation(this.rotation, 1);
  }

  clone() {
    return new Piece(this.name, this.rotation, this.row, this.col);
  }
}

export default Piece;
