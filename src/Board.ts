import * as THREE from 'three';

const CELL = 1;
const GRID = 6;
const HALF = GRID / 2;
const BOARD_Y = 0;
const WALL_HEIGHT = 0.35;
const WALL_THICKNESS = 0.15;
const EXIT_ROW = 2; // row index where the exit gap is (hero block row in grid coords)

export class Board {
  readonly group: THREE.Group;

  constructor() {
    this.group = new THREE.Group();
    this.buildSurface();
    this.buildWalls();
    this.buildGridLines();
  }

  private buildSurface() {
    const geo = new THREE.BoxGeometry(GRID * CELL, 0.3, GRID * CELL);
    const mat = new THREE.MeshStandardMaterial({
      color: 0x2a2a3e,
      roughness: 0.7,
      metalness: 0.2,
    });
    const mesh = new THREE.Mesh(geo, mat);
    mesh.position.set(0, BOARD_Y - 0.15, 0);
    mesh.receiveShadow = true;
    this.group.add(mesh);
  }

  private buildWalls() {
    const wallMat = new THREE.MeshStandardMaterial({
      color: 0x3a3a55,
      roughness: 0.6,
      metalness: 0.3,
    });

    const makeWall = (
      w: number, h: number, d: number,
      x: number, y: number, z: number
    ) => {
      const geo = new THREE.BoxGeometry(w, h, d);
      const mesh = new THREE.Mesh(geo, wallMat);
      mesh.position.set(x, y, z);
      mesh.castShadow = true;
      mesh.receiveShadow = true;
      this.group.add(mesh);
    };

    const wy = BOARD_Y + WALL_HEIGHT / 2;

    // Left wall
    makeWall(WALL_THICKNESS, WALL_HEIGHT, GRID * CELL, -HALF - WALL_THICKNESS / 2, wy, 0);
    // Bottom wall
    makeWall(GRID * CELL + WALL_THICKNESS * 2, WALL_HEIGHT, WALL_THICKNESS, 0, wy, HALF + WALL_THICKNESS / 2);
    // Top wall
    makeWall(GRID * CELL + WALL_THICKNESS * 2, WALL_HEIGHT, WALL_THICKNESS, 0, wy, -HALF - WALL_THICKNESS / 2);

    // Right wall — with exit gap at row EXIT_ROW
    // Gap spans from cell top-edge to cell bottom-edge
    const exitStart = -HALF + EXIT_ROW * CELL; // top edge of gap
    const exitEnd = exitStart + CELL;           // bottom edge of gap

    // Wall above exit
    const topLen = exitStart - (-HALF);
    if (topLen > 0.01) {
      makeWall(WALL_THICKNESS, WALL_HEIGHT, topLen,
        HALF + WALL_THICKNESS / 2, wy, -HALF + topLen / 2);
    }

    // Wall below exit
    const bottomLen = HALF - exitEnd;
    if (bottomLen > 0.01) {
      makeWall(WALL_THICKNESS, WALL_HEIGHT, bottomLen,
        HALF + WALL_THICKNESS / 2, wy, exitEnd + bottomLen / 2);
    }
  }

  private buildGridLines() {
    const lineMat = new THREE.LineBasicMaterial({
      color: 0x444466,
      transparent: true,
      opacity: 0.3,
    });

    for (let i = 0; i <= GRID; i++) {
      const pos = -HALF + i * CELL;

      // Vertical lines
      const vGeo = new THREE.BufferGeometry().setFromPoints([
        new THREE.Vector3(pos, BOARD_Y + 0.005, -HALF),
        new THREE.Vector3(pos, BOARD_Y + 0.005, HALF),
      ]);
      this.group.add(new THREE.Line(vGeo, lineMat));

      // Horizontal lines
      const hGeo = new THREE.BufferGeometry().setFromPoints([
        new THREE.Vector3(-HALF, BOARD_Y + 0.005, pos),
        new THREE.Vector3(HALF, BOARD_Y + 0.005, pos),
      ]);
      this.group.add(new THREE.Line(hGeo, lineMat));
    }
  }

  addToScene(scene: THREE.Scene) {
    scene.add(this.group);
  }

  removeFromScene(scene: THREE.Scene) {
    scene.remove(this.group);
  }
}

export { CELL, GRID, HALF, BOARD_Y, EXIT_ROW };
